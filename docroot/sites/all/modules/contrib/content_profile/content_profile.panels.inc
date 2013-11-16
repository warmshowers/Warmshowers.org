<?php
// $Id: content_profile.panels.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Panels integration
 */

/**
 * Plugin to provide an relationship handler for node from user
 */
function content_profile_panels_relationships() {
  $args['node_from_user'] = array(
    'title' => t("Profile Node from user"),
    'keyword' => 'content_profile',
    'description' => t('Adds a Content Profile from user context'),
    'required context' => new panels_required_context(t('User'), 'user'),
    'context' => 'content_profile_panels_context',
    'settings form' => 'content_profile_panels_settings_form',
    'settings form validate' => 'content_profile_panels_settings_form_validate',
  );
  return $args;
}

/**
 * Return a new context based on an existing context
 */
function content_profile_panels_context($context = NULL, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL
  if (empty($context->data)) {
    return panels_context_create_empty('node', NULL);
  }

  if (isset($context->data->uid)) {
    // Load the node for the requested type
    $uid = $context->data->uid;
    $content_profile_node = content_profile_load($conf['type'], $uid);

    // Send it to panels
    return panels_context_create('node', $content_profile_node);
  }
  else {
    return panels_context_create_empty('node', NULL);
  }
}

/**
 * Settings form for the relationship
 */
function content_profile_panels_settings_form($conf) {
  $options = content_profile_get_types('names');
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Relationship type'),
    '#options' => $options,
    '#default_value' => $conf['type']
  );

  return $form;
}