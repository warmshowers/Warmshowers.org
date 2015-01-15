<?php

/**
 * @file
 * Catalog administration menu items.
 */


/**
 * Display an overview of the catalog settings.
 */
function uc_catalog_settings_overview() {
  // Theme all the pages beneath this path into summary overviews.
  return theme('summary_overview', summarize_child_form_pages('admin/store/settings/catalog/edit'));
}

/**
 * Catalog settings form.
 *
 * Configure the display of the catalog breadcrumb.
 *
 * @ingroup forms
 */
function uc_catalog_settings_form() {
  $form = array();

  $vid = variable_get('uc_catalog_vid', NULL);
  if ($vid) {
    $catalog = taxonomy_vocabulary_load($vid);

    $form['catalog_vid'] = array(
      '#prefix' => '<div>',
      '#summary' => t('The taxonomy vocabulary <a href="!edit-url">%name</a> is set as the product catalog.', array('!edit-url' => url('admin/content/taxonomy/'. $vid), '%name' => $catalog->name)),
      '#suffix' => '</div>',
    );
    $form['catalog_vid']['#value'] = $form['catalog_vid']['#summary'];
  }

  $vocabs = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vid => $vocabulary) {
    $vocabs[$vid] = $vocabulary->name;
  }

  // JTR - catalog-top-level sub-textfield
  $form['catalog-top-level'] = array(
    '#type' => 'fieldset',
    '#title' => t('Catalog top level'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#summary callback' => 'summarize_form',
    '#attributes' => array('class' => 'catalog-top-level'),
  );
  $form['catalog-top-level']['uc_catalog_vid'] = array(
    '#type' => 'select',
    '#title' => t('Catalog vocabulary'),
    '#description' => t("The taxonomy vocabulary that will be considered the product catalog."),
    '#default_value' => variable_get('uc_catalog_vid', 0),
    '#summary callback' => 'summarize_null',
    '#options' => $vocabs,
  );
  $form['catalog-top-level']['uc_catalog_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the catalog breadcrumb'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Catalog breadcrumb is being displayed.'),
      t('Catalog breadcrumb is hidden.'),
    ),
    '#default_value' => variable_get('uc_catalog_breadcrumb', TRUE),
  );
  $form['catalog-top-level']['uc_catalog_breadcrumb_nodecount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display node counts in the catalog breadcrumb'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Node count is being displayed in the catalog breadcrumb.'),
      t('Node count is not being displayed in the catalog breadcrumb.'),
    ),
    '#default_value' => variable_get('uc_catalog_breadcrumb_nodecount', FALSE),
  );
  $form['catalog-top-level']['uc_catalog_show_subcategories'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display subcategories in the catalog view'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('The catalog view is displaying subcategories.'),
      t('The catalog view is not displaying subcategories.'),
    ),
    '#default_value' => variable_get('uc_catalog_show_subcategories', TRUE),
  );
  $form['catalog-top-level']['uc_catalog_category_columns'] = array(
    '#type' => 'select',
    '#title' => t('Number of columns in the grid of categories'),
    '#default_value' => variable_get('uc_catalog_category_columns', 3),
    '#summary' => t('Subcategories are being displayed in @columns columns.', array('@columns' => variable_get('uc_catalog_category_columns', 3))),
    '#options' => drupal_map_assoc(uc_range(1, 5)),
  );

  $form['catalog-products-list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Catalog products list'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#summary callback' => 'summarize_form',
    '#attributes' => array('class' => 'catalog-products-list'),
  );
  $form['catalog-products-list']['uc_product_nodes_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Product nodes per page'),
    '#summary' => t('There are @nodes product nodes displayed per page.', array('@nodes' => variable_get('uc_product_nodes_per_page', 12))),
    '#default_value' => variable_get('uc_product_nodes_per_page', 12),
    '#description' => t("Determines how many products will be listed on every catalog category. Notice that if you are using grid display it must be multiple of the grid width value, otherwise the last row will not match."),
    '#size' => 2,
  );

  $form['block-display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Catalog block settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#summary callback' => '_uc_catalog_block_summarize',
    '#attributes' => array('class' => 'block-display'),
  );
  $form['block-display']['uc_catalog_block_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make the block title a link to the top-level catalog page.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Block title is pointing to the top-level catalog page.'),
      t('Block title is not pointing to the top-level catalog page.'),
    ),
    '#default_value' => variable_get('uc_catalog_block_title', FALSE),
  );
  $form['block-display']['uc_catalog_expand_categories'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always expand categories in the catalog block'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Expanding categories in the catalog block.'),
      t('Not expanding categories in the catalog block.'),
    ),
    '#default_value' => variable_get('uc_catalog_expand_categories', FALSE),
  );
  $form['block-display']['uc_catalog_block_nodecount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display node counts in the catalog block'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('The number of nodes in a category are being shown in the catalog block.'),
      t('The number of nodes in a category are not being shown in the catalog block.'),
    ),
    '#default_value' => variable_get('uc_catalog_block_nodecount', TRUE),
  );

  return system_settings_form($form);
}

/**
 * Settings forms for using a product grid instead of a table in the catalog.
 *
 * @ingroup forms
 */
function uc_catalog_grid_settings_form() {
  $form = array();

  $form['uc_catalog_grid_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display products in the catalog in a grid.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Products are displayed in a grid.'),
      t('Products are displayed in a table list.'),
    ),
    '#default_value' => variable_get('uc_catalog_grid_display', FALSE),
    '#description' => t('If unchecked, the default table display will be used and the settings on this form will be ignored.'),
  );
  $form['uc_catalog_grid_display_width'] = array(
    '#type' => 'select',
    '#title' => t('Number of columns in the product grid'),
    '#options' => drupal_map_assoc(uc_range(1, 5)),
    '#default_value' => variable_get('uc_catalog_grid_display_width', 3),
    '#summary' => t('The grid will be displayed in @columns columns.', array('@columns' => variable_get('uc_catalog_grid_display_width', 3))),
  );

  $form['displayed_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Displayed fields'),
    '#description' => t('The checked fields will be included in the product grid for each product in your catalog.'),
  );
  $form['displayed_fields']['uc_catalog_grid_display_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Product title'),
    '#default_value' => variable_get('uc_catalog_grid_display_title', TRUE),
  );
  $form['displayed_fields']['uc_catalog_grid_display_model'] = array(
    '#type' => 'checkbox',
    '#title' => t('Product SKU'),
    '#default_value' => variable_get('uc_catalog_grid_display_model', TRUE),
  );
  $form['displayed_fields']['uc_catalog_grid_display_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default product image'),
    '#description' => t('The image will always be included in the product grid.'),
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  $form['displayed_fields']['uc_catalog_grid_display_sell_price'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sell price'),
    '#default_value' => variable_get('uc_catalog_grid_display_sell_price', TRUE),
  );
  $form['displayed_fields']['uc_catalog_grid_display_add_to_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add to cart form'),
    '#default_value' => variable_get('uc_catalog_grid_display_add_to_cart', TRUE),
  );
  $form['displayed_fields']['uc_catalog_grid_display_attributes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attribute selection elements'),
    '#description' => t('Adds any available attribute selection form elements to the add to cart form if displayed in the grid.'),
    '#default_value' => variable_get('uc_catalog_grid_display_attributes', TRUE),
  );

  return system_settings_form($form);
}

/**
 * Display links to all products that have not been categorized.
 */
function uc_catalog_orphaned_products() {
  $output = '<p>'. t('Orphaned products are products that you have created but
     not yet assigned to a category in your product catalog. All such
     products will appear as links below that you can follow to edit
     the product listings to assign them to categories.') .'</p>';

  $query = "SELECT DISTINCT n.nid, n.title FROM {node} AS n LEFT JOIN {term_node} AS tn ON n.nid = tn.nid LEFT JOIN {vocabulary_node_types} AS vnt ON n.type = vnt.type WHERE n.type <> 'image' AND tn.tid IS NULL AND vnt.vid = %d";
  $vid = variable_get('uc_catalog_vid', 0);
  $result = db_query($query, $vid);

  $rows = array();
  while ($node = db_fetch_object($result)) {
    $rows[] = l($node->title, 'node/'. $node->nid .'/edit', array('query' => array('destination' => 'admin/store/products/orphans')));
  }

  if (count($rows) > 0) {
    $output .= theme('item_list', $rows);
  }
  else {
    $output .= '<p>'. t('All products are currently listed in the catalog.') .'</p>';
  }

  return $output;
}
