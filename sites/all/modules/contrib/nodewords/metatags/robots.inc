<?php
// $Id: robots.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Support file for ROBOTS meta tag.
 */

function nodewords_robots_prepare($type, $ids, $value, $settings) {
  if (!isset($value) || $value == '') {
    $value = $settings['global']['robots'];
  }
  return $value;
}

function nodewords_robots_form($type, $value, $settings) {
  $options = array(
    '' => t('Use default setting'),
    'index,follow' => 'ALL=INDEX,FOLLOW',
    'noindex,follow' => 'NOINDEX,FOLLOW',
    'index,nofollow' => 'INDEX,NOFOLLOW',
    'noindex,nofollow' => 'NONE=NOINDEX,NOFOLLOW',
  );
  return array(
    '#type' => 'select',
    '#title' => t('Robots meta tag'),
    '#options' => $options,
    '#multiple' => FALSE,
    '#default_value' => $value,
    '#description' => t('The ROBOTS meta tag offers a simple mechanisme to indicate to web robots and crawlers wether the page should be indexed (INDEX or NOINDEX) and whether links on the page should be followed (FOLLOW or NOFOLLOW). The default setting is @default.', array('@default' => $options[$settings['global']['robots']])),
  );
}
