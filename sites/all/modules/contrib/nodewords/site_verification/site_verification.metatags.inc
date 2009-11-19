<?php
// $Id: site_verification.metatags.inc,v 1.1.2.3 2009/08/11 01:17:36 kiam Exp $

/**
 * @file
 * Support file for the site verification meta tags.
 */

/**
 * @addtogroup site_verification
 * @{
 */

/*****************************************************************************
 * Site verification codes meta tags.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function site_verification_bing_webmaster_center_form(&$form, $content, $options) {
  $form['bing_webmaster_center'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bing Webmaster Center verification code'),
    '#description' => t('<a href="@bing-url">Bing Webmaster Center</a> will ask you to add a META tag in order to verify you have write access to the pages of the web site you registered with their services.', array('@bing-url' => 'http://www.bing.com/webmaster')),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($content['value']),
    '#tree' => TRUE,
  );

  $form['bing_webmaster_center']['value'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function site_verification_google_webmaster_tools_form(&$form, $content, $options) {
  $form['google_webmaster_tools'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Webmaster Tools verification code'),
    '#description' => t('<a href="@google-url">Google Webmaster Tools</a> will ask you to add a META tag in your web site to rovide you with an easy way to make your site more <a href="http://google.com">Google</a>-friendly.', array('@google-url' => 'http://www.google.com/webmasters/tools')),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($content['value']),
    '#tree' => TRUE,
  );

  $form['google_webmaster_tools']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function site_verification_yahoo_site_explorer_form(&$form, $content, $options) {
  $form['yahoo_site_explorer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yahoo! Site Explorer'),
    '#description' => t('<a href="@yahoo-url">Yahoo! Site Explorer</a> will ask you to add a META tag in your web site to allow you to explore all the web pages indexed by <a href="http://search.yahoo.com">Yahoo! Search</a>.', array('@yahoo-url' => 'https://siteexplorer.search.yahoo.com/')),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($content['value']),
    '#tree' => TRUE,
  );

  $form['yahoo_site_explorer']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function site_verification_bing_webmaster_center_prepare(&$tags, $content, $options) {
  if (!empty($content)) {
    $tags['bing_webmaster_center:msvalidate.01'] = $content['value'];
  }
}

/**
 * Set the meta tag content.
 */
function site_verification_google_webmaster_tools_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['google_webmaster_tools:verify-v1'] = $content['value'];
  }
}

/**
 * Set the meta tag content.
 */
function site_verification_yahoo_site_explorer_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['yahoo_site_explorer:y_key'] = $content['value'];
  }
}

/**
 * @} End of "addtogroup site_verification".
 */
