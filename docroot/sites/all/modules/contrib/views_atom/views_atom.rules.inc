<?php

/**
 * Implements hook_rules_action_info().
 */
function views_atom_rules_action_info() {

  $actions = array();

  $actions['views_atom_rules_action_add_to_feed'] = array(
    'label' => t('Add an entity to a feed'),
    'group' => t('Views Atom'),
    'arguments' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
    ),
    'parameter' => array(
      // @todo, should this be "node" instead of "entity" ?
      'data' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t(''),
        'restriction' => 'selector',
        'wrapped' => TRUE,
      ),
      'view_display' => array(
        'type' => 'list<text>',
        'label' => t('View and Display'),
        'options list' => 'views_atom_get_feed_displays_options',
        'description' => t('Select the view and display that is responsible for generating the feed for this node. Only Feed displays are available.'),
      ),
    ),
    'base' => 'views_atom_rules_action_add_to_feed',
  );

  $actions['views_atom_rules_action_send_tombstone'] = array(
    'label' => t('Send node tombstone'),
    'group' => t('Views Atom'),
    'arguments' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
    ),
    'parameter' => array(
      // @todo, should this be "node" instead of "entity" ?
      'data' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t(''),
        'restriction' => 'selector',
        'wrapped' => TRUE,
      ),
      'topic_url' => array(
        'type' => 'text',
        'label' => t('Topic URL --- TEXT'),
        'description' => t(' '),
      ),
    ),
    'base' => 'views_atom_rules_action_send_tombstone',
  );

  return $actions;
}

/**
 * Rule implementation callback for sending a tombstone for a specific node.
 *
 * @param $data
 *   A wrapped Drupal entity
 * @param $views_diplay
 *   A concatenation of views name and display id
 */
function views_atom_rules_action_send_tombstone($data, $topic_url, $settings, RulesState $state, RulesPlugin $element) {

  // @todo, ideal this rules_action would go in push_hub or a more generic atom module.
  // That generic module might follow this discussion: http://groups.drupal.org/node/197583
  if (module_exists('push_hub')) {

    $nid = $data->getIdentifier();

    // Note the confusing alias flag. TRUE means "this has already been aliased,
    // which is the way to prevent url from trying to find an alias.
    $topic_url = url($topic_url, array('absolute' => TRUE, 'alias' => TRUE));
    $updated = date('c', REQUEST_TIME);

    $items = array(
      array(
        // Note the confusing alias flag. TRUE means "this has already been aliased,
        // which is the way to prevent url from trying to find an alias.
        'guid' => url('node/' . $nid, array('absolute' => TRUE, 'alias' => TRUE)),
        'when' => $updated,
      )
    );

    $tombstone_array = array(
        'title' => t('Tombstone'),
        'use_push' => TRUE,
        'hub_url' => url('pubsubhubbub/endpoint', array('absolute' => TRUE)),
        'feed_url' => $topic_url,
        'updated' => $updated,
        'show_updated_comment' => FALSE,
        'items' => $items,
      );

    $tombstone = theme('views_atom_tombstone', $tombstone_array);
    // @todo, make send_mode configurable.
    push_hub_notify($topic_url, $tombstone, PUSH_HUB_IMMEDIATE|PUSH_HUB_CRON);
  }
}

/**
 * Returns views_atom displays in a form usable for Rules forms.
 *
 * @return array
 */
function views_atom_get_feed_displays_options() {

  $options = array();
  foreach (views_atom_get_feed_displays() as $entry) {
    $options[$entry['title']][$entry['name'] . '-' . $entry['display']] = $entry['display_title'];
  }

  return $options;
}

/**
 * Rule implementation callback for adding a node to a feed.
 *
 * @param $data
 *   A wrapped Drupal entity
 * @param $views_diplay
 *   A concatenation of views name and display id
 */
function views_atom_rules_action_add_to_feed($data, $views_display, $settings, RulesState $state, RulesPlugin $element) {

  // @todo, ideally this rules_action would go in push_hub or a more generic atom module.
  if (module_exists('push_hub')) {

    // @todo, revisit this variable name.
    // There is inconsistency between plural and singular because it is unclear
    // if a rule should only deal with a single display or n displays.
    $views_displays = $views_display;

    // @todo, question whether Views is necessary at all for the atomization.
    // parts of render() inside of views_plugin_row_rdf_node could be abstracted out.
    $nid = $data->getIdentifier();

    foreach($views_displays as $view_name_and_display) {
      list($view_name, $view_display_name) = explode('-', $view_name_and_display);

      $view = views_get_view($view_name);
      $view->set_display($view_display_name);

      if ($view->access($view_display_name)) {

        // Set this version of the node object on the View so it can be accessed by views_plugin_row_rdf_node.inc
        // @todo, This is somewhat of a hack and should be replaced by a proper external atom libary.
        $node = $data->value();
        $view->views_atom_nodes = array(
          $nid => $node,
        );

        // Get the atom-formatted-data for this node.
        $result = $view->preview($view_display_name, array($nid));

        // Only PuSH the feed if there is actual data.  The view may have filtered
        // our nids down to 0, in which case we don't want to send anything. Because
        // normal View requests (Pull-based) are unaffected, Atom subscriptions still
        // work as normal.
        if (!empty($view->result)) {

          // @todo, make aliasing configurable.
          // Note the confusing alias flag. TRUE means "this has already been aliased,
          // which is the way to prevent url from trying to find an alia.
          $topic_url = url($view->get_url(), array('absolute' => TRUE, 'alias' => TRUE));
          // @todo, make a send_mode configurable?
          push_hub_notify($topic_url, $result, PUSH_HUB_IMMEDIATE|PUSH_HUB_CRON);
        }
      }
    }
  }
}
