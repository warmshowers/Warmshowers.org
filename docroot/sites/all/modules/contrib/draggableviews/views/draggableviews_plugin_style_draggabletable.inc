<?php
// $Id: draggableviews_plugin_style_draggabletable.inc,v 1.1.2.18 2009/03/30 23:16:23 sevi Exp $

/**
 * @file
 * Draggableviews style plugin definition.
 */

/**
 * Style plugin to render each item as a row in a draggable table;
 * Inherits all from views_plugin_table.
 */
class draggableviews_plugin_style_draggabletable extends views_plugin_style_table {
  function option_definition() {
    $options = parent::option_definition();

    $options['columns'] = array('default' => array());
    $options['default'] = array('default' => '');
    $options['info'] = array('default' => array());
    $options['override'] = array('default' => TRUE);
    $options['sticky'] = array('default' => FALSE);
    $options['order'] = array('default' => 'asc');

    return $options;
  }

  function validate() {
    $errors = parent::validate();

    if (!isset($this->options['tabledrag_order'])) return $errors;

    $fields = $this->view->get_items('field', $this->display->id);

    // Check if all configured fields are available.
    if (isset($this->options['tabledrag_order'])) {
      if ($this->options['tabledrag_order']['field'] != 'none' && !isset($fields[$this->options['tabledrag_order']['field']])) {
        $errors[] = t('Display "@display": Draggableviews: order field <i>@field</i> could not be found (..afterwards removed from Fields section?). Check your settings.', array('@display' => $this->display->display_title, '@field' => $this->options['tabledrag_order']['field']));
        return $errors;
      }
    }
    if (isset($this->options['tabledrag_hierarchy'])) {
      if ($this->options['tabledrag_hierarchy']['field'] != 'none' && !isset($fields[$this->options['tabledrag_hierarchy']['field']])) {
        $errors[] = t('Display "@display": Draggableviews: hierarchy field <i>@field</i> could not be found (..afterwards removed from Fields section?). Check your settings.', array('@display' => $this->display->display_title, '@field' => $this->options['tabledrag_hierarchy']['field']));
        return $errors;
      }
    }

    // Check if first field is draggableviews handled and will be hidden.
    $found = FALSE;
    $first_field = key($fields);
    if (isset($this->options['tabledrag_order'])) {
      if ($this->options['tabledrag_order']['field'] == $first_field && $this->options['tabledrag_order_visible']['visible'] !== 'visible') {
        $found = TRUE;
      }
    }
    if (isset($this->options['tabledrag_hierarchy'])) {
      if ($this->options['tabledrag_hierarchy']['field'] == $first_field && $this->options['tabledrag_hierarchy_visible']['visible'] !== 'visible') {
        $found = TRUE;
      }
    }
    if ($fields[$first_field]['exclude']) {
      $found = TRUE;
    }
    if ($found) {
      $errors[] = t('Display "@display": Draggableviews: The drag-handles will be attached to the first field. But the currently configured first field will not be shown. Move the currently configured first field to another position or choose <i>Show input fields?</i> on the settings page.', array('@display' => $this->display->display_title));
    }

    // Check sort criteria.
    if (isset($this->options['tabledrag_order']['field']) && $this->options['tabledrag_order']['field'] != 'none') {
      $field_alias = $this->options['tabledrag_order']['field'];
      $sorts = $this->view->get_items('sort', $this->display->id);
      $sort = current($sorts);
      if (strcmp($fields[$field_alias]['field'], $sort['field']) != 0 || $sort['order'] == 'DESC') {
        $tabledrag_order_field = $fields[$field_alias]['label'];
        $errors[] = t('Display "@display": Draggableviews: You must sort by <i>@fields ascending</i> to display the structure correctly.', array('@display' => $this->display->display_title, '@fields' => $tabledrag_order_field));
      }
    }

    // Check depth limit.
    $limit = $this->options['draggableviews_depth_limit'];
    if (!(is_numeric($limit) && $limit >= -1)) {
      $errors[] = t('Depth limit must be a numeric value >= 1');
    }

    // Check page extensions.
    if (!is_numeric($this->options['draggableviews_extensions']['extension_top']) || $this->options['draggableviews_extensions']['extension_top'] < 0) {
      $errors[] = t('Display "@display": Draggableviews: Number rows of last page must be a numeric value >= 0.', array('@display' => $this->display->display_title));
    }
    if (!is_numeric($this->options['draggableviews_extensions']['extension_bottom']) || $this->options['draggableviews_extensions']['extension_bottom'] < 0) {
      $errors[] = t('Display "@display": Draggableviews: Number rows of previous page must be a numeric value >= 0.', array('@display' => $this->display->display_title));
    }

    return $errors;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    // inherit options from style_table
    parent::options_form($form, $form_state);

    // set theme handler
    // theme function is registered in *.module file
    $form['#theme'] = 'draggableviews_ui_style_plugin_draggabletable';

    // get field handler
    $handlers = $this->display->handler->get_handlers('field');


    // DRAGGABLE VIEW OPTIONS
    //
    // + Set field(s) to save the order in
    // + Set field to save the parent in
    // + Apply tabledrag-type to content-types (Root (can't have parent), Leaf (can't have children)).
    // + Set if expand/collapse links should be shown

    $input = array();
    $input = $form_state['input'];

    // Get all system-wide node types as a keyed array.
    foreach (node_get_types('types') AS $node_type) {
      $node_types[$node_type->type] = $node_type->type;
    }

    $form['tabledrag_hierarchy']       = array();
    $form['tabledrag_order']           = array();
    $form['tabledrag_types']           = array();
    $form['draggableviews_extensions'] = array();

    // Check for input.
    if (count($input) > 0) {
      // Define the input data as the current data.
      $current = $form_state['input']['style_options'];
    }
    else {
      // Define the already stored data as the current data.
      $current = $this->options;
    }

    $form['tabledrag_header'] = array(
      '#prefix' => '<div style="background: #F6F6F6; border-top: 1px solid #D6DBDE; font-weight: bold; padding: 1em 1em 0;">',
      '#suffix' => '</div>',
      '#value' => t('@display: Style options: Draggable Table Settings:', array('@display' => $this->display->display_title)),
    );
    $form['tabledrag_description'] = array(
      '#prefix' => '<div class="description form-item">',
      '#suffix' => '</div>',
      '#value' => t('Within the following section all the <i>Draggable Table</i> settings can be configured. Don\'t be confused by the options above. This style plugin inherits all options that the table style plugin offers.<br />Draggableviews provides two special fields to ease your life. They are called <i>Draggableviews: Order</i> and <i>Draggableviews: Parent</i>. These fields are designed to be used in combination with the "Native" handler.'),
    );
    $form['tabledrag_description_order'] = array(
      '#prefix' => '<div class="description form-item">',
      '#suffix' => '</div>',
      '#value' => t('In order to make rows of a table draggable the order field must be specified. To use hierarchies you must set the parent field.'),
    );
    $form['tabledrag_description_hierarchy'] = array(
      '#prefix' => '<div class="description form-item">',
      '#suffix' => '</div>',
      '#value' => t('Set a parent field if you want to use hierarchies.'),
    );
    $form['tabledrag_description_types'] = array(
      '#prefix' => '<div class="description form-item">',
      '#suffix' => '</div>',
      '#value' => t('You can constrain the user when he drags rows.<br />Type "root" cannot be subordinated.<br />Type "leaf" cannot have child nodes.'),
    );
    $form['draggableviews_description_extensions'] = array(
      '#prefix' => '<div class="description form-item">',
      '#suffix' => '</div>',
      '#value' => t('When you use paging and you want to drag a row from one page to another then you need to show some rows of the previous/next page.'),
    );
    $form['description_tabledrag_lock'] = array(
      '#prefix' => '<div class="description form-item">',
      '#suffix' => '</div>',
      '#value' => t('If you don\'t wan\'t that the hierarchy can be changed use the following option.'),
    );

    $options = _draggableviews_filter_fields(array('number'), $handlers);
    $draggableviews_handlers = draggableviews_get_handlers_list();

    $form['tabledrag_order'] = array(
      '#weight' => 9,
      'field' => array(
        '#type' => 'select',
        '#options' => array('none' => t('none')) + _draggableviews_filter_fields(array('order'), $handlers),
        '#value' => isset($current['tabledrag_order']['field']) ? $current['tabledrag_order']['field'] : 'none',
      ),
      'handler' => array(
        '#type' => 'select',
        '#options' => $draggableviews_handlers,
        '#value' => isset($current['tabledrag_order']['handler']) ? $current['tabledrag_order']['handler'] : key($draggableviews_handlers),
      ),
    );

    // Show or hide input fields.
    $form['tabledrag_order_visible'] = array(
      '#type' => 'checkboxes',
      '#name' => 'tabledrag_order_visible',
      '#options' => array('visible' => t('Show input fields?')),
      '#title' => t('Decide whether order input fields should be visible or not'),
      '#default_value' => isset($current['tabledrag_order_visible']) ? $current['tabledrag_order_visible'] : array(),
    );

    $form['tabledrag_hierarchy'] = array(
      '#weight' => 10,
      'field' => array(
        '#type' => 'select',
        '#options' => array('none' => t('none')) + _draggableviews_filter_fields(array('nodereference'), $handlers),
        '#value' => isset($current['tabledrag_hierarchy']['field']) ? $current['tabledrag_hierarchy']['field'] : 'none',
      ),
      'handler' => array(
        '#type' => 'select',
        '#options' => $draggableviews_handlers,
        '#value' => isset($current['tabledrag_hierarchy']['handler']) ? $current['tabledrag_hierarchy']['handler'] : key($draggableviews_handlers),
      ),
    );

    // Show or hide input fields.
    $form['tabledrag_hierarchy_visible'] = array(
        '#type' => 'checkboxes',
        '#name' => 'tabledrag_hierarchy_visible',
        '#options' => array('visible' => t('Show input fields?')),
        '#title' => t('Decide whether parent input fields should be visible or not'),
        '#default_value' => isset($current['tabledrag_hierarchy_visible']) ? $current['tabledrag_hierarchy_visible'] : array(),
    );

    $form['draggableviews_depth_limit'] = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#description' => t('Only use this option in combination with hierarchies. Type -1 for no limit.'),
      '#title' => t('Define the depth limit'),
      '#default_value' => (isset($current['draggableviews_depth_limit']) && is_numeric($current['draggableviews_depth_limit'])) ? $current['draggableviews_depth_limit'] : -1,
    );

    // Tabledrag types (root/leafe).
    // These fields will save the behaviour of a node-type (root, leaf).
    $extra_row = isset($input['tabledrag_types_add']) ? TRUE : FALSE;
    for ($i = 0, $index = 0; $i < count($current['tabledrag_types']) + ($extra_row == TRUE ? 1 : 0); $i++) {

      // if option should be deleted, continue loop
      if (isset($input['tabledrag_types_del_'. $i])) continue;

      $form['tabledrag_types'][$index] = array(
        'node_type' => array(
          '#type' => 'select',
          '#options' => $node_types,
          '#value' => isset($current['tabledrag_types'][$i]['node_type']) ? $current['tabledrag_types'][$i]['node_type'] : $node_types[0],
        ),
        'type' => array(
          '#type' => 'select',
          '#options' => array('root' => 'root', 'leaf' => 'leaf'),
          '#value' => isset($current['tabledrag_types'][$i]['type']) ? $current['tabledrag_types'][$i]['type'] : 'root',
        ),
        'tabledrag_type_del_button' => array(
          '#type' => 'button',
          '#name' => 'tabledrag_types_del_'. $index,
          '#value' => t('Delete'),
        ),
      );
      $index++;
    }
    $form['tabledrag_types_add'] = array(
      '#type' => 'button',
      '#name' => 'tabledrag_types_add',
      '#value' => t('Add type'),
    );

    // Expand/collapse options.
    $form['tabledrag_expand'] = array(
      '#type' => 'checkboxes',
      '#name' => 'tabledrag_expand',
      '#options' => array('expand_links' => 'Show expand Links', 'collapsed' => 'Default is collapsed', 'by_uid' => 'Unique for each user'),
      '#title' => t('Decide whether expand/collapse links should be shown or not'),
      '#default_value' => isset($current['tabledrag_expand']) ? $current['tabledrag_expand'] : array('expand_links'),
    );

    // Extensions when using paging.
    $form['draggableviews_extensions'] = array(
      'extension_top' => array(
        '#type' => 'textfield',
        '#size' => 2,
        '#title' => t('How many rows should be displayed of the previous page'),
        '#default_value' => (isset($current['draggableviews_extensions']['extension_top']) && is_numeric($current['draggableviews_extensions']['extension_top'])) ? $current['draggableviews_extensions']['extension_top'] : 3,
      ),
      'extension_bottom' => array(
        '#type' => 'textfield',
        '#size' => 2,
        '#title' => t('How many rows should be displayed of the next page'),
        '#default_value' => (isset($current['draggableviews_extensions']['extension_bottom']) && is_numeric($current['draggableviews_extensions']['extension_bottom'])) ? $current['draggableviews_extensions']['extension_bottom'] : 3,
      ),
    );

    // Lock option.
    $form['tabledrag_lock'] = array(
      '#type' => 'checkboxes',
      '#name' => 'tabledrag_lock',
      '#options' => array('lock' => t('The hierarchy cannot be changed (locked)')),
      '#title' => t('Lock the hierarchy'),
      '#default_value' => isset($current['tabledrag_lock']) ? $current['tabledrag_lock'] : array(),
    );

    $form['draggableviews_default_on_top'] = array(
      '#type' => 'radios',
      '#name' => 'draggableviews_default_on_top',
      '#title' => t('Behaviour of new nodes'),
      '#default_value' => isset($current['draggableviews_default_on_top']) ? $current['draggableviews_default_on_top'] : 1,
      '#description' => t('Nodes without a numerical order value assigned will be treated as new nodes. Decide where they should appear by default.'),
      '#options' => array(t('On Bottom'), t('On Top')),
    );

    // Let extension modules alter the output
    foreach (module_implements('draggableviews_style_plugin_form_alter') as $module) {
      $function = $module .'_draggableviews_style_plugin_form_alter';
      $function($form, $form_state, $this);
    }
  }

  /**
   * Render the draggable table style.
   */
  function render() {
    // We need to wrap around a form to make it possible to submit changes.
    // Due to the fact that multiple views can be shown on the same page we need to use
    // an unique form_id for each view and display. Same displays of the same view can
    // share the same id because the concerned views object is the same.
    $form_id = 'draggableviews_view_draggabletable_form_'. $this->view->vid;
    if (is_numeric($this->view->vid)) {
      // If the view has already been saved attach the display id to the form id.
      $form_id .= '_'. $this->view->current_display;
    }
    return drupal_get_form($form_id, $this);
  }
}
