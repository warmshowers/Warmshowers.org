<?php
// $Id: metacafe.inc,v 1.1.2.11 2010/01/06 14:43:13 aaron Exp $

/**
 * @file
 *   This include processes metacafe.com media files for use by emfield.module.
 */

define('EMVIDEO_METACAFE_MAIN_URL', 'http://www.metacafe.com/');

function emvideo_metacafe_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), ''),
  );
  return array(
    'provider' => 'metacafe',
    'name' => t('MetaCafe'),
    'url' => EMVIDEO_METACAFE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@provider" target="_blank">MetaCafe</a>.', array('@provider' => EMVIDEO_METACAFE_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_metacafe_settings() {
  $form = array();
  return $form;
}

/**
 *  we're going to handle our own matches, unless someone can come up with a regex that will match this better
 */
function emvideo_metacafe_extract($embed) {
  // http://www.metacafe.com/watch/479957/gorilla_prank/
  if ($embed && preg_match('@metacafe\.com/watch/(.[^/]*)/(.[^/]*)/@i', $embed, $matches)) {
    return $matches[1] .'/'. $matches[2];
  }
  else if ($embed && preg_match('@metacafe\.com/watch/(.[^/]*)/(.*)@i', $embed, $matches)) {
    return $matches[1] .'/'. $matches[2];
  }
  // <embed src="http://www.metacafe.com/fplayer/479957/gorilla_prank.swf" width="400" height="345" wmode="transparent" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash"></embed><br><font size = 1><a href="http://www.metacafe.com/watch/479957/gorilla_prank/">Gorilla Prank</a> - <a href='http://www.metacafe.com/'>Celebrity bloopers here</a></font>
  else if ($embed && preg_match('@metacafe\.com/fplayer/(.[^/]*)/(.[^\.]*)\.@i', $embed, $matches)) {
    return $matches[1] .'/'. $matches[2];
  }

  return FALSE;
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_metacafe_embedded_link($video_code) {
  return 'http://www.metacafe.com/watch/'. $video_code .'/';
}

function theme_emvideo_metacafe_flash($embed, $width, $height, $autoplay) {
  $output = '';
  if ($embed) {
    $autoplay = $autoplay ? '?playerVars=autoPlay=yes' : '';
    $output .= '<embed src="http://www.metacafe.com/fplayer/'. $embed .'.swf'. $autoplay .'" width="'. $width .'" height="'. $height .'" wmode="transparent" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash"></embed>';
  }
  return $output;
}

function emvideo_metacafe_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // http://www.metacafe.com/thumb/[VIDEOID].jpg
  $values = explode('/', $item['value'], 2);
  return 'http://www.metacafe.com/thumb/'. $values[0] .'.jpg';
}

function emvideo_metacafe_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_metacafe_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_metacafe_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_metacafe_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_metacafe_emfield_subtheme() {
  return array(
    'emvideo_metacafe_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/metacafe.inc'
    )
  );
}
