<?php

/**
 * Callback function to supply a list of content types.
 */
function advanced_forum_forum_topic_list_sort_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Forum topic list sort widget'),
    'icon' => 'icon_forum.png',
    'description' => t('A small select widget to change the sort order of the forum topic list'),
    'required context' => new ctools_context_required(t('Forum'), 'forum'),
    'category' => t('Forum'),
    'defaults' => array(),
  );
}

/**
 * Render the content.
 */
function advanced_forum_forum_topic_list_sort_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return;
  }

  if (empty($context)) {
    $tid = 0;
  }
  else {
    $tid = $context->data->tid;
  }

  if (empty($tid)) {
    return;
  }

  $block = new stdClass();
  $block->module = 'forum-secondary-links';
  $block->delta  = $tid;

  $block->title = ''; // by default this has no title.
  _advanced_forum_add_files();

  $block->content = advanced_forum_forum_topic_list_sort();

  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function advanced_forum_forum_topic_list_sort_content_type_edit_form(&$form, &$form_state) {
}

function advanced_forum_forum_topic_list_sort_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function advanced_forum_forum_topic_list_sort_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" topic list sort widget', array('@s' => $context->identifier));
}

