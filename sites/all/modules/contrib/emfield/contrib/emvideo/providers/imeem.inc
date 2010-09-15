<?php
// $Id: imeem.inc,v 1.1.2.7 2009/10/29 13:29:51 aaron Exp $

define('EMVIDEO_IMEEM_MAIN_URL', 'http://www.imeem.com/');
define('EMVIDEO_IMEEM_API_INFO', 'http://www.imeem.com/developers/documentation/');
define('EMVIDEO_IMEEM_API_APPLICATION_URL', 'http://www.imeem.com/developers/');
define('EMVIDEO_IMEEM_XML', 'http://www.imeem.com/api/xml/mediaGetInfo?&amp;mediaIds=');

function emvideo_imeem_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Show related videos'), t('No'), t('This is embedded in the video itself when enabled; currently not available with other providers. Set the feature above.')),
    array(t('Thumbnails'), t('Yes'), t('May not currently resize thumbnails. Must have an API key for thumbnails at the moment, although research is underway to determine an alternative to this. Set your API key above.')),
  );
  return array(
    'provider' => 'imeem',
    'name' => t('IMEEM'),
    'url' => EMVIDEO_IMEEM_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@imeem" target="_blank">IMEEM</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@imeem' => EMVIDEO_IMEEM_MAIN_URL, '@api' => EMVIDEO_IMEEM_API_INFO)),
    'supported_features' => $features,
  );
}

function emvideo_imeem_settings() {
  $form['imeem']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('IMEEM API'),
    '#description' => t('If you wish to be able to display IMEEM thumbnails automatically, you will first need to apply for an API Developer Key from the <a href="@imeem" target="_blank">IMEEM Developer Profile page</a>. Note that you do not need this key to display IMEEM videos themselves.', array('@imeem' => EMVIDEO_IMEEM_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['imeem']['api']['emvideo_imeem_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('IMEEM API Key'),
    '#default_value' => variable_get('emvideo_imeem_api_key', ''),
    '#description' => t('Please enter your IMEEM Developer Key here.'),
  );
  $form['imeem']['api']['emvideo_imeem_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('IMEEM API Secret'),
    '#default_value' => variable_get('emvideo_imeem_api_secret', ''),
    '#description' => t('If you have a secret for the IMEEM API, enter it here.'),
  );
  return $form;
}

function emvideo_imeem_request($embed, $cacheable = TRUE) {
  $args = array('docid' => $embed);
  return module_invoke('emfield', 'request_xml', 'imeem', EMVIDEO_IMEEM_XML . $embed .'&amp;version=1.0', $args, $cacheable);
}

function emvideo_imeem_extract($embed = '') {
  return array(
    '@http://media\.imeem\.com/[a-z]{1,2}/([^"\?/]+)/@i',
  );
}

function emvideo_imeem_embedded_link($video_code) {
  return 'http://www.imeem.com/v/'. $video_code;
}

function theme_emvideo_imeem_flash($embed, $width, $height, $autoplay) {
  $autoplay = $autoplay ? '/aus=true' : '/aus=false';
  $colors = 'backColor=666666&amp;primaryColor=cccccc&amp;secondaryColor=333333&amp;linkColor=cccccc';

  $output .= '<object width="'. $width .'" height="'. $height .'">';
  $output .= '<param name="movie" value="http://media.imeem.com/v/'. $embed . $autoplay .'/pv=2"></param>';
  $output .= '<param name="allowFullScreen" value="true"></param>';
  $output .= '<param name="wmode" value="transparent"></param>';
  $output .= '<param name="FlashVars" value="'. $colors .'"></param>';
  $output .= '<embed src="http://media.imeem.com/v/'. $embed . $autoplay .'/pv=2" type="application/x-shockwave-flash" width="'. $width .'" height="'. $height .'" allowFullScreen="true" wmode="transparent" FlashVars="'. $colors .'"></embed>';
  $output .= '</object>';
  return $output;
}

function emvideo_imeem_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $xml = emvideo_imeem_request($item['value']);
  $tmpthm = $xml['ITEM']['ICONURL'][0];
  $imgmax = $xml['ITEM']['DURATION'][0];
  $formatnum = str_pad(rand(0, $imgmax) .'.jpg', 9, "0", STR_PAD_LEFT);;
  $thm = str_replace('00010.jpg', $formatnum, $tmpthm);
  return $thm;
}

function emvideo_imeem_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_imeem_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_imeem_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_imeem_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_imeem_emfield_subtheme() {
  return array(
    'emvideo_imeem_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/imeem.inc'
    )
  );
}
