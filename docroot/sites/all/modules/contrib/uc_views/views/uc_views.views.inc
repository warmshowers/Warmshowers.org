<?php
/**
 * @file
 * Views 2 hooks and callback registries.
 */

define('UC_PAYER_PREFIX', "Payer's");
define('UC_RECIPIENT_PREFIX', "Recipient's");

/**
 * Implementation of hook_views_data().
 */
function uc_views_views_data() {
  // uc_order_products table
  $order_products = drupal_get_schema('uc_order_products');
  $data['uc_order_products']['table']['group'] = t('Ubercart order product');
  $data['uc_order_products']['table']['base'] = array(
    'field' => 'order_product_id',
    'title' => t('Ubercart order product'),
    'help' => t("Products that have been ordered in your store."),
  );

  $data['uc_order_products']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_order_products']['table']['join']['uc_orders'] = array(
    'left_field' => 'order_id',
    'field' => 'order_id',
  );

  $data['uc_order_products']['order_product_id'] = array(
    'title' => t('Order product ID'),
    'help' => $order_products['fields']['order_product_id']['description'],
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'order_product_id',
    ),
  );

  $data['uc_order_products']['model'] = array(
    'title' => t('SKU'),
    'help' => $order_products['fields']['model']['description'],
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
    'title' => t('Qty'),
    'help' => $order_products['fields']['qty']['description'],
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
    'help' => $order_products['fields']['price']['description'],
    'field' => array(
      'handler' => 'uc_views_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );

  $data['uc_order_products']['cost'] = array(
    'title' => t('Cost'),
    'help' => $order_products['fields']['cost']['description'],
    'field' => array(
      'handler' => 'uc_views_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );

  $data['uc_order_products']['weight'] = array(
    'title' => t('Weight'),
    'help' => $order_products['fields']['weight']['description'],
    'field' => array(
      'additional fields' => array(
        'weight_units' => array(
          'table' => 'uc_products',
          'field' => 'weight_units',
        ),
      ),
      'handler' => 'uc_product_handler_field_weight',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );

  $data['uc_order_products']['nid'] = array(
    'title' => t('Product nid'),
    'help' => t('The node ID of the ordered product'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'title' => t('Product'),
    ),
  );

  $data['uc_order_products']['title'] = array(
    'title' => t('Title'),
    'help' => $order_products['fields']['title']['description'],
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

  // uc_orders table
  $order_schema = drupal_get_schema('uc_orders');
  $data['uc_orders']['table']['group'] = t('Ubercart order');
  $data['uc_orders']['table']['base'] = array(
    'field' => 'order_id',
    'title' => t('Ubercart orders'),
    'help' => t('Orders placed in your Ubercart store.'),
  );

  $data['uc_orders']['table']['join']['node'] = array(
    'left_table' => 'uc_order_products',
    'left_field' => 'order_id',
    'field' => 'order_id',
  );

  $data['uc_orders']['table']['join']['uc_order_products'] = array(
    'left_field' => 'order_id',
    'field' => 'order_id',
  );

  $data['uc_orders']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  $data['uc_orders']['order_id'] = array(
    'title' => t('Order ID'),
    'help' => $order_schema['fields']['order_id']['description'],
    'field' => array(
      'handler' => 'uc_views_handler_field_order_id',
      'click sortable' => TRUE,
      'additional fields' => array(
        'uid' => 'uid',
      ),
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

  $data['uc_orders']['order_status'] = array(
    'title' => t('Status'),
    'help' => $order_schema['fields']['order_status']['description'],
    'field' => array(
      'handler' => 'uc_views_handler_field_order_status',
      'click sortable' => TRUE,
    ),
     'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_order_status',
    ),
  );

  $data['uc_orders']['uid'] = array(
    'title' => t('Uid'),
    'help' => $order_schema['fields']['uid']['description'],
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'uid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
      'title' => t('Name'),
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Order user'),
    ),
  );

  $data['uc_orders']['order_total'] = array(
    'title' => t('Total'),
    'help' => $order_schema['fields']['order_total']['description'],
    'field' => array(
      'handler' => 'uc_views_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );

  $data['uc_orders']['order_balance'] = array(
    'title' => t('Balance'),
    'help' => ('The total balance of the order.'),
    'field' => array(
      'handler' => 'uc_views_handler_field_order_balance',
      'float' => TRUE,
      'additional fields' => array(
        'order_id' => 'order_id',
        'order_total' => 'order_total',
      ),
    ),
  );

  $data['uc_orders']['order_total_cost'] = array(
    'title' => t('Total cost'),
    'help' => t('The total cost of the products in the order.'),
    'field' => array(
      'handler' => 'uc_views_handler_field_order_cost',
      'float' => TRUE,
    ),
  );

  $data['uc_orders']['payment_method'] = array(
    'title' => t('Payment method'),
    'help' => $order_schema['fields']['payment_method']['description'],
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'uc_views_handler_filter_payment_method',
    ),
  );

  $data['uc_orders']['cc_type'] = array(
    'title' => t('Credit card type'),
    'help' => t('Credit card type used if the payment method was credit.'),
    'real field' => 'data',
    'field' => array(
      'handler' => 'uc_views_handler_field_order_cc_data',
      'cc field' => 'cc_type',
    ),
  );

  $data['uc_orders']['cc_number'] = array(
    'title' => t('Credit card number'),
    'help' => t('Credit card number (last 4) used if the payment method was credit.'),
    'real field' => 'data',
    'field' => array(
      'handler' => 'uc_views_handler_field_order_cc_data',
      'cc field' => 'cc_number',
    ),
  );

  $data['uc_orders']['created'] = array(
    'title' => t('Creation date'),
    'help' => $order_schema['fields']['created']['description'],
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

  $data['uc_orders']['product_count'] = array(
    'title' => t('Product count'),
    'help' => $order_schema['fields']['product_count']['description'],
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

  $data['uc_orders']['modified'] = array(
    'title' => t('Modification date'),
    'help' => $order_schema['fields']['modified']['description'],
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

  $data['uc_orders']['actions'] = array(
    'title' => t('Actions'),
    'help' => t('Returns the actions a user may perform on an order.'),
    'real field' => 'order_id',
    'field' => array(
      'handler' => 'uc_views_handler_field_order_actions',
      'click sortable' => FALSE,
    ),
  );

  $data['uc_orders']['primary_email'] = array(
    'title' => t('Email address'),
    'help' => $order_schema['fields']['primary_email']['description'],
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

  $string_fields = array(
    'first_name' => 'first name',
    'last_name' => 'last name',
    'phone' => 'phone number',
    'company' => 'company',
    'street1' => 'address line 1',
    'street2' => 'address line 2',
    'city' => 'city',
    'postal_code' => 'zip/postal code',
  );

  foreach (array('delivery', 'billing') as $prefix) {
    $args = array(
      '!titleprefix' => ($prefix == 'delivery' ? UC_RECIPIENT_PREFIX : UC_PAYER_PREFIX),
    );
    foreach ($string_fields as $field => $suffix) {
      $data['uc_orders'][$prefix.'_'.$field] = array(
        'title' => t('!titleprefix !titlesuffix', $args + array('!titlesuffix' => $suffix)),
        'help' => $order_schema['fields'][$prefix.'_'.$field]['description'],
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

    $data['uc_orders'][$prefix.'_full_name'] = array(
      'title' => t('!titleprefix full name', $args),
      'help' => $prefix == 'delivery' ? t('The full name (first and last) of the person receiving shipment.') : t('The full name (first and last) of the person paying for the order.'),
      'field' => array(
        'additional fields' => array(
          $prefix . '_first_name',
          $prefix . '_last_name'
        ),
        'handler' => 'uc_views_handler_field_order_fullname',
        'prefix' => $prefix,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
  }

  $data['uc_orders']['weight'] = array(
    'title' => t('Order total\'s weight'),
    'help' => t('The total weight of the order, based on each product\'s weight.'),
    'field' => array(
      'handler' => 'uc_views_handler_field_order_weight',
      'additional fields' => array(
        'order_id' => 'order_id',
      ),
      'float' => TRUE,
    ),
  );

  //Added by openbook
  $comments_schema = drupal_get_schema('uc_order_comments');
  $data['uc_order_comments']['table']['join']['uc_orders'] = array(
    'left_field' => 'order_id',
    'field' => 'order_id',
  );
  $data['uc_order_comments']['table']['join']['uc_order_products'] = array(
    'left_field' => 'order_id',
    'field' => 'order_id',
  );
  $data['uc_order_comments']['table']['group'] = t('Ubercart order comments');
  $data['uc_order_comments']['message'] = array(
    'title' => t('Comment'),
    'help' => $comments_schema['fields']['message']['description'],
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  /**
   * Countries: added by sammys
   */

  $country_schema = drupal_get_schema('uc_countries');

  $country_views = array(
    'billing_countries' => array(
      'field' => 'billing_country',
      'prefix' => UC_PAYER_PREFIX,
    ),
    'delivery_countries' => array(
      'field' => 'delivery_country',
      'prefix' => UC_RECIPIENT_PREFIX,
    ),
  );

  foreach ($country_views as $key => $value) {
    $data['uc_orders'][$value['field']] = array(
      'title' => t($value['prefix'].' country'),
      'help' => t('Relate orders to '.str_replace('_', ' ', $value['field'])),
      'filter' => array(
        'handler' => 'uc_views_handler_filter_country',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'uc_countries',
        'relationship table' => $key,
        'field' => 'country_id',
        'label' => t($value['prefix'].' country'),
      ),
    );

    $data[$key]['table']['group'] = t('Ubercart order');

    $data[$key]['table']['join']['uc_orders'] = array(
      'table' => 'uc_countries',
      'left_field' => $value['field'],
      'field' => 'country_id',
    );
    // join to the uc_order_products, if used as base, via uc_orders
    $data[$key]['table']['join']['uc_order_products'] = $data[$key]['table']['join']['uc_orders'];
    $data[$key]['table']['join']['uc_order_products']['left_table'] = 'uc_orders';

    $data[$key]['country_id'] = array(
      'title' => t($value['prefix'].' country id'),
      'help' => $order_schema['fields'][$value['field']]['description'],
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      // Information for accepting an order id as an argument
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'country_iso_code_2',
        'numeric' => TRUE,
        'validate type' => 'country_id',
      ),
      // Information for accepting an order id as a filter
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      // Information for sorting on an order id.
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
    $data[$key]['country_name'] = array(
      'title' => t($value['prefix'].' country name'),
      'help' => t(str_replace(' ID ', ' ', $order_schema['fields'][$value['field']]['description'])),
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
    $data[$key]['country_iso_code_2'] = array(
      'title' => t($value['prefix'].' country ISO code (2 characters)'),
      'help' => $country_schema['fields']['country_iso_code_2']['description'],
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
    $data[$key]['country_iso_code_3'] = array(
      'title' => t($value['prefix'].' country ISO code (3 characters)'),
      'help' => $country_schema['fields']['country_iso_code_3']['description'],
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

  /**
   * Zones: added by sammys
   */
  $zone_schema = drupal_get_schema('uc_zones');

  $zone_views = array(
    'billing_zones' => array(
      'field' => 'billing_zone',
      'prefix' => UC_PAYER_PREFIX,
    ),
    'delivery_zones' => array(
      'field' => 'delivery_zone',
      'prefix' => UC_RECIPIENT_PREFIX,
    ),
  );

  foreach ($zone_views as $key => $value) {
    $data['uc_orders'][$value['field']] = array(
      'title' => t($value['prefix'].' zone'),
      'help' => t('Relate orders to '.str_replace('_', ' ', $value['field'])),
      'filter' => array(
        'handler' => 'uc_views_handler_filter_zone',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'uc_zones',
        'relationship table' => $key,
        'field' => 'zone_id',
        'label' => t($value['prefix'].' zone'),
      ),
    );

    $data[$key]['table']['group'] = t('Ubercart order');

    $data[$key]['table']['join']['uc_orders'] = array(
      'table' => 'uc_zones',
      'left_field' => $value['field'],
      'field' => 'zone_id',
    );

    // join to the uc_order_products, if used as base, via uc_orders
    $data[$key]['table']['join']['uc_order_products'] = $data[$key]['table']['join']['uc_orders'];
    $data[$key]['table']['join']['uc_order_products']['left_table'] = 'uc_orders';

    $data[$key]['zone_id'] = array(
      'title' => t($value['prefix'].' zone id'),
      'help' => $order_schema['fields'][$value['field']]['description'],
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      // Information for accepting an order id as an argument
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'zone_code',
        'numeric' => TRUE,
        'validate type' => 'zone_id',
      ),
      // Information for accepting an order id as a filter
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      // Information for sorting on an order id.
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
    $data[$key]['zone_name'] = array(
      'title' => t($value['prefix'].' zone name'),
      'help' => t(preg_replace('/id|ID/i', ' ', $order_schema['fields'][$value['field']]['description'])),
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

    $data[$key]['zone_code'] = array(
      'title' => t($value['prefix'].' zone code'),
      'help' => t(str_replace('of the ', 'of the '.strtolower($value['prefix']).' ', $zone_schema['fields']['zone_code']['description'])),
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

  // Support for any module's line item, if new modules defines other line items
  // the views cache will have to be rebuilt
  // Although new line items views support should be defined on each module,
  // I don't think this wider apporach would harm. At most, it will duplicate
  // line items
  $line_items = array();
  foreach (_line_item_list() as $line_item) {
    if (!in_array($line_item['id'], array('subtotal', 'tax_subtotal', 'total')) && $line_item['stored']) {
      $line_items[$line_item['id']] = $line_item['title'];
    }
  }
  foreach ($line_items as $line_item_id => $line_item_desc) {
    $data['uc_order_line_items_' . $line_item_id]['table']['group'] = t('Ubercart order line item');

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
        'handler' => 'uc_views_handler_field_money_amount',
        'click sortable' => TRUE,
        'float' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_float',
      ),
    );
  }

  // uc_cart_products table
  $data['uc_cart_products']['table']['group'] = t('Ubercart cart item');
  $data['uc_cart_products']['table']['base'] = array(
    'field' => 'cart_item_id',
    'title' => t('Ubercart cart item'),
    'help' => t('Items in customer carts.'),
  );

  $data['uc_cart_products']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_cart_products']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of the product in the cart.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_cart_products']['cart_id'] = array(
    'title' => t('Cart ID'),
    'help' => t('The ID of the cart (user ID, or session ID for anonymous users).'),
    'field' => array(
      'handler' => 'uc_views_handler_field_cart_user',
      'click sortable' => TRUE,
    ),
    /*
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'cart_id',
    ),
    */
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
  );

  $data['uc_cart_products']['cart_id_current'] = array(
    'real field' => 'cart_id',
    'title' => t('Current cart'),
    'help' => t('Filter the view to the current cart.'),
    'filter' => array(
      'handler' => 'uc_views_handler_filter_cart_current',
      'type' => 'yes-no',
    ),
  );

  $data['uc_cart_products']['qty'] = array(
    'title' => t('Qty'),
    'help' => t('The amount ordered.'),
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

  $data['uc_cart_products']['changed'] = array(
    'title' => t('Modification date'),
    'help' => t('The date the cart item was last changed.'),
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

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function uc_views_views_data_alter(&$data) {
  $data['node']['table']['join']['uc_order_products'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['node']['table']['join']['uc_orders'] = array(
    'left_table' => 'uc_order_products',
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['node']['table']['join']['uc_cart_products'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['users']['table']['join']['uc_orders'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  /*
  $data['users']['table']['join']['uc_cart_products'] = array(
    'left_field' => 'cart_id',
    'field' => 'uid',
  );
  */

  $data['users_roles']['table']['join']['uc_orders'] = array(
    'left_table' => 'users',
    'left_field' => 'uid',
    'field' => 'uid',
  );

  $data['term_node']['table']['join']['uc_orders'] = array(
    'left_table' => 'uc_order_products',
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_products']['table']['join']['uc_order_products'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_products']['table']['join']['uc_cart_products'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_products']['conditionalbuyitnowbutton'] = array(
    'title' => t('Conditional buy it now button'),
    'help' => t('A button to add a product to the cart without quantity or attribute fields, if certain conditions are met.'),
    'group' => t('Product'),
    'field' => array(
      'table' => 'node',
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),      ),
      'handler' => 'uc_views_handler_field_conditional_buyitnow',
    ),
  );

  $data['uc_products']['conditionaladdtocart'] = array(
    'title' => t('Conditional add to cart form'),
    'help' => t("Form to put the product in the customer's cart, if certain conditions are met."),
    'group' => t('Product'),
    'field' => array(
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),
        'type' => array(
          'table' => 'node',
          'field' => 'type',
        ),
      ),
      'handler' => 'uc_views_handler_field_conditional_addtocart',
    ),
  );

  $data['uc_products']['ordering']['field'] = array(
    'help' => t("The product list position."),
    'handler' => 'views_handler_field_numeric',
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function uc_views_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_views') . '/views',
    ),
    'handlers' => array(
      'uc_views_handler_field_order_id' => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_cart_user' => array('parent' => 'views_handler_field',),
      'uc_views_handler_field_order_actions' => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_country' => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_zone' => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_order_fullname'  => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_order_weight'  => array( 'parent' => 'views_handler_field_numeric',),
      'uc_views_handler_field_order_status' => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_order_cc_data' => array( 'parent' => 'views_handler_field',),
      'uc_views_handler_field_order_cost' => array( 'parent' => 'uc_views_handler_field_money_amount',),
      'uc_views_handler_field_order_balance' => array( 'parent' => 'uc_views_handler_field_money_amount',),
      'uc_views_handler_field_conditional_buyitnow' => array('parent' => 'views_handler_field',),
      'uc_views_handler_field_conditional_addtocart' => array('parent' => 'views_handler_field',),
      'uc_views_handler_field_money_amount' => array('parent' => 'uc_product_handler_field_price',),
      'views_handler_filter_order_status' => array('parent' => 'views_handler_filter_in_operator',),
      'uc_views_handler_filter_country' => array('parent' => 'views_handler_filter_in_operator',),
      'uc_views_handler_filter_zone' => array('parent' => 'views_handler_filter_in_operator',),
      'uc_views_handler_filter_payment_method' => array('parent' => 'views_handler_filter_in_operator',),
      'uc_views_handler_filter_cart_current' => array('parent' => 'views_handler_filter_boolean_operator'),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function uc_views_views_plugins() {
  return array(
    'module' => 'uc_views', // This just tells our themes are elsewhere.
    'row' => array(
      'uc_views_invoice' => array(
        'title' => t('Invoice'),
        'help' => t('Display the order with standard invoice view.'),
        'handler' => 'uc_views_plugin_row_invoice_view',
        'path' => drupal_get_path('module', 'uc_views') . '/views',
        'theme' => 'uc_views_view_row_invoice',
        'theme path' => drupal_get_path('module', 'uc_views') . '/theme',
        'theme file' => 'theme.inc',
        'base' => array('uc_orders'), // only works with 'uc_orders' as base.
        //'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_query_substitutions().
 */
function uc_views_views_query_substitutions($view) {
  if (module_exists('uc_cart')) {
    if ($cart_id = uc_cart_get_id(FALSE)) {
      return array('***CURRENT_CART_ID***' => $cart_id);
    }
  }
}
