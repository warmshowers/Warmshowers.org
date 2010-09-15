<?php
// $Id: example.inc.txt,v 1.1.2.2 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 *  This is an example provider include file for Embedded Media Video.
 *  Use this as a base for creating new provider files.
 *
 *  When using this, first make the following global replacements:
 *    * Replace EXAMPLE with the name of your provider in all caps.
 *    * Replace example with the name of your provider in all lower case.
 *    * Replace Example with the name (to be translated) of your provider in
 *        uppercase.
 *
 *  You then need to go through each function and modify according to the
 *  requirements of your provider's API.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMVIDEO_EXAMPLE_MAIN_URL', 'http://www.example.com/');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMVIDEO_EXAMPLE_API_URL', 'http://www.example.com/api');

/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_example_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_EXAMPLE_DATA_VERSION', 1);

/**
 * hook emvideo_PROVIDER_info
 * This returns information relevant to a specific 3rd party video provider.
 *
 * @return
 *   A keyed array of strings requested by various admin and other forms.
 *    'provider' => The machine name of the provider. This must be the same as
 *      the base name of this filename, before the .inc extension.
 *    'name' => The translated name of the provider.
 *    'url' => The url to the main page for the provider.
 *    'settings_description' => A description of the provider that will be
 *      posted in the admin settings form.
 *    'supported_features' => An array of rows describing the state of certain
 *      supported features by the provider. These will be rendered in a table,
 *      with the columns being 'Feature', 'Supported', 'Notes'. In general,
 *      the 'Feature' column will give the name of the feature, 'Supported'
 *      will be Yes or No, and 'Notes' will give an optional description or
 *      caveats to the feature.
 */
function emvideo_example_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'example',
    'name' => t('Example'),
    'url' => EMVIDEO_EXAMPLE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !example. You can also read more about its !api.', array('!example' => l(t('Example.com'), EMVIDEO_EXAMPLE_MAIN_URL), '!api' => l(t("developer's API"), EMVIDEO_EXAMPLE_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emvideo_PROVIDER_settings
 *  This should return a subform to be added to the emvideo_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['example'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emvideo_example_settings() {
  // We'll add a field set of player options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the example provider.
  $form['example']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // This is an option to set the video to full screen. You should remove this
  // option if it is not provided by the example provider.
  $form['example']['player_options']['emvideo_example_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => variable_get('emvideo_example_full_screen', 1),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );

  return $form;
}

/**
 *  hook emvideo_PROVIDER_extract
 *
 *  This is called to extract the video code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a video when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting video code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    video code to be used. If the hook tests the code itself, it should
 *    return either the string of the video code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emvideo_example_extract($parse = '') {
  // Here we assume that a URL will be passed in the form of
  // http://www.example.com/video/text-video-title
  // or embed code in the form of <object value="http://www.example.com/embed...".

  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  return array(
    // In this expression, we're looking first for text matching the expression
    // between the @ signs. The 'i' at the end means we don't care about the
    // case. Thus, if someone enters http://www.Example.com, it will still
    // match. We escape periods as \., as otherwise they match any character.
    // The text in parentheses () will be returned as the provider video code,
    // if there's a match for the entire expression. In this particular case,
    // ([^?]+) means to match one more more characters (+) that are not a
    // question mark ([^\?]), which would denote a query in the URL.
    '@example\.com/video/([^\?]+)@i',

    // Now we test for embedded video code, which is similar in this case to
    // the above expression, except that we can safely assume we won't have a
    // query in the URL, and that the URL will be surrounded by quotation marks,
    // and have /embed/ rather than /video/ in the URL. Note that regular
    // expressions will be tested for matches in the order provided, so you
    // may need to move this value above the other in some cases. Obviously,
    // in the case of this example provider, you could easily improve the
    // regular expression to match against either a URL or embed code with
    // one expression, such as '@example\.com/[watch|embed]/([^"\?]+)@i'.
    // However, many other providers have more complex requirements, so
    // we split them up for this demonstration.
    '@example\.com/embed/([^"]+)=@i',
  );
}

/**
 *  hook emvideo_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the example provider.
 */
function emvideo_example_data($field, $item) {
  // Initialize the data array.
  $data = array();

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emvideo_example_version'] = EMVIDEO_EXAMPLE_DATA_VERSION;

  // We are using oEmbed to retrieve a standard set of data from the provider.
  // You should change the URL as specified by the example provider.
  // If the example provider does not support oEmbed, you must remove this
  // section entirely, or rewrite it to use their API.
  // See http://oembed.com/ for for information.
  $xml = emfield_request_xml('example', 'http://www.example.com/api/oembed.xml?url=http%3A//www.example.com/video/'. $item['value'], array(), TRUE, FALSE, $item['value']);

  // This stores a URL to the video's thumbnail.
  $data['thumbnail'] = $xml['OEMBED']['THUMBNAIL_URL'][0];
  return $data;
}

/**
 *  hook emvideo_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
function emvideo_example_rss($item, $teaser = NULL) {
  if ($item['value']) {
    $file['thumbnail']['filepath'] = $item['data']['thumbnail'];

    return $file;
  }
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_example_embedded_link($video_code) {
  return 'http://www.example.com/video/'. $video_code;
}

/**
 * The embedded flash displaying the example video.
 */
function theme_emvideo_example_flash($item, $width, $height, $autoplay) {
  $output = '';
  if ($item['embed']) {
    $autoplay = $autoplay ? 'true' : 'false';
    $fullscreen = variable_get('emvideo_example_full_screen', 1) ? 'true' : 'false';
    $output = '<object width="'. $width .'" height="'. $height .'">';
    $output .= '<param name="movie" value="http://www.example.com/embed/'. $item['data']['embed'] .'">';
    $output .= '<param name="quality" value="best" />';
    $output .= '<param name="wmode" value="transparent" />';
    $output .= '<param name="allowFullScreen" value="'. $fullscreen .'" />';
    $output .= '<param name="scale" value="showAll" />';
    $output .= '<param name="FlashVars" value="autoplay='. $autoplay .'" />';
    $output .= '<embed src="http://www.example.com/embed/'. $item['data']['embed'] .'" type="application/x-shockwave-flash" allowFullScreen="'. $fullscreen .'"  width="'. $width .'" height="'. $height .'"></embed>';
    $output .= '</object>';
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emvideo_example_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // In this demonstration, we previously retrieved a thumbnail using oEmbed
  // during the data hook.
  return $data['thumbnail'];
}

/**
 *  hook emvideo_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_example_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_example_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  hook emvideo_PROVIDER_video
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_example_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_example_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  Implementation of hook_emfield_subtheme.
 *  This returns any theme functions defined by this provider.
 */
function emvideo_example_emfield_subtheme() {
    $themes = array(
        'emvideo_example_flash'  => array(
            'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/example.inc',
            // If you don't provide a 'path' value, then it will default to
            // the emvideo.module path. Obviously, replace 'emexample' with
            // the actual name of your custom module.
            'path' => drupal_get_path('module', 'emexample'),
        )
    );
    return $themes;
}
