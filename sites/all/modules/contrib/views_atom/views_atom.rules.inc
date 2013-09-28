<?php

/**
 * Implementation of hook_rules_action_info
 */
function views_atom_rules_action_info() {
  $actions['views_atom_rules_action_add_to_feed'] = array(
    'label' => t('Add an entity to a feed'),
    'arguments' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
    ),
    'module' => 'Views Atom',
  );

  $actions['views_atom_rules_action_add_nids_to_feed'] = array(
    'label' => t('Add multiple nids to a feed'),
    'arguments' => array(
      'nids' => array(
        'type' => 'icalinstance_nids',
        'label' => t('Nids'),
      ),
    ),
    'module' => 'Views Atom',
  );

  return $actions;
}

/* ===== Action: Add To Feed ===== */

/**
 * Form callback for the views_atom_rules_action_add_to_feed action.
 */
function views_atom_rules_action_add_to_feed_form($settings, &$form) {

  foreach (views_atom_get_feed_displays() as $entry) {
    $options[$entry['title']][$entry['name'] .'-'. $entry['display']] = $entry['display_title'];
  }

  $form['generating_display'] = array(
    '#type' => 'select',
    '#title' => t('Generating view'),
    '#options' => $options,
    '#default_value' => $settings['view'] .'-'. $settings['view_display'],
    '#description' => t('Select the view and display that is responsible for generating the feed for this node. Only Feed displays are available.'),
  );
}

/**
 * Form submission callback for the views_atom_rules_action_add_to_feed action.
 */
function views_atom_rules_action_add_to_feed_submit(&$settings, $form, $form_state) {
  list($settings['view'], $settings['view_display']) = explode('-', $form_state['values']['generating_display']);
}

/**
 * Rule implementation callback for adding a node to a feed.
 *
 * @param $node
 *   The node that triggered this action.
 * @param $settings
 *   Configuration specified by the user.
 */
function views_atom_rules_action_add_to_feed($node, $settings) {
  $nid = $node->nid;

  $view = views_get_view($settings['view']);
  $view->set_display($settings['view_display']);

  if ($view->access($settings['view_display'])) {
    $result = $view->preview($settings['view_display'], array($nid));
    // Only PuSH the feed if there is actual data.  The view may have filtered
    // our nids down to 0, in which case we don't want to send anything. Because
    // normal View requests (Pull-based) are unaffected, Atom subscriptions still
    // work as normal.
    if (!empty($view->result)) {
      $topic_url = url($view->display[$view->current_display]->display_options['path'], array('absolute' => TRUE));
      // This uses the Drupal Queue module to actually send notifications later.
      push_hub_notify($topic_url, $result);
    }
  }
}

/* ===== Action: Add Nids To Feed ===== */

/**
 * Form callback for the viws_atom_rules_action_add_to_feed action.
 */
function views_atom_rules_action_add_nids_to_feed_form($settings, &$form) {

  foreach (views_atom_get_feed_displays() as $entry) {
    $options[$entry['title']][$entry['name'] .'-'. $entry['display']] = $entry['display_title'];
  }

  $form['generating_display'] = array(
    '#type' => 'select',
    '#title' => t('Generating view'),
    '#options' => $options,
    '#default_value' => $settings['view'] .'-'. $settings['view_display'],
    '#description' => t('Select the view and display that is responsible for generating the feed for these nodes. Only Feed displays are available.'),
  );
}

/**
 * Form submission callback for the views_atom_rules_action_add_to_feed action.
 */
function views_atom_rules_action_add_nids_to_feed_submit(&$settings, $form, $form_state) {
  list($settings['view'], $settings['view_display']) = explode('-', $form_state['values']['generating_display']);
}

/**
 * Rule implementation callback for adding multiple nodes to a feed.
 *
 * @param $nids
 *   An array of nids that we want to push using the configured view.
 * @param $settings
 *   Configuration specified by the user.
 */
function views_atom_rules_action_add_nids_to_feed($nids, $settings) {

  if ($nids) {
    $view = views_get_view($settings['view']);
    $view->set_display($settings['view_display']);

    if ($view->access($settings['view_display'])) {
      $nid_string = implode(',', $nids);
      $result = $view->preview($settings['view_display'], array($nid_string));
      // Only PuSH the feed if there is actual data.  The view may have filtered
      // our nids down to 0, in which case we don't want to send anything. Because
      // normal View requests (Pull-based) are unaffected, Atom subscriptions still
      // work as normal.
      if (!empty($view->result)) {
        $topic_url = url($view->display[$view->current_display]->display_options['path'], array('absolute' => TRUE));
        // This uses the Drupal Queue module to actually send notifications later.
        push_hub_notify($topic_url, $result);
      }
    }
  }
}

