<?php

/**
 * Handler that supports putting flagged content at the top or bottom
 * (compared to unflagged content).
 */
class views_handler_sort_flag_weights extends views_handler_sort {
  function option_definition() {
    $options = parent::option_definition();

    $options['flagorder'] = array('default' => 'NONE');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array(
      'NONE' => t('Default order'),
      'ASC' => t('Flagged at top'),
      'DESC' => t('Flagged at bottom'),
    );

    $form['flagorder'] = array(
      '#title' => t('Ordering of Flagged Values'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $this->options['flagorder'],
    );
  }

  function query() {
    if ($this->options['flagorder'] != 'NONE') {
      $this->ensure_my_table();
      $this->query->add_orderby(NULL, $this->table_alias . '.' . $this->real_field . ' IS NULL', $this->options['flagorder']);
    }
    parent::query();
  }
}
