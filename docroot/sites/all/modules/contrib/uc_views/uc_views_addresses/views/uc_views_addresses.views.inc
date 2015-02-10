<?php
/**
 * @file
 * Created with http://drupal.org/project/code_gen
 * Views 2 hooks and callback registries.
 */

/**
 * Implementation of hook_views_data().
 */
function uc_views_addresses_views_data() {
  $data = array();

   // Definitions for uc_addresses
  $data['uc_addresses']['table']['group'] = t('Ubercart Addresses');
  $uc_addresses = drupal_get_schema('uc_addresses');
  //serial
  $data['uc_addresses']['table']['base'] = array(
    'field' => 'aid',
    'title' => t('Ubercart Addresses'),
    'help' => t('User contact address'),
    'weight' => -10,
  );
  $data['uc_addresses']['aid'] = array(
    'title' => t('Address ID'),
    'help' => $uc_addresses['fields']['aid']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'uc_views_addresses_handler_filter_default_address',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['uc_addresses']['uid'] = array(
    'title' => 'User ID',
    'help' => $uc_addresses['fields']['uid']['description'],
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('User addresses'),
    ),
    'field' => array(
        'handler' => 'views_handler_field_user',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_user_current',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
    ),
  );
  $data['uc_addresses']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );
  //varchar
  $data['uc_addresses']['first_name'] = array(
    'title' => uc_get_field_name('first_name'),
    'help' => $uc_addresses['fields']['first_name']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['uc_addresses']['last_name'] = array(
    'title' => uc_get_field_name('last_name'),
    'help' => $uc_addresses['fields']['last_name']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['uc_addresses']['phone'] = array(
    'title' => uc_get_field_name('phone'),
    'help' => $uc_addresses['fields']['phone']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['uc_addresses']['company'] = array(
    'title' => uc_get_field_name('company'),
    'help' => $uc_addresses['fields']['company']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['uc_addresses']['street1'] = array(
    'title' => uc_get_field_name('street1'),
    'help' => $uc_addresses['fields']['street1']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['uc_addresses']['street2'] = array(
    'title' => uc_get_field_name('street2'),
    'help' => $uc_addresses['fields']['street2']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['uc_addresses']['city'] = array(
    'title' => uc_get_field_name('city'),
    'help' => $uc_addresses['fields']['city']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //int
  $data['uc_addresses']['zone'] = array(
    'title' => uc_get_field_name('zone'),
    'help' => $uc_addresses['fields']['zone']['description'],
    'field' => array(
        'handler' => 'uc_views_handler_field_zone',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'uc_views_handler_filter_zone',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //varchar
  $data['uc_addresses']['postal_code'] = array(
    'title' => uc_get_field_name('postal_code'),
    'help' => $uc_addresses['fields']['postal_code']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //int
  $data['uc_addresses']['country'] = array(
    'title' => uc_get_field_name('country'),
    'help' => $uc_addresses['fields']['country']['description'],
    'field' => array(
        'handler' => 'uc_views_handler_field_country',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'uc_views_handler_filter_country',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //varchar
  $data['uc_addresses']['address_name'] = array(
    'title' => 'Address name',
    'help' => $uc_addresses['fields']['address_name']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //int
  $data['uc_addresses']['created'] = array(
    'title' => 'Created',
    'help' => $uc_addresses['fields']['created']['description'],
    'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['uc_addresses']['modified'] = array(
    'title' => 'Modified',
    'help' => $uc_addresses['fields']['modified']['description'],
    'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
 // Definitions for uc_addresses_defaults
  $data['uc_addresses_defaults']['table']['group'] = t('uc addresses defaults');
  $uc_addresses_defaults = drupal_get_schema('uc_addresses_defaults');
  //int
  $data['uc_addresses_defaults']['aid'] = array(
    'title' => 'Address ID',
    'help' => $uc_addresses_defaults['fields']['aid']['description'],
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['uc_addresses_defaults']['uid'] = array(
    'title' => 'User ID',
    'help' => $uc_addresses_defaults['fields']['uid']['description'],
    'field' => array(
        'handler' => 'views_handler_field_user',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_user_current',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
    ),
  );
  $data['uc_addresses_defaults']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );


  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function uc_views_addresses_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_views_addresses') . '/views',
    ),
    'handlers' => array(
      // fields
      //fiters
      'uc_views_addresses_handler_filter_default_address' => array('parent' => 'views_handler_filter_boolean_operator',),
    ),
  );
}
