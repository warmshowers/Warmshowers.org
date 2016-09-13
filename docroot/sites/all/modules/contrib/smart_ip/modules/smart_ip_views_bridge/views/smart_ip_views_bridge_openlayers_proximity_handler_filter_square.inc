<?php

class smart_ip_views_bridge_openlayers_proximity_handler_filter_square extends openlayers_proximity_handler_filter_square {

  /**
   * Process operator.
   */
  function op_process($op = 'simple', $field) {
    if (isset($this->view->exposed_input) && !empty($this->view->exposed_input)) {
      $exposed_input = $this->view->exposed_input['flat']['location'];
      $this->options['value']['location'] = $exposed_input;
      foreach ($this->value as $key => $value) {
        if (is_numeric($key)) {
          $this->value[$key]['location'] = $exposed_input;
        }
        elseif ($key == 'location') {
          $this->value['location'] = $exposed_input;
        }
      }
    }
    else {
      $token = $this->options['value']['location'];
      smart_ip_views_bridge_substitute_token_value($this->options['value']['location'], $token);
      foreach ($this->value as $key => $value) {
        if (is_numeric($key)) {
          //smart_ip_views_bridge_substitute_token_value($this->value[$key]['location'], $token);
        }
        elseif ($key == 'location') {
          smart_ip_views_bridge_substitute_token_value($this->value['location'], $token);
        }
      }
    }
    //$this->table = 'openlayers_proximity';
    $this->table_alias = 'openlayers_proximity';
    //$this->field = 'flat';
    // $this->real_field = 'flat';
    // $this->options['table'] = 'openlayers_proximity';
    // $this->options['field'] = 'flat';
    parent::op_process($op, $field);
  }

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
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
}