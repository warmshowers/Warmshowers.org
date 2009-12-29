<?php
// $Id: htmlmail.admin.inc,v 1.1.2.3 2009/07/13 08:25:29 chrisherberte Exp $

function htmlmail_admin_settings() {
  $form['htmlmail_autop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Line break converter'),
    '#default_value' => variable_get('htmlmail_autop', '1'),
    '#description' => t('Converts line breaks into HTML (i.e. &lt;br&gt; and &lt;p&gt; tags, ala filter.module).'),
  );
  $form['htmlmail_urlfilter'] = array(
    '#type' => 'checkbox',
    '#title' => t('URL Filter'),
    '#default_value' => variable_get('htmlmail_urlfilter', '1'),
    '#description' => t('Automatically converts text web addresses (URLs, e-mail addresses, ftp links, etc.) into hyperlinks.'),
  );
  $form['htmlmail_emogrifier'] = array(
    '#type' => 'checkbox',
    '#title' => t('Emogrifier'),
    '#default_value' => variable_get('htmlmail_emogrifier', '0'),
    '#description' => t('Emogrifier automagically transmogrifies your HTML by parsing your CSS and inserting your CSS definitions into tags within your HTML based on your CSS selectors.<br /><strong><a href="http://www.pelagodesign.com/sidecar/emogrifier">Emogrifier</a></strong> needs to be installed in the module\'s emogrifier directory. <em>Recommended when using CSS.</em>'),
  );
  $form['htmlmail_preformat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preformat HTML code'),
    '#default_value' => variable_get('htmlmail_preformat', '1'),
    '#description' => t('Automagically inserts the &lt;HTML&gt; &lt;HEAD&gt; &lt;BODY&gt; tags, untick if you want control over this. Leave checked when using a WYSIWYG editor.<br /><em>Recommended when using template.</em>'),
  );
  return system_settings_form($form);
}

function htmlmail_template_settings() {
  $form['htmlmail_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Header HTML'),
    '#default_value' => variable_get('htmlmail_header', ''),
  );
  $form['htmlmail_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer HTML'),
    '#default_value' => variable_get('htmlmail_footer', ''),
  );
  $form['htmlmail_css'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS'),
    '#default_value' => variable_get('htmlmail_css', ''),
  );
  return system_settings_form($form);
}

function htmlmail_test_form($form_values = NULL) {
  $defaults = variable_get('htmlmail_test', array());
  $form['to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => $defaults['to'],
    '#maxlength' => 128,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $defaults['subject'],
    '#maxlength' => 128,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#rows' => 20,
    '#default_value' => $defaults['body'],
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function htmlmail_test_form_submit($form, &$form_state) {
  variable_set('htmlmail_test', array(
    'to' => $form_state['values']['to'],
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'])
  );
  $params = array(
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'],
  );
  if (drupal_mail('htmlmail', 'htmlmail_test', $form_state['values']['to'], language_default(), $params)) {
    drupal_set_message('HTML Mail test message sent.');
  }
}

