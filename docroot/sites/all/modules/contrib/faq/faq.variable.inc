<?php

/**
 * @file
 * FAQ module variables file.
 */

/**
 * Implements hook_variable_info().
 */
function faq_variable_info($options) {

  $variable['faq_title'] = array(
    'title' => t('FAQ page title', array(), $options),
    'description' => t('FAQ page title', array(), $options),
    'type' => 'string',
    'access' => 'administer faq',
  );

  $variable['faq_description'] = array(
    'title' => t('FAQ page description', array(), $options),
    'description' => t('FAQ page description', array(), $options),
    'type' => 'string',
    'access' => 'administer faq',
  );

  $variable['faq_question_label'] = array(
    'title' => t('FAQ question label', array(), $options),
    'description' => t('FAQ question label', array(), $options),
    'type' => 'string',
    'access' => 'administer faq',
  );

  $variable['faq_answer_label'] = array(
    'title' => t('FAQ answer label', array(), $options),
    'description' => t('FAQ answer label', array(), $options),
    'type' => 'string',
    'access' => 'administer faq',
  );

  $variable['faq_back_to_top'] = array(
    'title' => t('FAQ "back to top" link', array(), $options),
    'description' => t('FAQ "back to top" link', array(), $options),
    'type' => 'string',
    'access' => 'administer faq',
  );

  return $variable;
}
