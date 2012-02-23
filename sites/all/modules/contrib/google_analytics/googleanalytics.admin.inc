<?php

/**
 * @file
 * Administrative page callbacks for the googleanalytics module.
 */

/**
 * Implementation of hook_admin_settings() for configuring the module
 */
function googleanalytics_admin_settings_form(&$form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['account']['googleanalytics_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Web Property ID'),
    '#default_value' => variable_get('googleanalytics_account', 'UA-'),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('This ID is unique to each site you want to track separately, and is in the form of UA-xxxxxxx-yy. To get a Web Property ID, <a href="@analytics">register your site with Google Analytics</a>, or if you already have registered your site, go to your Google Analytics Settings page to see the ID next to every site profile. <a href="@webpropertyid">Find more information in the documentation</a>.', array('@analytics' => 'http://www.google.com/analytics/', '@webpropertyid' => 'http://code.google.com/apis/analytics/docs/concepts/gaConceptsAccounts.html#webProperty')),
  );

  $form['domain_tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Domains'),
  	'#collapsible' => TRUE,
  );

  global $cookie_domain;
  $multiple_sub_domains = array();
  foreach (array('www', 'app', 'shop') as $subdomain) {
    if (count(explode('.', $cookie_domain)) > 2 && !is_numeric(str_replace('.', '', $cookie_domain))) {
      $multiple_sub_domains[] = $subdomain . $cookie_domain;
    }
    // IP addresses or localhost.
    else {
      $multiple_sub_domains[] = $subdomain . '.example.com';
    }
  }

  $form['domain_tracking']['googleanalytics_domain_mode'] = array(
    '#type' => 'radios',
    '#title' => t('What are you tracking?'),
    '#options' => array(
      0 => t('A single domain (default)') . '<div class="description">' . t('Domain: @domain', array('@domain' => $_SERVER['HTTP_HOST'])) . '</div>',
      1 => t('One domain with multiple subdomains') . '<div class="description">' . t('Examples: @domains', array('@domains' => implode(', ', $multiple_sub_domains))) . '</div>',
    ),
    '#default_value' => variable_get('googleanalytics_domain_mode', 0),
  );

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('googleanalytics_visibility', 0);
  $pages = variable_get('googleanalytics_pages', GOOGLEANALYTICS_PAGES);

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['googleanalytics_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['googleanalytics_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
      '#wysiwyg' => FALSE,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific tracking settings'),
    '#collapsible' => TRUE,
  );

  $form['role_vis_settings']['googleanalytics_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('googleanalytics_visibility_roles', 0),
  );

  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings']['googleanalytics_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('googleanalytics_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Standard tracking configurations.
  $form['user_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('User specific tracking settings'),
    '#collapsible' => TRUE,
  );
  $form['user_vis_settings']['googleanalytics_custom'] = array(
    '#type' => 'radios',
    '#title' => t('Custom tracking settings'),
    '#options' => array(
      t('Users cannot control whether they are tracked or not.'),
      t('Track users by default, but let individual users to opt out.'),
      t('Do not track users by default, but let individual users to opt in.')
    ),
    '#description' => t('Allow individual users to customize the visibility of tracking in their account settings. Only users with %permission permission are allowed to set their own preference.', array('%permission' => t('opt-in or out of tracking'))),
    '#default_value' => variable_get('googleanalytics_custom', 0),
  );

  // Link specific configurations.
  $form['linktracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['linktracking']['googleanalytics_trackoutgoing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track outbound links'),
    '#default_value' => variable_get('googleanalytics_trackoutgoing', 1),
    '#description' => t('Enables tracking of clicks on outgoing links.')
  );
  $form['linktracking']['googleanalytics_trackmailto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track mailto links'),
    '#default_value' => variable_get('googleanalytics_trackmailto', 1),
    '#description' => t('Enables tracking of clicks on mailto links.')
  );
  $form['linktracking']['googleanalytics_trackfiles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track download links'),
    '#default_value' => variable_get('googleanalytics_trackfiles', 1),
    '#description' => t('Enables tracking of clicks on links to files based on the file extensions list below.')
  );
  $form['linktracking']['googleanalytics_trackfiles_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('File extensions to track'),
    '#default_value' => variable_get('googleanalytics_trackfiles_extensions', GOOGLEANALYTICS_TRACKFILES_EXTENSIONS),
    '#description' => t('A pipe separated list of file extensions that should be tracked when clicked with regular expression support. Example: !extensions', array('!extensions' => GOOGLEANALYTICS_TRACKFILES_EXTENSIONS)),
    '#maxlength' => 255,
  );
  $form['linktracking']['googleanalytics_trackoutboundaspageview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track clicks on outbound links as page views'),
    '#default_value' => variable_get('googleanalytics_trackoutboundaspageview', 0),
    '#description' => t('By default outbound links are tracked as <em>Events</em>. In rare situations like tracking of <em>Goals</em> it may be required to track outbound clicks as page views.'),
  );

  // Privacy specific configurations.
  $form['tracking']['privacy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privacy'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tracking']['privacy']['googleanalytics_tracker_anonymizeip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Anonymize visitors IP address'),
    '#description' => t('Tell Google Analytics to anonymize the information sent by the tracker objects by removing the last octet of the IP address prior to its storage. Note that this will slightly reduce the accuracy of geographic reporting. In some countries it is not allowed to collect personally identifying information for privacy reasons and this setting may help you to comply with the local laws.'),
    '#default_value' => variable_get('googleanalytics_tracker_anonymizeip', 0),
  );
  $form['tracking']['privacy']['googleanalytics_privacy_donottrack'] = array(
    '#type' => 'checkbox',
    '#title' => t('Universal web tracking opt-out'),
    '#description' => t('If enabled and your server receives the <a href="http://donottrack.us/">Do-Not-Track</a> header from the client browser, the Google Analytics module will not embed any tracking code into your site. Compliance with Do Not Track could be purely voluntary, enforced by industry self-regulation, or mandated by state or federal law. Please accept your visitors privacy. If they have opt-out from tracking and advertising, you should accept their personal decision. This feature is currently limited to logged in users and disabled page caching.'),
    '#default_value' => variable_get('googleanalytics_privacy_donottrack', 1),
  );

  // Backward compatibility only.
  // TODO: If currently not in use, hide the UI and later remove the code.
  $segmentation = variable_get('googleanalytics_segmentation', array());

  if (!empty($segmentation) || variable_get('googleanalytics_segmentation_DEPRECATED', FALSE)) {
    $profile_enabled = module_exists('profile');
    $form['segmentation'] = array(
      '#type' => 'fieldset',
      '#title' => t('User segmentation settings'),
      '#collapsible' => TRUE,
    );

    // Compile a list of fields to show.
    $fields = variable_get('googleanalytics_segmentation_default_fields', array('roles' => t('User roles')));
    if ($profile_enabled) {
      $result = db_query('SELECT name, title FROM {profile_fields} ORDER BY weight');
      while ($record = db_fetch_object($result)) {
        $fields[$record->name] = $record->title;
      }
    }

    $form['segmentation']['googleanalytics_segmentation'] = array(
      '#type' => 'select',
      '#title' => t('Add segmentation information to tracking code') . ' <span class="admin-disabled">***DEPRECATED***</span>',
      '#description' => t('Segment users based on different properties, additionally to the basic IP address based tracking provided by Google Analytics.') . ' <span class="admin-disabled">' . t('For most situations, Google recommend that you use Custom Variables to segment your visitors.') . '</span>' . (!$profile_enabled ? ' '. t('<a href="@module_list">Enable the profile module</a> to be able to use profile fields for more granular tracking.', array('@module_list' => url('admin/build/modules'))) : '') .' '. t('Make sure you will not associate (or permit any third party to associate) any data gathered from Your Website(s) (or such third parties\' website(s)) with any personally identifying information from any source as part of Your use (or such third parties\' use) of the Google Analytics service. For more information see section 8.1 in the <a href="@ga_tos">Google Analytics terms of use</a>.', array('@ga_tos' => 'http://www.google.com/analytics/en-GB/tos.html')) .' '. t('You can select multiple values.'),
      '#default_value' => $segmentation,
      '#options' => $fields,
      '#size' => count($fields)>3 ? 10 : 3,
      '#multiple' => TRUE
    );
  }

  $form['googleanalytics_custom_var'] = array(
  	'#collapsed' => TRUE,
  	'#collapsible' => TRUE,
    '#description' => t('You can add Google Analytics <a href="!custom_var_documentation">Custom Variables</a> here. These will be added to every page that Google Analytics tracking code appears on. Google Analytics will only accept custom variables if the <em>name</em> and <em>value</em> combined are less than 64 bytes after URL encoding. Keep the names as short as possible and expect long values to get trimmed. You may use tokens in custom variable values. Global and user tokens are always available; on node pages, node tokens are also available.', array('!custom_var_documentation' => 'http://code.google.com/intl/en/apis/analytics/docs/tracking/gaTrackingCustomVariables.html')),
    '#theme' => 'googleanalytics_admin_custom_var_table',
    '#title' => t('Custom variables'),
    '#tree' => TRUE,
    '#type' => 'fieldset',
  );

  $token_enabled = module_exists('token');
  $googleanalytics_custom_vars = variable_get('googleanalytics_custom_var', array());

  // Google Analytics supports up to 5 custom variables.
  for ($i = 1; $i < 6; $i++) {
    // TODO: '#default_value' is currently broken, see http://drupal.org/node/410926.
    $form['googleanalytics_custom_var']['slots'][$i]['slot'] = array(
      //'#attributes' => array('readonly' => 'readonly'),
      //'#default_value' => $i,
      '#description' => t('Slot number'),
      '#disabled' => TRUE,
      '#size' => 1,
      '#type' => 'textfield',
      '#value' => $i,
    );
    $form['googleanalytics_custom_var']['slots'][$i]['name'] = array(
      '#default_value' => !empty($googleanalytics_custom_vars['slots'][$i]['name']) ? $googleanalytics_custom_vars['slots'][$i]['name'] : '',
      '#description' => t('The custom variable name.'),
      '#size' => 20,
      '#type' => 'textfield',
    );
    $form['googleanalytics_custom_var']['slots'][$i]['value'] = array(
      '#default_value' => !empty($googleanalytics_custom_vars['slots'][$i]['value']) ? $googleanalytics_custom_vars['slots'][$i]['value'] : '',
      '#description' => ($token_enabled ? t('The custom variable value. You may use tokens in this field.') : t('The custom variable value.')),
      '#type' => 'textfield',
    );
    if ($token_enabled) {
      $form['googleanalytics_custom_var']['slots'][$i]['value']['#element_validate'][] = 'googleanalytics_token_element_validate';
      $form['googleanalytics_custom_var']['slots'][$i]['value']['#element_validate'][] = 'token_element_validate';
      $form['googleanalytics_custom_var']['slots'][$i]['value']['#token_types'][] = 'node';
      $form['googleanalytics_custom_var']['slots'][$i]['value']['#token_types'][] = 'user';
    }
    $form['googleanalytics_custom_var']['slots'][$i]['scope'] = array(
      '#default_value' => !empty($googleanalytics_custom_vars['slots'][$i]['scope']) ? $googleanalytics_custom_vars['slots'][$i]['scope'] : 3,
      '#description' => t('The scope for the custom variable.'),
      '#type' => 'select',
      '#options' => array(
        1 => t('Visitor'),
        2 => t('Session'),
        3 => t('Page'),
      ),
    );
  }

  $form['googleanalytics_custom_var']['googleanalytics_custom_var_description'] = array(
    '#type' => 'item',
    '#description' => t('You can supplement Google Analytics\' basic IP address tracking of visitors by segmenting users based on custom variables.') . (!$token_enabled ? ' '. t('<a href="@module_list">Enable the token module</a> to be able to use tokens for more granular tracking.', array('@module_list' => url('admin/build/modules'))) : '') .' '. t('Section 8.1 of the <a href="@ga_tos">Google Analytics terms of use</a> requires you to make sure you will not associate (or permit any third party to associate) any data gathered from your websites (or such third parties\' websites) with any personally identifying information from any source as part of your use (or such third parties\' use) of the Google Analytics\' service.', array('@ga_tos' => 'http://www.google.com/analytics/en-GB/tos.html')),
  );
  $form['googleanalytics_custom_var']['googleanalytics_custom_var_token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node', 'user'),
  );

  // Advanced feature configurations.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['googleanalytics_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Locally cache tracking code file'),
    '#description' => t("If checked, the tracking code file is retrieved from Google Analytics and cached locally. It is updated daily from Google's servers to ensure updates to tracking code are reflected in the local copy. Do not activate this until after Google Analytics has confirmed that site tracking is working!"),
    '#default_value' => variable_get('googleanalytics_cache', 0),
  );
  if (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PRIVATE) {
    $form['advanced']['googleanalytics_cache']['#disabled'] = TRUE;
    $form['advanced']['googleanalytics_cache']['#description'] .= ' '. t('<a href="@url">Public file transfers</a> must be enabled to allow local caching.', array('@url' => url('admin/settings/file-system', array('query' => drupal_get_destination()))));
  }

  // Allow for tracking of the originating node when viewing translation sets.
  if (module_exists('translation')) {
    $form['advanced']['googleanalytics_translation_set'] = array(
      '#type' => 'checkbox',
      '#title' => t('Track translation sets as one unit'),
      '#description' => t('When a node is part of a translation set, record statistics for the originating node instead. This allows for a translation set to be treated as a single unit.'),
      '#default_value' => variable_get('googleanalytics_translation_set', 0),
    );
  }

  $site_search_dependencies = '<div class="admin-dependencies">';
  $site_search_dependencies .= t('Depends on: !dependencies', array('!dependencies' => (module_exists('search') ? t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => 'Search')) : t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => 'Search')))));
  $site_search_dependencies .= '</div>';

  // Google already have many translations, if not - they display a note to change the language.
  global $language;
  $form['advanced']['googleanalytics_site_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track internal search'),
    '#description' => t('If checked, internal search keywords are tracked. You must configure your Google account to use the internal query parameter <strong>search</strong>. For more information see <a href="@url">How do I set up Site Search for my profile</a>.', array('@url' => 'http://www.google.com/support/analytics/bin/answer.py?hl='. $language->language .'&answer=75817')) . $site_search_dependencies,
    '#default_value' => variable_get('googleanalytics_site_search', FALSE),
    '#disabled' => (module_exists('search') ? FALSE : TRUE),
  );

  $form['advanced']['googleanalytics_trackadsense'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track AdSense ads'),
    '#description' => t('If checked, your AdSense ads will be tracked in your Google Analytics account.'),
    '#default_value' => variable_get('googleanalytics_trackadsense', FALSE),
  );

  $form['advanced']['codesnippet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom JavaScript code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('You can add custom Google Analytics <a href="@snippets">code snippets</a> here. These will be added to every page that Google Analytics appears on. Before you add custom code to the below textarea\'s you should read <a href="@ga_concepts_overview">Google Analytics Tracking Code - Functional Overview</a> and the <a href="@ga_js_api">Google Analytics Tracking API</a> documentation. <strong>Do not include the &lt;script&gt; tags</strong>, and always end your code with a semicolon (;).', array('@snippets' => 'http://drupal.org/node/248699', '@ga_concepts_overview' => 'http://code.google.com/apis/analytics/docs/gaConceptsOverview.html', '@ga_js_api' => 'http://code.google.com/apis/analytics/docs/gaJSApi.html'))
  );
  $form['advanced']['codesnippet']['googleanalytics_codesnippet_before'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (before)'),
    '#default_value' => variable_get('googleanalytics_codesnippet_before', ''),
    '#rows' => 5,
    '#wysiwyg' => FALSE,
    '#description' => t("Code in this textarea will be added <strong>before</strong> _gaq.push(['_trackPageview'])."),
  );
  $form['advanced']['codesnippet']['googleanalytics_codesnippet_after'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (after)'),
    '#default_value' => variable_get('googleanalytics_codesnippet_after', ''),
    '#rows' => 5,
    '#wysiwyg' => FALSE,
    '#description' => t("Code in this textarea will be added <strong>after</strong> _gaq.push(['_trackPageview']). This is useful if you'd like to track a site in two accounts."),
  );

  /* hook_footer() is not able to add code to head. Upgrade to D7 required.
  $form['advanced']['googleanalytics_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#description' => t("Google recommends adding the external JavaScript files to header for performance reasons."),
    '#options' => array(
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    '#default_value' => variable_get('googleanalytics_js_scope', 'header'),
  ); */

  return system_settings_form($form);
}

function googleanalytics_admin_settings_form_validate($form, &$form_state) {
  // Custom variables validation.
  foreach ($form_state['values']['googleanalytics_custom_var']['slots'] as $custom_var) {
    // Validate empty names/values.
    if (empty($custom_var['name']) && !empty($custom_var['value'])) {
      form_set_error("googleanalytics_custom_var][slots][" . $custom_var['slot'] . "][name", t('The custom variable @slot-number requires a <em>Value</em> if a <em>Name</em> has been provided.', array('@slot-number' =>  $custom_var['slot'])));
    }
    elseif (!empty($custom_var['name']) && empty($custom_var['value'])) {
      form_set_error("googleanalytics_custom_var][slots][" . $custom_var['slot'] . "][value", t('The custom variable @slot-number requires a <em>Name</em> if a <em>Value</em> has been provided.', array('@slot-number' =>  $custom_var['slot'])));
    }

    // If token module is not available, block tokens with personally identifying
    // information with an "ugly" value check. Normally the values are validated
    // via '#element_validate' functions that are not available without token module.
    if (!module_exists('token') && _googleanalytics_contains_forbidden_token($custom_var['value'])) {
      form_set_error("googleanalytics_custom_var][slots][" . $custom_var['slot'] . "][value", t('The custom variable %element-title is using forbidden tokens with personal identifying information.', array('%element-title' => $custom_var['name'] ? $custom_var['name'] : $custom_var['slot'])));
    }
  }

  // Trim some text values.
  $form_state['values']['googleanalytics_account'] = trim($form_state['values']['googleanalytics_account']);
  $form_state['values']['googleanalytics_pages'] = trim($form_state['values']['googleanalytics_pages']);
  $form_state['values']['googleanalytics_codesnippet_before'] = trim($form_state['values']['googleanalytics_codesnippet_before']);
  $form_state['values']['googleanalytics_codesnippet_after'] = trim($form_state['values']['googleanalytics_codesnippet_after']);

  if (!preg_match('/^UA-\d{4,}-\d+$/', $form_state['values']['googleanalytics_account'])) {
    form_set_error('googleanalytics_account', t('A valid Google Analytics Web Property ID is case sensitive and formatted like UA-xxxxxxx-yy.'));
  }

  // Clear obsolete local cache if cache has been disabled.
  if (empty($form_state['values']['googleanalytics_cache']) && $form['advanced']['googleanalytics_cache']['#default_value']) {
    googleanalytics_clear_js_cache();
  }

  // This is for the Newbie's who cannot read a text area description.
  if (stristr($form_state['values']['googleanalytics_codesnippet_before'], 'google-analytics.com/ga.js')) {
    form_set_error('googleanalytics_codesnippet_before', t('Do not add the tracker code provided by Google into the javascript code snippets! This module already builds the tracker code based on your Google Analytics account number and settings.'));
  }
  if (stristr($form_state['values']['googleanalytics_codesnippet_after'], 'google-analytics.com/ga.js')) {
    form_set_error('googleanalytics_codesnippet_after', t('Do not add the tracker code provided by Google into the javascript code snippets! This module already builds the tracker code based on your Google Analytics account number and settings.'));
  }
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['googleanalytics_codesnippet_before'])) {
    form_set_error('googleanalytics_codesnippet_before', t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['googleanalytics_codesnippet_after'])) {
    form_set_error('googleanalytics_codesnippet_after', t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }
}

/**
 * Layout for the custom variables table in the admin settings form.
 */
function theme_googleanalytics_admin_custom_var_table($form) {

  $header = array(
    array('data' => t('Slot')),
    array('data' => t('Name')),
    array('data' => t('Value')),
    array('data' => t('Scope')),
  );

  $rows = array();
  foreach (element_children($form['slots']) as $key => $id) {
    $rows[] = array(
      'data' => array(
        drupal_render($form['slots'][$id]['slot']),
        drupal_render($form['slots'][$id]['name']),
        drupal_render($form['slots'][$id]['value']),
        drupal_render($form['slots'][$id]['scope']),
      ),
    );
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form['googleanalytics_custom_var_description']);
  $output .= drupal_render($form['googleanalytics_custom_var_token_tree']);

  return $output;
}

/**
 * Validate a form element that should have tokens in it.
 *
 * For example:
 * @code
 * $form['my_node_text_element'] = array(
 *   '#type' => 'textfield',
 *   '#title' => t('Some text to token-ize that has a node context.'),
 *   '#default_value' => 'The title of this node is [node:title].',
 *   '#element_validate' => array('googleanalytics_token_element_validate'),
 * );
 * @endcode
 */
function googleanalytics_token_element_validate(&$element, &$form_state) {
  $value = isset($element['#value']) ? $element['#value'] : $element['#default_value'];

  if (!drupal_strlen($value)) {
    // Empty value needs no further validation since the element should depend
    // on using the '#required' FAPI property.
    return $element;
  }

  $tokens = token_scan($value);
  $title = empty($element['#title']) ? $element['#parents'][0] : $element['#title'];

  $invalid_tokens = _googleanalytics_get_forbidden_tokens($tokens);
  if ($invalid_tokens) {
    form_error($element, t('The %element-title is using the following forbidden tokens with personal identifying information: @invalid-tokens.', array('%element-title' => $title, '@invalid-tokens' => implode(', ', $invalid_tokens))));
  }

  return $element;
}

function _googleanalytics_get_forbidden_tokens($value) {
  $invalid_tokens = array();
  $value_tokens = is_string($value) ? token_scan($value) : $value;

  // For porting/compatibility reasons with _googleanalytics_contains_forbidden_token()
  // the leading and trailing token separator need to be added to every value.
  $value_tokens = token_prepare_tokens($value_tokens);

  foreach ($value_tokens as $token) {
    // If token is forbidden, add it to invalid tokens array.
    if (_googleanalytics_contains_forbidden_token($token)) {
      $invalid_tokens[] = $token;
    }
  }

  array_unique($invalid_tokens);
  return $invalid_tokens;
}

/**
 * Validate if a string contains forbidden tokens not allowed by privacy rules.
 *
 * @param $token_string
 *   A string with one or more tokens to be validated.
 * @return boolean
 *   TRUE if blacklisted token has been found, otherwise FALSE.
 */
function _googleanalytics_contains_forbidden_token($token_string) {
  // List of strings in tokens with personal identifying information not allowed
  // for privacy reasons. See section 8.1 of the Google Analytics terms of use
  // for more detailed information.
  //
  // This list can never ever be complete. For this reason it tries to use a
  // regex and may kill a few other valid tokens, but it's the only way to
  // protect users as much as possible from admins with illegal ideas.
  //
  // User tokens are not prefixed with colon to catch 'current-user' and 'user'.
  //
  // TODO: If someone have better ideas, share them, please!
  $token_blacklist = array(
    'author-uid]',
    'author-name',
    'author-mail',
    'author-homepage]',
    '[user-name]',
    '[user-id]',
    '[user-mail]',
    // [user] tokens
    '[user]',
    '[user-raw]',
    '[uid]',
    '[mail]',
    '[account-url]',
    '[account-edit]',
  );

  return preg_match('/' . implode('|', array_map('preg_quote', $token_blacklist)) . '/i', $token_string);
}
