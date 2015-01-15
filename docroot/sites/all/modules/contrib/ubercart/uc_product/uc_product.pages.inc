<?php

/**
 * @file
 * Defines page callbacks for the product module.
 */

/**
 * Returns an autocomplete list for product nodes.
 *
 * Using this autocomplete on a textfield will autocomplete based on product
 * titles or SKUs and leave the nid in the textfield.
 */
function uc_product_title_sku_autocomplete($string = '') {
  $matches = array();

  if ($string) {
    $result = db_query_range(db_rewrite_sql("SELECT n.nid, n.title, p.model, p.unique_hash FROM {uc_products} AS p LEFT JOIN {node} AS n ON n.nid = p.nid WHERE p.unique_hash <> '' AND (LOWER(n.title) LIKE '%s%%' OR LOWER(p.model) LIKE '%s%%')"), strtolower($string), strtolower($string), 0, 10);
    while ($node = db_fetch_object($result)) {
      $matches[$node->nid] = t('@title [@sku]', array('@title' => $node->title, '@sku' => $node->model));
    }
  }

  drupal_json($matches);
}
