<?php
// $Id: dailymotion.inc,v 1.1.2.13 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 *   This include processes dailymotion media files for use by emfield.module.
 */

define('EMVIDEO_DAILYMOTION_MAIN_URL', 'http://www.dailymotion.com/');

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
function emvideo_dailymotion_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('Although thumbnails are generated, they are restricted to a size of 160x120.')),
  );
  return array(
    'provider' => 'dailymotion',
    'name' => t('Dailymotion'),
    'url' => EMVIDEO_DAILYMOTION_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@dailymotion" target="_blank">Dailymotion</a>.', array('@dailymotion' => EMVIDEO_DAILYMOTION_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['dailymotion'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_dailymotion_settings() {
  $form = array();
  return $form;
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
function emvideo_dailymotion_extract($embed = '') {
  // http://www.dailymotion.com/us/cluster/news/featured/video/x3xk8v_primary-smackdown-obama-girl-return_fun
  // http://www.dailymotion.com/barelypolitical/video/x3xk8v_primary-smackdown-obama-girl-return_fun
  // http://www.dailymotion.com/barelypolitical/video/x3xk8v
  // <div><object width="420" height="252"><param name="movie" value="http://www.dailymotion.com/swf/x3xk8v" /></param><param name="allowFullScreen" value="true"></param><param name="allowScriptAccess" value="always"></param><embed src="http://www.dailymotion.com/swf/x3xk8v" type="application/x-shockwave-flash" width="420" height="252" allowFullScreen="true" allowScriptAccess="always"></embed></object><br /><b><a href="http://www.dailymotion.com/video/x3xk8v_primary-smackdown-obama-girl-return_fun">Primary Smackdown: Obama Girl Returns</a></b><br /><i>Uploaded by <a href="http://www.dailymotion.com/BarelyPolitical">BarelyPolitical</a></i></div>
  // <div><object width="420" height="252"><param name="movie" value="http://www.dailymotion.com/swf/x3xk8v"></param><param name="allowFullScreen" value="true"></param><param name="allowScriptAccess" value="always"></param><embed src="http://www.dailymotion.com/swf/x3xk8v" type="application/x-shockwave-flash" width="420" height="252" allowFullScreen="true" allowScriptAccess="always"></embed></object><br /><b><a href="http://www.dailymotion.com/video/x3xk8v_primary-smackdown-obama-girl-return_fun">Primary Smackdown: Obama Girl Returns</a></b><br /><i>Uploaded by <a href="http://www.dailymotion.com/BarelyPolitical">BarelyPolitical</a></i></div>
//   if (preg_match('@dailymotion\.com@i', $embed, $matches)) {
//     if (preg_match('@/([^/_]+)_@i', $embed, $matches)) {
//       return $matches[0];
//     }
//   }
  if (preg_match('@dailymotion\.com/swf/([^"\&]+)@i', $embed, $matches)) {
    return $matches[1];
  }
  if (preg_match('@dailymotion\.com@i', $embed, $matches)) {
    if (preg_match('@/([^/_]+)_@i', $embed, $matches)) {
      return $matches[1];
    }
  }
  return array();
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_dailymotion_embedded_link($video_code) {
  return 'http://www.dailymotion.com/swf/'. $video_code;
}

/**
 * the embedded flash displaying the dailymotion video
 */
function theme_emvideo_dailymotion_flash($embed, $width, $height, $autoplay) {
  $output = '';
  if ($embed) {
    $autoplay = $autoplay ? '&amp;autoStart=1' : '';
    $output = "    <object type=\"application/x-shockwave-flash\" height=\"$height\" width=\"$width\" data=\"http://www.dailymotion.com/swf/$embed$autoplay\" id=\"VideoPlayback\">
      <param name=\"movie\" value=\"http://www.dailymotion.com/swf/$embed$autoplay\" />
      <param name=\"allowScriptAcess\" value=\"always\" />
      <param name=\"allowFullScreen\" value=\"true\" />
      <param name=\"quality\" value=\"best\" />
      <param name=\"bgcolor\" value=\"#FFFFFF\" />
      <param name=\"scale\" value=\"noScale\" />
      <param name=\"salign\" value=\"TL\" />
      <param name=\"FlashVars\" value=\"playerMode=embedded$autoplay\" />
      <param name=\"wmode\" value=\"transparent\" />
    </object>\n";
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
function emvideo_dailymotion_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return 'http://www.dailymotion.com/thumbnail/160x120/video/'. $item['value'];
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
function emvideo_dailymotion_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_dailymotion_flash', $embed, $width, $height, $autoplay);
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
function emvideo_dailymotion_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_dailymotion_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_dailymotion_emfield_subtheme() {
  return array(
    'emvideo_dailymotion_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/dailymotion.inc'
    )
  );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_dailymotion_content_generate() {
  return array(
    'http://www.dailymotion.com/relevance/search/drupal/video/x4gfqu_introduction-to-drupal_tech',
    'http://www.dailymotion.com/relevance/search/drupal/video/x57pyj_drupal-6-hot-new-themes_tech',
    'http://www.dailymotion.com/relevance/search/drupal/video/x76hya_host-color-we-like-drupal_tech',
  );
}
