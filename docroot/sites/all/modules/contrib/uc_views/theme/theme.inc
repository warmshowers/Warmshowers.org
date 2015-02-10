<?php

/**
 * @file
 * Patch by hanoii
 */

/**
 * Template helper for theme_views_view_row_node
 */
function template_preprocess_uc_views_view_row_invoice(&$vars) {
  require_once drupal_get_path('module', 'uc_order') . '/uc_order.admin.inc';
  $order_id = $vars['row']->{$vars['field_alias']};

  if (module_exists('uc_credit')) {
    // Clear credit cache, otherwise the load function below will return
    // the same cached credit information for all the subsequent orders
    // invoices
    uc_credit_cache('clear');
  }
  $order = uc_order_load($order_id);

  $invoice = uc_order_invoice($order);
  $vars['invoice'] = $invoice;
}
