<?php

/**
 * @file
 * Views filter handler for the views_watchdog module.
 */


/**
 * Provides message filter options for the watchdog entry.
 *
 * @ingroup views_field_handlers
 */
class views_handler_filter_watchdog_message extends views_handler_filter_string {

  var $additional_search_field = 'variables';


  function option_definition() {

    $options = parent::option_definition();

    $options['watchdog_message_search_variables'] = array('default' => TRUE);

    return $options;
  }


  function _options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    $form['watchdog_message_search_variables'] = array(
      '#type' => 'checkbox',
      '#title' => t('Search also in field Variables'),
      '#description' => t('If field Message is formatted, it is recommended to search also in field Variables.'),
      '#default_value' => !empty($this->options['watchdog_message_search_variables']),
    );
  }


  function _query() {

    $this->ensure_my_table();

    $field_message = "$this->table_alias.$this->real_field";
    $field_variables = "$this->table_alias.$this->additional_search_field";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {

      // Search in database field message
      $this->{$info[$this->operator]['method']}($field_message);

      if (!empty($this->options['watchdog_message_search_variables'])) {
        // Search also in database field variables
        $this->{$info[$this->operator]['method']}($field_variables);
        $this->query->set_where_group('OR', $this->options['group']);
      }
    }
  }
}
