<?php

/**
 * @file
 * Views field handler for the views_watchdog module.
 */


/**
 * Provides severity display options for the watchdog entry.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_watchdog_severity extends views_handler_field {


  function option_definition() {

    $options = parent::option_definition();

    $options['watchdog_severity_icon'] = array('default' => TRUE);
    foreach (_views_watchdog_get_severity() as $key => $value) {
      $options['watchdog_severity_icon_'. $value['arg']] = array('default' => $value['icon']);
    }

    return $options;
  }


  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    $form['watchdog_severity_icon'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display severity level as icon'),
      '#default_value' => !empty($this->options['watchdog_severity_icon']),
    );
    foreach (_views_watchdog_get_severity() as $key => $value) {
      $form['watchdog_severity_icon_'. $value['arg']] = array(
        '#type' => 'textfield',
        '#title' => $value['title'],
        '#description' => t('The path to the image file you would like to use as icon.'),
        '#default_value' => !empty($this->options['watchdog_severity_icon_'. $value['arg']]) ? $this->options['watchdog_severity_icon_'. $value['arg']] : $value['icon'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-watchdog-severity-icon' => array(1),
        ),
      );
    }
  }


  function options_validate(&$form, &$form_state) {

    parent::options_validate($form, $form_state);

    foreach (_views_watchdog_get_severity() as $key => $value) {
      $path = trim($form_state['values']['options']['watchdog_severity_icon_'. $value['arg']]);
      if (!empty($path) && !is_file($path)) {
        $field = $form['watchdog_severity_icon_'. $value['arg']];
        form_error($field, t("File doesn't exist."));
      }
    }
  }


  function render($values) {

    $value = $values->{$this->field_alias};

    if (!empty($this->options['watchdog_severity_icon'])) {
      $arg = _views_watchdog_get_severity($value, 'arg');
      $output = _views_watchdog_get_severity_icon($value, $this->options['watchdog_severity_icon_'. $arg]);
    }
    else {
      $output = _views_watchdog_get_severity($value, 'title');
    }

    return $output;
  }
}
