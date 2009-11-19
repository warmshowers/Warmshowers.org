<?php
// $Id: filefield_meta_handler_field_duration.inc,v 1.2 2009/03/19 03:43:37 quicksketch Exp $

/**
 * @file
 * A special handler that properly formats duration fields as minutes:seconds.
 */

/**
 * Render a field as a readable value in hours, minutes, and seconds.
 *
 * @ingroup views_field_handlers
 */
class filefield_meta_handler_field_duration extends views_handler_field_numeric {
  function option_definition() {
    $options = parent::option_definition();

    $options['format'] = array('default' => 'default', 'translatable' => TRUE);

    // Remove the separator and alter options since we don't need them.
    unset($options['separator']);
    unset($options['alter']);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Remove the separator and alter options since we don't need them.
    unset($form['separator']);
    unset($form['alter']);

    $form['prefix']['#weight'] = 10;
    $form['suffix']['#weight'] = 10;
    $form['format'] = array(
      '#type' => 'select',
      '#title' => t('Time format'),
      '#default_value' => $this->options['format'],
      '#options' => array(
        'default' => t('Default (usually mm:ss)'),
        'hours' => t('Hours: h'),
        'minutes' => t('Minutes: mm'),
        'seconds' => t('Seconds: ss'),
        'total' => t('Total seconds'),
      ),
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    switch ($this->options['format']) {
      case 'hours':
        $output = date('g', (int) $value);
        break;
      case 'minutes':
        $output = date('i', (int) $value);
        break;
      case 'seconds':
        $output = date('s', (int) $value);
        break;
      case 'total':
        $output = $value;
        break;
      default:
        $output = theme('filefield_meta_duration', $value);
    }

    return check_plain($this->options['prefix']) . $output . check_plain($this->options['suffix']);
  }
}
