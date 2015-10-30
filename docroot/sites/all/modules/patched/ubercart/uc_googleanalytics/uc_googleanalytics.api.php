<?php

/**
 * @file
 * Hooks provided by the Google Analytics for Ubercart module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Determines whether e-commerce tracking code should be added to the page.
 *
 * The Google Analytics module takes care of adding the necessary .js file from
 * Google for tracking general statistics.  The UC Google Analytics module works
 * in conjunction with this code to add e-commerce specific code.  However, the
 * e-commerce code should only be added on appropriate pages.  Generally, the
 * correct page will be the checkout completion page at cart/checkout/complete.
 * However, because modules can change the checkout flow as necessary, it must
 * be possible for alternate pages to be used.
 *
 * This hook allows other modules to tell the UC Google Analytics module that
 * it should go ahead and add the e-commerce tracking code to the current page.
 * A module simply needs to implement this hook and return TRUE on the proper
 * order completion page to let UC Google Analytics know it should add the
 * e-commerce tracking code to the current page.
 *
 * The implementation below comes from the 2Checkout.com module which uses an
 * alternate checkout completion page.
 *
 * @return
 *   TRUE if e-commerce tracking code should be added to the current page.
 */
function hook_ucga_display() {
  // Tell UC Google Analytics to display the e-commerce JS on the custom
  // order completion page for this module.
  if (arg(0) == 'cart' && arg(1) == '2checkout' && arg(2) == 'complete') {
    return TRUE;
  }
}

/**
 * Allows modules to alter items passed to the e-commerce tracking code.
 *
 * The UC Google Analytics module constructs function calls that work through
 * the Google Analytics JS API to report purchased items for e-commerce tracking
 * purposes.  The module builds the argument list for each product on an order
 * and uses this hook to give other modules a chance to alter what gets reported
 * to Google Analytics.  Additional arguments passed to implementations of this
 * hook are provided for context.
 *
 * @param $item
 *   An array of arguments being passed to Google Analytics representing an item
 *   on the order, including order_id, sku, name, category, price, and qty.
 * @param $product
 *   The product object as found in the $order object.
 * @param $trans
 *   The array of arguments that were passed to Google Analytics to represent
 *   the transaction.
 * @param $order
 *   The order object being reported to Google Analytics.
 *
 * @return
 *   Nothing should be returned. Hook implementations should receive the $item
 *   array by reference and alter it directly.
 */
function hook_ucga_item_alter(&$item, $product, $trans, $order) {
  // Example implementation: always set the category to "UBERCART".
  $item['category'] = 'UBERCART';
}

/**
 * Allows modules to alter the transaction data passed to Google Analytics.
 *
 * The UC Google Analytics module constructs function calls that work through
 * the Google Analytics JS API to report order information for e-commerce
 * tracking purposes.  The module builds the argument list for the transaction
 * and uses this hook to give other modules a chance to alter what gets reported
 * to Google Analytics.
 *
 * @param $trans
 *   An array of arguments being passed to Google Analytics representing the
 *   transaction, including order_id, store, total, tax, shipping, city,
 *   state, and country.
 * @param $order
 *   The order object being reported to Google Analytics.
 *
 * @return
 *   Nothing should be returned. Hook implementations should receive the $trans
 *   array by reference and alter it directly.
 */
function hook_ucga_trans_alter(&$trans, $order) {
  // Example implementation: prefix all orders with "UC-".
  $trans['order_id'] = 'UC-' . $trans['order_id'];
}

/**
 * @} End of "addtogroup hooks".
 */
