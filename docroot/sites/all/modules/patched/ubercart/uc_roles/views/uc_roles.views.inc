<?php

/**
 * @file
 * Views hooks for Ubercart roles.
 */

/**
 * Implements hook_views_data().
 */
function uc_roles_views_data() {
  $data['uc_roles_expirations']['table']['group']  = t('User');

  $data['uc_roles_expirations']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Expose the role expiration date.
  $data['uc_roles_expirations']['expiration'] = array(
    'title' => t('Ubercart Role expiration date/time'),
    'help' => t('Date and time the role will expire. (See also Role expiration role.)'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Expose the role id from uc_roles_expirations.
  $data['uc_roles_expirations']['rid'] = array(
    'title' => t('Ubercart Role expiration role'),
    'help' => t('The Role that corresponds with the Role expiration date/time'),
    // Information for displaying the rid
    'field' => array(
      'handler' => 'uc_roles_handler_field_rid',
      'click sortable' => TRUE,
    ),
    // Information for accepting a rid as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_users_roles_rid',
      'name field' => 'title', // The field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'rid',
    ),
    // Information for accepting a uid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_user_roles',
    ),
    // Information for sorting on a uid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
