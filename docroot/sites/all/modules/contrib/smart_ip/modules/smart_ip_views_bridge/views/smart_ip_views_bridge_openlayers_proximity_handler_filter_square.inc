<?php

class smart_ip_views_bridge_openlayers_proximity_handler_filter_square extends openlayers_proximity_handler_filter_square {

  /**
   * Process operator.
   */
  function op_process($op = 'simple', $field) {
    if (empty($this->table_alias)) {
      $this->table_alias = 'openlayers_proximity';
    }
    $token = $this->value['location'];
    smart_ip_views_bridge_substitute_token_value($this->value['location'], $token);
    parent::op_process($op, $field);
  }

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    // Remove "Get location from a node" and "Expose this filter to visitors, to allow them to change it" options
    unset($form['location_as']['#options']['node'], $form['expose_button']);
    // Add description showing Smart IP tokens
    $form['value']['location']['#description'] .= smart_ip_views_bridge_form_fields_description();
  }

  /**
   * Render our chunk of the exposed filter form when selecting
   */
  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    if (isset($this->view->exposed_input) && empty($this->view->exposed_input)) {
      // Replace field value with the query input
      $token = $form['flat']['location']['#default_value'];
      smart_ip_views_bridge_substitute_token_value($form['flat']['location']['#default_value'], $token);
      smart_ip_views_bridge_substitute_token_value($form['flat']['location']['#value'], $token);
    }
  }

  /**
   * Validate the exposed filter form
   */
  function exposed_validate(&$form, &$form_state) {
    // This Smart IP views filter doesn't support exposed filter form. Thus, we don't need exposed 
    // filter form validation here (it just causes issues with other exposed filter form if enabled).
    // This handler inherits the parent's filter form validation, to disable it, this function should
    // be present and the parent validation code below should be commented.
    //parent::exposed_validate($form, $form_state);
  }
}