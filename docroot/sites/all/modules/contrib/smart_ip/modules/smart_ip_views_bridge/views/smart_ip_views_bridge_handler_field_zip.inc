<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_field_zip' field handler.
 */

/**
 * Field handler to display visitor's zip.
 */
class smart_ip_views_bridge_handler_field_zip extends views_handler_field {

  function query() {
    $this->field_alias = 'smart_ip_views_bridge_zip_'. $this->position;
    $this->query->add_field('', 1, $this->field_alias);
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $smart_ip_session = smart_ip_session_get('smart_ip');
    $zip = '';
    if (!empty($smart_ip_session['location']['zip'])) {
      $zip = $smart_ip_session['location']['zip'];
    }
    return $zip;
  }
}
