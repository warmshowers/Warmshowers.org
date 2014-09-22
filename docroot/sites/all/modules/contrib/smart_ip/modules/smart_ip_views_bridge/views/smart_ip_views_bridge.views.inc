<?php
// $Id$

/**
* @file
* Views hooks implemention.
*/

/**
 * Implements hook_views_handlers().
 *
 * @return array
 */
function smart_ip_views_bridge_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'smart_ip_views_bridge') .'/views',
    ),
    'handlers' => array(
      'smart_ip_views_bridge_handler_field_coordinates' => array(
        'parent' => 'views_handler_field',
      ),
      'smart_ip_views_bridge_handler_field_latitude' => array(
        'parent' => 'smart_ip_views_bridge_handler_field_coordinates',
      ),
      'smart_ip_views_bridge_handler_field_longitude' => array(
        'parent' => 'smart_ip_views_bridge_handler_field_coordinates',
      ),
      'smart_ip_views_bridge_handler_field_country' => array(
        'parent' => 'views_handler_field',
      ),
      'smart_ip_views_bridge_handler_field_region' => array(
        'parent' => 'views_handler_field',
      ),
      'smart_ip_views_bridge_handler_field_city' => array(
        'parent' => 'views_handler_field',
      ),
      'smart_ip_views_bridge_handler_field_zip' => array(
        'parent' => 'views_handler_field',
      ),
      'smart_ip_views_bridge_openlayers_proximity_handler_field' => array(
        'parent' => 'openlayers_proximity_handler_field',
      ),
      'smart_ip_views_bridge_openlayers_proximity_handler_sort' => array(
        'parent' => 'openlayers_proximity_handler_sort',
      ),
      'smart_ip_views_bridge_openlayers_proximity_handler_filter_square' => array(
        'parent' => 'openlayers_proximity_handler_filter_square',
      ),
      'smart_ip_views_bridge_openlayers_proximity_handler_filter_circle' => array(
        'parent' => 'openlayers_proximity_handler_filter_circle',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 *
 * @return array
 */
function smart_ip_views_bridge_views_data() {
  $data['smartip']['table']['group'] = t('Smart IP');
  $data['smartip']['table']['join'] = array(
    '#global' => array(),
  );
  
  $data['smartip']['latitude'] = array(
    'title' => t('Latitude'),
    'help'  => t("Display visitor's latitude."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_latitude',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['longitude'] = array(
    'title' => t('Longitude'),
    'help'  => t("Display visitor's longitude."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_longitude',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['coordinates'] = array(
    'title' => t('Coordinates'),
    'help'  => t("Display visitor's coordinates."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_coordinates',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['country'] = array(
    'title' => t('Country'),
    'help'  => t("Display visitor's country."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_country',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['region'] = array(
    'title' => t('Region'),
    'help'  => t("Display visitor's region."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_region',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['city'] = array(
    'title' => t('City'),
    'help'  => t("Display visitor's city."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_city',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['zip'] = array(
    'title' => t('ZIP code'),
    'help'  => t("Display visitor's ZIP code."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_zip',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  $data['smartip']['distance'] = array(
    'title' => t('Openlayers proximity distance'),
    'help' => t('Distance from a give point.'),
    'sort' => array(
      'handler' => 'smart_ip_views_bridge_openlayers_proximity_handler_sort',
    ),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_openlayers_proximity_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['smartip']['flat'] = array(
    'title' => t('Openlayers proximity square'),
    'help' => t('Gives locations contained within a square derived by a simple latitude/longitude comparison.'),
    'filter' => array(
      'handler' => 'smart_ip_views_bridge_openlayers_proximity_handler_filter_square',
    ),
  );
  $data['smartip']['circle'] = array(
    'title' => t('Openlayers proximity great-circle'),
    'help' => t('Uses the Great-circle distance formula to return locations within a circular area.'),
    'filter' => array(
      'handler' => 'smart_ip_views_bridge_openlayers_proximity_handler_filter_circle',
    ),
  );
  
  return $data;
}