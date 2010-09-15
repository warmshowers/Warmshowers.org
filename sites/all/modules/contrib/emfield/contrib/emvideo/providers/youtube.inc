<?php
// $Id: youtube.inc,v 1.1.2.52 2010/01/08 21:28:06 alexua Exp $

/**
 * @file
 *   This include processes youtube.com media files for use by emfield.module.
 */

define('EMVIDEO_YOUTUBE_MAIN_URL', 'http://youtube.com/');
define('EMVIDEO_YOUTUBE_API_INFO', 'http://code.google.com/apis/youtube/getting_started.html');
define('EMVIDEO_YOUTUBE_API_APPLICATION_URL', 'http://code.google.com/apis/youtube/dashboard/');
define('EMVIDEO_YOUTUBE_REST_ENDPOINT', 'http://gdata.youtube.com/feeds/api/videos');
define('EMVIDEO_YOUTUBE_COLOR1_DEFAULT', '#FFFFFF');
define('EMVIDEO_YOUTUBE_COLOR2_DEFAULT', '#CCCCCC');
define('EMVIDEO_YOUTUBE_DATA_VERSION', 3);

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
function emvideo_youtube_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Show related videos'), t('Yes'), t('This is embedded in the video itself when enabled; currently not available with other providers. Set the feature above.')),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Duration'), t('Yes'), ''),
    array(t('Custom player colors'), t('Yes'), t("You may customize the player's skin by choosing your own colors, and/or border in that setting field set.")),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
    array(t('Use JW FLV Media Player'), t('Yes'), t("You may opt to use the !flvplayer to play YouTube videos if it's installed on your server.", array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')))),
  );
  return array(
    'provider' => 'youtube',
    'name' => t('YouTube'),
    'url' => EMVIDEO_YOUTUBE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@youtube" target="_blank">YouTube</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@youtube' => EMVIDEO_YOUTUBE_MAIN_URL, '@api' => EMVIDEO_YOUTUBE_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['youtube'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_youtube_settings() {
  $form['emvideo_youtube_show_related_videos'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show related videos'),
    '#default_value' => variable_get('emvideo_youtube_show_related_videos', 0),
    '#description' => t('If checked, then when playing a video from YouTube, users may hover over the video to see thumbnails & links to related videos.'),
  );
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('YouTube API'),
    '#description' => t('This is not currently required. At some point, you may need to apply for an API Developer Key from the <a href="@youtube" target="_blank">YouTube Developer Profile page</a>. Note that you do not need this key to display YouTube videos or their thumbnails.', array('@youtube' => EMVIDEO_YOUTUBE_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['api']['emvideo_youtube_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube Developer Key'),
    '#default_value' => variable_get('emvideo_youtube_api_key', ''),
    '#description' => t('Please enter your YouTube Developer Key here.'),
  );
  $form['colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded Video Player Colors'),
    '#description' => t('If allowed, these two colors, in hexidecimal form (#RRGGBB), will be used to skin the YouTube player.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['colors']['emvideo_youtube_show_colors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override player colors'),
    '#default_value' => variable_get('emvideo_youtube_show_colors', FALSE),
  );
  $form['colors']['emvideo_youtube_show_border'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display border around player'),
    '#default_value' => variable_get('emvideo_youtube_show_border', FALSE),
  );
  $form['colors']['emvideo_youtube_colors_color1'] = array(
    '#type' => 'textfield',
    '#title' => t('Color 1'),
    '#default_value' => variable_get('emvideo_youtube_colors_color1', EMVIDEO_YOUTUBE_COLOR1_DEFAULT),
  );
  $form['colors']['emvideo_youtube_colors_color2'] = array(
    '#type' => 'textfield',
    '#title' => t('Color 2'),
    '#default_value' => variable_get('emvideo_youtube_colors_color2', EMVIDEO_YOUTUBE_COLOR2_DEFAULT),
  );
  $form['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['player_options']['emvideo_youtube_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => variable_get('emvideo_youtube_full_screen', 1),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );
  $form['player_options']['emvideo_youtube_high_quality'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use YouTube high quality content'),
    '#default_value' => variable_get('emvideo_youtube_high_quality', FALSE),
    '#description' => t("If checked, then a parameter will be set to request high quality content.  Note: Not all videos from youtube are available in high quality. Those that aren't will play in normal quality."),
  );
   $form['player_options']['emvideo_youtube_display_info'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove titles and ratings on videos'),
    '#default_value' => variable_get('emvideo_youtube_display_info', FALSE),
    '#description' => t("If checked, additional info, such as title and rating, will NOT be displayed on the YouTube player."),
  );

  if (module_exists('colorpicker')) {
    $form['colors']['emvideo_youtube_colors_color1']['#type'] = 'colorpicker_textfield';
    $form['colors']['emvideo_youtube_colors_color1']['#colorpicker'] = 'colorpicker_1';
    $form['colors']['emvideo_youtube_colors_color2']['#type'] = 'colorpicker_textfield';
    $form['colors']['emvideo_youtube_colors_color2']['#colorpicker'] = 'colorpicker_2';
    $form['colors']['colorpicker_1'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Color 1 picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
    $form['colors']['colorpicker_2'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Color 2 picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
  }
  else {
    $form['colors']['#description'] .= ' '. t('The <a href="@colorpicker">Colorpicker module</a>, if active, gives an easy way to select these colors.', array('@colorpicker' => 'http://drupal.org/project/colorpicker'));
  }

  $form['jwflv'] = array(
    '#type' => 'fieldset',
    '#title' => t('JW FLV Media Player Options'),
    '#collapsible' => TRUE,
    '#collapsed' => !($flv_path = emfield_flvmediaplayer_url()),
  );
  $form['jwflv']['emvideo_youtube_use_jw_flv'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use JW FLV Media Player'),
    '#description' => t('If checked, and the server is properly configured, YouTube videos will be played using the !flvplayer.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/'))),
    '#default_value' => variable_get('emvideo_youtube_use_jw_flv', FALSE),
  );
  if (!$flv_path) {
    $form['jwflv']['#description'] = t('You must first install the !flvplayer, preferably in your /sites/all/plugins folder.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')));
  }
  if ($flv_path && !_emvideo_youtube_check_flv_player_setup()) {
    $form['jwflv']['#description'] = t('You must have the yt.swf file installed in the same folder as your !flvplayer.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')));
  }
  $form['jwflv']['#description'] = $form['jwflv']['#description'] ? $form['jwflv']['#description'] : t('!flvplayer is properly configured on your server, so may be used to play YouTube videos if desired.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')));
  return $form;
}

/**
 *  Ensure we're able to run YouTube videos from the JW FLV Media Player.
 *  This requires that we both have the player installed, and the included
 *  yt.swf file is located in the same folder.
 *  @return boolean
 *    Returns TRUE if both checks are TRUE.
 */
function _emvideo_youtube_check_flv_player_setup() {
  static $check;

  if (is_null($check)) {
    // We set up a static cache.

    // First check that the JW FLV Player is installed.
    $flv_path = emfield_flvmediaplayer_url();
    if (!$flv_path || !file_exists($flv_path)) {
      // There's no player installed, so yt.swf is moot.
      $check = FALSE;
    }
    else {
      // Now check that the yt.swf file is also present in the same folder.
      $path = dirname($flv_path);
      $check = file_exists($path . '/yt.swf');
    }
  }
  return $check;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_youtube_data_version() {
  return EMVIDEO_YOUTUBE_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_youtube_data($field, $item) {
  $data = array();

  // Create some 'field' version control.
  $data['emvideo_youtube_version'] = $data['emvideo_data_version'] = EMVIDEO_YOUTUBE_DATA_VERSION;

  // Store the raw data from YouTube's API.
  $data['raw'] = emvideo_youtube_request($item['value']);

  // Store the video's duration.
  $data['duration'] = intval($data['raw']['MEDIA:GROUP']['YT:DURATION'][1]['SECONDS']);

  

  // Gather info about the item's raw flash video.
  // RSS / MRSS feeds with the item would have enough info.
  // Alternatively try getting the minimum from an HTTP get.
  
  // Get info from a youtube playlist.
  if (strpos($item['value'], 'PLAYLIST_') === 0) {
    $playlist_id = substr($item['value'], 9);
    $url = 'http://youtube.com/p/'. $playlist_id;
    $data['playlist'] = 1;
    
    // Get the large thumbnail of the first video.
    // Use the Youtube Google API to get this data.
    $api_url = 'http://gdata.youtube.com/feeds/api/playlists/' . $playlist_id;
    $result = drupal_http_request($api_url); 
    if ($result->code == 200) {
      $parser = drupal_xml_parser_create($result->data);
      $vals = array();
      $index = array();
      xml_parse_into_struct($parser, $result->data, $vals, $index);
      xml_parser_free($parser);
      if (count($vals)) {
        foreach ($vals as $val) {
          if ($val['tag'] == 'MEDIA:THUMBNAIL' && $val['attributes']['HEIGHT'] == '240') {
            $data['thumbnail']['url'] = $val['attributes']['URL'];
            break;
          }
        }
      }
    }
  }
  // Get info from a single video.
  else {
    $url = 'http://youtube.com/v/'. $item['value'];
    $data['playlist'] = 0;
    // Get the large thumbnail.
    $data['thumbnail']['url'] = 'http://img.youtube.com/vi/'. $item['value'] .'/0.jpg';
  }
  $response = emfield_request_header('youtube', $url);

  if ($response->code == 200) {
    // Don't give the 303 path.
    $data['flash']['url'] = $url;
    $data['flash']['size'] = $response->headers['Content-Length'];
    $data['flash']['mime'] = $response->headers['Content-Type'];
  }
  return $data;
}

/**
 *  hook emfield_PROVIDER_rss
 *
 *  Embeds the video in the RSS feed.
 */
function emvideo_youtube_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if (!empty($item['data']['emvideo_youtube_data_version']) && $item['data']['emvideo_youtube_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_youtube_data(NULL, $item);
    }

    $file = array();
    if (is_array($data['flash'])) {
      $file['filepath'] = $data['flash']['url'];
      $file['filesize'] = $data['flash']['size'];
      $file['filemime'] = $data['flash']['mime'];
    }
    $file['thumbnail']['filepath'] = $data['thumbnail']['url'];

    return $file;
  }
}

/**
 * this is a wrapper for emvideo_request_xml that includes youtube's api key
 */
function emvideo_youtube_request($code, $cached = TRUE) {
  $args['dev_id'] = trim(variable_get('emvideo_youtube_api_key', ''));
  $args['method'] = $method;

  // if we've got a secret sign the arguments
  // TODO: doesn't seem to matter
//  if ($secret = trim(variable_get('emvideo_youtube_api_secret', ''))) {
//    $args['api_sig'] = md5($secret . $arghash);
//  }

  $request = module_invoke('emfield', 'request_xml', 'youtube', EMVIDEO_YOUTUBE_REST_ENDPOINT .'/'. $code, array('video' => $code), $cached);
  return $request;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $code
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_youtube_extract($code = '') {
  // Special extraction for playlists.
  $playlist_regex = array(
    '@youtube\.com/p/([^"\& ]+)@i', // A playlist
    '@youtube\.com/view_play_list\?p=([^"\& ]+)@i', // A playlist
  );
  foreach ($playlist_regex as $regex) {
    if (preg_match($regex, trim($code), $matches)) {
      return 'PLAYLIST_'. $matches[1];
    }
  }
  
  // Single videos.
  // src="http://www.youtube.com/v/nvbQQnvxXDk"
  // http://youtube.com/watch?v=nvbQQnvxXDk
  // http://www.youtube.com/watch?v=YzFCA-xUc8w&feature=dir
  return array(
    '@youtube\.com/v/([^"\& ]+)@i',
    '@youtube\.com/watch\?v=([^"\& ]+)@i',
    '@youtube\.com/\?v=([^"\& ]+)@i',
  );
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_youtube_embedded_link($video_code) {
  if (strpos($video_code, 'PLAYLIST_') === 0) { // A youtube playlist
    return 'http://youtube.com/p/'. substr($video_code, 9);
  }
  return 'http://www.youtube.com/watch?v='. $video_code;
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_youtube_duration($item) {
  if (!isset($item['data']['emvideo_youtube_version']) || $item['data']['emvideo_youtube_version'] < 3) {
    $item['data'] = emvideo_youtube_data(NULL, $item);
  }
  return isset($item['data']['duration']) ? $item['data']['duration'] : 0;
}

function emvideo_youtube_convert_color($color = NULL) {
  if ($color{0} == '#') {
    return drupal_substr($color, 1);
  }

  return $color;
}

/**
 * The embedded flash displaying the youtube video.
 */
function theme_emvideo_youtube_flash($code, $width, $height, $autoplay, $options = array()) {
  static $count;
  $output = '';
  if ($code = check_plain($code)) {
    // Set the height and width.
    $width = intval($width);
    $height = intval($height);

    // Set the Full Screen option; if TRUE, then allow it.
    $fullscreen = isset($options['fullscreen']) ? $options['fullscreen'] : variable_get('emvideo_youtube_full_screen', 1);
    $fullscreen_value = $fullscreen ? "true" : "false";
    $fs = $fullscreen ? "&fs=$fullscreen" : "";

    // Set the 'related videos' youtube option.
    $related = isset($options['related']) ? $options['related'] : variable_get('emvideo_youtube_show_related_videos', 0);
    $related = "rel=$related";

    // Set the Autoplay option.
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
    $autoplay_value = $autoplay ? '&autoplay=1' : '';

    // Set the custom colors.
    $show_colors = isset($options['show_colors']) ? $options['show_colors'] : variable_get('emvideo_youtube_show_colors', FALSE);
    $colors = '';
    if ($show_colors) {
      $color1 = isset($options['color1']) ? $options['color1'] : variable_get('emvideo_youtube_colors_color1', emvideo_YOUTUBE_COLOR1_DEFAULT);
      $color2 = isset($options['color2']) ? $options['color2'] : variable_get('emvideo_youtube_colors_color2', emvideo_YOUTUBE_COLOR2_DEFAULT);
      $colors='&color1=0x'. emvideo_youtube_convert_color($color1) .'&color2=0x'. emvideo_youtube_convert_color($color2);
    }

    // Set the custom border; if TRUE, then display a big border.
    $border = isset($options['border']) ? $options['border'] : variable_get('emvideo_youtube_show_border', FALSE);
    $border = $border ? '&border=1' : '';

    // Enable JS for playback control.
    $enablejsapi = isset($options['enablejsapi']) ? $options['enablejsapi'] : variable_get('emvideo_youtube_enablejsapi', TRUE);
    $enablejsapi = $enablejsapi ? '&enablejsapi=1&playerapiid=ytplayer' : '';

    // Set the div ID for CSS.
    $id = check_plain(isset($options['id']) ? $options['id'] : 'emvideo-youtube-flash-'. (++$count));
    $div_id = check_plain(isset($options['div_id']) ? $options['div_id'] : 'emvideo-youtube-flash-wrapper-'. $count);

    // Whether to allow high quality videos or not.
    $high_quality = isset($options['high_quality']) ? $options['high_quality'] : variable_get('emvideo_youtube_high_quality', FALSE);
    $high_quality = $high_quality ? '&hd=1' : '';

    // Whether to display the video's info from YouTube in the player.
    $display_info = isset($options['display_info']) ? $options['display_info'] : variable_get('emvideo_youtube_display_info', FALSE);
    $display_info  = $display_info  ? '&showinfo=0' : '';
    
    // Create a URL from our options.
    if ($options['item']['data']['playlist']) {
      $code = substr($code, 9);
      $url = check_plain("http://www.youtube.com/p/$code&$related$autoplay_value$colors$border$high_quality$display_info$enablejsapi$fs");
    }
    else {
      $url = check_plain("http://www.youtube.com/v/$code&$related$autoplay_value$colors$border$high_quality$display_info$enablejsapi$fs");
    }
    

    // Find the path to the JW FLV Media Player.
    $path = emfield_flvmediaplayer_url();

    // If the JW FLV Player has been installed, the yt.swf file is in the same
    // folder, and the server has been configured to use it for YouTube videos,
    // then we'll do so now.
    $use_flv = isset($options['use_flv']) ? $options['use_flv'] : variable_get('emvideo_youtube_use_jw_flv', FALSE);

    // Set up the FLV Media Player options, assuming we're supposed to,
    // and the player is actually installed on the system.
    if ($use_flv && ($flv_path = emfield_flvmediaplayer_url()) && _emvideo_youtube_check_flv_player_setup()) {
      global $base_path;

      // Build the Thumbnail image for the player.
      $tn_options = array(
        'width' => $width,
        'height' => $height,
        'return_url' => TRUE,
      );

      $flashvars = array();

      // Grab the thumbnail for this video and tell JW FLV Player about it.
      $flashvars['image'] = theme('emvideo_video_thumbnail', NULL, $options['item'], 'emvideo_thumbnail', $options['node'], TRUE, $tn_options);

      // Tell the FLV Player we're sending a YouTube video.
      $flashvars['type'] = 'youtube';

      // We need to set the file to the original YouTube video.
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
        $data['params']['wmode'] = 'transparent';
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
      $params['wmode'] = 'transparent';
      $output = theme('emfield_swfobject', $url, $params, $flashvars, $id);
    }
    else if ($use_flv) {
      // We need to grab the thumbnail again for the NOEMBED option,
      // for accessibility compliance.
      // See http://www.w3.org/TR/WCAG10-HTML-TECHS/#text-equivs-multimedia.
      unset($tn_options['return_url']);
      $noembed = theme('emvideo_video_thumbnail', NULL, $options['item'], 'emvideo_thumbnail', $options['node'], TRUE, $tn_options);
      $flv_path = url($flv_path);
      $flashvars = check_plain(drupal_query_string_encode($flashvars));
      // Use object tags rather than embed.
      // See http://www.alistapart.com/articles/flashsatay
      $output = <<<FLV
        <div id="$div_id">
          <object
            type="application/x-shockwave-flash" data="$flv_path"
            width="$width" height="$height" id="$id">
            <param name="movie" value="$flv_path" />
            <param name="flashvars" value="$flashvars" />
            <param name="wmode" value="transparent" />
            $noembed
          </object>
        </div>
FLV;
    }
    else {
      // The fallback is to display plain old vanilla youtube.
      $flashvars = check_plain(drupal_query_string_encode(array('playerMode' => 'embedded')));
      $output = theme('emvideo_youtube_default_external', $height, $width, $url, $id, $div_id, $fullscreen_value, $flashvars);
    }
  }
  return '<div class="emfield-emvideo emfield-emvideo-youtube">'. $output .'</div>';
}

/**
 *  This is the fallback display, for when we don't have SWF Object or
 *  JW Flash Media Player.
 */
function theme_emvideo_youtube_default_external($height, $width, $url, $id, $div_id, $fullscreen_value, $flashvars) {
  $output = <<<FLASH
        <div id="$div_id"><object type="application/x-shockwave-flash" height="$height" width="$width" data="$url" id="$id">
          <param name="movie" value="$url" />
          <param name="allowScriptAccess" value="sameDomain"/>
          <param name="quality" value="best"/>
          <param name="allowFullScreen" value="$fullscreen_value"/>
          <param name="bgcolor" value="#FFFFFF"/>
          <param name="scale" value="noScale"/>
          <param name="salign" value="TL"/>
          <param name="FlashVars" value="$flashvars" />
          <param name="wmode" value="transparent" />
        </object></div>
FLASH;
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($code, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_youtube_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  if ($item['data']['thumbnail']['url']) {
    return $item['data']['thumbnail']['url'];
  }
  // Always return the larger image, since we're storing images locally.
  $tn = "http://img.youtube.com/vi/{$item['value']}/0.jpg";

  return $tn;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $code
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
function emvideo_youtube_video($code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['item'] = $item;
  $options['node'] = $node;
  $output = theme('emvideo_youtube_flash', $code, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $code
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
function emvideo_youtube_preview($code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['item'] = $item;
  $options['node'] = $node;
  $output = theme('emvideo_youtube_flash', $code, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_youtube_emfield_subtheme() {
  return array(
      'emvideo_youtube_flash'  => array(
          'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
          'file' => 'providers/youtube.inc'
      ),
      'emvideo_youtube_default_external' => array(
        'arguments' => array('height' => NULL, 'width' => NULL, 'url' => NULL, 'id' => NULL, 'div_id' => NULL, 'fullscreen_value' => NULL, 'flashvars' => NULL)
      ),
  );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_youtube_content_generate() {
  return array(
    'http://www.youtube.com/watch?v=-jubiv7QUco',
    'http://www.youtube.com/watch?v=VG_ss5QT03Y',
    'http://www.youtube.com/watch?v=LrUMJgyQVE8',
    'http://www.youtube.com/watch?v=H2eI02rK9_U',
    'http://www.youtube.com/watch?v=B3qxki6H8Fk',
    'http://www.youtube.com/watch?v=Rsaw5NuFIto',
    'http://www.youtube.com/watch?v=J3CaN-g5mQQ',
    'http://www.youtube.com/watch?v=L2pXcNu4GRo',
  );
}
