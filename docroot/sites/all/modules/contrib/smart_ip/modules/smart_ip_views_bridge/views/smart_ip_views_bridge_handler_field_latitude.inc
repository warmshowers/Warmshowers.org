<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_field_latitude' field handler.
 */

/**
 * Field handler to display visitor's latitude.
 */
class smart_ip_views_bridge_handler_field_latitude extends smart_ip_views_bridge_handler_field_coordinates {

  function construct() {
    parent::construct();
  }

  function render($values) {
    $smart_ip_session = smart_ip_session_get('smart_ip');
    $latitude = '';
    if (!empty($smart_ip_session['location']['latitude'])) {
       $latitude = $smart_ip_session['location']['latitude'];
    }
    if ($this->options['style'] == 'dd') {
      return $latitude;
    }
    else {
      return theme('smart_ip_longitude_dms', array('view' => $latitude));
    }
  }
}
