<?php

/**
 * @file
 * Views field handler for the views_watchdog module.
 */


/**
 * Provides message display options for the watchdog entry.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_watchdog_message extends views_handler_field {


  function option_definition() {

    $options = parent::option_definition();

    $options['watchdog_message_format'] = array('default' => TRUE);
    $options['watchdog_message_link'] = array('default' => TRUE);

    return $options;
  }


  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    $form['watchdog_message_format'] = array(
      '#type' => 'checkbox',
      '#title' => t('Format this field'),
      '#description' => t('Format as log message otherwise raw text.'),
      '#default_value' => !empty($this->options['watchdog_message_format']),
    );
    $form['watchdog_message_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link this field to its message'),
      '#description' => t('This will override any other link you have set.'),
      '#default_value' => !empty($this->options['watchdog_message_link']),
    );
  }


  function render($values) {

    $value = $values->{$this->field_alias};
    $variables = $values->{$this->aliases['variables']};

    $message = !empty($this->options['watchdog_message_format']) ? _views_watchdog_format_message($value, $variables) : $value;

    return $this->render_link($message, $values);
  }


  function render_link($data, $values) {

    $wid = $values->{$this->aliases['wid']};

    if (!empty($this->options['watchdog_message_link']) && user_access('access site reports') && $wid && $data !== NULL && $data !== '') {

      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/reports/event/'. $wid;
    }

    return $data;
  }
}
