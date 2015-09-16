<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_field_longitude' markup field handler.
 */

/**
 * Field handler to display visitor's longitude.
 */
class smart_ip_views_bridge_handler_field_longitude extends smart_ip_views_bridge_handler_field_coordinates {

  function construct() {
    parent::construct();
  }

  function render($values) {
    $smart_ip_session = smart_ip_session_get('smart_ip');
    $longitude = '';
    if (!empty($smart_ip_session['location']['longitude'])) {
       $longitude = $smart_ip_session['location']['longitude'];
    }
    if ($this->options['style'] == 'dd') {
      return $longitude;
    }
    else {
      return theme('smart_ip_longitude_dms', array('view' => $longitude));
    }
  }
}
