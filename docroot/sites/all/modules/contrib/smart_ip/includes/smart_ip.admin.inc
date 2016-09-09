<?php

/**
 * @file
 * Admin interface callbacks/handlers to configure Smart IP.
 */

/**
 * Smart IP administration settings.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 */
function smart_ip_admin_settings($form, &$form_state) {
  // Define submit handler function
  $form['#submit'][] = 'smart_ip_admin_settings_submit';
  $form['#validate'][] = 'smart_ip_admin_settings_validate';
  $smart_ip_source = variable_get('smart_ip_source', 'ipinfodb_service');
  
  // MaxMind's Apache module mod_geoip, X-GeoIP-Country: XX header and Cloudflare IP Geolocation don't support manual lookup and admin debug
  if ($smart_ip_source == 'mod_geoip' || $smart_ip_source == 'x_header' || $smart_ip_source == 'cf_header') {
    $can_query_ip = FALSE;
  }
  else {
    $can_query_ip = TRUE;
  }

  if ($smart_ip_source == 'local_db') {
    $recover = !variable_get('smart_ip_db_update_busy', FALSE) & (variable_get('smart_ip_get_zip_done', FALSE) | variable_get('smart_ip_extract_zip_done', FALSE) | variable_get('smart_ip_store_location_csv_done', FALSE));
    // Container for database update preference forms 
    $form['smart_ip_database_update'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Manual database update'),
      '#collapsible' => FALSE,
      '#collapsed'   => FALSE,
      '#states'      => array(
        'visible' => array(
          ':input[name="smart_ip_source"]' => array('value' => 'local_db'),
        ),
      ),
    );
    $update_time = variable_get('smart_ip_last_update', 0);
    // Form for message markup
    $form['smart_ip_database_update']['smart_ip_update_message'] = array(
      '#type'   => 'item',
      '#markup' => '<div>' . t('The Smart IP database may be updated manually by pressing the "Update database now" button below. Note, this may take more or less five hours (it depends on the speed of the server). This process can recover from where it stopped or failed.') . '</div>',
      '#suffix' => isset($form_state['storage']['smart_ip_db_message']) ? '<div id="database-update" class="messages">' . $form_state['storage']['smart_ip_db_message'] . '</div>' : '<div id="database-update" class="messages">' . t('Database last updated on ') . format_date($update_time, 'custom', 'n/j/Y') . t(' at ') . format_date($update_time, 'custom', 'H:i:s T') . '</div>',
    );
    $form['smart_ip_database_update']['smart_ip_csv_source'] = array(
      '#type'          => 'textfield',
      '#title'         => t('CSV URL source'),
      '#default_value' => smart_ip_get_csv_source_filename(),
      '#description'   => t("Enter the Maxmind's CSV URL, source for updating the Smart IP database"),
      '#size'          => 120,
    );
    // Form for manual updating of the Smart IP database
    $form['smart_ip_database_update']['smart_ip_update_database'] = array(
      '#type'     => 'submit',
      '#value'    => $recover ? t('Recover database update') : t('Update database now'),
      '#submit'   => array('_smart_ip_database_update_submit'),
      '#validate' => array('_smart_ip_database_update_validate'),
      '#suffix'   => $recover ? '<div class="messages error">' . t('The previous attempt of updating Smart IP database has failed. Press the button above to continue the unfinished task.') . '</div>' : '',
    );
    // Form to reset (ongoing) update process
    $form['smart_ip_database_update']['smart_ip_reset_update_database'] = array(
      '#type'        => 'submit',
      '#value'       => t('Reset update process'),
      '#submit'      => array('_smart_ip_database_reset_update_submit'),
      '#prefix'      => '<div>', 
      '#suffix'      => '<div class="messages warning">' . t('Caution: The "Reset update process" button above will reset (ongoing) update process and will erase the data stored in database came from maxmind CSV. Please use this function only if the update hanged. It will lead to abnormal update processing if misused.') . '</div></div>',
    );
  }
  if ($smart_ip_source == 'maxmind_bin' || $smart_ip_source == 'maxmindgeoip2_bin') {
    if ($smart_ip_source == 'maxmind_bin') {
      $bin_update_error = variable_get('smart_ip_maxmind_bin_db_update_error', FALSE);
      $update_time = variable_get('smart_ip_maxmind_bin_db_last_update', 0);
    }
    elseif ($smart_ip_source == 'maxmindgeoip2_bin') {
      $bin_update_error = variable_get('smart_ip_maxmind_geoip2_bin_db_update_error', FALSE);
      $update_time = variable_get('smart_ip_maxmind_geoip2_bin_db_last_update', 0);
    }
    $last_updated = $update_time ? t('Database last updated on ') . format_date($update_time, 'custom', 'n/j/Y') . t(' at ') . format_date($update_time, 'custom', 'H:i:s T') : t('Database has not been updated');
    // Container for Maxmind bin database update status and manual update
    $form['smart_ip_bin_database_update'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Database Update Status'),
      '#collapsible' => FALSE,
      '#collapsed'   => FALSE,
      '#states'      => array(
        'visible' => array(
          ':input[name="smart_ip_source"]' => array('value' => $smart_ip_source),
        ),
      ),
    );
    $form['smart_ip_bin_database_update']['smart_ip_update_message'] = array(
      '#type'   => 'item',
      '#markup' => '<div>' . t('The MaxMind binary database may be updated manually by pressing the "Update database now" button below.') . '</div>',
      '#suffix' => isset($form_state['storage']['smart_ip_bin_db_message']) ? '<div id="bin-database-update" class="messages">' . $form_state['storage']['smart_ip_bin_db_message'] . '</div>' : '<div id="bin-database-update" class="messages">' . $last_updated . '</div>',
    );
    // Form for manual updating of the Smart IP database
    $form['smart_ip_bin_database_update']['smart_ip_bin_update_database'] = array(
      '#type'   => 'submit',
      '#value'  => t('Update database now'),
      '#submit' => array('_smart_ip_bin_database_update_submit'),
      '#suffix' => $bin_update_error ? '<div class="messages error">' . t('The previous attempt of updating MaxMind binary database has failed with the error: ') . $bin_update_error . '</div>' : '',
    );
  }

  // Container for manual lookup
  $form['smart_ip_manual_lookup'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Manual lookup'),
    '#description' => t('Examine database values'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
    '#access' => $can_query_ip,
  );

  // Form for manual lookups
  $form['smart_ip_manual_lookup']['smart_ip_lookup'] = array(
    '#type'        => 'textfield',
    '#title'       => t('IP address'),
    '#description' => t('An IP address may be looked up in the database by entering the address above then pressing the %lookup button below.', array('%lookup' => t('Lookup'))),
  );

  // Form for manual manual lookups button
  $form['smart_ip_manual_lookup']['smart_ip_lookup_button'] = array(
    '#type'   => 'submit',
    '#value'  => t('Lookup'),
    '#submit' => array('_smart_ip_lookup_submit'),
    '#ajax'   => array(
      'callback' => '_smart_ip_lookup_js',
    ),
    '#suffix' => isset($form_state['storage']['smart_ip_message']) ? '<div id="lookup-message" class="messages">' . $form_state['storage']['smart_ip_message'] . '</div>' : '',
  );

  // Container for Smart IP source forms
  $form['smart_ip_data_source_selection'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Smart IP source'),
    '#description'   => t('Please select only one source:'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );
  
  // Form for Smart IP source selection
  $private_folder = drupal_realpath(file_stream_wrapper_uri_normalize('private://smart_ip'));
  $form['smart_ip_data_source_selection']['smart_ip_source'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select Smart IP source'),
    '#description'   => t('Please populate the appropriate fields below according to your selected Smart IP source.'),
    '#options' => array(
      'ipinfodb_service'      => t("Use !ipinfodb web service. The !ip2location free version database is used by !ipinfodb in their web service. You will need an API key to use this and you must be !login to get it. Note: if !ipinfodb respond too slow to geolocation request, your site's performance will be affected specially if Smart IP is configured to geolocate anonymous users.", array('!ipinfodb' => l(t('IPInfoDB.com'), 'http://www.ipinfodb.com'), '!ip2location' => l(t('IP2Location'), 'http://ip2location.com'), '!login' => l(t('logged in'), 'http://ipinfodb.com/login.php'))),
      'maxmindgeoip_service'  => t('Use GeoIP Legacy Web Services, geoip.maxmind.com service will be used to query Geo IP information. To use this, you need to buy a developer key.'),
      'maxmindgeoip2_service' => t('Use GeoIP2 Precision Web Services, this is the next version of GeoIP Legacy Web Services. The geoip.maxmind.com service will be used to query Geo IP information. To use this, you need to buy their service(s).'),
      'ip2location_bin'       => t("Use IP2Location binary database. It uses two binary database files; one is for IPV4 address support and the other is for IPV6 address support. You will manually download these two binary database files and upload them to your server. Automatic update is not yet supported here and its your responsibility to update them manually every month. Paid and free versions are available. You need to register first for an account !here for lite version and login !here2 in able to download the two binary database files. For licensed version, you need to buy their product and they will provide you the login details and use it to login !here3. You can download the files !here4 for lite version and !here5 for licensed version. Recommended product ID are DB1 (if you need country level only and more faster query) or DB11 (if you need more details but this is less faster than DB1).", array('!here' => l(t('here'), 'http://lite.ip2location.com/sign-up'), '!here2' => l(t('here'), 'http://lite.ip2location.com/login'), '!here3' => l(t('here'), 'https://www.ip2location.com/login'), '!here4' => l(t('here'), 'http://lite.ip2location.com/database'), '!here5' => l(t('here'), 'https://www.ip2location.com/download'))),
      'local_db'              => t("Use Smart IP database, the site's local database populated from Maxmind's CSV will be used to query Geo IP information. Note: this option is very expensive to run and it requires about 450MB database and 600MB additional database space for temporary working table."),
      'maxmind_bin'           => t("Use MaxMind GeoIP Legacy binary database. Licensed purchased from maxmind.com; you will need to enter your license below. MaxMind GeoIP Legacy's binary database uses \"a custom binary format to maximize lookup speed\", made accessible via their !api. The binary database is roughly 40MB, and will be downloaded and extracted automatically. Lite version binary database can be downloaded !here. The downloaded GeoLiteCity.dat must be uploaded to your server at !path (default) or to your defined custom path.", array('!api' => l(t('custom API'), 'https://github.com/maxmind/geoip-api-php/blob/master/README.md'), '!here' => l(t('here'), 'http://dev.maxmind.com/geoip/legacy/geolite'), '!path' => $private_folder)),
      'maxmindgeoip2_bin'     => t("Use MaxMind GeoIP2 binary database. It is the evolution of MaxMind's original GeoIP binary database or now called GeoIP Legacy. This MaxMind's binary database uses a custom binary format to maximize lookup speed and accessible via two available APIs: !maxmind_db_reader_api which includes an optional C extension that you may install to dramatically increase the performance of lookups in GeoIP2 binary database and the default !geoip2_api. Lite version binary database can be downloaded !here. For licensed version, you will need to enter your license below and the binary database file can be downloaded !here2 (you will need to login to your MaxMind account first). The binary database is roughly 130MB, and there's       an option below to enable the automatic download/extraction of it. The downloaded file %file_lite_city (if Lite version City edition) or %file_licensed_city (if Licensed version City edition) or %file_lite_country (if Lite version Country edition) or %file_licensed_country (if Licensed version Country edition) must be uploaded to your server at !path or to your defined custom path.", array('!maxmind_db_reader_api' => l(t('MaxMind DB Reader PHP API'), 'https://github.com/maxmind/MaxMind-DB-Reader-php'), '!geoip2_api' => l(t('GeoIP2 PHP API'), 'http://maxmind.github.io/GeoIP2-php'), '!here' => l(t('here'), 'https://dev.maxmind.com/geoip/geoip2/geolite2'), '!here2'      => l(t('here'), 'https://www.maxmind.com/en/download_files'), '%file_lite_city' => SMART_IP_MAXMIND_GEOIP2_FILENAME_LITE_CITY, '%file_licensed_city' => SMART_IP_MAXMIND_GEOIP2_FILENAME_LINCENSED_CITY, '%file_lite_country' => SMART_IP_MAXMIND_GEOIP2_FILENAME_LITE_COUNTRY, '%file_licensed_country' => SMART_IP_MAXMIND_GEOIP2_FILENAME_LINCENSED_COUNTRY, '!path' => $private_folder,)),
      'mod_geoip'             => t("Use MaxMind's Apache module mod_geoip... !read_more.", array('!read_more' => l(t('read more'), 'http://dev.maxmind.com/geoip/legacy/mod_geoip2'))),
      'x_header'              => t('Use the X-GeoIP-Country: XX header, set by e.g. nginx'),
      'cf_header'             => t('Use Cloudflare IP Geolocation: your website must be using Cloudflare CDN and "IP Geolocation" option must be enabled at your !settings.', array('!settings' => l(t('Cloudflare settings'), 'https://www.cloudflare.com/cloudflare-settings'))),
    ),
    '#default_value' => variable_get('smart_ip_source', 'ipinfodb_service'),
  );
  
  // Form to select IPInfoDB API version
  $form['smart_ip_data_source_selection']['smart_ip_use_ipinfodb_api_version'] = array(
    '#type'          => 'select',
    '#title'         => t('IPInfoDB API version'),
    '#default_value' => variable_get('smart_ip_use_ipinfodb_api_version', 3),
    '#options'       => array(2 => 2, 3 => 3),
    '#description'   => t('IPInfoDB.com version 2 do have region code, in version 3 it was removed.'),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'ipinfodb_service'),
      ),
    ),
  );
  
  // Form for entering IPInfoDB key
  $form['smart_ip_data_source_selection']['smart_ip_ipinfodb_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('IPInfoDB key'),
    '#description'   => t('The use of IPInfoDB.com service requires API key. Registration for the new API key is free, sign up !here.', array('!here' => l(t('here'), 'http://www.ipinfodb.com/register.php'))),
    '#default_value' => variable_get('smart_ip_ipinfodb_key', ''),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'ipinfodb_service'),
      ),
    ),
  );

  // Form for choosing MaxMind GeoIP Legacy Web Services
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_service'] = array(
    '#type'          => 'select',
    '#title'         => t('MaxMind GeoIP Legacy Web Services'),
    '#description'   => t('Choose type of service.'),
    '#options' => array(
        'country'      => t('Country'),
        'city'         => t('City'),
        'city_isp_org' => t('City/ISP/Organization'),
        'omni'         => t('Insights (formerly Omni)'),
    ),
    '#default_value' => variable_get('smart_ip_maxmind_service', 'country'),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip_service'),
      ),
    ),
  );

  // Form for entering MaxMind GeoIP Legacy Web Services developer key
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP Legacy Web Services developer key'),
    '#description'   => t('The use of geoip.maxmind.com service requires developer key.'),
    '#default_value' => variable_get('smart_ip_maxmind_key', ''),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip_service'),
      ),
    ),
  );

  // Form for choosing MaxMind GeoIP2 Precision Web Services
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip_service'] = array(
    '#type'          => 'select',
    '#title'         => t('MaxMind GeoIP2 Precision Web Services'),
    '#description'   => t('Choose type of service.'),
    '#options' => array(
      'country'  => t('Country'),
      'city'     => t('City'),
      'insights' => t('Insights'),
    ),
    '#default_value' => variable_get('smart_ip_maxmind_geoip_service', 'country'),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_service'),
      ),
    ),
  );

  // Form for MaxMind GeoIP2 Precision user ID
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip_uid'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP2 Precision user ID'),
    '#description'   => t("Enter your MaxMind GeoIP2 Precision account's user ID (view your user ID !here).", array('!here' => l(t('here'), 'https://www.maxmind.com/en/my_license_key'))),
    '#default_value' => variable_get('smart_ip_maxmind_geoip_uid', ''),
    '#size'          => 30,
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_service'),
      ),
    ),
  );
  
  // Form for MaxMind GeoIP2 Precision license key
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip_license'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP2 Precision license key'),
    '#default_value' => variable_get('smart_ip_maxmind_geoip_license', ''),
    '#description'   => t("Enter your MaxMind GeoIP2 Precision account's license key (view your license key !here).", array('!here' => l(t('here'), 'https://www.maxmind.com/en/my_license_key'))),
    '#size'          => 30,
    '#states'      => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_service'),
      ),
    ),
  );

  // Form for choosing IP2Location cache
  $form['smart_ip_data_source_selection']['smart_ip_ip2location_bin_cache'] = array(
    '#type'        => 'select',
    '#title'       => t('IP2Location caching method'),
    '#description' => t('"No cache" - standard lookup with no cache and directly reads from the database file. "Memory cache" - cache the database into memory to accelerate lookup speed and read the whole database into a variable for caching. "Shared memory" - cache whole database into system memory and share among other scripts and websites. Please make sure your system have sufficient RAM if enabling "Memory cache" or "Shared memory".'),
    '#options' => array(
      'no_cache'      => t('No cache'),
      'memory_cache'  => t('Memory cache'),
      'shared_memory' => t('Shared memory'),
    ),
    '#default_value' => variable_get('smart_ip_ip2location_bin_cache', 'no_cache'),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'ip2location_bin'),
      ),
    ),
  );
  
  // Form for entering IP2Location IPV4 binary database file path
  $form['smart_ip_data_source_selection']['smart_ip_ip2location_bin_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('IP2Location binary database IPV4 file full path'),
    '#description'   => t('Define the full path where the IP2Location IPV4 binary database file is located in your server (Note: it is your responsibility to add security on this path. See the online handbook for !security). Include the filename. Eg. /var/www/sites/default/private/smart_ip/IP2LOCATION-LITE-DB11.BIN', array('!security' => l(t('more information about securing private files'), 'https://www.drupal.org/documentation/modules/file'))),
    '#default_value' => variable_get('smart_ip_ip2location_bin_path', ''),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'ip2location_bin'),
      ),
    ),
  );
  
  // Form for entering IP2Location IPV6 binary database file path
  $form['smart_ip_data_source_selection']['smart_ip_ip2location_ipv6_bin_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('IP2Location binary database IPV6 file full path'),
    '#description'   => t('Define the full path where the IP2Location IPV6 binary database file is located in your server (Note: it is your responsibility to add security on this path. See the online handbook for !security). Include the filename. Eg. /var/www/sites/default/private/smart_ip/IP2LOCATION-LITE-DB11.IPV6.BIN', array('!security' => l(t('more information about securing private files'), 'https://www.drupal.org/documentation/modules/file'))),
    '#default_value' => variable_get('smart_ip_ip2location_ipv6_bin_path', ''),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'ip2location_bin'),
      ),
    ),
  );
  
  // Form selection for MaxMind GeoIP Legacy binary database lite or paid version
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_bin_version'] = array(
    '#type'          => 'select',
    '#title'         => t('MaxMind GeoIP Legacy binary database version'),
    '#description'   => t('Select version of MaxMind GeoIP Legacy binary database.'),
    '#options' => array(
       SMART_IP_MAXMIND_BIN_LICENSED_VERSION => t('Licensed'),
       SMART_IP_MAXMIND_BIN_LITE_VERSION => t('Lite'),
     ),
    '#default_value' => variable_get('smart_ip_maxmind_bin_version', SMART_IP_MAXMIND_BIN_LICENSED_VERSION),
    '#states'      => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmind_bin'),
      ),
    ),
  );

  // Form selection for MaxMind GeoIP Legacy binary database edition
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_bin_edition'] = array(
    '#type'          => 'select',
    '#title'         => t('MaxMind GeoIP Legacy binary database edition'),
    '#description'   => t('Select edition of MaxMind GeoIP Legacy binary database.'),
    '#options' => array(
      SMART_IP_MAXMIND_BIN_EDITION_CITY => t('City'),
      SMART_IP_MAXMIND_BIN_EDITION_COUNTRY => t('Country'),
    ),
    '#default_value' => variable_get('smart_ip_maxmind_bin_edition', SMART_IP_MAXMIND_BIN_EDITION_CITY),
    '#states'      => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmind_bin'),
      ),
    ),
  );
  
  // Form for MaxMind GeoIP Legacy binary database license key
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_license'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP Legacy license key'),
    '#default_value' => variable_get('smart_ip_maxmind_license', ''),
    '#description'   => t("Enter your MaxMind GeoIP Legacy account's license key (view your license key !here). This is required for licensed version.", array('!here' => l(t('here'), 'https://www.maxmind.com/en/my_license_key'))),
    '#size'          => 30,
    '#states'      => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmind_bin'),
        ':input[name="smart_ip_maxmind_bin_version"]' => array('value' => SMART_IP_MAXMIND_BIN_LICENSED_VERSION),
      ),
    ),
  );

  // Form to enable automatic MaxMind GeoIP Legacy binary database update every week
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_bin_db_auto_update'] = array(
    '#type'          => 'select',
    '#title'         => t('Automatic MaxMind GeoIP Legacy binary database update'),
    '#description'   => t('MaxMind GeoIP Legacy binary database will be automatically updated via cron.php every Wednesday (for licensed version) and every first Wednesday of the month (for lite or free version). Maxmind updates their database every Tuesday for licensed version and every first Tuesday of the month for lite or free version. !cron must be enabled for this to work.', array('!cron' => l(t('Cron'), 'admin/config/system/cron'))),
    '#options' => array(
       TRUE  => t('Yes'),
       FALSE => t('No'),
     ),
    '#default_value' => variable_get('smart_ip_maxmind_bin_db_auto_update', TRUE),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmind_bin'),
      ),
    ),
  );

  // Form for entering MaxMind GeoIP Legacy binary database custom path
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_bin_custom_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP Legacy binary database custom path'),
    '#description'   => t('Define the path where the MaxMind GeoIP Legacy binary database file is located in your server (Note: it is your responsibility to add security on this path. See the online handbook for !security). Include preceding slash but do not include trailing slash. This is useful for multi Drupal sites with each of their Smart IP module looks to a common MaxMind GeoIP Legacy binary database and it can be also useful for server with installed GeoIPLookup CLI tool where its MaxMind GeoIP Legacy binary database file can be used here (usually its path is located at /usr/share/GeoIP). This path will be ignored if "Automatic MaxMind GeoIP Legacy binary database update" is enabled which uses the Drupal private file system path. Leave it blank if you prefer the default Drupal private file system path.', array('!security' => l(t('more information about securing private files'), 'https://www.drupal.org/documentation/modules/file'))),
    '#default_value' => variable_get('smart_ip_maxmind_bin_custom_path', ''),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmind_bin'),
        ':input[name="smart_ip_maxmind_bin_db_auto_update"]' => array('value' => '0'),
      ),
    ),
  );
  
  // Form selection for MaxMind GeoIP2 binary database lite or paid version
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip2_bin_version'] = array(
    '#type'          => 'select',
    '#title'         => t('MaxMind GeoIP2 binary database version'),
    '#description'   => t('Select version of MaxMind GeoIP2 binary database.'),
    '#options' => array(
       SMART_IP_MAXMIND_BIN_LICENSED_VERSION => t('Licensed'),
       SMART_IP_MAXMIND_BIN_LITE_VERSION => t('Lite'),
     ),
    '#default_value' => variable_get('smart_ip_maxmind_geoip2_bin_version', SMART_IP_MAXMIND_BIN_LICENSED_VERSION),
    '#states' => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_bin'),
      ),
    ),
  );

  // Form selection for MaxMind GeoIP2 binary database edition
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip2_bin_edition'] = array(
    '#type'          => 'select',
    '#title'         => t('MaxMind GeoIP2 binary database edition'),
    '#description'   => t('Select edition of MaxMind GeoIP2 binary database.'),
    '#options' => array(
      SMART_IP_MAXMIND_GEOIP2_BIN_EDITION_CITY => t('City'),
      SMART_IP_MAXMIND_GEOIP2_BIN_EDITION_COUNTRY => t('Country'),
    ),
    '#default_value' => variable_get('smart_ip_maxmind_geoip2_bin_edition', SMART_IP_MAXMIND_GEOIP2_BIN_EDITION_CITY),
    '#states' => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_bin'),
      ),
    ),
  );

  // Form for MaxMind GeoIP2 binary database license key
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip2_license'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP2 license key'),
    '#default_value' => variable_get('smart_ip_maxmind_geoip2_license', ''),
    '#description'   => t("Enter your MaxMind GeoIP2 account's license key (view your license key !here). This is required for licensed version.", array('!here' => l(t('here'), 'https://www.maxmind.com/en/my_license_key'))),
    '#size'          => 30,
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_bin'),
        ':input[name="smart_ip_maxmind_geoip2_bin_version"]' => array('value' => SMART_IP_MAXMIND_BIN_LICENSED_VERSION),
      ),
    ),
  );

  // Form to enable automatic MaxMind GeoIP2 binary database update every week
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip2_bin_db_auto_update'] = array(
    '#type'          => 'select',
    '#title'         => t('Automatic MaxMind GeoIP2 binary database update'),
    '#description'   => t('MaxMind GeoIP2 binary database will be automatically updated via cron.php every Wednesday (for licensed version) and every first Wednesday of the month (for lite or free version). MaxMind GeoIP2 updates their database every Tuesday for licensed version and every first Tuesday of the month for lite or free version. !cron must be enabled for this to work.', array('!cron' => l(t('Cron'), 'admin/config/system/cron'))),
    '#options' => array(
       TRUE  => t('Yes'),
       FALSE => t('No'),
     ),
    '#default_value' => variable_get('smart_ip_maxmind_geoip2_bin_db_auto_update', TRUE),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_bin'),
      ),
    ),
  );

  // Form for entering MaxMind GeoIP2 binary database custom path
  $form['smart_ip_data_source_selection']['smart_ip_maxmind_geoip2_bin_custom_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('MaxMind GeoIP2 binary database custom path'),
    '#description'   => t('Define the path where the MaxMind GeoIP2 binary database file is located in your server (Note: it is your responsibility to add security on this path. See the online handbook for !security). Include preceding slash but do not include trailing slash. This is useful for multi Drupal sites with each of their Smart IP module looks to a common Maxmind binary database and it can be also useful for server with installed GeoIPLookup CLI tool where its Maxmind binary database file can be used here (usually its path is located at /usr/share/GeoIP). This path will be ignored if "Automatic Maxmind binary database update" is enabled which uses the Drupal private file system path. Leave it blank if you prefer the default Drupal private file system path.', array('!security' => l(t('more information about securing private files'), 'https://www.drupal.org/documentation/modules/file'))),
    '#default_value' => variable_get('smart_ip_maxmind_geoip2_bin_custom_path', ''),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'maxmindgeoip2_bin'),
        ':input[name="smart_ip_maxmind_geoip2_bin_db_auto_update"]' => array('value' => '0'),
      ),
    ),
  );
  
  // Form to enable automatic Smart IP database update every one month
  $form['smart_ip_data_source_selection']['smart_ip_auto_update'] = array(
    '#type'          => 'select',
    '#title'         => t('Automatic Smart IP database update'),
    '#description'   => t('Database will be automatically updated via cron.php every one month (Maxmind updates every first day of a month). !cron must be enabled for this to work.', array('!cron' => l(t('Cron'), 'admin/config/system/cron'))),
    '#options' => array(
       TRUE  => t('Yes'),
       FALSE => t('No'),
     ),
    '#default_value' => variable_get('smart_ip_auto_update', TRUE),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_ip_source"]' => array('value' => 'local_db'),
      ),
    ),
  );
  
  // Container for preference forms
  $form['smart_ip_preferences'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Smart IP settings'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );
  
  // Form to select user roles to geolocate
  drupal_add_js(drupal_get_path('module', 'smart_ip') . '/js/smart_ip-admin.js');
  $form['smart_ip_preferences']['smart_ip_roles_to_geolocate'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Roles To Geolocate'),
    '#default_value' => variable_get('smart_ip_roles_to_geolocate', array(DRUPAL_AUTHENTICATED_RID)),
    '#options'       => user_roles(),
    '#description'   => t('Select the roles you wish to geolocate. Note that selecting the anonymous role will add substantial overhead.'),
  );
  
  // Form to turn on save user location in $user object upon registration
  $form['smart_ip_preferences']['smart_ip_save_user_location_creation'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Save user's location details upon creation"),
    '#default_value' => variable_get('smart_ip_save_user_location_creation', TRUE),
    '#description'   => t("One time storing of user's location details upon registration."),
  );
  
  $form['smart_ip_preferences']['smart_ip_allowed_pages'] = array(
    '#title'         => t("Acquire/update user's geolocation on specific Drupal native pages"),
    '#type'          => 'textarea',
    '#rows'          => 5,
    '#description'   => t("Specify pages by using their paths (not the aliased path). Enter one path per line. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. &lt;front&gt; is the front page. Leave blank if all pages."),
    '#default_value' => variable_get('smart_ip_allowed_pages', ''),
  );

  // Container for Smart IP debug tool
  $form['smart_ip_debug_tool'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Smart IP debug tool'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
    '#access' => $can_query_ip,
  );

  // Form to turn on debugging
  $form['smart_ip_debug_tool']['smart_ip_debug'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Admin debug'),
    '#default_value' => variable_get('smart_ip_debug', FALSE),
    '#description'   => t('Enables administrator to spoof an IP Address for debugging purposes.'),
  );

  $form['smart_ip_debug_tool']['smart_ip_test_ip_address'] = array(
    '#type'          => 'textfield',
    '#title'         => t('IP address to use for testing'),
    '#default_value' => variable_get('smart_ip_test_ip_address', ip_address()),
  );
  return system_settings_form($form);
}

/**
 * Validation handler to update the Smart IP database.
 */
function _smart_ip_database_update_validate($form, &$form_state) {
  global $_smart_ip_db_update_access;
  if (variable_get('smart_ip_db_update_busy', FALSE) && !$_smart_ip_db_update_access) {
    form_set_error('smart_ip_update_database', t('Smart IP database update has already been initiated by cron'));
  }
}

/**
 * Submit handler to update the Smart IP database.
 */
function _smart_ip_database_update_submit($form, &$form_state) {
  global $_smart_ip_db_update_access;
  // Utility functions for loading database from external sources
  module_load_include('inc', 'smart_ip', 'includes/smart_ip.utility');
  if ($_smart_ip_db_update_access) {
   $watchdog_msg = 'The Smart IP database has been updated via cron.';
    $_smart_ip_db_update_access = FALSE;
  }
  else {
    $watchdog_msg = 'The Smart IP database has been manually updated.';
  }
  batch_set(smart_ip_update_db_batch(check_url($form_state['values']['smart_ip_csv_source'])));
  watchdog('smart_ip', $watchdog_msg);
  $form_state['storage']['smart_ip_db_message'] = t('The Smart IP database has been updated. @rows rows affected.', array('@rows' => smart_ip_get_count()));
  $form_state['rebuild'] = TRUE;
}

/**
 * Reset Smart IP local database update process.
 */
function _smart_ip_database_reset_update_submit($form, &$form_state) {
  smart_ip_reset_db_update_meta_data();
  drupal_set_message(t('The Smart IP local database update process is now reset.'));
}

/**
 * Submit handler to update the Maxmind binary file.
 */
function _smart_ip_bin_database_update_submit($form, &$form_state) {
  global $_smart_ip_db_update_access;
  if (!$_smart_ip_db_update_access) {
    // Utility functions for loading database from external sources
    module_load_include('inc', 'smart_ip', 'includes/smart_ip.utility');
    // Force the update even if the database is up-to-date
    if ($form_state['values']['smart_ip_source'] == 'maxmind_bin') {
      smart_ip_maxmind_bin_db_update($form_state['values']['smart_ip_maxmind_license'], FALSE, TRUE);
    }
    elseif ($form_state['values']['smart_ip_source'] == 'maxmindgeoip2_bin') {
      smart_ip_maxmind_bin_db_update($form_state['values']['smart_ip_maxmind_geoip2_license'], FALSE, TRUE);
    }
    $form_state['storage']['smart_ip_db_message'] = t('The MaxMind binary database has been updated.');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Submit handler to lookup an IP address in the database.
 *
 * @see _smart_ip_lookup_js()
 */
function _smart_ip_lookup_submit($form, &$form_state) {
  $lookup    = $form_state['values']['smart_ip_lookup'];
  // Return results of manual lookup
  $location = smart_ip_get_location($lookup);
  if (isset($location['latitude']) && isset($location['longitude'])) {
    $message = '<p>' . t('IP Address @ip is assigned to the following location details:', array('@ip' => $lookup)) . '</p><dl>' .
      '<dt>' . t('Country:') . '</dt>' .
      '<dd>' . t('%country', array('%country' => $location['country'])) . '</dd>' .
      '<dt>' . t('Country code:') . '</dt>' .
      '<dd>' . t('%country_code', array('%country_code' => $location['country_code'])) . '</dd>' .
      '<dt>' . t('Region:') . '</dt>' .
      '<dd>' . t('%region', array('%region' => $location['region'])) . '</dd>' .
      '<dt>' . t('City:') . '</dt>' .
      '<dd>' . t('%city', array('%city' => $location['city'])) . '</dd>' .
      '<dt>' . t('Postal code:') . '</dt>' .
      '<dd>' . t('%zip', array('%zip' => $location['zip'])) . '</dd>' .
      '<dt>' . t('Latitude:') . '</dt>' .
      '<dd>' . t('%latitude', array('%latitude' => $location['latitude'])) . '</dd>' .
      '<dt>' . t('Longitude:') . '</dt>' .
      '<dd>' . t('%longitude', array('%longitude' => $location['longitude'])) . '</dd>' .
      '<dt>' . t('Time zone:') . '</dt>' .
      '<dd>' . t('%time_zone', array('%time_zone' => $location['time_zone'])) . '</dd>' .
    '</dl>';
  }
  else {
    $message = t('IP Address @ip is not assigned to any location.', array('@ip' => $lookup));
  }
  $form_state['storage']['smart_ip_message'] = $message;
  $form_state['rebuild'] = TRUE;
}

function smart_ip_admin_settings_validate($form, &$form_state) {
  $list = check_plain($form_state['values']['smart_ip_allowed_pages']);
  $form_state['values']['smart_ip_allowed_pages'] = str_replace('&lt;front&gt;', '<front>', $list);
  
  if ($form_state['values']['smart_ip_source'] == 'local_db') {
    $db_path = drupal_realpath(file_stream_wrapper_uri_normalize('private://smart_ip'));
    if (empty($db_path)) {
      form_set_error('smart_ip_source', t('Please define the "Private file system path" !here.', array('!here' => l(t('here'), 'admin/config/media/file-system'))));
    }
  }
  if ($form_state['values']['smart_ip_source'] == 'mod_geoip' && !isset($_SERVER['GEOIP_COUNTRY_NAME'])) {
    form_set_error('smart_ip_source', t("Maxmind's Apache module mod_geoip is not installed in your server."));
  }
  if ($form_state['values']['smart_ip_source'] == 'x_header' && !isset($_SERVER['HTTP_X_GEOIP_COUNTRY'])) {
    form_set_error('smart_ip_source', t('Your server does not support X-header GeoIP functionality.'));
  }
  if ($form_state['values']['smart_ip_source'] == 'cf_header' && !isset($_SERVER['HTTP_CF_IPCOUNTRY'])) {
    form_set_error('smart_ip_source', t('Your site is not using Cloudflare CDN or your Cloudflare "IP Geolocation" is disabled.'));
  } 
  if ($form_state['values']['smart_ip_source'] == 'ipinfodb_service' && empty($form_state['values']['smart_ip_ipinfodb_key'])) {
    form_set_error('smart_ip_ipinfodb_key', t('Please provide IPInfoDB API key.'));
  }
  $ipinfodb_key = variable_get('smart_ip_ipinfodb_key', '');
  if ($form_state['values']['smart_ip_ipinfodb_key'] != $ipinfodb_key) {
    variable_set('smart_ip_correct_ipinfodb_key', FALSE);
  }
  if ($form_state['values']['smart_ip_source'] == 'ip2location_bin') {
    if (empty($form_state['values']['smart_ip_ip2location_bin_path'])) {
      form_set_error('smart_ip_ip2location_bin_path', t('lease provide the IP2Location binary database IPV4 file full path.'));
    }
    if (empty($form_state['values']['smart_ip_ip2location_ipv6_bin_path'])) {
      form_set_error('smart_ip_ip2location_ipv6_bin_path', t('lease provide the IP2Location binary database IPV6 file full path.'));
    }
    if (!empty($form_state['values']['smart_ip_ip2location_bin_path'])) {
      // Check IP2Location binary database IPV4 file path if valid
      if (!file_exists($form_state['values']['smart_ip_ip2location_bin_path'])) {
        form_set_error('smart_ip_ip2location_bin_path', t('The IP2Location binary database IPV4 file path is not valid.'));
      }
      else {
        // Check IP2Location binary database IPV4 file if valid
        try {
          module_load_include('php', 'smart_ip', 'includes/vendor/autoload');          
          $location_data = new IP2Location\Database($form_state['values']['smart_ip_ip2location_bin_path'], IP2Location\Database::FILE_IO);          
          $record = $location_data->lookup('8.8.8.8', IP2Location\Database::COUNTRY);
          if (strtotime($location_data->getDate()) <= 0) {
            form_set_error('smart_ip_ip2location_bin_path', t('The IP2Location binary database IPV4 file is not valid or corrupted.'));
          }
        }
        catch(Exception $error) {
          form_set_error('smart_ip_ip2location_bin_path', t('The IP2Location binary database IPV4 file is not valid or corrupted.'));
        }
      }
    }
    if (!empty($form_state['values']['smart_ip_ip2location_ipv6_bin_path'])) {
      // Check IP2Location binary database IPV6 file path if valid
      if (!file_exists($form_state['values']['smart_ip_ip2location_ipv6_bin_path'])) {
        form_set_error('smart_ip_ip2location_ipv6_bin_path', t('The IP2Location binary database IPV6 file path is not valid.'));
      }
      else {
        // Check IP2Location binary database IPV6 file if valid
        try {
          module_load_include('php', 'smart_ip', 'includes/vendor/autoload');          
          $location_data = new IP2Location\Database($form_state['values']['smart_ip_ip2location_ipv6_bin_path'], IP2Location\Database::FILE_IO);          
          $record = $location_data->lookup('2001:4860:4860::8888', IP2Location\Database::COUNTRY);
          if (strtotime($location_data->getDate()) <= 0) {
            form_set_error('smart_ip_ip2location_ipv6_bin_path', t('The IP2Location binary database IPV6 file is not valid or corrupted.'));
          }
        }
        catch(Exception $error) {
          form_set_error('smart_ip_ip2location_ipv6_bin_path', t('The IP2Location binary database IPV6 file is not valid or corrupted.'));
        }
      }
    }
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmindgeoip_service' && empty($form_state['values']['smart_ip_maxmind_key'])) {
    form_set_error('smart_ip_maxmind_key', t('Please provide Maxmind GeoIP Legacy Web Services developer key.'));
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmindgeoip2_service' && empty($form_state['values']['smart_ip_maxmind_geoip_uid'])) {
    form_set_error('smart_ip_maxmind_geoip_uid', t('Please enter your Maxmind GeoIP2 Precision Web Services user ID.'));
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmindgeoip2_service' && empty($form_state['values']['smart_ip_maxmind_geoip_license'])) {
    form_set_error('smart_ip_maxmind_geoip_license', t('Please enter your Maxmind GeoIP2 Precision Web Services license key.'));
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmind_bin' && empty($form_state['values']['smart_ip_maxmind_license']) && $form_state['values']['smart_ip_maxmind_bin_version'] == SMART_IP_MAXMIND_BIN_LICENSED_VERSION) {
    form_set_error('smart_ip_maxmind_license', t('Please provide MaxMind GeoIP Legacy license key'));
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmind_bin') {
    $file = smart_ip_get_bin_source_filename($form_state['values']['smart_ip_maxmind_bin_version'], $form_state['values']['smart_ip_maxmind_bin_edition']) . '.dat';
    if (empty($form_state['values']['smart_ip_maxmind_bin_custom_path'])) {
      $db_path = drupal_realpath(file_stream_wrapper_uri_normalize('private://smart_ip'));
      if (empty($db_path)) {
        form_set_error('smart_ip_maxmind_bin_version', t('Please define the "Private file system path" !here.', array('!here' => l(t('here'), 'admin/config/media/file-system'))));
      }
      elseif (!file_exists("$db_path/$file") && !$form_state['values']['smart_ip_maxmind_bin_db_auto_update']) {
        form_set_error('smart_ip_maxmind_bin_db_auto_update', t('Please upload the @file at !path.', array(
          '@file' => $file,
          '!path' => $db_path,
        )));
      }
    }
    else {
      $db_path = $form_state['values']['smart_ip_maxmind_bin_custom_path'];
      if (!file_exists("$db_path/$file")) {
        form_set_error('smart_ip_maxmind_bin_custom_path', t('Please upload the @file at !path.', array(
          '@file' => $file,
          '!path' => $db_path,
        )));
      }
    }
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmindgeoip2_bin' && empty($form_state['values']['smart_ip_maxmind_geoip2_license']) && $form_state['values']['smart_ip_maxmind_geoip2_bin_version'] == SMART_IP_MAXMIND_BIN_LICENSED_VERSION) {
    form_set_error('smart_ip_maxmind_geoip2_license', t('Please provide MaxMind GeoIP2 license key'));
  }
  if ($form_state['values']['smart_ip_source'] == 'maxmindgeoip2_bin') {
    $file = smart_ip_get_geoip2_bin_source_filename($form_state['values']['smart_ip_maxmind_geoip2_bin_version'], $form_state['values']['smart_ip_maxmind_geoip2_bin_edition']);
    if (empty($form_state['values']['smart_ip_maxmind_geoip2_bin_custom_path'])) {
      $db_path = drupal_realpath(file_stream_wrapper_uri_normalize('private://smart_ip'));
      if (empty($db_path)) {
        form_set_error('smart_ip_maxmind_geoip2_bin_version', t('Please define the "Private file system path" !here.', array('!here' => l(t('here'), 'admin/config/media/file-system'))));
      }
      elseif (!file_exists("$db_path/$file") && !$form_state['values']['smart_ip_maxmind_geoip2_bin_db_auto_update']) {
        form_set_error('smart_ip_maxmind_geoip2_bin_db_auto_update', t('Please upload the @file at !path.', array(
          '@file' => $file,
          '!path' => $db_path,
        )));
      }
    }
    else {
      $db_path = $form_state['values']['smart_ip_maxmind_geoip2_bin_custom_path'];
      if (!file_exists("$db_path/$file")) {
        form_set_error('smart_ip_maxmind_geoip2_bin_custom_path', t('Please upload the @file at !path.', array(
          '@file' => $file,
          '!path' => $db_path,
        )));
      }
    }
  }
  if ($form_state['values']['smart_ip_debug'] == TRUE && empty($form_state['values']['smart_ip_test_ip_address'])) {
    form_set_error('smart_ip_test_ip_address', t('Please enter the IP address to use for testing.'));
  }  
}

/**
 * AJAX callback to lookup an IP address in the database.
 *
 * @see _smart_ip_lookup_submit()
 */
function _smart_ip_lookup_js($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_remove('#lookup-message');
  $commands[] = ajax_command_after('#edit-smart-ip-lookup-button', '<div id="lookup-message" class="messages"></div>');
  $commands[] = ajax_command_html('#lookup-message', $form_state['storage']['smart_ip_message']);
  return array('#type' => 'ajax', '#commands' => $commands);  
}

/**
 * Process Forms submitted by IP to Country administration page
 */
function smart_ip_admin_settings_submit($form, &$form_state) {
  global $user;

  // Save the roles to geolocate
  $roles_to_geolocate = array();
  foreach ($form_state['values']['smart_ip_roles_to_geolocate'] as $rid => $value) {
    if($rid == $value){
      $roles_to_geolocate[] = $rid;
    }
  }
  variable_set('smart_ip_roles_to_geolocate', $roles_to_geolocate);
  // Exclude unnecessary elements from being saved in variable table
  unset(
    $form_state['values']['smart_ip_lookup'], 
    $form_state['values']['smart_ip_csv_source'],
    $form_state['values']['smart_ip_roles_to_geolocate']
  );

  // Download the MaxMind GeoIP Legacy binary database if necessary before proceeding
  if ($form_state['values']['smart_ip_source'] == 'maxmind_bin') {
    module_load_include('inc', 'smart_ip', 'includes/smart_ip.utility');
    smart_ip_maxmind_bin_db_update($form_state['values']['smart_ip_maxmind_license']);
  }

  // Download the MaxMind GeoIP2 binary database if necessary before proceeding
  if ($form_state['values']['smart_ip_source'] == 'maxmindgeoip2_bin') {
    module_load_include('inc', 'smart_ip', 'includes/smart_ip.utility');
    smart_ip_maxmind_bin_db_update($form_state['values']['smart_ip_maxmind_geoip2_license']);
  }

  // Check to see if debug set
  if ($form_state['values']['smart_ip_debug']) {
    // Debug on
    $ip = $form_state['values']['smart_ip_test_ip_address'];
    $smart_ip_session = smart_ip_session_get('smart_ip');
    $smart_ip_session['debug_mode'] = TRUE;
    smart_ip_session_set('smart_ip', $smart_ip_session);
    $location = smart_ip_get_location($ip);
    // Immediately set the debug mode
    variable_set('smart_ip_debug', TRUE);
    $type = t('debug');
  }
  else {
    // Debug off - make sure we set/reset to their real values
    $location = smart_ip_get_current_visitor_location_data();
    $ip   = $location['ip_address'];
    $type = t('actual');
  }
  if ($location) {
    drupal_set_message(t('Using @type IP: %ip / Country: %country / Region: %region / City: %city / Postal code: %zip / Longitude: %long / Latitude: %lat / Time zone: %time_zone', array(
      '@type'      => $type,
      '%ip'        => $ip, 
      '%country'   => isset($location['country']) ? $location['country'] : '',
      '%region'    => isset($location['region']) ? $location['region'] : '',
      '%city'      => isset($location['city']) ? $location['city'] : '',
      '%zip'       => isset($location['zip']) ? $location['zip'] : '',
      '%long'      => isset($location['longitude']) ? $location['longitude'] : '',
      '%lat'       => isset($location['latitude']) ? $location['latitude'] : '',
      '%time_zone' => isset($location['time_zone']) ? $location['time_zone'] : '',
    )));
    $smart_ip_session = smart_ip_session_get('smart_ip');
    // Finally if it has been determined, store the location in the $user object
    if ($form_state['values']['smart_ip_debug']) {
      smart_ip_set_user_data($user, $location);
      $smart_ip_session['location'] = $location;
      smart_ip_session_set('smart_ip', $smart_ip_session);
      if (module_exists('device_geolocation')) {
        smart_ip_session_set('device_geolocation', NULL);
        device_geolocation_get_coordinates();
      }
    }
    else {
      if (isset($smart_ip_session['debug_mode']) && !is_null($smart_ip_session['debug_mode'])) {
        unset($user->data['geoip_location'], $smart_ip_session['debug_mode']);
        $smart_ip_session['location'] = $location;
        smart_ip_session_set('device_geolocation', NULL);
        smart_ip_session_set('smart_ip', $smart_ip_session);
        smart_ip_set_user_data($user, $location);
        if (module_exists('device_geolocation')) {
          device_geolocation_get_coordinates();
        }
      }
      else {
        smart_ip_set_session_data($location);
        smart_ip_set_user_data($user, $smart_ip_session['location']);
      }
    }
  }
  else {
    drupal_set_message(t('IP Address @ip is not assigned to any location.', array('@ip' => $ip)));
  }
}