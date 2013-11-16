<?php
// $Id: imagefield.inc,v 1.2 2010/01/08 18:23:12 quicksketch Exp $

/**
 * @file
 * Insert support for ImageField module.
 */

/**
 * Implementation of hook_insert_widgets().
 */
function imagefield_insert_widgets() {
  return array(
    'imagefield_widget' => array(
      'wrapper' => '.filefield-element',
      'fields' => array(
        'alt' => 'input[name$="[alt]"]',
        'title' => 'input[name$="[title]"], textarea[name$="[title]"]',
        'description' => 'input[name$="[description]"], textarea[name$="[description]"]',
      ),
    ),
  );
}
