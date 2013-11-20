<?php
// $Id: spike.inc,v 1.1.2.11 2010/01/06 16:53:47 aaron Exp $

/**
 * @file
 *   This include processes Spike media files for use by emfield.module.
 */

define('EMVIDEO_SPIKE_MAIN_URL', 'http://www.spike.com/');

function emvideo_spike_info() {
  $features = array(
    array(t('Autoplay'), t('No'), t('In the works...')),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('In the works...'), ''),
  );
  return array(
    'provider' => 'spike tv',
    'name' => t('Spike TV'),
    'url' => EMVIDEO_SPIKE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@spike" target="_blank">Spike TV</a>.', array('@spike' => EMVIDEO_SPIKE_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_spike_settings() {
  $form = array();
  return $form;
}

function emvideo_spike_extract($embed) {
  // <embed width="320" height="240" src="http://www.spike.com/efp" quality="high" bgcolor="000000" name="efp" align="middle" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" flashvars="flvbaseclip=2796729" allowfullscreen="true"> </embed> <div style="font-family: arial,helvetica,sans-serif;font-size:12px; background-color: #000; width: 448px; padding: 3px 0; color: #fff;"><a href="http://www.spike.com/video/simpsons-movie/2796729" style="color: #ffcc35; margin-left: 5px;">The Simpsons Movie - Trailer </a> | <a href="http://www.spike.com/channel/movies" style="color: #ffcc35">Movies & TV</a> | <a href="http://www.spike.com/" style="color: #ffcc35">SPIKE.com</a></div>
  // http://www.spike.com/video/simpsons-movie/2796729
  return array(
    // http://www.spike.com/video/2836119/collection/19459/channel/comedy
    // <embed width="448" height="365" src="http://www.spike.com/efp" quality="high" bgcolor="000000" name="efp" align="middle" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" flashvars="flvbaseclip=2836119&"> </embed> <h1><a href="http://www.spike.com/video/2836119">"I Lost It" with Tom Cruize and Jesus</a></h1><span>Posted Mar 25, 2007</span><p>What happens when Jesus, Abe Lincoln, Tom Cruise, Dustin Hoffman, Buddha and Einstein play a friendly game of poker?</p>
    '@spike\.com/efp"(?:.*)flvbaseclip=([^/&"\?]+)@i',
    '@spike\.com/video/[^/&"\?]+/([^/&"\?]+)@i',
    '@spike\.com/video/([^/&"\?]+)@i',
  );
}

function emvideo_spike_request($embed, $cacheable = TRUE) {
  return NULL;
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_spike_embedded_link($video_code) {
  return 'http://www.spike.com/video/'. $video_code;
}

function theme_emvideo_spike_flash($embed, $width, $height, $autoplay) {
  // TODO: figure out autoplay...
  //<embed width="448" height="365" src="http://www.spike.com/efp" quality="high" bgcolor="000000" name="efp" align="middle" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" flashvars="flvbaseclip=2952560&"> </embed>
  if ($embed) {
    $autoplay = $autoplay ? ' autoplay="true"' : '';
    $output = '<embed width="'. $width .'" height="'. $height .'" src="http://www.spike.com/efp" quality="high" bgcolor="000000" name="efp" align="middle" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" flashvars="flvbaseclip='. $embed .'"> </embed>';
  }

  return $output;
}

/**
 *  TODO: Is this correct? Using ifilmpro for thumbnails.
 */
function emvideo_spike_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $picturelink = drupal_substr($item['value'], 0, 7);

  return 'http://img1.ifilmpro.com/resize/image/stills/films/resize/istd/'. $picturelink .'.jpg?width='. $width;
}

function emvideo_spike_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_spike_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_spike_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_spike_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_spike_emfield_subtheme() {
  return array(
    'emvideo_spike_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/spike.inc'
    )
  );
}
