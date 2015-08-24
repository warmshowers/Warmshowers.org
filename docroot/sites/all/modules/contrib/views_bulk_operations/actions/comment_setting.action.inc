<?php

function views_bulk_operations_comment_setting_action_info() {
  if (!module_exists('comment')) return array();
  return array(
    'views_bulk_operations_comment_setting_action' => array(
      'type' => 'node',
      'description' => t('Change comment settings'),
      'configurable' => TRUE,
      'behavior' => array('changes_node_property'),
    ),
  );
}

function views_bulk_operations_comment_setting_action(&$node, $context) {
  $node->comment = $context['comment_setting'];
}

function views_bulk_operations_comment_setting_action_form($context) {
  $form['comment_setting'] = array(
    '#type' => 'radios',
    '#title' => t('Comment setting'),
    '#default_value' => !empty($context['comment_setting']) ? $context['comment_setting'] : COMMENT_NODE_DISABLED,
    '#options' => array(
      COMMENT_NODE_DISABLED => t('Disabled'),
      COMMENT_NODE_READ_ONLY => t('Read-only'),
      COMMENT_NODE_READ_WRITE => t('Read/write'),
    ),
    '#required' => TRUE,
  );
  return $form;
}

function views_bulk_operations_comment_setting_action_submit($form, $form_state) {
  return array(
    'comment_setting' => $form_state['values']['comment_setting'],
  );
}

