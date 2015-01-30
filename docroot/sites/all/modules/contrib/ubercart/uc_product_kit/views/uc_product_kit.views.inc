<?php

/**
 * @file
 * Views 2 hooks and callback registries.
 */

/**
 * Implements hook_views_data()
 */
function uc_product_kit_views_data() {
  $data = array();

  $data['uc_product_kits']['table'] = array(
    'group' => t('Product Kit'),
    'join' => array(
      'uc_products' => array(
        'left_field' => 'nid',
        'field' => 'product_id',
      ),
      'node' => array(
        'left_field' => 'nid',
        'field' => 'product_id',
      ),
    ),
  );

  $data['uc_product_kits']['nid'] = array(
    'title' => t('Node ID'),
    'help' => t('The {node}.nid of the product kit.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('kit'),
    ),
  );

  $data['uc_product_kits']['product_id'] = array(
    'title' => t('Product'),
    'help' => t('The {uc_products}.nid of a product contained in the kit.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'uc_products',
      'base field' => 'nid',
      'label' => t('product'),
    ),
  );

  $data['uc_product_kits']['mutable'] = array(
    'title' => t('Mutable'),
    'help' => t('A flag indicating whether the contents of the kit can be changed by the customer.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_product_kits']['qty'] = array(
    'title' => t('Quantity'),
    'help' => t('The number of this product contained in the kit.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_product_kits']['discount'] = array(
    'title' => t('Discount'),
    'help' => t('The adjustment to the {uc_products}.sell_price of the product.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
      'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_product_kits']['ordering'] = array(
    'title' => t('Weight'),
    'help' => ('The weight of this product in relation to other products in the kit.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_product_kits']['synchronized'] = array(
    'title' => t('Synchronized'),
    'help' => t('A flag indicating that changes to the sell price of this product will change the total price of the kit. 1 => Yes. 0 => No.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_product_kits']['is_kit_item'] = array(
    'title' => t('Is part of a product kit'),
    'help' => t('Check for the fields provided by the Ubercart product_kit module.'),
    'group' => t('Node'),
    'filter' => array(
      'handler' => 'uc_product_kit_handler_filter_product_kit_item',
      'label' => t('Is part of a product kit'),
    ),
  );

  $data['uc_product_kits']['is_kit'] = array(
    'title' => t('Is a product kit'),
    'help' => t('Check for the fields provided by the Ubercart product_kit module.'),
    'group' => t('Node'),
    'filter' => array(
      'handler' => 'uc_product_kit_handler_filter_product_kit',
      'label' => t('Is a product kit'),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function uc_product_kit_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_product_kit') .'/views',
    ),
    'handlers' => array(
      'uc_product_kit_handler_filter_product_kit' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'uc_product_kit_handler_filter_product_kit_item' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}
