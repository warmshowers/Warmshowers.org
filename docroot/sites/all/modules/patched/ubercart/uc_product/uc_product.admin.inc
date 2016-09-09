<?php

/**
 * @file
 * Product administration menu items.
 */

/**
 * Displays a list of product classes.
 */
function uc_product_class_default() {
  $classes = uc_product_class_load();
  $header = array(t('Class ID'), t('Name'), t('Description'), t('Operations'));
  $rows = array();
  foreach ($classes as $class) {
    $ops = array(l(t('edit'), 'admin/store/products/classes/' . $class->pcid . '/edit'));
    if (empty($class->locked)) {
      $ops[] = l(t('delete'), 'admin/store/products/classes/' . $class->pcid . '/delete');
    }
    $rows[] = array(
      check_plain($class->pcid),
      check_plain($class->name),
      filter_xss_admin($class->description),
      implode(' ', $ops),
    );
  }

  $build = array();
  $build['product_classes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No product classes have been defined yet.'),
  );
  $build['header'] = array(
    '#markup' => '<h2>' . t('Add a class') . '</h2>',
  );
  $build['form'] = drupal_get_form('uc_product_class_form');

  return $build;
}

/**
 * Form to change product settings.
 *
 * @ingroup forms
 */
function uc_product_settings_form($form, &$form_state) {

  // Put fieldsets into vertical tabs
  $form['product-settings'] = array('#type' => 'vertical_tabs');

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product settings'),
    '#group' => 'product-settings',
    '#weight' => -10,
  );

  // Loop through all the integrated image widgets and build an options array.
  $options = array();
  foreach (module_invoke_all('uc_image_widget') as $key => $widget) {
    $options[$key] = check_plain($widget['name']);
  }
  if (empty($options)) {
    $options[NULL] = t('No image widgets installed.');
  }
  else {
    // If we have widgets installed, add option to not use any of them
    $options['none'] = t("Don't use any image widgets.");
  }

  $form['product']['uc_product_image_widget'] = array(
    '#type' => 'radios',
    '#title' => t('Product image widget'),
    '#description' => t('The selected widget will be used to display a zoomed version of product images when they are clicked.'),
    '#options' => $options,
    '#default_value' => variable_get('uc_product_image_widget', NULL),
  );

  if (module_exists('uc_cart')) {
    $form['product']['uc_product_add_to_cart_qty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display an optional quantity field in the <em>Add to Cart</em> form.'),
      '#default_value' => variable_get('uc_product_add_to_cart_qty', FALSE),
    );
    $form['product']['uc_product_update_node_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update product display based on customer selections'),
      '#default_value' => variable_get('uc_product_update_node_view', FALSE),
      '#description' => t('Check this box to dynamically update the display of product information such as display-price or weight based on customer input on the add-to-cart form (e.g. selecting a particular attribute option).'),
    );
  }

  foreach (module_invoke_all('uc_product_feature') as $feature) {
    if (isset($feature['settings']) &&
        function_exists($feature['settings'])) {
      $form[$feature['id']] = array(
        '#type' => 'fieldset',
        '#title' => t('@feature settings', array('@feature' => $feature['title'])),
        '#group' => 'product-settings',
      );
      $form[$feature['id']] += $feature['settings'](array(), $form_state);

      if (function_exists($feature['settings'] . '_validate')) {
        $form['#validate'][] = $feature['settings'] . '_validate';
      }
      if (function_exists($feature['settings'] . '_submit')) {
        $form['#submit'][] = $feature['settings'] . '_submit';
      }
    }
  }

  return system_settings_form($form);
}

/**
 * Displays the product features tab on a product node edit form.
 */
function uc_product_features($node, $fid = NULL, $pfid = NULL) {
  drupal_set_title($node->title);

  $header = array(t('Type'), t('Description'), t('Operations'));
  $rows = array();

  $features = uc_product_feature_load_multiple($node->nid);
  foreach ($features as $feature) {
    $operations = array(
      'edit' => array('title' => t('edit'), 'href' => 'node/' . $node->nid . '/edit/features/' . $feature->fid . '/' . $feature->pfid),
      'delete' => array('title' => t('delete'), 'href' => 'node/' . $node->nid . '/edit/features/' . $feature->fid . '/' . $feature->pfid . '/delete'),
    );
    $rows[] = array(
      array('data' => uc_product_feature_data($feature->fid, 'title')),
      array('data' => $feature->description),
      theme('links', array(
        'links' => $operations,
        'attributes' => array('class' => array('links', 'inline')),
      )),
    );
  }

  $build['features'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('uc-product-features')),
    '#empty' => t('No features found for this product.'),
  );
  $build['add_form'] = drupal_get_form('uc_product_feature_add_form', $node);

  return $build;
}

/**
 * Handles adding or editing product features.
 */
function uc_product_feature_edit($node, $fid, $pfid) {
  $func = uc_product_feature_data($fid, 'callback');

  if (function_exists($func)) {
    $form_state = array(
      'build_info' => array(
        'args' => array(
          $node,
        ),
      ),
      'wrapper_callback' => 'uc_product_feature_form',
    );

    if ($pfid == 'add') {
      $form_state['build_info']['args'][] = array();
      $build = drupal_build_form($func, $form_state);
    }
    elseif (intval($pfid) > 0) {
      $feature = uc_product_feature_load($pfid);

      if (isset($feature)) {
        $form_state['build_info']['args'][] = $feature;
        $build = drupal_build_form($func, $form_state);
      }
    }
    else {
      drupal_goto('node/' . $node->nid . '/edit/features');
    }
  }
  else {
    drupal_set_message(t('Error: Attempted to add a non-existent product feature type.'), 'error');
    drupal_goto('node/' . $node->nid . '/edit/features');
  }

  if (empty($build)) {
    drupal_set_message(t('Error: No form data was returned for that operation.'), 'error');
    drupal_goto('node/' . $node->nid . '/edit/features');
  }

  return $build;
}

/**
 * Adds the form for adding a product feature to the features tab.
 *
 * @see uc_product_feature_add_form_submit()
 * @see theme_uc_product_feature_add_form()
 * @ingroup forms
 */
function uc_product_feature_add_form($form, &$form_state, $node) {
  foreach (module_invoke_all('uc_product_feature') as $feature) {
    $options[$feature['id']] = $feature['title'];
  }
  ksort($options);

  $form['feature'] = array(
    '#type' => 'select',
    '#title' => t('Add a new feature'),
    '#options' => $options,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Theme function for uc_product_feature_add_form().
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see uc_product_feature_add_form()
 * @ingroup themeable
 */
function theme_uc_product_feature_add_form($variables) {
  return '<table class="add-feature"><tr><td>' . drupal_render_children($variables['form']) . '</td></tr></table>';
}

/**
 * Form submission handler for uc_product_feature_add_form().
 *
 * @see uc_product_feature_add_form()
 */
function uc_product_feature_add_form_submit($form, &$form_state) {
  $node = $form_state['build_info']['args'][0];

  $form_state['redirect'] = 'node/' . $node->nid . '/edit/features/' . $form_state['values']['feature'] . '/add';
}

/**
 * Confirmation form to delete a product feature.
 *
 * @see uc_product_feature_confirm_delete_submit()
 * @ingroup forms
 */
function uc_product_feature_confirm_delete($form, &$form_state, $node, $fid, $feature) {
  $description = t('Are you sure you wish to delete this %feature?', array('%feature' => uc_product_feature_data($fid, 'title')))
    . '<div><b>' . t('Description') . ':</b><br />' . $feature['description'] . '</div><br />';

  return confirm_form($form, check_plain($node->title), 'node/' . $node->nid . '/edit/features', $description, t('Delete'), t('Cancel'), 'pf_delete');
}

/**
 * Form submission handler for uc_product_feature_confirm_delete().
 *
 * @see uc_product_feature_confirm_delete()
 */
function uc_product_feature_confirm_delete_submit($form, &$form_state) {
  $node = $form_state['build_info']['args'][0];
  $feature = $form_state['build_info']['args'][2];

  if ($form_state['values']['pf_delete']) {
    uc_product_feature_delete($feature['pfid']);

    drupal_set_message(t('The product feature has been deleted.'));
  }

  $form_state['redirect'] = 'node/' . $node->nid . '/edit/features';
}

/**
 * Sets up image field for products.
 *
 * @see uc_product_uc_store_status()
 */
function uc_product_image_defaults() {
  uc_product_add_default_image_field();

  drupal_set_message(t('Default image support configured for Ubercart products.'));

  drupal_goto('admin/store');
}

/**
 * Form builder for product classes.
 *
 * @see uc_product_class_form_validate()
 * @see uc_product_class_form_submit()
 * @ingroup forms
 */
function uc_product_class_form($form, &$form_state, $class = NULL) {
  if (!is_null($class)) {
    $classname = $class->name;
    $classdesc = $class->description;
    drupal_set_title($classname);
    $form['pcid'] = array(
      '#type' => 'hidden',
      '#value' => $class->pcid
    );
  }
  else {
    $classname = '';
    $classdesc = '';
    $form['pcid'] = array(
      '#type' => 'textfield',
      '#title' => t('Class ID'),
      '#required' => TRUE,
      '#maxlength' => 32,
      '#description' => t('The machine-readable name of this content type. This text will be used for constructing the URL of the <em>create content</em> page for this content type. This name may consist only of lowercase letters, numbers, and underscores. Dashes are not allowed. Underscores will be converted into dashes when constructing the URL of the <em>create content</em> page. This name must be unique to this content type.'),
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Class name'),
    '#description' => t('The human-readable name of this content type as it should appear on the !create_content page.  There are no character restrictions on this name.', array('!create_content' => l(t('Create content'), 'node/add'))),
    '#default_value' => $classname,
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('This text describes the content type to administrators.'),
    '#default_value' => $classdesc,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Ensures the new product class is unique.
 *
 * @see uc_product_class_form()
 * @see uc_product_class_form_submit()
 */
function uc_product_class_form_validate($form, &$form_state) {
  if ($form['pcid']['#type'] == 'textfield') {
    $type = node_type_get_type($form_state['values']['pcid']);
    if ($type) {
      if ($type->base == 'uc_product') {
        form_set_error('pcid', t('This product class already exists.'));
      }
      elseif ($type->custom == 0) {
        form_set_error('pcid', t('This is a node type provided by another module. Only custom node types may become product classes.'));
      }
    }
  }
}

/**
 * Form submission handler for uc_product_class_form().
 *
 * @see uc_product_class_form()
 * @see uc_product_class_form_validate()
 */
function uc_product_class_form_submit($form, &$form_state) {
  $is_new = $form['pcid']['#type'] == 'textfield';

  $pcid = $form_state['values']['pcid'];

  // Convert whitespace to underscores, and remove other non-alphanumeric characters.
  $pcid = preg_replace(array('/\s+/', '/\W/'), array('_', ''), strtolower($pcid));

  $result = db_merge('uc_product_classes')
    ->key(array('pcid' => $pcid))
    ->fields(array(
      'name' => $form_state['values']['name'],
      'description' => $form_state['values']['description'],
    ))
    ->execute();

  db_update('node_type')
    ->fields(array(
      'name' => $form_state['values']['name'],
      'description' => $form_state['values']['description'],
    ))
    ->condition('type', $pcid)
    ->execute();

  uc_product_node_info(TRUE);

  if ($result == MergeQuery::STATUS_INSERT) {
    variable_set('node_options_' . $pcid, variable_get('node_options_product', array('status', 'promote')));

    if (module_exists('comment')) {
      variable_set('comment_' . $pcid, variable_get('comment_product', COMMENT_NODE_OPEN));
    }

    module_invoke_all('uc_product_class', $pcid, 'insert');
  }
  else {
    module_invoke_all('uc_product_class', $pcid, 'update');
  }

  node_types_rebuild();
  if ($is_new) {
    $type = node_type_get_type($pcid);
    node_add_body_field($type, t('Description'));
    uc_product_add_default_image_field($pcid);
  }
  menu_rebuild();

  drupal_set_message(t('Product class saved.'));
}

/**
 * Confirms the deletion of a product class.
 *
 * @see uc_product_class_delete_confirm_submit()
 */
function uc_product_class_delete_confirm($form, &$form_state, $class) {
  $form['pcid'] = array(
    '#type' => 'value',
    '#value' => $class->pcid,
  );

  $question = t('Are you sure you want to delete the %type product class?', array('%type' => $class->pcid));
  $description = t('The node type associated with this product class will revert to a standard node type.');

  // Find out how many nodes of this class exist and add to the description.
  $count = db_query("SELECT COUNT(*) FROM {node} WHERE type = :pcid", array(':pcid' => $class->pcid))->fetchField();

  if ($count > 0) {
    $description .= '<br />' . format_plural($count, 'There is 1 node of this type.', 'There are @count nodes of this type.');
  }

  return confirm_form($form, $question, 'admin/store/products/classes', $description, t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for uc_product_class_delete_confirm().
 *
 * @see uc_product_class_delete_confirm()
 */
function uc_product_class_delete_confirm_submit($form, &$form_state) {
  $type = node_type_get_type($form_state['values']['pcid']);
  $type->base = 'node_content';
  $type->custom = 1;
  node_type_save($type);

  db_delete('uc_product_classes')
    ->condition('pcid', $form_state['values']['pcid'])
    ->execute();
  module_invoke_all('uc_product_class', $form_state['values']['pcid'], 'delete');
  uc_product_node_info(TRUE);
  node_types_rebuild();
  menu_rebuild();

  drupal_set_message(t('Product class %type deleted.', array('%type' => $form_state['values']['pcid'])));

  $form_state['redirect'] = 'admin/store/products/classes';
}
