<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_field_coordinates' field handler.
 */

/**
 * Field handler to display visitor's coordinates.
 */
class smart_ip_views_bridge_handler_field_coordinates extends views_handler_field {

  function query() {
    $this->field_alias = 'smart_ip_views_bridge_coordinates_'. $this->position;
    $this->query->add_field('', 1, $this->field_alias);
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['style'] = array('default' => 'dd');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['style'] = array(
      '#type'  => 'select',
      '#title' => t('Display as'),
      '#default_value' => $this->options['style'],
      '#options' => array('dd' => t('Decimal degrees format'), 'dms' => t('Degrees, minutes, seconds format')),
    );
  }

  function render($values) {
    $smart_ip_session = smart_ip_session_get('smart_ip');
    if ($this->options['style'] == 'dd') {
      return $smart_ip_session['location']['latitude'] . ', ' . $smart_ip_session['location']['longitude'];
    }
    else {
      return theme('smart_ip_longitude_dms', $smart_ip_session['location']['latitude']) . ', ' . theme('smart_ip_longitude_dms', $smart_ip_session['location']['longitude']);
    }
  }
}
