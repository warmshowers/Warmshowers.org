<?php

/**
 * @file
 * Contains the node view row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 */
class uc_order_plugin_row_invoice extends views_plugin_row {

  // Basic properties that let the row style follow relationships.
  var $base_table = 'uc_orders';
  var $base_field = 'order_id';

}
