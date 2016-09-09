<?php
// $Id$

/**
* @file
* Views hooks implemention.
*/

/**
 * Implements hook_views_query_alter().
 */
function smart_ip_views_bridge_views_query_alter(&$view, &$query) {
  foreach ($query->where as $where_index => $items) {
    foreach ($items as $item_index => $conditions) {
      if (is_array($conditions)) {
        foreach ($conditions as $condition_index => $condition) {
          $condition_value = $condition['value'];
          if (is_array($condition_value)) {
            foreach ($condition_value as $value) {
              $smart_ip_index_count = 0;
              if (!is_array($value)) {
                $smart_ip_index = explode('][', $value);
                $smart_ip_index_count = count($smart_ip_index);
              }
              if ($smart_ip_index_count == 1) {
                $smart_ip_index = explode('.', $value);
                $smart_ip_index_count = count($smart_ip_index);
              }
              elseif ($smart_ip_index_count == 3 && isset($_SESSION[$smart_ip_index[0]][$smart_ip_index[1]][$smart_ip_index[2]])) {
                $values[] = $_SESSION[$smart_ip_index[0]][$smart_ip_index[1]][$smart_ip_index[2]];
              }
            }
            if (isset($values)) {
              $view->query->where[$where_index][$item_index][$condition_index]['value'] = $values;
            }
          }
          else {
            $smart_ip_index = explode('][', $condition_value);
            $smart_ip_index_count = count($smart_ip_index);
            if ($smart_ip_index_count == 1) {
              $smart_ip_index = explode('.', $condition_value);
              $smart_ip_index_count = count($smart_ip_index);
            }
            if ($smart_ip_index_count == 3 && isset($_SESSION[$smart_ip_index[0]][$smart_ip_index[1]][$smart_ip_index[2]])) {
              $view->query->where[$where_index][$item_index][$condition_index]['value'] = $_SESSION[$smart_ip_index[0]][$smart_ip_index[1]][$smart_ip_index[2]];
            }
          }
        }
      }
    }
  }
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
    ),
  );
  $data['smartip']['longitude'] = array(
    'title' => t('Longitude'),
    'help'  => t("Display visitor's longitude."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_longitude',
      'click sortable' => FALSE,
    ),
  );
  $data['smartip']['coordinates'] = array(
    'title' => t('Coordinates'),
    'help'  => t("Display visitor's coordinates."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_coordinates',
      'click sortable' => FALSE,
    ),
  );
  $data['smartip']['country'] = array(
    'title' => t('Country'),
    'help'  => t("Display visitor's country."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_country',
      'click sortable' => FALSE,
    ),
  );
  $data['smartip']['region'] = array(
    'title' => t('Region'),
    'help'  => t("Display visitor's region."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_region',
      'click sortable' => FALSE,
    ),
  );
  $data['smartip']['city'] = array(
    'title' => t('City'),
    'help'  => t("Display visitor's city."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_city',
      'click sortable' => FALSE,
    ),
  );
  $data['smartip']['zip'] = array(
    'title' => t('ZIP code'),
    'help'  => t("Display visitor's ZIP code."),
    'field' => array(
      'handler' => 'smart_ip_views_bridge_handler_field_zip',
      'click sortable' => FALSE,
    ),
  );
  if (module_exists('openlayers_proximity')) {
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
  }
  
  return $data;
}