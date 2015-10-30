<?php
/**
 * @file
 *
 * Contains the controller for uc_cart_item entities.
 */
class UcCartItemController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::attachLoad().
   */
  public function attachLoad(&$items, $revision_id = FALSE) {
    foreach ($items as &$item) {
      $product = uc_product_load_variant($item->nid, $item->data);
      // Merge in fields from the product.
      foreach ($product as $key => $value) {
        $item->$key = $value;
      }
      $item->module = $item->data['module'];
    }
    parent::attachLoad($items, $revision_id);
  }

 /**
   * Saves a cart item entity.
   *
   * Cart items are deleted if saved with a quantity of zero.
   */
  public function save($item, DatabaseTransaction $transaction = NULL) {
    if ($item->qty < 1) {
      if (isset($item->cart_item_id)) {
        parent::delete(array($item->cart_item_id), $transaction);
      }
    }
    else {
      $item->changed = REQUEST_TIME;
      parent::save($item, $transaction);
    }
  }

  /**
   * Overrides EntityAPIController::buildContent().
   */
  public function buildContent($product, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content += module_invoke($product->data['module'], 'uc_cart_display', $product);
    if (!empty($content)) {
      $content['cart_item_id'] = array(
        '#type' => 'hidden',
        '#value' => isset($product->cart_item_id) ? $product->cart_item_id : 0,
      );
    }

    return parent::buildContent($product, $view_mode, $langcode, $content);
  }
}
