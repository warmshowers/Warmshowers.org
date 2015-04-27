<?php

/**
 * Define the recurring payment method/gateway function callbacks.
 *
 * This hook enables payment modules to register that they support
 * ubercart recurring fees and define the callbacks to trigger when
 * a recurring operation is required using the specific payment
 * method or gateway.
 *
 * @return
 *   An array of recurring fee handler items, each fee handler has a key
 *   corresponding to the unique payment method or gateway id. The item is
 *   an associative array that may contain the following key-value pairs:
 *
 *   - "name": Required. The untranslated title of the menu item.
 *   - "payment method": Required. The type of payment method, this needs
 *     to correspond to another recurring fee handler (e.g. credit).
 *   - "fee handler": the unique id of the payment gateway or
 *     another handler that should handle the recurring fee.
 *   - "module": name of the module that implements this fee handler.
 *   - "process callback":  The function to call when setting up the recurring
 *     fee.
 *   - "renew callbak": Function to call when renewing the recurring fee.
 *   - "cancel callback": Function to call when cancelling a recurring fee.
 *   - "own handler": set to TRUE if this recurring handler will be responsible
 *     for processing renewals and not uc_recurring. (Default: FALSE)
 *   - "saved profile": if set to TRUE then this payment method will be
 *     available to other charges.
 *   - "menu": Array of menu items that provide the user operations.
 *     uc_recurring does provide some common default operations for charge,
 *     edit and cancel which can be reused by setting these to either:
 *     - UC_RECURRING_MENU_DISABLED (default)
 *     - UC_RECURRING_MENU_DEFAULT
 *
 * For a detailed usage example, see modules/uc_recurring.test_gateway.inc.
 *
 * ~~~~ We should put some developer docs online somewhere ~~~~
 * For comprehensive documentation on the ubercart recurring system, see
 * @link http:// drupal.org/node/<nid> http:// drupal.org/node/<nid> @endlink .
 */
function hook_recurring_info() {
  $items = array();
  $items['test_gateway'] = array(
    'name' => t('Test Gateway'),
    'payment method' => 'credit',
    'module' => 'uc_recurring',
    'fee handler' => 'test_gateway',
    'renew callback' => 'uc_recurring_test_gateway_renew',
    'process callback' => 'uc_recurring_test_gateway_process',
    'own handler' => FALSE,
    'saved profile' => FALSE,
    'menu' => array(
      'charge' => UC_RECURRING_MENU_DEFAULT,
      'edit' => array(
        'title' => 'Edit',
        'page arguments' => array('uc_recurring_admin_edit_form'),
        'access callback' => 'user_access',
        'access arguments' => array('administer recurring fees'),
        'file' => 'uc_recurring.admin.inc',
      ),
      'cancel' => array(
        'title' => 'Cancel',
        'page arguments' => array('uc_recurring_user_cancel_form'),
        'file' => 'uc_recurring.pages.inc',
      ),
    ), // Use the default user operation defined in uc_recurring.
  );
  return $items;
}

/**
 * Alter the recurring method/ gateway info.
 *
 * @param $info
 *   Array of the recurring fee handlers.
 */
function hook_recurring_info_alter(&$info) {
  if (!empty($info['test_gateway'])) {
    // Change the permission on the test_gateway so only user with the
    // administer recurring fee permissions can cancel recurring fees.
    $info['test_gateway']['menu']['cancel'] = array(
      'title' => 'Cancel',
      'page arguments' => array('uc_recurring_user_cancel_form'),
      'file' => 'uc_recurring.pages.inc',
      'access callback' => 'user_access',
      'access arguments' => array('administer recurring fees'),
    );
  }
}

/**
 * Act on recurring renewal event.
 *
 * @param $order
 *   The order object.
 * @param $fee
 *   The recurring Fee object.
 */
function hook_recurring_renewal_pending(&$order, &$fee) { }

/**
 * Act on recurring renewal completed event.
 *
 * @param $order
 *   The order object.
 * @param $fee
 *   The recurring Fee object.
 */
function hook_recurring_renewal_completed(&$order, &$fee) { }

/**
 * Act on recurring renewal failed event.
 *
 * @param $order
 *   The order object.
 * @param $fee
 *   The recurring fee object.
 */
function hook_recurring_renewal_failed(&$order, &$fee) { }

/**
 * Act on recurring product deleted.
 *
 * @param $pfid
 *   The product fee ID.
 */
function hook_recurring_product_deleted($pfid) { }

/**
 * Act on recurring user deleted.
 *
 * @param $rfid
 *   the recurring fee ID.
 */
function hook_recurring_user_deleted($rfid) { }

/**
 * Act on recurring user that has just been saved (inserted or updated).
 *
 * @param $fee
 *   The recurring fee object.
 */
function hook_recurring_fee_user_saved($fee) {
}

/**
 * Set the access permission on user operations.
 *
 * @param $fee
 *   The recurring fee object.
 * @param $op
 *   The operation being performed, e.g. cancel, edit, update.
 * @param $account
 *   The account of the user.
 */
function hook_recurring_access($fee, $op, $account) {
  // Deny access to the cancel operation for recurring fees that do not have
  // have an unlimited number of remaining intervals (e.g payment plan)
  if ($op == 'cancel' && $fee->remaining_intervals != UC_RECURRING_UNLIMITED_INTERVALS) {
    return UC_RECURRING_ACCESS_DENY;
  }
  return UC_RECURRING_ACCESS_IGNORE;
}
