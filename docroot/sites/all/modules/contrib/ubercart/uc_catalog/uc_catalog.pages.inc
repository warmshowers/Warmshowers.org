<?php

/**
 * @file
 * Catalog menu items.
 */

/**
 * Display a formatted catalog page.
 *
 * If the category has products in it, display them in a TAPIr table. Subcategories
 *   are linked along the top of the page. If it does not have products, display
 *   subcategories in a grid with their images and subcategories.
 *
 * @param $tid
 *   Catalog term id from URL.
 * @return
 *   Formatted HTML of the catalog page.
 * @ingroup themeable
 */
function theme_uc_catalog_browse($tid = 0) {
  drupal_add_css(drupal_get_path('module', 'uc_catalog') .'/uc_catalog.css');

  $output = '';
  $catalog = uc_catalog_get_page((int)$tid);
  drupal_set_title(check_plain($catalog->name));
  drupal_set_breadcrumb(uc_catalog_set_breadcrumb($catalog->tid));
  $types = uc_product_types();
  $links = array();
  $child_list = array();
  foreach ($catalog->children as $child) {
    if (!empty($child->nodes)) {
      $links[] = array('title' => $child->name . (variable_get('uc_catalog_breadcrumb_nodecount', FALSE) ? ' ('. $child->nodes .')' : ''), 'href' => uc_catalog_path($child),
        'attributes' => array('rel' => 'tag'),
      );
    }
    if (!empty($child->image)) {
      $image = '<div>';
      if (module_exists('imagecache')) {
        $image .= l(theme('imagecache', 'uc_category', $child->image['filepath']), uc_catalog_path($child), array('html' => TRUE));
      }
      else {
        $image .= l(theme('image', $child->image['filepath']), uc_catalog_path($child), array('html' => TRUE));
      }
      $image .= '</div>';
    }
    else {
      $image = '<div></div>';
    }
    $grandchildren = array();
    $j = 0;
    $max_gc_display = 3;
    foreach ($child->children as $i => $grandchild) {
      if ($j > $max_gc_display) {
        break;
      }
      $g_child_nodes = 0;
      foreach ($types as $type) {
        $g_child_nodes += taxonomy_term_count_nodes($grandchild->tid, $type);
      }
      if ($g_child_nodes) {
        $grandchildren[$i] = l($grandchild->name, uc_catalog_path($grandchild), array('class' => 'subcategory'));
        $j++;
      }
    }
    //$grandchildren = array_slice($grandchildren, 0, intval(count($grandchildren) / 2) + 1, TRUE);
    if ($j > $max_gc_display) {
      array_push($grandchildren, l(t('More...'), uc_catalog_path($child), array('class' => 'subcategory')));
    }
    if ($child->nodes) {
      $cell_link = $image .'<strong>'. l($child->name, uc_catalog_path($child)) .'</strong>';
      if (variable_get('uc_catalog_show_subcategories', TRUE)) {
        $cell_link .= "<br /><span>". implode(', ', $grandchildren) ."</span>\n";
      }
      $child_list[] = $cell_link;
    }
  }
  if (!empty($catalog->image)) {
    if (module_exists('imagecache')) {
      $output .= theme('imagecache', 'uc_thumbnail', $catalog->image['filepath'], $catalog->name, $catalog->name, array('class' => 'category'));
    }
    else {
      $output .= theme('image', $catalog->image['filepath'], $catalog->name, $catalog->name, array('class' => 'category'));
    }
  }

  // Build an ORDER BY clause for the SELECT query based on table sort info.
  if (empty($_REQUEST['order'])) {
    $order = 'ORDER BY p.ordering, n.title, n.nid';
  }
  else {
    $order = tapirsort_sql(uc_product_table_header());
  }

  $sql = "SELECT DISTINCT(n.nid), n.sticky, n.title, n.created, p.model, p.sell_price, p.ordering
    FROM {node} n
      INNER JOIN {term_node} tn ON n.vid = tn.vid
      INNER JOIN {uc_products} AS p ON n.vid = p.vid
    WHERE tn.tid = %d AND n.status = 1
      AND n.type IN (". db_placeholders($types, 'varchar') .") ". $order;

  $sql_count = "SELECT COUNT(DISTINCT(n.nid))
    FROM {node} n
      INNER JOIN {term_node} tn ON n.vid = tn.vid
      INNER JOIN {uc_products} AS p ON n.vid = p.vid
    WHERE tn.tid = %d
      AND n.status = 1
      AND n.type IN (". db_placeholders($types, 'varchar') .")";

  $sql = db_rewrite_sql($sql);
  $sql_count = db_rewrite_sql($sql_count);
  $sql_args = array($catalog->tid);
  foreach ($types as $type) {
    $sql_args[] = $type;
  }
  $catalog->products = array();
  $result = pager_query($sql, variable_get('uc_product_nodes_per_page', 12), 0, $sql_count, $sql_args);
  while ($node = db_fetch_object($result)) {
    $catalog->products[] = $node->nid;
  }
  if (count($catalog->products)) {
    if (count($links)) {
      $output .= theme('links', $links, array('class' => 'links inline uc-categories')) ."<br />\n";
    }
    $output .= $catalog->description;
    $output .= theme('uc_catalog_products', $catalog->products);
    $output .= theme('pager');
  }
  else {
    // Display table of child categories similar to an osCommerce site's front page.
    $columns = variable_get('uc_catalog_category_columns', 3);
    $cat_rows = array();
    $row = array();
    $i = 1;
    foreach ($child_list as $cell) {
      $row[] = array('data' => $cell, 'class' => 'category width-' . $columns);
      if ($i % $columns == 0) {
        $cat_rows[] = $row;
        $row = array();
      }
      $i++;
    }
    if (count($row) > 0 && count($row) < $columns) {
      if (count($cat_rows) >= 1) {
        $row = array_merge($row, array_fill(count($row), $columns - count($row), array('data' => '&nbsp;', 'class' => 'category width-' . $columns)));
      }
      $cat_rows[] = $row;
    }
    $output .= $catalog->description;
    $output .= theme('table', array(), $cat_rows, array('class' => 'category'));
  }

  return $output;
}
