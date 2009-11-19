<?php
// $Id: abstract.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Support file for ABSTRACT meta tag.
 */

function nodewords_abstract_form($type, $value, $settings) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Abstract'),
    '#default_value' => $value,
    '#size' => 60,
    '#maxlength' => $settings['max_size'],
    '#description' => t('Enter a short abstract for this page. Typically it is one sentence.'),
  );
}
