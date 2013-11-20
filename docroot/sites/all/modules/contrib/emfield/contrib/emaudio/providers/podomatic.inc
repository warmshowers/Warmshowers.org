<?php
// $Id: podomatic.inc,v 1.1.4.12 2009/10/29 13:32:55 aaron Exp $

/**
 * @file
 * This include processes Podomatic audio files for use by emaudio.module.
 */

define('EMAUDIO_PODOMATIC_MAIN_URL', 'http://www.podomatic.com/');

/**
 * hook emaudio_PROVIDER_info
 * this returns information relevant to a specific 3rd party audio provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emaudio_podomatic_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'podomatic',
    'name' => t('podOmatic'),
    'url' => EMAUDIO_PODOMATIC_MAIN_URL,
    'settings_description' => t('These settings specifically affect audio podcasts displayed from <a href="@podomatic" target="_blank">podOmatic</a>.', array('@podomatic' => EMAUDIO_PODOMATIC_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 * hook emaudio_PROVIDER_settings
 * this should return a subform to be added to the emaudio_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['podomatic'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emaudio_podomatic_settings() {
  $form = array();
  return $form;
}

/**
 * hook emaudio_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the audio code to be used
 *   if the hook tests the code itself, it should return either the string of the audio code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emaudio_podomatic_extract($embed = '') {
  // http://www.podomatic.com/podcast/embed/funkylondon
  // <object width="320" height="315"><param name="movie" value="http://www.podOmatic.com/flash/flashcatcher.swf"></param><embed type="application/x-shockwave-flash" src="http://www.podOmatic.com/flash/flashcatcher.swf" width="320" height="315" flashvars="playlist_url=http://funkylondon.podOmatic.com/xspf.xspf" ></embed></object><br /><a href="http://www.podOmatic.com/podcast/embed/funkylondon" style="text-decoration: none"><font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#0033ff"><strong>Click here to get your own player.</strong></font></a><br><br>
  // http://funkylondon.podomatic.com
  return array(
    '@podomatic\.com/podcast/embed/([^"\&]+)@i',
    '@playlist_url\=http://([^\.]+)\.podOmatic\.com@i',
    '@http://([^\.]+)\.podomatic\.com@i',
  );
}

/**
 * hook emaudio_PROVIDER_embedded_link($audio_code)
 * returns a link to view the audio at the provider's site
 *  @param $audio_code
 *    the string containing the audio to watch
 *  @return
 *    a string containing the URL to view the audio at the original provider's site
 */
function emaudio_podomatic_embedded_link($audio_code) {
  return 'http://www.podomatic.com/podcast/embed/'. $audio_code;
}

/**
 * hook theme_emaudio_PROVIDER_flash
 * the embedded flash displaying the podomatic audio
 */

function theme_emaudio_podomatic_flash($embed, $width, $height, $autoplay) {
  // <object width="320" height="315"><param name="movie" value="http://www.podOmatic.com/flash/flashcatcher.swf"></param><embed type="application/x-shockwave-flash" src="http://www.podOmatic.com/flash/flashcatcher.swf" width="320" height="315" flashvars="playlist_url=http://funkylondon.podOmatic.com/xspf.xspf" ></embed></object><br /><a href="http://www.podOmatic.com/podcast/embed/funkylondon" style="text-decoration: none"><font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#0033ff"><strong>Click here to get your own player.</strong></font></a><br><br>

   if ($embed) {
   /*
    if ($autoplay) {
      $autoplay_value = '&autostart=1';
    }
	*/
    $output = '<embed type="application/x-shockwave-flash"
	src="http://www.podomatic.com/swf/mediaplayer.swf" width="320" height="340"
	allowscriptaccess="always" allowfullscreen="true"
	flashvars="thumbsinplaylist=true&width=320&height=340&file=http://'.$embed.'.podOmatic.com/xspf_stream.xml&autoscroll=false&displayheight=240&searchbar=false" />';
  }
  return $output;
}

/**
 * hook emaudio_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific audio
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emaudio_podomatic_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return $tn;
}

/**
 * hook emaudio_PROVIDER_audio
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
 *    the video code for the audio to embed
 *  @param $width
 *    the width to display the audio
 *  @param $height
 *    the height to display the audio
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded audio
 */
function emaudio_podomatic_audio($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emaudio_podomatic_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * hook emaudio_PROVIDER_preview
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $embed
 *    the video code for the audio to embed
 *  @param $width
 *    the width to display the audio
 *  @param $height
 *    the height to display the audio
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded audio
 */
function emaudio_podomatic_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emaudio_podomatic_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emaudio_podomatic_emfield_subtheme() {
  return array (
    'emaudio_podomatic_flash'  => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/podomatic.inc'
        )
    );
}
