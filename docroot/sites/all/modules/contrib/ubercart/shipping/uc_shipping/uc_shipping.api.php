<?php

/**
 * @file
 * Hooks provided by the Shipping module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Handles additional data and activity for shipments.
 *
 * Adds additional activity after shipment objects are loaded from,
 * saved to, or deleted from the database.  This is useful for shipment
 * modules that store method-specific shipment data in separate tables that
 * need to be kept in sync with the uc_shipments table.
 *
 * The members of the shipment object are the fields in the corresponding
 * record of the uc_shipments table, plus $shipment->packages, an array
 * of package objects as returned by uc_shipping_package_load().
 *
 * @param $op
 *   The action being taken on the shipment. One of the following values:
 *   - load: The shipment and its packages are loaded from the database.
 *   - save: Changes to the shipment have been written.
 *   - delete: The shipment has been deleted and the packages are available
 *     for reshipment.
 * @param $shipment
 *   The shipment object.
 *
 * @return
 *   Only given when $op is "load". An associative array of extra data to
 *   be added to the shipment object. Each key/value element of the array
 *   becomes a separate member of the shipment object. Elements of the array
 *   with the same key as members of the shipment object replace those members
 *   of the shipment object.
 */
function hook_uc_shipment($op, $shipment) {
  switch ($op) {
    case 'save':
      $google_order_number = uc_google_checkout_get_google_number($shipment->order_id);
      if ($google_order_number && $shipment->is_new) {
        $xml_data = '';
        foreach ($shipment->packages as $package) {
          if ($package->tracking_number) {
            $tracking_number = $package->tracking_number;
          }
          elseif ($shipment->tracking_number) {
            $tracking_number = $shipment->tracking_number;
          }
          if ($tracking_number) {
            foreach ($package->products as $product) {
              $xml_data .= '<item-shipping-information>';
              $xml_data .= '<item-id>';
              $xml_data .= '<merchant-item-id>' . check_plain($product->nid . '|' . $product->model) . '</merchant-item-id>';
              $xml_data .= '</item-id>';
              $xml_data .= '<tracking-data-list>';
              $xml_data .= '<tracking-data>';
              $xml_data .= '<carrier>' . check_plain($shipment->carrier) . '</carrier>';
              $xml_data .= '<tracking-number>' . check_plain($tracking_number) . '</tracking-number>';
              $xml_data .= '</tracking-data>';
              $xml_data .= '</tracking-data-list>';
              $xml_data .= '</item-shipping-information>';
            }
          }
        }
        if ($xml_data) {
          $request = '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
          $request .= '<ship-items xmlns="http://checkout.google.com/schema/2" google-order-number="' . $google_order_number . '">';
          $request .= '<item-shipping-information-list>';
          $request .= $xml_data;
          $request .= '</item-shipping-information-list>';
          $request .= '<send-email>true</send-email>';
          $request .= '</ship-items>';
          $response = uc_google_checkout_send_request('request', $request);
        }
      }
    break;
    case 'delete':
      $google_order_number = uc_google_checkout_get_google_number($shipment->order_id);
      if ($google_order_number) {
        foreach ($shipment->packages as $package) {
          foreach ($package->products as $product) {
            $reset_ids[] = check_plain($product->nid . '|' . $product->model);
          }
        }
        $request = '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
        $request .= '<reset-items-shipping-information xmlns="http://checkout.google.com/schema/2" google-order-number="' . $google_order_number . '">';
        $request .= '<item-ids>';
        foreach (array_unique($reset_ids) as $item_id) {
          $request .= '<item-id>';
          $request .= '<merchant-item-id>' . $item_id . '</merchant-item-id>';
          $request .= '</item-id>';
        }
        $request .= '</item-ids>';
        $request .= '<send-email>false</send-email>';
        $request .= '</reset-items-shipping-information>';
      }
      $response = uc_google_checkout_send_request('request', $request);
    break;
  }
}

/**
 * @} End of "addtogroup hooks".
 */
