<?php

/**
 * @file
 * Hooks provided by the Shipping Quotes module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Defines callbacks and service options for shipping methods.
 *
 * @return
 *   An array of shipping methods, keyed by the unique method ID, and with the
 *   following members:
 *   - id: The unique method ID, the same as the array key for this method.
 *   - module: The name of the implementing module.
 *   - title: The shipping method title.
 *   - description: (optional) A short description of the shipping method.
 *   - operations: (optional) A set of links that can be used to edit or
 *     configure the shipping method, suitable for passing to theme_links().
 *   - quote: (optional) An associative array, with the following members:
 *     - type: The quote and shipping types are ids of the product shipping type
 *       that these methods apply to. type may also be 'order' which indicates
 *       that the quote applies to the entire order, regardless of the shipping
 *       types of its products. This is used by quote methods that are based on
 *       the location of the customer rather than their purchase.
 *     - callback: The function that is called by uc_quote when a shipping quote
 *       is requested. Its arguments are the array of products and an array of
 *       order details (the shipping address). The return value is an array
 *       representing the rates quoted and errors returned (if any) for each
 *       option in the accessorials array.
 *     - file: (optional) The name of the file that contains the callback
 *       function.
 *     - accessorials: This array represents the different options the customer
 *       may choose for their shipment. The callback function should generate a
 *       quote for each option in accessorials and return them via an array.
 *       @code
 *       return array(
 *         '03' => array('rate' => 15.75,  'option_label' => t('UPS Ground'),
 *                       'error' => 'Additional handling charge applied.'),
 *         '14' => array('error' => 'Invalid package type.'),
 *         '59' => array('rate' => 26.03, 'option_label' => t('UPS 2nd Day'))
 *       );
 *       @endcode
 *     - pkg_types: The list of package types that the shipping method can
 *       handle. This should be an associative array that can be used as the
 *       #options of a select form element. It is recommended that a function
 *       be written to output this array so the method doesn't need to be found
 *       just for the package types.
 *   - ship: (optional) An associative array, in the same format as 'quote'.
 *   - enabled: (optional) Whether the method should be enabled by default.
 *     Defaults to FALSE.
 *   - weight: (optional) The default position of the method in the list of
 *     shipping quotes. Defaults to 0.
 */
function hook_uc_shipping_method() {
  $methods = array();

  $methods['ups'] = array(
    'id' => 'ups',
    'title' => t('UPS'),
    'quote' => array(
      'type' => 'small package',
      'callback' => 'uc_ups_quote',
      'accessorials' => array(
        '03' => t('UPS Ground'),
        '11' => t('UPS Standard'),
        '01' => t('UPS Next Day Air'),
        '13' => t('UPS Next Day Air Saver'),
        '14' => t('UPS Next Day Early A.M.'),
        '02' => t('UPS 2nd Day Air'),
        '59' => t('UPS 2nd Day Air A.M.'),
        '12' => t('UPS 3-Day Select'),
      ),
    ),
    'ship' => array(
      'type' => 'small package',
      'callback' => 'uc_ups_fulfill_order',
      'pkg_types' => array(
        '01' => t('UPS Letter'),
        '02' => t('Customer Supplied Package'),
        '03' => t('Tube'),
        '04' => t('PAK'),
        '21' => t('UPS Express Box'),
        '24' => t('UPS 25KG Box'),
        '25' => t('UPS 10KG Box'),
        '30' => t('Pallet'),
      ),
    ),
  );

  return $methods;
}

/**
 * Defines shipping types for shipping methods.
 *
 * This hook defines a shipping type that this module is designed to handle.
 * These types are specified by a machine- and human-readable name called 'id',
 * and 'title' respectively. Shipping types may be set for individual products,
 * manufacturers, and for the entire store catalog. Shipping modules should be
 * careful to use the same shipping type ids as other similar shipping modules
 * (i.e., FedEx and UPS both operate on "small package" shipments). Modules that
 * do not fulfill orders may not need to implement this hook.
 *
 * @return
 *   An array of shipping types keyed by a machine-readable name.
 */
function hook_uc_shipping_type() {
  $weight = variable_get('uc_quote_type_weight', array('small_package' => 0));

  $types = array();
  $types['small_package'] = array(
    'id' => 'small_package',
    'title' => t('Small package'),
    'weight' => $weight['small_package'],
  );

  return $types;
}

/**
 * @} End of "addtogroup hooks".
 */
