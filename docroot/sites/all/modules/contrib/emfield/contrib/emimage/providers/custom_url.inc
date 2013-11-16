<?php
// $Id: custom_url.inc,v 1.1.2.5 2010/07/19 15:57:58 aaron Exp $

/**
 * This hook of the emimage_PROVIDER_info() returns information relevant to
 * the Global Image Provider, and not to an specific 3rd party image provider.
 *
 * @return  An object of type array of strings requested by admin & other forms
 *      'name' => The translated name of the provider
 *      'url' => The url to the main page for the provider
 *      'settings_description' => a description of the provider that will be
 *        posted in the admin settings form
 *      'supported_features' => an array of rows describing the state of
 *        certain supported features by the provider. These will be
 *        rendered in a table, with the columns being 'Feature',
 *        'Supported', 'Notes'.
 */
function emimage_custom_url_info() {
  $name = t('Custom URL');
  $features = array(
    array(t('Thumbnails'), t('Yes'), ''),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS attachment'), t('No'), ''),
  );
  return array(
    'provider' => 'custom_url',
    'name' => $name,
    'settings_description' => t('The settings of this provider will allow images from any public hosting server.'),
    'supported_features' => $features,
  );
}

/**
 * This function will get additional information of the image source input by
 * the end user. It will also use a helper function to validate the source of
 * the image.
 *
 * @param  $field An object of type array
 * @param  $item  An object of type array containing information about the image
 *          URL; e.g. embed - which contains the full URL path.
 * @see         _validate_image_helper function
 * @return      An object of type array; which contains
 */
function emimage_custom_url_data($field, $item) {
  $data = array();

  // Validate the source.
  if (_emimage_custom_url_validate_image_helper($item['embed'])) {
    $data = array(
      'emimage_custom_url_data_version' => 1,
      'server' => _emimage_custom_url_get_domain_server($item['embed']),
      'file' => _emimage_custom_url_get_domain_file($item['embed']),
      'album' => _emimage_custom_url_get_domain_album($item['embed'], $data['server']),
      'owner' => $item['embed'],
    );
    $data['title'] = emimage_custom_url_image_title($data['file'], $data);
  }

  return $data;
}

/**
 * Thumbnail support
 */
function emimage_custom_url_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return $item['value'];
}

function _emimage_custom_url_get_domain_server($url){
  return _emimage_custom_url_get_domain_helper($url);
}

function _emimage_custom_url_get_domain_file($url){
  return substr(strrchr($url, '/'), 1);
}

function _emimage_custom_url_get_domain_album($url, $server){
  $albumBegin = strpos($url, $server) + strlen($server);
  $albumEnd = strrpos($url, '/')+1;
  return substr($url, $albumBegin, ($albumEnd-$albumBegin));
}

/**
 * Function uses helper function to validate the source of the image. If the
 * source of the image is validated then its return, otherwise an empty array
 * is returned so that the invoking function emfield_parse_embed() performs
 * its own image source validation.
 *
 * @param  $embed An object of type string with the image URL input by the end
 *          user as its value.
 * @see       function emfield.module line #273
 * @return      An object of type string with the validated image URL.
 * @return      An empty object of type array, which indicates the invoking
 *            function that it needs to validate the image URL.
 */
function emimage_custom_url_extract($embed = '') {
  if($embed != '' && _emimage_custom_url_validate_image_helper($embed)) {
    return $embed;
  }

  return array();
}

/**
 * This hook of the emimage_PROVIDER_embedded_link($code) returns a link to
 * view the content at the provider's site.
 *
 *  @param  $code The string containing the content to watch
 *  @return       a string containing the URL to view the image at the original
 *          provider's site.
 */
function emimage_custom_url_embedded_link($code, $data) {
  return $code;
}

/**
 *  This function is an implementation of the emimage_PROVIDER_image_url()
 *
 *  @param  $code The code of the image
 *  @param  $data Any stored data for the image, which may already have the title
 *  @return       The url directly to the image to display
 */
function emimage_custom_url_image_url($code, $data) {
  return $code;
}

/**
 *  This function is an implementation of the emimage_PROVIDER_image_title()
 *
 *  @param  $code The code of the image
 *  @param  $data Any stored data for the image, which may already have the title
 *  @return       The title as the 3rd party provider knows it, if accessible to
 *          us. otherwise, return an empty string.
 */
function emimage_custom_url_image_title($url, $data) {
  if (isset($data['title'])) {
    return $data['title'];
  }

  return _emimage_custom_url_scrape_image_title($url);
}

/**
 * Function uses a helper function _get_domain_helper to get the name of the
 * domain where the image is hosted.
 *
 * @param  $url An object of type String with the image URL as its value.
 * @return    An object of type String with the image title as its value.
 */
function _emimage_custom_url_scrape_image_title($url) {
  static $title;
  if (isset($title[$url])) {
    return $title[$url];
  }

  return $title[$url] = _emimage_custom_url_get_domain_helper($url);
}

/**
 * Function gets all the information from an image and stores it in an array of
 * 7 elements. It helps to avoid getting php warning if the image is not valid.
 *
 * Index 0 and 1 contains respectively the width and the height of the image.
 * Index 2 is one of the IMAGETYPE_XXX constants indicating the type of the image.
 * Index 3 is a text string with the correct height="yyy" width="xxx" string
 *  that can be used directly in an IMG tag.
 * Index mime is the correspondant MIME type of the image. This information can
 *  be used to deliver images with correct the HTTP Content-type header.
 * Index channels will be 3 for RGB pictures and 4 for CMYK pictures.
 * Index bits is the number of bits for each color.
 *
 * @param  $color A hexadecimal value representing a color.
 * @return        A boolean value, true if the image source is valid.
 */
function _emimage_custom_url_validate_image_helper($_filename) {
  $_filedata = @getimagesize($_filename);

  if($_filedata['mime'] != 'image/jpeg' && $_filedata['mime'] != 'image/gif'
      && $_filedata['mime'] != 'image/png'
      && $_filedata['mime'] != 'image/bmp') {
//     form_set_error(
//       'name',
//       t('There were problems encountered while processing this image.',
//       array('%name' => ''))
//     );
    return false;
  }

  return true;
}

/**
 * Function carves out the domain name out of any URL, including URLs with sub-
 * domains and additional extensions.
 *
 * @param  $url An object of type String with a URL as its value.
 * @return    An object of type String with the carved domain name as its
 *        value.
 */
function _emimage_custom_url_get_domain_helper($url) {
  $url = strtolower($url);

  $slds =
    '\.co\.uk|\.me\.uk|\.net\.uk|\.org\.uk|\.sch\.uk|
    \.ac\.uk|\.gov\.uk|\.nhs\.uk|\.police\.uk|
    \.mod\.uk|\.asn\.au|\.com\.au|\.net\.au|\.id\.au|
    \.org\.au|\.edu\.au|\.gov\.au|\.csiro\.au';

    preg_match (
        "/^(http:\/\/|https:\/\/)([^\/]+)/i",
        $url, $matches
    );

    $host = $matches[2];

    if (preg_match("/$slds$/", $host, $matches)) {
        preg_match (
            "/[^\.\/]+\.[^\.\/]+\.[^\.\/]+$/",
            $host, $matches
        );
    }
    else {
        preg_match (
            "/[^\.\/]+\.[^\.\/]+$/",
            $host, $matches
        );
    }
    return "{$matches[0]}";
}
