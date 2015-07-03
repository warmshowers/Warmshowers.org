<?php

/**
 * @file
 * Definition of variables for Variable API module.
 */

/**
 * Implements hook_variable_info().
 */
function robotstxt_variable_info($options = array()) {
  $variables['robotstxt'] = array(
    'title' => t('Contents of robots.txt', array(), $options),
    'type' => 'text',
    'group' => 'robotstxt',
    'localize' => TRUE,
    'multidomain' => TRUE,
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function robotstxt_variable_group_info() {
  $groups['robotstxt'] = array(
    'title' => t('RobotsTxt'),
    'description' => t('Configure robots.txt file.'),
    'access' => 'administer robots.txt',
    'path' => array('admin/config/search/robotstxt'),
  );

  return $groups;
}
