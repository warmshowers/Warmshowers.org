<?php

/**
 * @file
 * Field handler to display the topic pager.
 */

class advanced_forum_handler_field_node_topic_pager extends views_handler_field{
  function construct() {
    parent::construct();
    $this->additional_fields = array('nid' => 'nid', 'type' => 'type');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['total_pages_shown'] = array('default' => 6);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['total_pages_shown'] = array(
      '#title' => t('Total pages shown'),
      '#description' => t('Total number of page numbers to show, including initial numbers and one final. 0 hides pager.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['total_pages_shown'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
    $this->field_alias = $this->table . '_' . $this->field;
  }

  function render($values) {
    // Make a fake topic object with the information that the core one has.
    $topic = new stdClass();
    $topic->nid = $values->nid;
    $topic->type = $values->node_type;
    $topic->num_comments = $values->node_comment_statistics_comment_count;

    return theme('advanced_forum_topic_pager', $this->options['total_pages_shown'], $topic);
  }
}
