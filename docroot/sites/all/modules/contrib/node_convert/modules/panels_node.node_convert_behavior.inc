<?php

/**
 * @file
 * Node convert panels_node.module include
 *
 * Performs necessary changes regarding panels_node type conversions.
 *
 * @TODO Fix for Drupal 7.
 */

/**
 * Implementation of node_convert_change().
 */
function panels_node_node_convert_change($data, $op) {
  if (node_type_get_base($data['dest_node_type']) == 'panels_node' && node_type_get_base($data['node']->type) != 'panels_node') {
    switch ($op) {
      case 'insert':
        $data['node']->panels_node['layout'] = $data['hook_options']['layout'];
        $data['node']->panels_node['css_id'] = '';
        panels_node_insert($data['node']);
        break;
      case 'options':
        ctools_include('plugins', 'panels');
        ctools_include('display-layout', 'panels');
        ctools_include('common', 'panels');

        $finish = '';
        $destination = '';
        $form_state = array(
          'display' => (object) array('layout' => NULL),
          'finish' => $finish,
          'destination' => $destination,
          'allowed_layouts' => panels_common_get_allowed_layouts('panels_node'),
          're_render' => FALSE,
          'no_redirect' => TRUE,
          'no buttons' => TRUE,
        );

        $form = panels_choose_layout($form_state);

        // Prepend instructions
        $form['categories']['#prefix'] = '<h3>' . t('Select a layout') . '</h3>' . $form['categories']['#prefix'];

        // Enforce the element id used for ctools dependant javascript in panels_choose_layout().
        $form['categories']['#attributes']['id'] = 'edit-categories';

        // Inject hook_options into the layout radios #parents
        foreach (element_children($form['layout']) as $i) {
          foreach (element_children($form['layout'][$i]) as $j) {
            array_unshift($form['layout'][$i][$j]['#parents'], 'hook_options');
          }
        }
        return $form;

      case 'options validate':
        $data['node']->panels_node['layout'] = $data['form_state']['values']['hook_options']['layout'];
        $data['node']->panels_node['css_id'] = '';
        return panels_node_validate($data['node']);
    }
  }
  // Ensure we're not moving converting to another panels_node node type
  if (node_type_get_base($data['node']->type) == 'panels_node' && node_type_get_base($data['dest_node_type']) != 'panels_node') {
    panels_node_delete($data['node']);
  }
}
