<?php

/**
 * @file
 * Field handler to display the topic icon.
 */

class advanced_forum_handler_field_node_topic_icon extends views_handler_field{
  function construct() {
    parent::construct();
    $this->additional_fields = array('nid' => 'nid', 'type' => 'type');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['hot_topic_threshold'] = array('default' => 15);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['hot_topic_threshold'] = array(
      '#title' => t('Hot topic threshold'),
      '#description' => t('The number of posts a topic must have to be considered "hot".'),
      '#type' => 'textfield',
      '#default_value' => $this->options['hot_topic_threshold'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
    $this->field_alias = $this->table . '_' . $this->field;
  }

  function render($values) {
    $new_posts = advanced_forum_reply_num_new($values->nid);
    return theme('forum_icon', $new_posts, $values->node_comment_statistics_comment_count, $values->node_comment, $values->node_sticky);
  }
}
