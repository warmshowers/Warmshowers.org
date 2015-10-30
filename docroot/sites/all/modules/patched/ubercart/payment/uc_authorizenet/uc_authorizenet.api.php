<?php

/**
 * @file
 * Hooks provided by the Authorize.net module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows transaction data to be altered before sending to Authorize.net.
 *
 * @param $data
 *   The transaction data as specified by the Authorize.net API.
 */
function hook_uc_authorizenet_transaction_alter(&$data) {
  $data['x_description'] = 'Custom Authorize.Net transaction description.';
}

/**
 * @} End of "addtogroup hooks".
 */
