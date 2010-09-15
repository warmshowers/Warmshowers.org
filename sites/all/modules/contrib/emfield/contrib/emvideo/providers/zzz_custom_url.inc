<?php
// $Id: zzz_custom_url.inc,v 1.1.2.21 2010/07/19 17:08:04 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

function _emvideo_zzz_custom_url_default_types() {
  return array('wmv', 'wma', 'swf', 'flv', 'mov', 'rm', 'mp4', 'm4v');
}

function emvideo_zzz_custom_url_info() {
  $name = t('Custom URL');
  $features = array(
    array(t('Thumbnails'), t('No'), ''),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS attachment'), t('No'), ''),
  );
  return array(
    'provider' => 'zzz_custom_url',
    'name' => $name,
    'url' => '',
    'settings_description' => t('These settings specifically affect videos displayed from custom URLs. When a field uses a URL it determines to be a link directly to a video file, it will embed that file into the content.'),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emvideo_zzz_custom_url_settings() {
  $options = array(
    'wmv' => t('Windows Media (wmv)'),
    'wma' => t('Windows Media (wma)'),
    'swf' => t('Flash (swf)'),
    'flv' => t('Flash Video (flv)'),
    'mov' => t('Quicktime (mov)'),
    'mp4' => t('Quicktime (mp4)'),
    'm4v' => t('Quicktime (m4v)'),
    'rm' => t('Real Media (rm)'),
  );
  $form = array();
  $form['emvideo_zzz_custom_url_supported_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported Types'),
    '#options' => $options,
    '#default_value' => variable_get('emvideo_zzz_custom_url_supported_types', _emvideo_zzz_custom_url_default_types()),
    '#description' => t('Select the video types you wish to support. When a custom url with that type is entered into an embedded video field, it will be parsed and displayed appropriately. If a type is not supported, then it will be ignored.'),
  );
  return $form;
}

function _emvideo_zzz_custom_url_implode_types() {
  return implode('|', variable_get('emvideo_zzz_custom_url_supported_types', _emvideo_zzz_custom_url_default_types()));
}

function emvideo_zzz_custom_url_extract($embed = '') {
  $types = _emvideo_zzz_custom_url_implode_types();
  $baseurl = preg_quote(url(null, array('absolute' => TRUE)), '@');

  return array(
    '@' . $baseurl . '(.*\.(?:' . $types . ')' . '(\?.*)?)@i',
    '@(.*\.(?:'. $types .')(\?.*)?)@i',
    '@'. $baseurl.'(.*(?:xspf))@i',
    '@(.*(?:xspf))@i',
  );
}

function emvideo_zzz_custom_url_data($field, $item) {
  $data = array();
  // adding the version control
  $data['emvideo_zzz_custom_url_data_version'] = 1;

  // attempt to get info from headers
  $response = emfield_request_header('zzz_custom_url', $item['embed']);

  if ($response->code == 200) {
    $data['url'] = $item['embed'];
    $data['size'] = $response->headers['Content-Length'];
    $data['mime'] = $response->headers['Content-Type'];
  }
  // @todo replace ['type'] with converted mime info if available
  $types = _emvideo_zzz_custom_url_implode_types();
  $regex = '@\.('. $types .')$@i';
  if (preg_match($regex, $item['embed'], $matches)) {
    $data['type'] = $matches[1];
  }
  return $data;
}

/**
 * hook emfield_PROVIDER_rss
 */
function emvideo_zzz_custom_url_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if ($item['data']['emvideo_zzz_custom_url_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_zzz_custom_url_data(NULL, $item);
    }

    $file = array();
    if ($data['size'] > 0) {
      $file['filepath'] = $data['url'];
      $file['filesize'] = $data['size'];
      $file['filemime'] = $data['mime'];
    }

    return $file;
  }
}


function emvideo_zzz_custom_url_embedded_link($video_code) {
  return $video_code;
}

function theme_emvideo_zzz_custom_url_embedded_video($type, $url, $width, $height, $autoplay = FALSE, $field = NULL, $item = NULL, $options = array()) {
  if ($url = url($url, array('external' => TRUE, 'absolute' => TRUE))) {
    global $base_path;
    $url = $base_path.$url;

    if (module_exists('flowplayer')) {
      $config = array(
        'clip' => array(
          'autoPlay' => $autoplay,
          'url' => $url,
        ),
      );
      $attributes = array(
        'style' => "width:{$width}px;height:{$height}px;",
      );
      // Build the Thumbnail image for the player.
      $tn_options = array(
        'width' => $width,
        'height' => $height,
      );
      $thumbnail = '<span></span>'. theme('emvideo_video_thumbnail', NULL, $item, 'emvideo_thumbnail', $options['node'], TRUE, $tn_options);
      drupal_add_css(drupal_get_path('module', 'emvideo') .'/emvideo.custom-url.css');
      return theme('flowplayer', $config, 'emvideo-zzz-custom-url-embedded-video', $attributes, $thumbnail);
    }

    // Find the path to the JW FLV Media Player.
    $path = emfield_flvmediaplayer_url();

    // If the JW FLV Player has been installed, the yt.swf file is in the same
    // folder, and the server has been configured to use it for YouTube videos,
    // then we'll do so now.
    $use_flv = isset($options['use_flv']) ? $options['use_flv'] : variable_get('emvideo_zzz_custom_url_use_jw_flv', TRUE);

    // Set up the FLV Media Player options, assuming we're supposed to,
    // and the player is actually installed on the system.
    if ($use_flv && ($flv_path = emfield_flvmediaplayer_url())) {
      global $base_path;

      // Build the Thumbnail image for the player.
      $tn_options = array(
        'width' => $width,
        'height' => $height,
        'return_url' => TRUE,
      );

      $flashvars = array();

      // Grab the thumbnail for this video and tell JW FLV Player about it.
      $flashvars['image'] = theme('emvideo_video_thumbnail', NULL, $item, 'emvideo_thumbnail', $options['node'], FALSE, $tn_options);

      // We need to set the file to the original video.
      $flashvars['file'] = $url;

      // The JW FLV Player uses its own autoplay flashvar.
      $flashvars['autostart'] = $autoplay ? 'true' : 'false';

      // The URL will now be the JW FLV Player.
      $url = $base_path . $flv_path;
    }
    else {
      // We don't have access to FLV Media Player, so reset the option.
      $use_flv = FALSE;
    }
    if ($use_flv && module_exists('flvmediaplayer')) {
      // If we are using the JW FLV Player, defer to the flvmediaplayer module
      // for display.
      $params['width'] = $width;
      $params['height'] = $height;
      $params['div_id'] = $id;
      $params['allowFullScreen'] = $fullscreen_value;

      // If we have been told to use a specific FLV Player Option from
      // that module, then use it here.
      $flv_profile = isset($options['flv_profile']) ? $options['flv_profile'] : variable_get('emfield_flv_profile', '');
      if ($flv_profile) {
        // Get the configuration data for this profile.
        $data = flvmediaplayer_build_player_data($options['node'], $flv_profile, array('file' => $flashvars['file']));
        $data['params']['width'] = $width;
        $data['params']['height'] = $height;
        $data['flashvars']['autostart'] = $flashvars['autostart'];
        $data['params']['allowFullScreen'] = $fullscreen_value;
        $data['flashvars']['image'] = $flashvars['image'];
        $output = theme('flvmediaplayer_render_player', $url, $data['params'], $data['flashvars']);
      }
      else {
        // Just display the default jw flv media player.
        $output = theme('flvmediaplayer_render_player', $url, $params, $flashvars);
      }
    }
    else if (variable_get('emfield_swfobject', FALSE) && (module_exists('swfobject_api') || variable_get('emfield_swfobject_location', ''))) {
      // Use SWFObject API module if it's installed.
      // Note that we're going to try to send the Flv Media player as well,
      // assuming it's been set up and the flvmediaplayer module has not.
      $params['width'] = $width;
      $params['height'] = $height;
      $params['div_id'] = $id;
      $params['allowFullScreen'] = $fullscreen_value;
      $output = theme('emfield_swfobject', $url, $params, $flashvars, $id);
    }
    else if ($use_flv) {
      $flashvars = drupal_query_string_encode($flashvars);
      $output = <<<FLV
        <div id="$div_id"><embed src="$url" width="$width" height="$height" allowscriptaccess="always" allowfullscreen="$fullscreen_value" flashvars="$flashvars" /></div>
FLV;
    }
    else {
      switch ($type) {
        case 'wmv':
        case 'wma':
          $autostart = $autoplay ? '1' : '0';
          $output = '<embed src="'. $url .'" width="'. $width .'" height="'. $height .'" autostart="'. $autostart .'" showcontrols="1" type="application/x-mplayer2" pluginspage="http://www.microsoft.com/windows/windowsmedia/download/"> </embed>';
          break;
        case 'mov':
        case 'mp4':
        case 'm4v':
          $autostart = $autoplay ? 'true' : 'false';
          $output = '<embed src="'. $url .'" width="'. $width .'" height="'. $height .'" autoplay="'. $autostart .'" controller="true" type="video/quicktime" scale="tofit" pluginspage="http://www.apple.com/quicktime/download/"> </embed>';
          break;
        case 'rm':
          $autostart = $autoplay ? 'true' : 'false';
          $output = '<embed type="audio/x-pn-realaudio-plugin" src="'. $url .'" width="'. $width .'" height="'. $height .'" autostart="'. $autostart .'" controls="imagewindow" nojava="true" console="c1183760810807" pluginspage="http://www.real.com/"></embed><br /><embed type="audio/x-pn-realaudio-plugin" src="'. $url .'" width="'. $width .'" height="26" autostart="'. $autostart .'" nojava="true" controls="ControlPanel" console="c1183760810807"> </embed>';
          break;
        case 'swf':
          $output = '<embed src="'. $url .'" width="'. $width .'" height="'. $height .'" quality="high" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed>';
          break;
        case 'flv':
          $autostart = $autoplay ? 'true' : 'false';
          $output = '<embed src="http://freevideocoding.com/flvplayer.swf?file='. $url .'&amp;autoStart='. $autostart .'" width="'. $width .'" height="'. $height .'" quality="high" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed>';
          break;
      }
    }
    return $output;
  }
}

function emvideo_zzz_custom_url_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  return '';
}

function emvideo_zzz_custom_url_video($code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $type = $item['data']['type'];
  $options['node'] = $node;
  $output = theme('emvideo_zzz_custom_url_embedded_video', $type, $code, $width, $height, $autoplay, $field, $item, $options);
  return $output;
}

function emvideo_zzz_custom_url_preview($code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $type = $item['data']['type'];
  $options['node'] = $node;
  $output = theme('emvideo_zzz_custom_url_embedded_video', $type, $code, $width, $height, $autoplay, $field, $item, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_zzz_custom_url_emfield_subtheme() {
  return array(
    'emvideo_zzz_custom_url_embedded_video' => array(
      'arguments' => array('type' => NULL, 'url' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => FALSE, 'field' => NULL, 'item' => NULL),
      'file' => 'providers/zzz_custom_url.inc'
    )
  );
}
