<?php

/**
 * @file
 * Contains this module's extended table style plugin, 
 * which allows rows to be broken into two.
 */

class views_secondary_row_plugin_style_table extends views_plugin_style_table {

/*
  function option_definition() {
    $options = parent::option_definition();

    $options['break'] = array('default' => '');
    $options['separator2'] = array('default' => '');
    $options['colspan'] = array('default' => '');

    return $options;
  }
*/

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['#theme'] = 'views_secondary_row_style_plugin_table';

    $columns = $this->sanitize_columns($this->options['columns']);

    // Create an array of allowed columns from the data we know:
    $field_names = array(t('None')) + $this->display->handler->get_field_labels();

    foreach ($columns as $field => $column) {
      $safe = str_replace(array('][', '_', ' '), '-', $field);
      // the $id of the column for dependency checking.
      $id = 'edit-style-options-columns-' . $safe;

      $form['info'][$field]['break'] = array(
        '#type' => 'select',
        '#default_value' => isset($this->options['info'][$field]['break']) ? $this->options['info'][$field]['break'] : '',
        '#options' => $field_names,
        '#dependency' => array($id => array($field)),
      );
      $form['info'][$field]['separator2'] = array(
        '#type' => 'textfield',
        '#size' => 10,
        '#default_value' => isset($this->options['info'][$field]['separator2']) ? $this->options['info'][$field]['separator2'] : '',
        '#dependency' => array($id => array($field)),
      );
      $form['info'][$field]['colspan'] = array(
        '#type' => 'textfield',
        '#size' => 5,
        '#default_value' => isset($this->options['info'][$field]['colspan']) ? $this->options['info'][$field]['colspan'] : '',
        '#dependency' => array($id => array($field)),
      );
    }
  }
}
