<?php
/**
 * @file
 * Contains controller classes for uc_order and uc_order_product entities.
 */

/**
 * Controller class for uc_order entity.
 */
class UcOrderController extends DrupalDefaultEntityController {

  function attachLoad(&$orders, $revision_id = FALSE) {
    foreach ($orders as &$order) {
      $order->data = unserialize($order->data);
      $efq = new EntityFieldQuery();
      $result = $efq->entityCondition('entity_type', 'uc_order_product')
        ->propertyCondition('order_id', $order->order_id)
        ->propertyOrderBy('order_product_id', 'ASC')
        ->execute();
      if (!empty($result['uc_order_product'])) {
        $order->products = uc_order_product_load_multiple(array_keys($result['uc_order_product']), TRUE);
        foreach ($order->products as $product) {
          $product->order = $order;
          $product->order_uid = $order->uid;
        }
      }
      else {
        $order->products = array();
      }

      uc_order_module_invoke('load', $order, NULL);

      // Load line items... has to be last after everything has been loaded.
      $order->line_items = uc_order_load_line_items($order);

      $fields = array();

      // Make sure the total still matches up...
      if (($total = uc_order_get_total($order)) !== $order->order_total) {
        $fields['order_total'] = $total;
        $order->order_total = $total;
      }

      if (($count = uc_order_get_product_count($order)) !== $order->product_count) {
        $fields['product_count'] = $count;
        $order->product_count = $count;
      }

      if (count($fields)) {
        $query = db_update('uc_orders')
          ->fields($fields)
          ->condition('order_id', $order->order_id)
          ->execute();
      }
    }

    parent::attachLoad($orders, $revision_id);
  }

}

/**
 * Controller class for the uc_order_product entity.
 */
class UcOrderProductController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::buildContent().
   */
  public function buildContent($product, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content['qty'] = array(
      '#theme' => 'uc_qty',
      '#qty' => $product->qty,
      '#cell_attributes' => array('class' => array('qty')),
    );
    $node = node_load($product->nid);
    $title = node_access('view', $node) ? l($product->title, 'node/' . $node->nid) : check_plain($product->title);
    $content['product'] = array(
      '#markup' => $title . uc_product_get_description($product),
      '#cell_attributes' => array('class' => array('product')),
    );
    $content['model'] = array(
      '#markup' => check_plain($product->model),
      '#cell_attributes' => array('class' => array('sku')),
    );
    if (user_access('administer products')) {
      $content['cost'] = array(
        '#theme' => 'uc_price',
        '#price' => $product->cost,
        '#cell_attributes' => array('class' => array('cost')),
      );
    }
    $content['price'] = array(
      '#theme' => 'uc_price',
      '#price' => $product->price,
      '#suffixes' => array(),
      '#cell_attributes' => array('class' => array('price')),
    );
    $content['total'] = array(
      '#theme' => 'uc_price',
      '#price' => $product->price * $product->qty,
      '#suffixes' => array(),
      '#cell_attributes' => array('class' => array('total')),
    );

    return parent::buildContent($product, $view_mode, $langcode, $content);
  }

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($product, DatabaseTransaction $transaction = NULL) {
    // Product kits, particularly, shouldn't actually be added to an order,
    // but instead they cause other products to be added.
    if (isset($product->skip_save) && $product->skip_save == TRUE) {
      return;
    }

    if (empty($product->weight_units)) {
      if (empty($product->nid)) {
        $product->weight_units = variable_get('uc_weight_unit', 'lb');
      }
      else {
        $units = db_query("SELECT weight_units FROM {node} n JOIN {uc_products} p ON n.vid = p.vid WHERE n.nid = :nid", array(':nid' => $product->nid))->fetchField();
        $product->weight_units = empty($units) ? variable_get('uc_weight_unit', 'lb') : $units;
      }
    }
    return parent::save($product, $transaction);
  }
}
