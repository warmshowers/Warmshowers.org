<?php

/**
 * @file
 * Token hooks for the uc_store module.
 */

/**
 * Implements hook_token_info().
 */
function uc_store_token_info() {
  $type = array(
    'name' => t('Store information'),
    'description' => t('Tokens for store-specific, but globally available, information.'),
  );

  $site['login-link'] = array(
    'name' => t('Login URL'),
    'description' => t('A link to the site login page.'),
  );
  $site['logo'] = array(
    'name' => t('Logo'),
    'description' => t('The image showing the site logo.'),
  );

  $store['name'] = array(
    'name' => t('Store name'),
    'description' => t('The Ubercart store name.'),
  );
  $store['link'] = array(
    'name' => t('Store link'),
    'description' => t('A link to the Ubercart store using the store name.'),
  );
  $store['owner'] = array(
    'name' => t('Owner'),
    'description' => t('The Ubercart store owner.'),
  );
  $store['email'] = array(
    'name' => t('Email'),
    'description' => t('The Ubercart store e-mail address.'),
  );
  $store['phone'] = array(
    'name' => t('Phone number'),
    'description' => t('The Ubercart store phone number.'),
  );
  $store['fax'] = array(
    'name' => t('Fax number'),
    'description' => t('The Ubercart store fax number.'),
  );
  $store['address'] = array(
    'name' => t('Address'),
    'description' => t('The Ubercart store mailing address.'),
  );
  $store['help-url'] = array(
    'name' => t('Help page URL'),
    'description' => t('The URL to the store help page.'),
    'type' => 'url',
  );

  return array(
    'types' => array('store' => $type),
    'tokens' => array(
      'site' => $site,
      'store' => $store,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function uc_store_tokens($type, $tokens, $data = array(), $options = array()) {
  $replacements = array();

  if ($type == 'site') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'login-link':
          $login_link = url('user', array('absolute' => TRUE));
          $replacements[$original] = l($login_link, $login_link);
          break;

        case 'logo':
          // Use a logo; but only if we have one to use.
          $replacements[$original] = '';
          if ($uri = theme_get_setting('logo')) {
            $replacements[$original] = theme('image', array('path' => $uri));
          }
          break;
      }
    }
  }

  if ($type == 'store') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'name':
          $replacements[$original] = uc_store_name();
          break;

        case 'link':
          $replacements[$original] = l(uc_store_name(), '<front>', array('absolute' => TRUE));
          break;

        case 'owner':
          $replacements[$original] = variable_get('uc_store_owner', '');
          break;

        case 'email':
          $replacements[$original] = uc_store_email();
          break;

        case 'phone':
          $replacements[$original] = variable_get('uc_store_phone', '');
          break;

        case 'fax':
          $replacements[$original] = variable_get('uc_store_fax', '');
          break;

        case 'address':
          $replacements[$original] = uc_store_address();
          break;

        case 'help-url':
          $replacements[$original] = url(variable_get('uc_store_help_page', ''), array('absolute' => TRUE));
          break;
      }
    }

    // Handle chaining for tokens that have 'type' defined in hook_token_info()
    if ($link_tokens = token_find_with_prefix($tokens, 'help-url')) {
      $replacements += token_generate('url', $link_tokens, array('path' => variable_get('uc_store_help_page', '')), $options);
    }
  }

  return $replacements;
}
