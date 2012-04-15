<?php

/**
 * Callback function to supply a list of content types.
 */
function advanced_forum_forum_list_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Forum list'),
    'icon' => 'icon_forum.png',
    'description' => t('A list of forums for the forum.'),
    'required context' => new ctools_context_required(t('Forum'), 'forum'),
    'category' => t('Forum'),
    'defaults' => array(),
  );
}

/**
 * Render the content.
 */
function advanced_forum_forum_list_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return;
  }

  if (empty($context)) {
    $tid = 0;
  }
  else {
    $tid = $context->data->tid;
  }

  $block = new stdClass();
  $block->module = 'forum-list';
  $block->delta  = $tid;

  _advanced_forum_add_files();

  $forums = advanced_forum_get_forums($tid);

  if ($forums) {
    // No parents on the main forum page.
    $parents = isset($context->parents) ? $context->parents : array();

    $block->content = theme('forum_list', $forums, $parents, $tid);

    if (user_access('administer forums')) {
      $block->admin_links['administer_forums'] = array(
        'title' => t('Administer forums'),
        'alt' => t("Add, delete or modify forums"),
        'href' => "admin/content/forum",
      );

      if ($tid) {
        $block->admin_links['edit_forum'] = array(
          'title' => t('Edit forum'),
          'alt' => t("Modify this forum"),
          'href' => "admin/content/forum/edit/" . ($context->data->container ? 'container/' : 'forum/') . $tid,
          'query' => drupal_get_destination(),
        );
      }
    }
  }
  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function advanced_forum_forum_list_content_type_edit_form(&$form, &$form_state) {
}

function advanced_forum_forum_list_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function advanced_forum_forum_list_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" forum list', array('@s' => $context->identifier));
}

