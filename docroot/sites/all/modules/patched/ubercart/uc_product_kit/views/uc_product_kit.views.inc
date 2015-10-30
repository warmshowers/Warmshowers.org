<?php

/**
 * @file
 * Views hooks for Ubercart product kits.
 */

/**
 * Implements hook_views_data().
 */
function uc_product_kit_views_data() {
  $data['uc_product_kits']['table']['group'] = t('Product kit');

  // Explain how to link product kit nodes to back to their parents.
  $data['node']['table']['join']['uc_product_kits'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['uc_products']['table']['join']['uc_product_kits'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );

  // Relate components to their parent kits.
  $data['uc_products']['product_kit'] = array(
    'group' => t('Product kit'),
    'relationship' => array(
      'title' => t('Parent'),
      'help' => t('Relate product kit components to their parent kits.'),
      'handler' => 'views_handler_relationship',
      'base' => 'uc_product_kits',
      'base field' => 'product_id',
      'relationship field' => 'nid',
      'label' => t('kit'),
    ),
  );

  // Explain how to link parents back to their components.
  $data['uc_product_kit_parents']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'table' => 'uc_product_kits',
  );

  // Relate kits to their components.
  $data['uc_products']['product_kit_component'] = array(
    'group' => t('Product kit'),
    'relationship' => array(
      'title' => t('Component'),
      'help' => t('Relate product kits to their components. This relationship will create one record for each component of the kit.'),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'relationship table' => 'uc_product_kit_parents',
      'relationship field' => 'product_id',
      'label' => t('component'),
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
