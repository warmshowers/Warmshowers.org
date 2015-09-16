<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_field_country' field handler.
 */

/**
 * Field handler to display visitor's country.
 */
class smart_ip_views_bridge_handler_field_country extends views_handler_field {

  function query() {
    $this->field_alias = 'smart_ip_views_bridge_country_' . $this->position;
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
      '#title' => t('Display style'),
      '#type' => 'select',
      '#options' => array(
        'name' => t('Country name'),
        'code' => t('ISO 3166 2-character country code'),
      ),
      '#default_value' => $this->options['style'],
    );
  }

  function render($values) {
    $smart_ip_session = smart_ip_session_get('smart_ip');
    $country = '';
    $country_code = '';
    if (!empty($smart_ip_session['location']['country'])) {
       $country = $smart_ip_session['location']['country'];
    }
    if (!empty($smart_ip_session['location']['country_code'])) {
       $country_code = $smart_ip_session['location']['country_code'];
    }
    if ($this->options['style'] == 'name') {
      return $country;
    }
    else {
      return $country_code;
    }
  }
}
