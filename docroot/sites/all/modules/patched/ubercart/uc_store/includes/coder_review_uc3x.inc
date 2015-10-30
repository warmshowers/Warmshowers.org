<?php

/**
 * @file
 * Implements Coder Upgrade functionality for Ubercart 6.x -> 7.x upgrades.
 */

/**
 * Warns about namespace and function name changes.
 */
function coder_review_uc3x_reviews() {
  $rules = array(
    // Ubercart hook names were namespaced.
    array(
      '#type' => 'regex',
      '#value' => '(?<!uc)_(add_to_cart|add_to_cart_data|calculate_tax|cart_display|(?<!update_)cart_item|cart_pane|checkout_pane|download_authorize|file_action|file_transfer_alter|line_item|line_item_alter|line_item_data_alter|order|order_actions|order_pane|order_product|order_product_alter|order_state|payment_method|payment_gagteway|product_class|product_description|product_description_alter|product_feature|product_types|shipment|shipping_type|shipping_method|store_status|update_cart_item)\s*\(',
      '#source' => 'all',
      '#never' => 'function theme',
      '#warning_callback' => '_coder_review_uc3x_hook_warning',
    ),

    // Private functions were moved into uc_ namespace.
    array(
      '#type' => 'regex',
      // (?<![(|]) prevents this very line from generating a warning.
      '#value' => '\b(?<![(|])(_2checkout_post_url|_call_order_pane_byref|_checkout_pane_(data|list)|_country_import_(include|list)|_get_order_screen_titles|_line_item_(data|list)|_order_pane_(data|list)|_parse_cs_avs_code|_parse_cs_cvv_code|_parse_cs_reason_code|_payment_gateway_(data|list)|_payment_method_(data|list)|_save_cc_data_to_order|_store_footer_options|summarize_(checkbox|child_form_pages|element|form|null)|_total_sort|_valid_card_(expiration|issue|number|start)|_valid_cvv)\b',
      '#source' => 'all',
      '#warning_callback' => '_coder_review_uc3x_function_warning',
    ),

    // Theme functions were namespaced.
    array(
      '#type' => 'regex',
      '#value' => 'theme\([\'"](address_pane|cart_review_table|summary_overview)[\'"]',
      '#warning_callback' => '_coder_review_uc3x_function_warning',
    ),

    // TAPIr table ids were namespaced.
    array(
      '#type' => 'regex',
      '#value' => '(?<!uc_)op_((admin|order)_comments|products)_(customer|edit|view)_table',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_uc3x_function_warning',
    ),

    // uc_payment_process() is now uc_payment_process_payment().
    array(
      '#type' => 'regex',
      '#value' => '\buc_payment_process\s*\(',
      '#warning' => 'uc_payment_process() is now uc_payment_process_payment() because of hook_process().',
    ),

    // uc_add_js() was removed, use drupal_add_js() instead.
    array(
      '#type' => 'grep',
      '#value' => 'uc_add_js(',
      '#warning' => 'uc_add_js() has been removed.  Use drupal_add_js() instead.',
    ),

    // First argument to uc_product_get_models() and hook_uc_product_models()
    // changed from $node to $nid.
    array(
      '#type' => 'regex',
      '#value' => '(\buc_product_get_models|\B_uc_product_models)\s*\(',
      '#warning' => 'First argument to uc_product_get_models() and hook_uc_product_models() has changed - it is now an integer node id instead of a node object.',
    ),

    // Checkout and order pane callback arguments changed.
    array(
      '#type' => 'regex',
      '#value' => 'pane.*\(\$op,\s*&?\$(arg1|order)(,\s*&?\$arg2)?',
      '#never' => '\(\$op,\s*&?\$order,\s*&?\$form\s*=\s*NULL,\s*&\$form_state\s*=\s*NULL\)',
      '#warning' => 'Checkout and order pane callbacks take the arguments ($op, $order, &$form = NULL, &$form_state = NULL).',
    ),

    // uc_price() removed.
    array(
      '#type' => 'grep',
      '#value' => 'uc_price(',
      '#warning' => 'The function uc_price() was removed.',
    ),

    // hook_uc_price_handler() removed.
    array(
      '#type' => 'regex',
      '#value' => '\B_uc_price_handler\(',
      '#warning' => 'hook_uc_price_handler() was removed.',
      '#severity' => 'normal',
    ),

    // theme_uc_price() has only one argument.
    array(
      '#type' => 'regex',
      '#value' => 'theme\s*\(\s*[\'"]uc_price[\'"]\s*,[^,]+,',
      '#warning' => 'theme_uc_price() takes only one argument: "price", which is a float.',
    ),

    // Specify the allowed arguments to theme_uc_product_price() before it is ported.
    array(
      '#type' => 'regex',
      '#value' => 'theme\s*\(\s*[\'"]uc_product_price[\'"]\s*,[^,]+,',
      '#not' => '[\'"]#price',
      '#warning' => 'theme_uc_product_price() takes a render element with a "#value" and optional "#title" and "#attributes".'
    ),

    // uc_order_load_line_items() has only one argument.
    array(
      '#type' => 'regex',
      '#value' => 'uc_order_load_line_items\([^,]+,',
      '#warning' => 'uc_order_load_line_items() takes only one argument, the order object. Both stored and calculated line items are returned at once.',
    ),
  );

  $review = array(
    '#title' => t('Converting Ubercart 2.x modules to 3.x'),
    '#rules' => $rules,
    '#severity' => 'critical',
  );

  return array('ubercart3x' => $review);
}

function _coder_review_uc3x_hook_warning() {
  return array(
    '#warning' => 'Ubercart hooks have been namespaced with "uc_" in the front.',
    '#link' => 'http://drupal.org/node/510382',
  );
}

function _coder_review_uc3x_function_warning() {
  return array(
    '#warning' => 'Ubercart private functions, theme functions, and TAPIr table ids have been namespaced with "uc_" in the front.',
    '#link' => 'http://drupal.org/node/750784',
  );
}
