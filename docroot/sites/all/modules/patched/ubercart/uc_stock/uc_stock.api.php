<?php

/**
 * @file
 * Hooks provided by the Stock module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows modules to take action when a stock level is changed.
 *
 * @param $sku
 *   The SKU whose stock level is being changed.
 * @param $stock
 *   The stock level before the adjustment.
 * @param $qty
 *   The amount by which the stock level was changed.
 */
function hook_uc_stock_adjusted($sku, $stock, $qty) {
  $params = array(
    'sku' => $sku,
    'stock' => $stock,
    'qty' => $qty,
  );

  drupal_mail('uc_stock_notify', 'stock-adjusted', uc_store_email_from(), language_default(), $params);
}

/**
 * @} End of "addtogroup hooks".
 */
