<?php
// $Id: twistage.inc,v 1.1.2.2 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 * Provide support for Twistage to the emfield.module (http://twistage.com).
 */

/**
 * Implementation of hook_PROVIDER_info().
 */
function emvideo_twistage_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), t('Provides the main asset of the video in the RSS enclosure.')),
    array(t('Show related videos'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('Allows customized sizing of thumbnail images.')),
  );
  return array(
    'provider' => 'twistage',
    'name' => t('Twistage'),
    'url' => 'http://www.twistage.com',
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@twistage" target="_blank">Twistage</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@twistage' => 'http://twistage.com', '@api' => 'http://console.twistage.com/doc/api/index')),
    'supported_features' => $features,
  );
}

/**
 * Implementation of hook_PROVIDER_settings().
 */
function emvideo_twistage_settings() {
  $form['twistage']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twistage Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Ability to change the player profile.
  $form['twistage']['settings']['emvideo_twistage_playerprofile'] = array(
    '#type' => 'textfield',
    '#title' => t('Player Profile'),
    '#default_value' => variable_get('emvideo_twistage_playerprofile', ''),
    '#description' => t('If desired, enter the player profile to use for videos.'),
  );
  // Allow for fullscreen video player.
  $form['twistage']['settings']['emvideo_twistage_allowfullscreen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Fullscreen'),
    '#default_value' => variable_get('emvideo_twistage_allowfullscreen', TRUE),
  );
  return $form;
}

/**
 * Implementation of hook_PROVIDER_extract().
 */
function emvideo_twistage_extract($embed = '') {
  return array(
    // http://console.twistage.com/videos/5567da521fb40
    '@http://console\.twistage\.com/videos/([^"\?]+)@i',
    // http://service.twistage.com/plugins/player.swf?v=5567da521fb40
    '@http://service\.twistage\.com/plugins/player\.swf\?v=([^"\&]*)@i',
  );
}

/**
 * Implementation of hook_PROVIDER_data().
 */
function emvideo_twistage_data($field, $item) {
  // Create the initial data for the enclosure with the thumbnail and asset.
  $data = array(
    'emvideo_twistage_version' => 1,
    'thumbnail' => array(
      'filepath' => emvideo_twistage_thumbnail(NULL, $item, NULL, NULL, 300, 250),
      'width' => 300,
    ),
    'filepath' => 'http://service.twistage.com/videos/'. $item['value'] .'/play',
  );

  // Obtain the file types.
  $response = emfield_request_header('twistage', $data['filepath']);
  if ($response->code == 200) {
    $data['filesize'] = $response->headers['Content-Length'];
    $data['filemime'] = $response->headers['Content-Type'];
  }

  return $data;
}

/**
 * Implementation of hook_PROVIDER_rss().
 */
function emvideo_twistage_rss($item, $teaser = NULL) {
  // Create the data values that are sent to the RSS feed.
  if ($item['value']) {
    if (!empty($item['data']['emvideo_twistage_version']) && $item['data']['emvideo_twistage_version'] == 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_twistage_data(NULL, $item);
    }

    // Remove unnecessary data.
    unset($data['emvideo_twistage_version']);
    return $data;
  }
}

/**
 * Implementation of hook_PROVIDER_link().
 */
function emvideo_twistage_embedded_link($video_code) {
  // Provide a straight embed link.
  return 'http://service.twistage.com/plugins/player.swf?v='. $video_code;
}

/**
 * Returns HTML for an embedded Twistage player.
 */
function theme_emvideo_twistage_flash($embed, $width, $height, $autoplay, $item) {
  if ($embed) {
    // Prepare the variables to be sent to the embed code.
    $playerprofile = variable_get('emvideo_twistage_playerprofile', '');
    if (!empty($playerprofile)) {
      $playerprofile = "&p=$playerprofile";
    }
    $allowfullscreen = variable_get('emvideo_twistage_allowfullscreen', TRUE) ? 'true' : 'false';
    $autoplay = $autoplay ? 'true' : 'false';
    $id = form_clean_id('twistage-'. $embed);

    // Create the embed code using the embedded player API:
    // http://console.twistage.com/doc/api/video/embed
    $output = <<<TWISTAGECODE
      <object width="$width" height="$height" id="$id" type="application/x-shockwave-flash" data="http://service.twistage.com/plugins/player.swf?v=$embed$playerprofile">
        <param name="movie" value="http://service.twistage.com/plugins/player.swf?v=$embed$playerprofile"/>
        <param name="allowfullscreen" value="$allowfullscreen"/>
        <param name="allowscriptaccess" value="always"/>
        <param name="base" value="http://service.twistage.com"/>
        <param name="flashvars" value="v=$embed&config={config:{autoplay:$autoplay}}"/>
      </object>
TWISTAGECODE;
  }
  return $output;
}

/**
 * Implementation of hook_PROVIDER_thumbnail().
 */
function emvideo_twistage_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // Create a thumbnail URL from the Still Frames API:
  // http://console.twistage.com/doc/api/video/still_frames
  return "http://service.twistage.com/videos/{$item['value']}/screenshots/{$width}w.jpg";
}

/**
 * Implementation of hook_PROVIDER_video().
 */
function emvideo_twistage_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  return theme('emvideo_twistage_flash', $embed, $width, $height, $autoplay, $item);
}

/**
 * Implementation of hook_PROVIDER_preview().
 */
function emvideo_twistage_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  return theme('emvideo_twistage_flash', $embed, $width, $height, $autoplay, $item);
}

/**
 * Implementation of hook_PROVIDER_subtheme().
 */
function emvideo_twistage_emfield_subtheme() {
  return array(
    'emvideo_twistage_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/twistage.inc'
    )
  );
}
