<?php
// $Id: yahoomusic.inc,v 1.1.2.4 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 *   This include processes Yahoo Music API media files for use by emfield.module.
 */

define('EMVIDEO_YAHOOMUSIC_MAIN_URL', 'http://new.music.yahoo.com/');
define('EMVIDEO_YAHOOMUSIC_API_INFO', 'http://developer.yahoo.com/music/api_guide/');
define('EMVIDEO_YAHOOMUSIC_API_APPLICATION_URL', 'http://developer.yahoo.com/music/');
define('EMVIDEO_YAHOOMUSIC_BGCOLOR_DEFAULT', '#000000');
define('EMVIDEO_YAHOOMUSIC_DATA_VERSION', 1);

/**
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emvideo_yahoomusic_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'yahoomusic',
    'name' => t('Yahoo! Music'),
    'url' => EMVIDEO_YAHOOMUSIC_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@yahoomusic" target="_blank">Yahoo Music</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@yahoomusic' => EMVIDEO_YAHOOMUSIC_MAIN_URL, '@api' => EMVIDEO_YAHOOMUSIC_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['yahoomusic'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_yahoomusic_settings() {
  $form['yahoomusic']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yahoo! Music API'),
    '#description' => t('You will first need to apply for a Yahoo App ID from the <a href="@yahoomusic" target="_blank">Yahoo! Developer Network</a>.', array('@yahoomusic' => EMVIDEO_YAHOOMUSIC_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['yahoomusic']['api']['emvideo_yahoomusic_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo! App ID'),
    '#default_value' => variable_get('emvideo_yahoomusic_app_id', ''),
    '#description' => t('Please enter your Yahoo! App ID here.'),
  );
  $form['yahoomusic']['api']['emvideo_yahoomusic_locale'] = array(
    '#type' => 'select',
    '#title' => t('Locale'),
    '#default_value' => variable_get('emvideo_yahoomusic_locale','us'),
    '#options' => array(
      'us' => t('United States'),
      'e1' => t('United States (Español)'),
      'ca' => t('Canada'),
      'mx' => t('Mexico'),
      'au' => t('Australia'),
      'nz' => t('New Zealand'),
      'uk' => t('United Kingdom'),
      'de' => t('Germany'),
      'es' => t('Spain'),
      'it' => t('Italy'),
      'fr' => t('France'),
    ),
    '#description' => t('If your application is designed for use by a web site or service in a particular country, Yahoo! Music requires that you use the locale for that country.'),
  );
  $form['yahoomusic']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => variable_get('emvideo_yahoomusic_full_screen', 1),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_close_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Close'),
    '#default_value' => variable_get('emvideo_yahoomusic_close_enable', 0),
    '#description' => t('Sets visibility of the close button; button action will throw a callback event of type "close".'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_controls_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Controls'),
    '#default_value' => variable_get('emvideo_yahoomusic_controls_enable', 1),
    '#description' => t('Enables the player controls.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_info_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Info'),
    '#default_value' => variable_get('emvideo_yahoomusic_info_enable', 1),
    '#description' => t('Sets visibility of the "more info" button.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_now_playing_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Intro'),
    '#default_value' => variable_get('emvideo_yahoomusic_now_playing_enable', 1),
    '#description' => t('Enables the intro "Now Playing: ..." component during the first few seconds of playback.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_post_panel_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Post Panel'),
    '#default_value' => variable_get('emvideo_yahoomusic_post_panel_enable', 1),
    '#description' => t('Enables the post panel; displayed after a clip completes playback.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_pre_panel_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Pre Panel'),
    '#default_value' => variable_get('emvideo_yahoomusic_pre_panel_enable', 1),
    '#description' => t('Enables the pre meta panel on player initialization when autoStart is disabled and a valid id has been passed into the player.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_share_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Share'),
    '#default_value' => variable_get('emvideo_yahoomusic_share_enable', 1),
    '#description' => t('Enables the Share panel.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background Color'),
    '#default_value' => variable_get('emvideo_yahoomusic_bgcolor', EMVIDEO_YAHOOMUSIC_BGCOLOR_DEFAULT),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_bandwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Bandwidth'),
    '#default_value' => variable_get('emvideo_yahoomusic_bandwidth', ''),
    '#description' => t('Used to force a bandwidth; if bw is not specified the player will attempt to determine the best video bitrate quality for the user.'),
  );
  $form['yahoomusic']['player_options']['emvideo_yahoomusic_event_handler'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Handler'),
    '#default_value' => variable_get('emvideo_yahoomusic_event_handler', ''),
    '#description' => t('Callback event handler that the player should make calls to.'),
  );

  return $form;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_yahoomusic_data_version() {
  return EMVIDEO_YAHOOMUSIC_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_yahoomusic_data($field, $item) {
  $data = array();
  // create some 'field' version control
  $data['emvideo_yahoomusic_version'] = $data['emvideo_data_version'] = EMVIDEO_YAHOOMUSIC_DATA_VERSION;
  return $data;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_yahoomusic_extract($embed = '') {
  // http://new.music.yahoo.com/videos/--205781579
  // http://new.uk.music.yahoo.com/videos/Coldplay/Violet-Hill--60339090;_ylt=ApVMaK9zo0I8Fb03YCMv5qz1ECYv
  // http://new.music.yahoo.com/T-I-/videos/view/Whatever-You-Like--184525308
  if ($embed && preg_match('@(.*)yahoo.com(.*)--([0-9]+)@i', $embed, $matches)) {
    return $matches[3];
  }
  // <param name='flashvars' value='id=v205781579&shareEnable=1&infoEnable=1' />
  else if ($embed && preg_match('@id=v([^"\&]+)@i', $embed, $matches)) {
    return $matches[1];
  }

  return FALSE;

}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_yahoomusic_embedded_link($video_code) {
  $locale = _emvideo_yahoomusic_locale_lookup();
  return 'http://new.' . $locale . '.music.yahoo.com/videos/--' . $video_code;
}

function _emvideo_yahoomusic_locale_lookup($value = 'code') {
// Taken from http://developer.yahoo.com/music/api_guide/GettingStartedLocales.html
//  Locale                    API Hostname            eID       ympsc       lang
//  United States             us.music.yahooapis.com  1301797   4195351     en
//  United States (Espa�ol)   e1.music.yahooapis.com  1307666   559940629   es
//  Canada                    ca.music.yahooapis.com  1307409   642778131   en
//  Mexico                    mx.music.yahooapis.com  8257040   640680961   es
//  Australia                 au.music.yahooapis.com  1307669   638583826   en
//  New Zealand               nz.music.yahooapis.com  5300947   638583833   en
//  United Kingdom            uk.music.yahooapis.com  1301655   529531933   uk
//  Germany                   de.music.yahooapis.com  1307668   535823380   de
//  Spain                     es.music.yahooapis.com  1307660   534774806   es
//  Italy                     it.music.yahooapis.com  1307667   533726232   it
//  France                    fr.music.yahooapis.com  1307665   536871959   fr

  static $locales = array();

  $code = variable_get('emvideo_yahoomusic_locale','us');

  switch ($code) {
    case 'us':
      $locales[$code] = array('name' => 'United States', 'eid' => 1301797, 'ympsc' => 4195351, 'lang' => 'en');
      break;
    case 'e1':
      $locales[$code] = array('name' => 'United States (Espanol)', 'eid' => 1307666, 'ympsc' => 559940629, 'lang' => 'es');
      break;
    case 'ca':
      $locales[$code] = array('name' => 'Canada', 'eid' => 1307409, 'ympsc' => 642778131, 'lang' => 'en');
      break;
    case 'mx':
      $locales[$code] = array('name' => 'Mexico', 'eid' => 8257040, 'ympsc' => 640680961, 'lang' => 'es');
      break;
    case 'au':
      $locales[$code] = array('name' => 'Australia', 'eid' => 1307669, 'ympsc' => 638583826, 'lang' => 'en');
      break;
    case 'nz':
      $locales[$code] = array('name' => 'New Zealand', 'eid' => 5300947, 'ympsc' => 638583833, 'lang' => 'en');
      break;
    case 'uk':
      $locales[$code] = array('name' => 'United Kingdom', 'eid' => 1301655, 'ympsc' => 529531933, 'lang' => 'uk');
      break;
    case 'de':
      $locales[$code] = array('name' => 'Germany', 'eid' => 1307668, 'ympsc' => 535823380, 'lang' => 'de');
      break;
    case 'es':
      $locales[$code] = array('name' => 'Spain', 'eid' => 1307660, 'ympsc' => 534774806, 'lang' => 'es');
      break;
    case 'it':
      $locales[$code] = array('name' => 'Italy', 'eid' => 1307667, 'ympsc' => 533726232, 'lang' => 'it');
      break;
    case 'fr':
      $locales[$code] = array('name' => 'France', 'eid' => 1307665, 'ympsc' => 536871959, 'lang' => 'fr');
      break;
    default:
      $code = 'us';
      $locales[$code] = array('name' => 'United States', 'eid' => 1301797, 'ympsc' => 4195351, 'lang' => 'en');
  }
  $locales[$code]['hostname'] = $code . '.music.yahooapis.com';

  switch ($value) {
    case 'code':
      return $code;
      break;
    case 'name':
      return $locales[$code]['name'];
      break;
    case 'hostname':
      return $locales[$code]['hostname'];
      break;
    case 'eid':
      return $locales[$code]['eid'];
      break;
    case 'ympsc':
      return $locales[$code]['ympsc'];
      break;
    case 'lang':
      return $locales[$code]['lang'];
      break;
    default:
      drupal_set_message(t('Bad locale lookup request.'), 'warning');
      return '';
  }
}

/**
 * The embedded flash displaying the yahoomusic video.
 */
function theme_emvideo_yahoomusic_flash($embed, $width, $height, $autoplay, $options = array()) {
  static $count;
  if ($embed) {

    $variables = array();
    $variables['id'] = 'v' . $embed;
    $variables['eID'] = _emvideo_yahoomusic_locale_lookup('eid');
    $variables['ympsc'] = _emvideo_yahoomusic_locale_lookup('ympsc');
    $variables['lang'] = _emvideo_yahoomusic_locale_lookup('lang');

    $fullscreen = variable_get('emvideo_yahoomusic_full_screen', 1);
    $fullscreen_value = $fullscreen ? "true" : "false";
    $variables['enableFullScreen'] = $fullscreen ? "1" : "0";
    $variables['autoStart'] = $autoplay ? '1' : '0';

    $variables['bw'] = variable_get('emvideo_yahoomusic_bandwidth', '');
    $variables['eh'] = variable_get('emvideo_yahoomusic_event_handler', '');
    $variables['closeEnable'] = variable_get('emvideo_yahoomusic_close_enable', 0);
    $variables['controlsEnable'] = variable_get('emvideo_yahoomusic_controls_enable', 1);
    $variables['infoEnable'] = variable_get('emvideo_yahoomusic_info_enable', 1);

    $variables['nowplayingEnable'] = variable_get('emvideo_yahoomusic_now_playing_enable', 1);
    $variables['postpanelEnable'] = variable_get('emvideo_yahoomusic_post_panel_enable', 1);
    $variables['prepanelEnable'] = variable_get('emvideo_yahoomusic_pre_panel_enable', 1);
    $variables['shareEnable'] = variable_get('emvideo_yahoomusic_share_enable', 1);
    $bgcolor = variable_get('emvideo_yahoomusic_bgcolor', EMVIDEO_YAHOOMUSIC_BGCOLOR_DEFAULT);

    foreach ($variables as $name => $value) {
      if ($value !== '') {
        // Add to flashvars string.
        if ($name == 'id') {
          $flash_vars = $name . '=' . $value;
        }
        else {
          $flash_vars .= '&' . $name . '=' . $value;
        }
      }
      else {
        // Remove from variables array
        unset($variables[$name]);
      }
    }

    $id = isset($options['id']) ? $options['id'] : 'video-cck-yahoomusic-flash-'. (++$count);
    $div_id = isset($options['div_id']) ? $options['div_id'] : 'video-cck-yahoomusic-flash-wrapper-'. $count;
    $url = "http://d.yimg.com/cosmos.bcst.yahoo.com/up/fop/embedflv/swf/fop.swf";

    if (variable_get('emfield_swfobject', FALSE) && (module_exists('swfobject_api') || variable_get('emfield_swfobject_location', ''))) {
      if (module_exists('swfobject_api')) {
        $params['width'] = $width;
        $params['height'] = $height;
        $params['div_id'] = $id;
        $params['allowFullScreen'] = $fullscreen_value;
        $params['bgcolor'] = $bgcolor;
        $output .= theme('swfobject_api', $url, $params, $variables, $id);
      }
      else {
        drupal_add_js(variable_get('emfield_swfobject_location', ''));
        $flashvars = array();
        foreach ($variables as $name => $value) {
          $flashvars[] = "$name: '$value'";
        }
        $flashvars = implode(',', $flashvars);
        $output .= <<<FLASH
          <div id="$div_id">
            Sorry, you need to install flash to see this content.
          </div>
          <script type="text/javascript">
            var params = { allowFullScreen: "$fullscreen_value", bgcolor: "$bgcolor", allowScriptAccess: "always" };
            var flashvars = { $flashvars };
            swfobject.embedSWF("$url", "$div_id", "$width", "$height", "9.0.0", false, flashvars, params);
          </script>
FLASH;
      }
    }
    else {
      $output .= <<<FLASH
        <div id="$div_id">
        <object height="$height" width="$width" type="application/x-shockwave-flash" data="$url" id="$id" style="visibility: visible;">
          <param name="div_id" value="$div_id"/>
          <param name="allowFullScreen" value="$fullscreen_value"/>
          <param name="type" value="movie"/>
          <param name="flashvars" value="$flash_vars"/>
          <param name="bgcolor" value="$bgcolor"/>
        </object>
        </div>
FLASH;
    }
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_yahoomusic_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $yahoomusic_id = $item['value'];

  $locale = _emvideo_yahoomusic_locale_lookup();
  $tn = "http://d.yimg.com/img.music.yahoo.com/image/v1/video/$yahoomusic_id;encoding=jpg;size=" . $width . 'x' .$height . ";locale=$locale;";

  // if we have a large thumbnail size, then get the largest version available.
  if ($width > 385 || $height > 231) {
    $tn = "http://d.yimg.com/img.music.yahoo.com/image/v1/video/$yahoomusic_id;encoding=jpg;locale=$locale;";
  }

  return $tn;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_yahoomusic_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_yahoomusic_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_yahoomusic_preview($embed, $width, $height, $field, $item, $autoplay) {
  $output = theme('emvideo_yahoomusic_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_yahoomusic_emfield_subtheme() {
    return array(
        'emvideo_yahoomusic_flash'  => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/yahoomusic.inc'
        )
    );
}
