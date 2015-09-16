<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_field_region' field handler.
 */

/**
 * Field handler to display visitor's region.
 */
class smart_ip_views_bridge_handler_field_region extends views_handler_field {

  function query() {
    $this->field_alias = 'smart_ip_views_bridge_region_'. $this->position;
    $this->query->add_field('', 1, $this->field_alias);
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['style'] = array('default' => 'name');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['style'] = array(
      '#title'   => t('Display style'),
      '#type'    => 'select',
      '#options' => array(
        'name' => t('Region name'),
        'code' => t('Region code (FIPS)'),
      ),
      '#default_value' => $this->options['style'],
    );
  }

  function render($values) {
    $smart_ip_session = smart_ip_session_get('smart_ip');
    $region = '';
    $region_code = '';
    if (!empty($smart_ip_session['location']['region'])) {
       $region = $smart_ip_session['location']['region'];
    }
    if (!empty($smart_ip_session['location']['region_code'])) {
       $region_code = $smart_ip_session['location']['region_code'];
    }
    if ($this->options['style'] == 'name') {
      return $region;
    }
    else {
      return $region_code;
    }
  }
}
