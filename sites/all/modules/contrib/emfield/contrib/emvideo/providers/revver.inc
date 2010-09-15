<?php
// $Id: revver.inc,v 1.1.2.9 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 *   This include processes revver.com media files for use by emfield.module.
 */

define('EMVIDEO_REVVER_MAIN_URL', 'http://one.revver.com/revver');
define('EMVIDEO_REVVER_AFFILIATE_PROGRAM_URL', 'http://one.revver.com/revver/go/faq#makingmoney1');

function emvideo_revver_info() {
  $features = array(
    array(t('Affliate program'), t('Yes'), t('Not currently supported by other providers. Set affiliate ID above.')),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('May not currently resize thumbnails, but this will be easy to add. Meanwhile, you can override theme_emvideo_revver_thumbnail if you have a pressing need for this and know how to theme.')),
  );
  return array(
    'provider' => 'revver',
    'name' => t('Revver'),
    'url' => EMVIDEO_REVVER_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@provider" target="_blank">Revver</a>.', array('@provider' => EMVIDEO_REVVER_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_revver_settings() {
  $form = array();
  $form['revver']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revver affiliate program'),
    '#description' => t('You may share ad revenue if you publish Revver videos through their <a href="@program" target="_blank">Affiliate Program</a>.', array('@program' => EMVIDEO_REVVER_AFFILIATE_PROGRAM_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['revver']['api']['emvideo_revver_affiliate_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Revver associate key'),
    '#default_value' => variable_get('emvideo_revver_affiliate_id', '0'),
    '#description' => t('If you have a Revver Affiliate ID, you may enter it here. This is not required for viewing videos, but will give you proper credit when doing so. You can find your Affiliate ID by examining the URL of a video while logged into their site.'),
  );
  return $form;
}

function emvideo_revver_extract($embed) {
  return array(
    '@revver\.com/video/([^/]*)/@i',
    '@revver\.com/video/(.*)@i',
    '@revver\.com/player/1\.0/player\.js\?mediaId\:([^;]*);@i',
  );
}

function emvideo_revver_video_link($video_code) {
  return 'http://one.revver.com/video/'. $video_code;
}

function theme_emvideo_revver_flash($embed, $width, $height, $autoplay) {
  if ($embed) {
    $autoplay = $autoplay ? ';flashvars:autoStart=true;' : '';
    $output .= '<script src="http://flash.revver.com/player/1.0/player.js?mediaId:'. $embed .';affiliateId:'. check_plain(variable_get('emvideo_revver_affiliate_id', '0')) .';height:'. $height .';width:'. $width . $autoplay .';" type="text/javascript"></script>';
  }
  return $output;
}

function emvideo_revver_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return 'http://frame.revver.com/frame/'. $width .'x'. $height .'/'. check_plain($item['value']) .'.jpg';
}

function emvideo_revver_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_revver_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_revver_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_revver_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_revver_emfield_subtheme() {
  return array(
    'emvideo_revver_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/revver.inc'
    )
  );
}
