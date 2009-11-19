<?php
// $Id: i18n.admin.inc,v 1.2.2.7 2009/01/21 13:08:35 jareyero Exp $

/**
 * @file
 * Extended multilanguage administration and module settings UI.
 */

/**
 * Form builder function.
 *
 * TO DO: Add exclude paths for content selection
 * Some options have been removed from previous versions:
 * - Languages are now taken from locale module unless defined in settings file.
 * - Language dependent tables are authomatically used if defined in settings file.
 */
function i18n_admin_settings() {
  // Content selection options.
  $form['selection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content selection'),
    //'#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );
  $form['selection']['i18n_selection_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Content selection mode'),
    '#default_value' => variable_get('i18n_selection_mode', 'simple'),
    '#options' => _i18n_selection_mode(),
    '#description' => t('Determines which content to show depending on the current page language and the default language of the site.'),
  );

  // Node translation links setting.
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content translation links'),
  );
  $form['links']['i18n_hide_translation_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide content translation links'),
    '#description' => t('Hide the links to translations in content body and teasers. If you choose this option, switching language will only be available from the language switcher block.'),
    '#default_value' => variable_get('i18n_hide_translation_links', 0),
  );
  $form['links']['i18n_translation_switch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Switch interface for translating'),
    '#default_value' => variable_get('i18n_translation_switch', 0),
    '#description' => t('Switch interface language to fit node language when creating or editing a translation. If not checked the interface language will be independent from node language.'),
  );
  return system_settings_form($form);
}

// List of selection modes
function _i18n_selection_mode() {
  return array(
    'simple' => t('Current language and language neutral.'),
    'mixed' => t('Mixed current language (if available) or default language (if not) and language neutral.'),
    'default' => t('Only default language and language neutral.'),
    'strict' => t('Only current language.'),
    'off' => t('All content. No language conditions apply.'),
  );
}

