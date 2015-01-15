<?php

/**
 * @file
 * Defines administrative pages and form for the product kit module.
 */

/**
 * Builds the product kit settings form.
 *
 * @ingroup forms
 */
function uc_product_kit_settings_form() {
  $form = array();

  $form['uc_product_kit_mutable'] = array(
    '#type' => 'radios',
    '#title' => t('Product kit cart display'),
    '#options' => array(
      UC_PRODUCT_KIT_UNMUTABLE_NO_LIST => t('As a unit. Customers may only change how many kits they are buying. Do not list component products.'),
      UC_PRODUCT_KIT_UNMUTABLE_WITH_LIST => t('As a unit. Customers may only change how many kits they are buying. List component products.'),
      UC_PRODUCT_KIT_MUTABLE => t('As individual products. Customers may add or remove kit components at will. Discounts entered below are not applied to the kit price'),
    ),
    '#default_value' => variable_get('uc_product_kit_mutable', 0),
    '#weight' => -5,
  );

  return system_settings_form($form);
}
