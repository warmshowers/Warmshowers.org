<?php
// $Id: guba.inc,v 1.1.2.10 2009/10/29 13:29:51 aaron Exp $
/**
 * @file
 *   Provide support for the Guba provider to the emfield.module.
 */

define('EMVIDEO_GUBA_MAIN_URL', 'http://www.guba.com/');

function emvideo_guba_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
  );
  return array(
    'provider' => 'guba',
    'name' => t('GUBA'),
    'url' => EMVIDEO_GUBA_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@guba" target="_blank">GUBA</a>.', array('@guba' => EMVIDEO_GUBA_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_guba_request($method, $args = array(), $cached = TRUE) {
  //$args = array('docid' => $embed);
  return module_invoke('emfield', 'request_xml', 'guba', "http://www.guba.com/rest/video/". $method ."/details", $args, $cacheable);
}

function emvideo_guba_extract($embed = '') {
  return array(
    '@http://www\.guba\.com/watch/([^"\?]+)@i',
    '@http://www\.guba\.com/f/root\.swf\?video_url=http://free\.guba\.com/uploaditem/([^"\?/]*)@i',
  );
}

function emvideo_guba_embedded_link($video_code) {
  return 'http://www.guba.com/watch/'. $video_code;
}

function theme_emvideo_guba_flash($embed, $width, $height, $autoplay) {
  $autoplay = $autoplay ? '&amp;autoPlay=TRUE' : '&amp;autoPlay=FALSE';
  $output .= '<embed src="http://www.guba.com/f/root.swf?video_url=http://free.guba.com/uploaditem/'. $embed .'/flash.flv&amp;isEmbeddedPlayer=FALSE'. $autoplay .'" quality="best" bgcolor="#FFFFFF" menu="TRUE" width="'. $width .'" height="'. $height .'" name="root" id="root" align="middle" scaleMode="noScale" allowScriptAccess="always" allowFullScreen="TRUE" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed>';
  return $output;
}

function emvideo_guba_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $thumbid = $item['value'];
  $thm = 'http://img.guba.com/public/video/f/58/'. $thumbid .'-m.jpg';
  return $thm;
}

function emvideo_guba_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_guba_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_guba_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_guba_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_guba_emfield_subtheme() {
  return array(
    'emvideo_guba_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/guba.inc'
    )
  );
}
