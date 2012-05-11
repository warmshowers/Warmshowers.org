<?php

/**
 * @file
 *
 * Plugin to provide a user context
 */

/**
 * Implementation of specially named hook_advanced_forum_forum_contexts().
 */
function advanced_forum_forum_ctools_contexts() {
  return array(
    'title' => t("Forum"),
    'description' => t('A single forum object.'),
    'context' => 'advanced_forum_forum_context_create_forum',
    'settings form' => 'advanced_forum_forum_context_settings_form',
    'settings form validate' => 'advanced_forum_forum_context_settings_form_validate',
    'keyword' => 'forum',
    'context name' => 'forum',
    'convert list' => array(
      'tid' => t('Forum ID'),
      'name' => t('Forum name'),
    ),
    'convert' => 'advanced_forum_forum_context_convert',
    'defaults' => array('tid' => 0),
  );
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function advanced_forum_forum_context_create_forum($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context(array('forum', 'term'));
  $context->plugin = 'forum';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    if ($data['tid']) {
      $data = taxonomy_get_term($data['tid']);
    }
    else {
      $data = taxonomy_vocabulary_load(variable_get('forum_nav_vocabulary', ''));
      $data->tid = 0;
    }
  }

  if (!empty($data)) {
    $data->container = (!$data->tid || in_array($data->tid, variable_get('forum_containers', array())));
    $context->data       = drupal_clone($data);
    $context->title      = $data->name;
    $context->argument   = $data->tid;
    $context->vid        = variable_get('forum_nav_vocabulary', '');
    $context->vocabulary = taxonomy_vocabulary_load($context->vid);
    if ($data->tid) {
      $context->parents = taxonomy_get_parents_all($data->tid);
    }

    return $context;
  }
}

function advanced_forum_forum_context_settings_form($conf) {
  if (empty($conf)) {
    $conf = array('tid' => 0);
  }

  $form = array();

  $options = array();
  $vocabulary = taxonomy_vocabulary_load(variable_get('forum_nav_vocabulary', ''));
  $options[0] = $vocabulary->name;

  $tree = taxonomy_get_tree($vocabulary->vid);
  if ($tree) {
    foreach ($tree as $term) {
      $choice = new stdClass();
      $choice->option = array($term->tid => str_repeat('-', $term->depth + 1) . $term->name);
      $options[] = $choice;
    }
  }

  $form['tid'] = array(
    '#type' => 'select',
    '#title' => t('Forum'),
    '#default_value' => $conf['tid'],
    '#options' => $options,
  );

  return $form;
}

/**
 * Convert a context into a string.
 */
function advanced_forum_forum_context_convert($context, $type) {
  switch ($type) {
    case 'tid':
      return $context->data->tid;
    case 'name':
      return $context->data->name;
  }
}
