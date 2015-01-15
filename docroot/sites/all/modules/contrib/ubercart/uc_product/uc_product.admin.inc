<?php

/**
 * @file
 * Product administration menu items.
 */

/**
 * Lists all products.
 */
function uc_product_administration() {
  $output = '';

  $header = array();
  $tbl_columns = uc_product_table_header();
  foreach ($tbl_columns as $column) {
    $header[] = $column['cell'];
  }
  $order = substr(tablesort_sql($header), 10);
  if (empty($_REQUEST['order'])) {
    $order = 'p.ordering, n.title, n.nid';
  }

  $nids = array();
  // Display 50 products per page.
  $types = "('" . implode("', '", uc_product_types()) . "')";
  $result = pager_query("SELECT n.nid, n.type FROM {node} AS n INNER JOIN {uc_products} AS p ON n.vid = p.vid WHERE n.type IN $types ORDER BY ". $order, 50, 0, NULL);
  while ($product = db_fetch_object($result)) {
    $nids[] = $product->nid;
  }

  $table = tapir_get_table('uc_product_table', $nids);

  // Display the product table and pager if necessary.
  $output .= drupal_render($table) . theme('pager');

  return $output;
}

/**
 * Displays a list of product classes.
 */
function uc_product_class_default() {
  $classes = uc_product_class_load();
  $header = array(t('Class ID'), t('Name'), t('Description'), t('Operations'));
  $rows = array();
  foreach ($classes as $class) {
    $ops = array(l(t('edit'), 'admin/store/products/classes/'. $class->pcid .'/edit'));
    if (empty($class->locked)) {
      $ops[] = l(t('delete'), 'admin/store/products/classes/'. $class->pcid .'/delete');
    }
    $rows[] = array(
      check_plain($class->pcid),
      check_plain($class->name),
      filter_xss_admin($class->description),
      implode(' ', $ops),
    );
  }
  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('No product classes have been defined yet.'), 'colspan' => '5'));
  }

  $output = theme('table', $header, $rows);
  $output .= '<h2>'. t('Add a class') .'</h2>';
  $output .= drupal_get_form('uc_product_class_form');

  return $output;
}

/**
 * Displays overview of product settings.
 *
 * @see uc_product_settings_form()
 * @see uc_product_field_settings_form()
 */
function uc_product_settings_overview() {
  // Load the form summaries for pages beneath this path.
  $summaries = summarize_child_form_pages('admin/store/settings/products/edit');

  // Theme it all up in a summaries overview.
  return theme('summary_overview', $summaries);

}

/**
 * Form to change product settings.
 *
 * @see uc_product_settings_overview()
 * @ingroup forms
 */
function uc_product_settings_form() {
  $form = array();

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
    $options['none'] = "Don't use any image widgets.";
  }

  $form['uc_product_image_widget'] = array(
    '#type' => 'radios',
    '#title' => t('Product image widget'),
    '#description' => t('The selected widget will be used to display a zoomed version of product images when they are clicked.'),
    '#options' => $options,
    '#default_value' => variable_get('uc_product_image_widget', NULL),
  );

  $form['uc_product_add_to_cart_qty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display an optional quantity field in the <em>Add to Cart</em> form.'),
    '#default_value' => variable_get('uc_product_add_to_cart_qty', FALSE),
    '#summary arguments' => array(
      t('The Quantity field in the <em>Add to Cart</em> form is enabled.'),
      t('The Quantity field in the <em>Add to Cart</em> form is disabled.'),
    ),
  );
  $form['uc_product_add_to_cart_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Add to cart</em> forms in product node teasers.'),
    '#default_value' => variable_get('uc_product_add_to_cart_teaser', TRUE),
    '#summary arguments' => array(
      t('The <em>Add to Cart</em> form is enabled in product teasers.'),
      t('The <em>Add to Cart</em> form is disabled in product teasers.'),
    ),
  );

  $form['uc_add_to_cart_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('<em>Add to cart</em> button text'),
    '#description' => t('Use the textboxes to adjust the text of the submit button for <em>Add to Cart</em> forms in various places on the site.'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['uc_add_to_cart_text']['uc_teaser_add_to_cart_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Teaser forms'),
    '#description' => t('For the form displayed on teasers and catalog pages.'),
    '#default_value' => variable_get('uc_teaser_add_to_cart_text', t('Add to cart')),
    '#summary' => t('Teaser and catalog pages: %text', array('%text' => variable_get('uc_teaser_add_to_cart_text', t('Add to cart')))),
    '#summary arguments' => array(FALSE),
  );
  $form['uc_add_to_cart_text']['uc_product_add_to_cart_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Product view'),
    '#description' => t('For the form displayed on the product view page.'),
    '#default_value' => variable_get('uc_product_add_to_cart_text', t('Add to cart')),
    '#summary' => t('Product view pages: %text', array('%text' => variable_get('uc_teaser_add_to_cart_text', t('Add to cart')))),
    '#summary arguments' => array(FALSE),
  );

  return system_settings_form($form);
}

/**
 * Allows store administrators to control what product information is relavent to their store.
 *
 * @see uc_product_settings_overview()
 * @see _uc_product_fields_summarize()
 * @see theme_uc_product_field_settings_form()
 * @see uc_product_field_settings_form_reset()
 * @see uc_product_field_settings_form_submit()
 * @ingroup forms
 */
function uc_product_field_settings_form() {
  $form = array();

  $options = array(
    'model' => t('SKU'),
    'image' => t('Image'),
    'display_price' => t('Display price'),
    'list_price' => t('List price'),
    'cost' => t("Cost (seen only by 'administer products' permission)"),
    'sell_price' => t('Sell price'),
    'weight' => t('Weight'),
    'dimensions' => t('Dimensions'),
    'add_to_cart' => variable_get('uc_product_add_to_cart_text', t('Add to cart')),
  );
  $enabled = variable_get('uc_product_field_enabled', array(
    'model' => 'model',
    'image' => 'image',
    'display_price' => 'display_price',
    'list_price' => FALSE,
    'cost' => FALSE,
    'sell_price' => 'sell_price',
    'weight' => FALSE,
    'dimensions' => FALSE,
    'add_to_cart' => 'add_to_cart',
  ));
  $weight = variable_get('uc_product_field_weight', array(
    'image' => -2,
    'display_price' => -1,
    'model' => 0,
    'list_price' => 2,
    'cost' => 3,
    'sell_price' => 4,
    'weight' => 5,
    'dimensions' => 6,
    'add_to_cart' => 10,
  ));
  $fields = array();
  foreach ($options as $field => $value) {
    $fields[$field] = array(
      'title' => $value,
      'enabled' => $enabled[$field],
      'weight' => $weight[$field],
    );
  }
  uasort($fields, 'uc_weight_sort');
  $form['fields'] = array('#tree' => TRUE);
  $form['fields']['#summary callback'] = '_uc_product_fields_summarize';
  $form['fields']['#summary arguments'] = array($options, $enabled, $weight);

  foreach ($fields as $label => $field) {
    $form['fields'][$label]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $field['enabled'],
    );
    $form['fields'][$label]['title'] = array(
      '#type' => 'markup',
      '#value' => $field['title'],
    );
    $form['fields'][$label]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 50,
      '#default_value' => $field['weight'],
    );
  }

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('uc_product_field_settings_form_reset'),
  );

  return $form;
}

/**
 * Summary callback for product fields settings.
 *
 * @see uc_product_field_settings_form()
 */
function _uc_product_fields_summarize($form, $options, $enabled, $weight) {

  $fields = array();
  foreach ($options as $field => $label) {
    if ($enabled[$field]) {
      $fields[$field] = array('enabled' => $enabled[$field],
        'weight' => $weight[$field],
        'data' => $label,
      );
    }
  }
  uasort($fields, 'uc_weight_sort');

  foreach ($fields as $field) {
    $item[] = $field['data'];
  }

  $items[] = array(
    'data' => t('Displayed product fields:'),
    'children' => $item,
  );

  return $items;
}

/**
 * Displays the product field settings form.
 *
 * @see uc_product_field_settings_form()
 * @ingroup themeable
 */
function theme_uc_product_field_settings_form($form) {
  $header = array(t('Enable'), t('Product field'), t('List position'));
  $rows = array();
  foreach (element_children($form['fields']) as $field) {
    $row = array();
    $row[] = drupal_render($form['fields'][$field]['enabled']);
    $row[] = drupal_render($form['fields'][$field]['title']);
    $row[] = drupal_render($form['fields'][$field]['weight']);
    $rows[] = $row;
  }
  $output  = theme('table', $header, $rows);

  $output .= drupal_render($form);
  return $output;
}

/**
 * Resets field settings to their default values.
 *
 * @see uc_product_field_settings_form()
 */
function uc_product_field_settings_form_reset($form, &$form_state) {
  variable_del('uc_product_field_enabled');
  variable_del('uc_product_field_weight');

  drupal_set_message(t('The configuration options have been reset to their default values.'));
}

/**
 * Saves the product fields settings.
 *
 * @see uc_product_field_settings_form()
 */
function uc_product_field_settings_form_submit($form, &$form_state) {
  $enabled = array();
  $weight = array();
  foreach ($form_state['values']['fields'] as $id => $field) {
    $enabled[$id] = $field['enabled'];
    $weight[$id] = $field['weight'];
  }
  variable_set('uc_product_field_enabled', $enabled);
  variable_set('uc_product_field_weight', $weight);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Displays the settings form for all product features.
 *
 * @see _uc_product_features_summarize()
 * @ingroup forms
 */
function uc_product_feature_settings_form() {
  $titles = array();
  $features = module_invoke_all('product_feature');
  foreach ($features as $feature) {
    $titles[] = $feature['title'];
  }
  if (empty($titles)) {
    $titles[] = '<em>'. t('No product features found.') .'</em>';
  }
  $form['features_list'] = array(
    '#value' => '<div><b>'. t('The following product features are enabled')
               .':</b><br />'. implode(', ', $titles) .'</div><br />',
    '#summary callback' => '_uc_product_features_summarize',
    '#summary arguments' => array($titles),
  );

  foreach ($features as $feature) {
    if (isset($feature['settings']) &&
        function_exists($feature['settings'])) {
      $form[$feature['id']] = array(
        '#type' => 'fieldset',
        '#title' => t('!feature settings', array('!feature' => $feature['title'])),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#summary callback' => 'summarize_null',
      );
      $form[$feature['id']] = array_merge($form[$feature['id']], $feature['settings']());
    }
  }

  return system_settings_form($form);
}

/**
 * Summary callback for uc_product_feature_settings_form().
 */
function _uc_product_features_summarize($form, $titles) {

  foreach ($titles as $title) {
    $item[] = $title;
  }

  $items[] = array(
    'data' => t('The following features are enabled:'),
    'children' => $item,
  );

  return $items;

}

/**
 * Displays the product features tab on a product node edit form.
 */
function uc_product_features($node) {
  drupal_set_title(check_plain($node->title));

  if (arg(4)) {
    // First check to see if we're trying to remove a feature.
    if (intval(arg(5)) > 0 && arg(6) == 'delete') {
      $feature = uc_product_feature_load(intval(arg(5)), arg(4));
      if (isset($feature)) {
        // If the user confirmed the delete, process it!
        if (isset($_POST['pf_delete']) && $_POST['pf_delete']) {
          uc_product_feature_delete(intval(arg(5)));

          drupal_set_message(t('The product feature has been deleted.'));
          drupal_goto('node/'. arg(1) .'/edit/features');
        }

        // Show the confirmation form for deleting this feature.
        $question = $node->title;
        $description = t('Are you sure you wish to delete this %feature?', array('%feature' => uc_product_feature_data($feature['fid'], 'title')))
                      .'<div><b>'. t('Description') .':</b><br />'. $feature['description'] .'</div><br />';
        return drupal_get_form('confirm_form', $question, 'node/'. arg(1) .'/edit/features', $description, t('Delete'), t('Cancel'), 'pf_delete');
      }
      else {
        drupal_set_message(t("That product feature doesn't exist."), 'error');
        drupal_goto('node/'. arg(1) .'/edit/features');
      }
    }

    // Handle adding or editing product features.
    $func = uc_product_feature_data(arg(4), 'callback');
    if (function_exists($func)) {
      if (arg(5) == 'add') {
        $output = drupal_get_form($func, $node, array());
      }
      elseif (intval(arg(5)) > 0) {
        $feature = uc_product_feature_load(intval(arg(5)), arg(4));
        if (isset($feature)) {
          $output = drupal_get_form($func, $node, $feature);
        }
      }
    }
    else {
      drupal_set_message(t('Error: Attempted to add a non-existent product feature type.'), 'error');
      drupal_goto('node/'. $node->nid .'/edit/features');
    }

    if (empty($output)) {
      drupal_set_message(t('Error: No form data was returned for that operation.'), 'error');
      drupal_goto('node/'. $node->nid .'/edit/features');
    }

    return $output;
  }

  $header = array(t('Type'), t('Description'), t('Operations'));

  $features = uc_product_feature_load_multiple($node->nid);
  if (empty($features)) {
    $rows[] = array(
      array('data' => t('No features found for this product.'), 'colspan' => 3),
    );
  }
  else {
    foreach ($features as $feature) {
      $operations = array(
        'edit' => array('title' => t('edit'), 'href' => 'node/'. $node->nid .'/edit/features/'. $feature->fid .'/'. $feature->pfid),
        'delete' => array('title' => t('delete'), 'href' => 'node/'. $node->nid .'/edit/features/'. $feature->fid .'/'. $feature->pfid .'/delete'),
      );
      $rows[] = array(
        array('data' => uc_product_feature_data($feature->fid, 'title')),
        array('data' => $feature->description),
        theme('links', $operations, array('class' => 'links inline')),
      );
    }
  }

  $output = theme('table', $header, $rows, array('class' => 'uc-product-features')) . drupal_get_form('uc_product_feature_add_form');

  return $output;
}

/**
 * Adds the form for adding a product feature to the features tab.
 *
 * @see theme_uc_product_feature_add_form()
 * @see uc_product_feature_add_form_submit()
 * @ingroup forms
 */
function uc_product_feature_add_form() {
  foreach (module_invoke_all('product_feature') as $feature) {
    $options[$feature['id']] = $feature['title'];
  }
  ksort($options);

  $form['feature'] = array(
    '#type' => 'select',
    '#title' => t('Add a new feature'),
    '#options' => $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Theme function for uc_product_feature_add_form().
 *
 * @see uc_product_feature_add_form()
 * @ingroup themeable
 */
function theme_uc_product_feature_add_form($form) {
  $output = '<table class="add-feature"><tr><td>'. drupal_render($form) .'</td></tr></table>';
  return $output;
}

/**
 * Form submission handler for uc_product_feature_add_form().
 *
 * @see uc_product_feature_add_form()
 */
function uc_product_feature_add_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'node/'. arg(1) .'/edit/features/'. $form_state['values']['feature'] .'/add';
}

/**
 * Sets up imagefield and imagecache for products.
 *
 * @param $checks
 *   A bitmap describing the state of the image modules. Four checks are made
 *   to fill this bitmap: an imagefield exists, an imagefield is attached to
 *   product, the imagecache presets exist, and each preset has at least one
 *   action.
 *
 * @see uc_product_store_status()
 */
function uc_product_image_defaults() {
  uc_product_add_default_image_field();

  drupal_set_message(t('Default image support configured for Ubercart products using CCK and ImageCache.'));

  drupal_goto('admin/store');
}

/**
 * Form builder for product classes.
 *
 * @see uc_product_class_form_validate()
 * @see uc_product_class_form_submit()
 * @ingroup forms
 */
function uc_product_class_form($form_state, $class = NULL) {
  if (!is_null($class)) {
    $classname = $class->name;
    $classdesc = $class->description;
    drupal_set_title(check_plain($classname));
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
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
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
    $type = node_get_types('type', $form_state['values']['pcid']);
    if ($type) {
      if ($type->module == 'uc_product') {
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

  if ($is_new) {
    // Convert whitespace to underscores, and remove other non-alphanumeric characters.
    $pcid = preg_replace(array('/\s+/', '/\W/'), array('_', ''), strtolower($pcid));

    db_query("INSERT INTO {uc_product_classes} (pcid, name, description) VALUES ('%s', '%s', '%s')", $pcid, $form_state['values']['name'], $form_state['values']['description']);
    uc_product_node_info(TRUE);
    variable_set('node_options_'. $pcid, variable_get('node_options_product', array('status', 'promote')));

    if (module_exists('comment')) {
      variable_set('comment_'. $pcid, variable_get('comment_product', COMMENT_NODE_READ_WRITE));
    }

    module_invoke_all('product_class', $pcid, 'insert');
  }
  else {
    db_query("UPDATE {uc_product_classes} SET name = '%s', description = '%s' WHERE pcid = '%s'", $form_state['values']['name'], $form_state['values']['description'], $pcid);
    db_query("UPDATE {node_type} SET name = '%s', description = '%s' WHERE type = '%s'", $form_state['values']['name'], $form_state['values']['description'], $pcid);
    uc_product_node_info(TRUE);

    module_invoke_all('product_class', $pcid, 'update');
  }

  node_types_rebuild();
  if ($is_new && module_exists('imagefield')) {
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
function uc_product_class_delete_confirm($form_state, $class) {
  $form = array();

  $form['pcid'] = array(
    '#type' => 'value',
    '#value' => $class->pcid,
  );

  $question = t('Are you sure you want to delete the %type product class?', array('%type' => $class->pcid));
  $description = t('The node type associated with this product class will revert to a standard node type.');

  // Find out how many nodes of this class exist and add to the description.
  $count = db_result(db_query("SELECT COUNT(*) FROM {node} WHERE type = '%s'", $class->pcid));

  if ($count > 0) {
    $description .= '<br />'. format_plural($count, 'There is @count node of this type.', 'There are @count nodes of this type.');
  }

  return confirm_form($form, $question, 'admin/store/products/classes', $description, t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for uc_product_class_delete_confirm().
 *
 * @see uc_product_class_delete_confirm()
 */
function uc_product_class_delete_confirm_submit($form, &$form_state) {
  $type = node_get_types('type', $form_state['values']['pcid']);
  $type->module = 'node';
  $type->custom = 1;
  node_type_save($type);

  db_query("DELETE FROM {uc_product_classes} WHERE pcid = '%s'", $form_state['values']['pcid']);
  module_invoke_all('product_class', $form_state['values']['pcid'], 'delete');
  uc_product_node_info(TRUE);
  node_types_rebuild();
  menu_rebuild();

  drupal_set_message(t('Product class %type deleted.', array('%type' => $form_state['values']['pcid'])));

  $form_state['redirect'] = 'admin/store/products/classes';
}
