<?php

/**
 * @file
 * Definition of workflow_views_handler_field_node_link_workflow.
 */

/**
 * Field handler to present a link 'node workflow form'.
 *
 * @ingroup views_field_handlers
 */
class workflow_views_handler_field_node_link_workflow extends views_handler_field_node_link {
  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    // Ensure user has access to edit this node.
    $entity_type = $this->entity_type;
    // if (!node_access('update', $node)) {
    if (!workflow_tab_access($entity_type, $node)) {
      return;
    }
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "node/$node->nid/workflow"; // @todo: add support for other entity types.
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('change state');
    return $text;
  }
}
