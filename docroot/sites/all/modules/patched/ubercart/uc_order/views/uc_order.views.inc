<?php

/**
 * @file
 * Views hooks and callback registries.
 */

/**
 * Implements hook_views_data().
 */
function uc_order_views_data() {
  // Orders table.
  $data['uc_orders']['table']['group'] = t('Order');
  $data['uc_orders']['table']['base'] = array(
    'field' => 'order_id',
    'title' => t('Orders'),
    'help' => t('Orders placed in your Ubercart store.'),
    'access query tag' => 'uc_order_access',
  );

  // Pull in user fields directly.
  $data['users']['table']['join']['uc_orders'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  // Order ID field.
  $data['uc_orders']['order_id'] = array(
    'title' => t('Order ID'),
    'help' => t('The order ID.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_order_id',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'order_id',
    ),
  );

  // Order status field.
  $data['uc_orders']['order_status'] = array(
    'title' => t('Order status'),
    'help' => t('The order status.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_order_status',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'uc_order_handler_filter_order_status',
    ),
  );

  // Expose the uid as a relationship.
  $data['uc_orders']['uid'] = array(
    'title' => t('Customer'),
    'help' => t('Relate an order to the user who placed it.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('customer'),
    ),
  );

  // Expose the uid as a relationship to users.
  $data['users']['uc_orders'] = array(
    'title' => t('Orders'),
    'help' => t('Relate a user to the orders they have placed. This relationship will create one record for each order placed by the user.'),
    'relationship' => array(
      'base' => 'uc_orders',
      'base field' => 'uid',
      'relationship field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('orders'),
    ),
  );

  // Changed field handler to display as a price
  $data['uc_orders']['order_total'] = array(
    'title' => t('Order total'),
    'help' => t('The total amount to be paid for the order.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_orders']['order_total_cost'] = array(
    'title' => t('Total cost'),
    'help' => t('The total cost of the products in the order.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_order_cost',
      'float' => TRUE,
    ),
  );

  $data['uc_orders']['payment_method'] = array(
    'title' => t('Payment method'),
    'help' => t('The method of payment.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_payment_method',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'uc_order_handler_filter_payment_method',
    ),
  );

  $data['uc_orders']['cc_type'] = array(
    'title' => t('Credit card type'),
    'help' => t('Credit card type used, if the payment method was credit.'),
    'real field' => 'data',
    'field' => array(
      'handler' => 'uc_order_handler_field_order_cc_data',
      'cc field' => 'cc_type',
    ),
  );

  $data['uc_orders']['cc_number'] = array(
    'title' => t('Credit card number'),
    'help' => t('Credit card number (last 4) used, if the payment method was credit.'),
    'real field' => 'data',
    'field' => array(
      'handler' => 'uc_order_handler_field_order_cc_data',
      'cc field' => 'cc_number',
    ),
  );

  $data['uc_orders']['product_count'] = array(
    'title' => t('Product count'),
    'help' => t('The total number of products in the order.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_orders']['created'] = array(
    'title' => t('Creation date'),
    'help' => t('The date and time the order was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['uc_orders']['modified'] = array(
    'title' => t('Last modified'),
    'help' => t('The time the order was last modified.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Use the sleeker Date module views handler if available.
  if (module_exists('date_views')) {
    $data['uc_orders']['created']['filter'] = array(
      'handler' => 'date_views_filter_handler_simple',
      'is date' => TRUE,
    );
    $data['uc_orders']['modified']['filter'] = array(
      'handler' => 'date_views_filter_handler_simple',
      'is date' => TRUE,
    );
  }

  $data['uc_orders']['actions'] = array(
    'title' => t('Actions'),
    'help' => t('Clickable links to actions a user may perform on an order.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_order_actions',
      'click sortable' => FALSE,
    ),
  );

  $data['uc_orders']['primary_email'] = array(
    'title' => t('Email address'),
    'help' => t('The email address of the customer.'),
    'field' => array(
      'handler' => 'views_handler_field_user_mail',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $addresses = array(
    'billing' => t('Billing address'),
    'delivery' => t('Delivery address'),
  );

  $fields = array(
    'first_name' => t('First name'),
    'last_name' => t('Last name'),
    'phone' => t('Phone number'),
    'company' => t('Company'),
    'street1' => t('Street address 1'),
    'street2' => t('Street address 2'),
    'city' => t('City'),
    'postal_code' => t('Postal code'),
  );

  foreach ($addresses as $prefix => $address) {
    $group = t('Order') . ': ' . $address;

    foreach ($fields as $field => $label) {
      $data['uc_orders'][$prefix . '_' . $field] = array(
        'group' => $group,
        'title' => $label,
        'help' => t('The !field of the !address of the order.', array('!field' => drupal_strtolower($label), '!address' => drupal_strtolower($address))),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
      );
    }

    $data['uc_orders'][$prefix . '_full_name'] = array(
      'group' => $group,
      'title' => t('Full name'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('full name'), '!address' => drupal_strtolower($address))),
      'field' => array(
        'additional fields' => array(
          $prefix . '_first_name',
          $prefix . '_last_name'
        ),
        'handler' => 'uc_order_handler_field_order_fullname',
        'prefix' => $prefix,
      ),
    );

    $data[$prefix . '_countries']['table']['group'] = $group;
    $data[$prefix . '_countries']['table']['join']['uc_orders'] = array(
      'table' => 'uc_countries',
      'left_field' => $prefix . '_country',
      'field' => 'country_id',
    );
    $data[$prefix . '_countries']['country_id'] = array(
      'title' => t('ISO country code (numeric)'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('numeric ISO country code'), '!address' => drupal_strtolower($address))),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'country_iso_code_2',
        'numeric' => TRUE,
        'validate type' => 'country_id',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    );
    $data[$prefix . '_countries']['country_name'] = array(
      'title' => t('Country'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('country name'), '!address' => drupal_strtolower($address))),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
    $data[$prefix . '_countries']['country_iso_code_2'] = array(
      'title' => t('ISO country code (2 characters)'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('ISO country code'), '!address' => drupal_strtolower($address))),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
    $data[$prefix . '_countries']['country_iso_code_3'] = array(
      'title' => t('ISO country code (3 characters)'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('ISO country code'), '!address' => drupal_strtolower($address))),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );

    $data[$prefix . '_zones']['table']['group'] = $group;
    $data[$prefix . '_zones']['table']['join']['uc_orders'] = array(
      'table' => 'uc_zones',
      'left_field' => $prefix . '_zone',
      'field' => 'zone_id',
    );
    $data[$prefix . '_zones']['zone_name'] = array(
      'title' => t('State/Province'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('state or province'), '!address' => drupal_strtolower($address))),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
    $data[$prefix . '_zones']['zone_code'] = array(
      'title' => t('State/Province code'),
      'help' => t('The !field of the !address of the order.', array('!field' => t('state or province code'), '!address' => drupal_strtolower($address))),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
  }

  $data['uc_orders']['total_weight'] = array(
    'title' => t('Total weight'),
    'help' => t('The physical weight of all the products (weight * quantity) in the order.'),
    'real field' => 'weight',
    'field' => array(
      'handler' => 'uc_order_handler_field_order_weight_total',
      'additional fields' => array(
        'order_id' => 'order_id',
      ),
      'float' => TRUE,
    ),
  );

  // Ordered products.
  // Get the standard EntityAPI Views data table.
  $data['uc_order_products'] =  entity_views_table_definition('uc_order_product');
  // Remove undesirable fields
  foreach(array('data') as $bad_field) {
    if (isset($data['uc_order_products'][$bad_field])) {
      unset($data['uc_order_products'][$bad_field]);
    }
  }
  // Fix incomplete fields
  $data['uc_order_products']['weight_units']['title'] = t('Weight units');

  $data['uc_order_products']['table']['group'] = t('Ordered product');
  $data['uc_order_products']['table']['base'] = array(
    'field' => 'order_product_id',
    'title' => t('Ordered products'),
    'help' => t('Products that have been ordered in your Ubercart store.'),
  );

  // Pull in node fields directly.
  $data['node']['table']['join']['uc_order_products'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  // Pull in order fields directly.
  $data['uc_orders']['table']['join']['uc_order_products'] = array(
    'left_field' => 'order_id',
    'field' => 'order_id',
  );

  // Expose products to their orders as a relationship.
  $data['uc_orders']['products'] = array(
    'relationship' => array(
      'title' => t('Products'),
      'help' => t('Relate products to an order. This relationship will create one record for each product ordered.'),
      'handler' => 'views_handler_relationship',
      'base' => 'uc_order_products',
      'base field' => 'order_id',
      'relationship field' => 'order_id',
      'label' => t('products'),
    ),
  );

  // Expose nodes to ordered products as a relationship.
  $data['uc_order_products']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The nid of the ordered product. If you need more fields than the nid: Node relationship'),
    'relationship' => array(
      'title' => t('Node'),
      'help' => t('Relate product to node.'),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'field' => 'nid',
      'label' => t('node'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
    ),
    'field' => array(
      'handler' => 'views_handler_field_node',
    ),
  );

  // Expose orders to ordered products as a relationship.
  $data['uc_order_products']['order_id'] = array(
    'title' => t('Order ID'),
    'help' => t('The order ID of the ordered product. If you need more fields than the order ID: Order relationship'),
    'relationship' => array(
      'title' => t('Order'),
      'help' => t('Relate product to order.'),
      'handler' => 'views_handler_relationship',
      'base' => 'uc_orders',
      'field' => 'order_id',
      'label' => t('order'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'uc_order_handler_field_order_id',
    ),
  );

  $data['uc_order_products']['model'] = array(
    'title' => t('SKU'),
    'help' => t('The product model/SKU.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['uc_order_products']['qty'] = array(
    'title' => t('Quantity'),
    'help' => t('The quantity ordered.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_order_products']['price'] = array(
    'title' => t('Price'),
    'help' => t('The price paid for one product.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_order_products']['total_price'] = array(
    'title' => t('Total price'),
    'help' => t('The price paid for all the products (price * quantity).'),
    'real field' => 'price',
    'field' => array(
      'handler' => 'uc_order_handler_field_money_total',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'uc_order_handler_sort_total',
    ),
    'filter' => array(
      'handler' => 'uc_order_handler_filter_total',
    ),
  );

  $data['uc_order_products']['cost'] = array(
    'title' => t('Cost'),
    'help' => t('The cost to the store for one product.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_order_products']['total_cost'] = array(
    'title' => t('Total cost'),
    'help' => t('The cost to the store for all the products (cost * quantity).'),
    'real field' => 'cost',
    'field' => array(
      'handler' => 'uc_order_handler_field_money_total',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'uc_order_handler_sort_total',
    ),
    'filter' => array(
      'handler' => 'uc_order_handler_filter_total',
    ),
  );

  $data['uc_order_products']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('The physical weight of one product.'),
    'field' => array(
      'additional fields' => array(
        'weight_units' => array(
          'field' => 'weight_units',
        ),
      ),
      'handler' => 'uc_product_handler_field_weight',
      'float' => TRUE,
    ),
  );

  $data['uc_order_products']['total_weight'] = array(
    'title' => t('Total weight'),
    'help' => t('The physical weight of all the products (weight * quantity).'),
    'real field' => 'weight',
    'field' => array(
      'additional fields' => array(
        'weight_units' => array(
          'field' => 'weight_units',
        ),
      ),
      'handler' => 'uc_order_handler_field_weight_total',
      'float' => TRUE,
    ),
  );

  $data['uc_order_products']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the product.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Order comments table.
  // TODO: refactor this into a groupwise max relationship.
  $data['uc_order_comments']['table']['group'] = t('Order comments');
  $data['uc_order_comments']['table']['join'] = array(
    'uc_orders' => array(
      'left_field' => 'order_id',
      'field' => 'order_id',
    ),
    'uc_order_products' => array(
      'left_table' => 'uc_orders',
      'left_field' => 'order_id',
      'field' => 'order_id',
    ),
  );

  $data['uc_order_comments']['message'] = array(
    'title' => t('Comment'),
    'help' => t('The comment body.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  // Support for any module's line item, if new modules defines other line items
  // the views cache will have to be rebuilt
  // Although new line items views support should be defined on each module,
  // I don't think this wider apporach would harm. At most, it will duplicate
  // line items
  $line_items = array();
  foreach (_uc_line_item_list() as $line_item) {
    if (!in_array($line_item['id'], array('subtotal', 'tax_subtotal', 'total', 'generic')) && $line_item['stored']) {
      $line_items[$line_item['id']] = $line_item['title'];
    }
  }
  foreach ($line_items as $line_item_id => $line_item_desc) {
    $data['uc_order_line_items_' . $line_item_id]['table']['join']['uc_orders'] = array(
      'table' => 'uc_order_line_items',
      'left_field' => 'order_id',
      'field' => 'order_id',
      'extra' => array(
        array(
          'field' => 'type',
          'value' => $line_item_id,
        ),
      ),
    );
    $data['uc_order_line_items_' . $line_item_id]['table']['join']['uc_order_products'] = $data['uc_order_line_items_' . $line_item_id]['table']['join']['uc_orders'];
    $data['uc_order_line_items_' . $line_item_id]['table']['join']['uc_order_products']['left_table'] = 'uc_orders';

    $data['uc_order_line_items_' . $line_item_id]['table']['group'] = t('Order: Line item');
    $data['uc_order_line_items_' . $line_item_id]['title'] = array(
      'title' => t('!line_item_desc title', array('!line_item_desc' => $line_item_desc)),
      'help' => t('!line_item_desc order line item', array('!line_item_desc' => $line_item_desc)),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );

    $data['uc_order_line_items_' . $line_item_id]['amount'] = array(
      'title' => t('!line_item_desc amount', array('!line_item_desc' => $line_item_desc)),
      'help' => t('!line_item_desc order line item', array('!line_item_desc' => $line_item_desc)),
      'field' => array(
        'handler' => 'uc_order_handler_field_money_amount',
        'click sortable' => TRUE,
        'float' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function uc_order_views_plugins() {
  return array(
    'row' => array(
      'uc_order_invoice' => array(
        'title' => t('Invoice'),
        'help' => t('Display the order with standard invoice view.'),
        'handler' => 'uc_order_plugin_row_invoice',
        'theme' => 'uc_order_view_row_invoice',
        'theme path' => drupal_get_path('module', 'uc_order') . '/views',
        'base' => array('uc_orders'), // only works with 'uc_orders' as base.
        'type' => 'normal',
      ),
    ),
    'argument validator' => array(
      'user_or_permission' => array(
        'title' => t('Current user or user has permission'),
        'handler' => 'uc_order_plugin_argument_validate_user_perm',
      ),
    ),
  );
}

/**
 * Preprocesses a views row invoice plugin.
 *
 * @see uc_order-view-row-invoice.tpl.php
 */
function template_preprocess_uc_order_view_row_invoice(&$vars) {
  $order_id = $vars['row']->{$vars['field_alias']};

  if (module_exists('uc_credit')) {
    // Clear credit cache, otherwise the load function below will return
    // the same cached credit information for all the subsequent orders
    // invoices
    uc_credit_cache('clear');
  }
  $order = uc_order_load($order_id);

  $build = array(
    '#theme' => 'uc_order',
    '#order' => $order,
    '#op' => 'view',
    '#template' => variable_get('uc_cust_order_invoice_template', 'customer'),
  );

  $vars['invoice'] = theme('uc_order_invoice_page', array('content' => drupal_render($build)));
}
