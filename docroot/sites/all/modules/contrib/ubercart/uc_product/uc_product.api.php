<?php

/**
 * @file
 * Hooks provided by the Product module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Make alterations to a specific variant of a product node.
 *
 * @param $node
 *   The product node to be altered.
 */
function hook_uc_product_alter(&$node) {
  if (isset($node->data['attributes']) && is_array($node->data['attributes'])) {
    $options = _uc_cart_product_get_options($node);
    foreach ($options as $option) {
      $node->cost += $option['cost'];
      $node->price += $option['price'];
      $node->weight += $option['weight'];
    }

    $combination = array();
    foreach ($node->data['attributes'] as $aid => $value) {
      if (is_numeric($value)) {
        $attribute = uc_attribute_load($aid, $node->nid, 'product');
        if ($attribute && ($attribute->display == 1 || $attribute->display == 2)) {
          $combination[$aid] = $value;
        }
      }
    }
    ksort($combination);

    $model = db_query("SELECT model FROM {uc_product_adjustments} WHERE nid = :nid AND combination LIKE :combo", array(':nid' => $node->nid, ':combo' => serialize($combination)))->fetchField();

    if (!empty($model)) {
      $node->model = $model;
    }
  }
}

/**
 * Performs actions on product classes.
 *
 * @param $type
 *   The node type of the product class.
 * @param $op
 *   The action being performed on the product class:
 *   - insert: A new node type is created, or an existing node type is being
 *     converted into a product type.
 *   - update: A product class has been updated.
 *   - delete: A product class has been deleted. Modules that have attached
 *     additional information to the node type because it is a product type
 *     should delete this information.
 */
function hook_uc_product_class($type, $op) {
  switch ($op) {
    case 'delete':
      db_delete('uc_class_attributes')
        ->condition('pcid', $type)
        ->execute();

      db_delete('uc_class_attribute_options')
        ->condition('pcid', $type)
        ->execute();

      break;
  }
}

/**
 * Define default product classes.
 *
 * The results of this hook are eventually passed through hook_node_info(),
 * so you may include any keys that hook_node_info() uses. Defaults will
 * be provided where keys are not set. This hook can also be used to
 * override the default "product" product class name and description.
 */
function hook_uc_product_default_classes() {
  return array(
    'my_class' => array(
      'name' => t('My product class'),
      'description' => t('Content type description for my product class.'),
    ),
  );
}

/**
 * Returns a structured array representing the given product's description.
 *
 * Modules that add data to cart items when they are selected should display it
 * with this hook. The return values from each implementation will be
 * sent through to hook_uc_product_description_alter() implementations and then
 * all descriptions are rendered using drupal_render().
 *
 * @param $product
 *   Product. Usually one of the values of the array returned by
 *   uc_cart_get_contents().
 *
 * @return
 *   A structured array that can be fed into drupal_render().
 */
function hook_uc_product_description($product) {
  $description = array(
    'attributes' => array(
      '#product' => array(
        '#type' => 'value',
        '#value' => $product,
      ),
      '#theme' => 'uc_product_attributes',
      '#weight' => 1,
    ),
  );

  $desc =& $description['attributes'];

  // Cart version of the product has numeric attribute => option values so we
  // need to retrieve the right ones
  $weight = 0;
  if (empty($product->order_id)) {
    foreach (_uc_cart_product_get_options($product) as $option) {
      if (!isset($desc[$option['aid']])) {
        $desc[$option['aid']]['#attribute_name'] = $option['attribute'];
        $desc[$option['aid']]['#options'] = array($option['name']);
      }
      else {
        $desc[$option['aid']]['#options'][] = $option['name'];
      }
      $desc[$option['aid']]['#weight'] = $weight++;
    }
  }
  else {
    foreach ((array)$product->data['attributes'] as $attribute => $option) {
      $desc[] = array(
        '#attribute_name' => $attribute,
        '#options' => $option,
        '#weight' => $weight++,
      );
    }
  }

  return $description;
}

/**
 * Alters the given product description.
 *
 * @param $description
 *   Description array reference.
 * @param $product
 *   The product being described.
 */
function hook_uc_product_description_alter(&$description, $product) {
  $description['attributes']['#weight'] = 2;
}

/**
 * Notifies core of any SKUs your module adds to a given node.
 *
 * NOTE: DO NOT map the array keys, as the possibility for numeric SKUs exists,
 * and this will conflict with the behavior of module_invoke_all(), specifically
 * array_merge_recursive().
 *
 * Code lifted from uc_attribute.module.
 */
function hook_uc_product_models($nid) {
  // Get all the SKUs for all the attributes on this node.
  $models = db_query("SELECT DISTINCT model FROM {uc_product_adjustments} WHERE nid = :nid", array(':nid' => $nid))->fetchCol();

  return $models;
}

/**
 * Lists node types which should be considered products.
 *
 * Trusts the duck philosophy of object identification: if it walks like a duck,
 * quacks like a duck, and has feathers like a duck, it's probably a duck.
 * Products are nodes with prices, SKUs, and everything else Ubercart expects
 * them to have.
 *
 * @return
 *   Array of node type ids.
 */
function hook_uc_product_types() {
  return array('product_kit');
}

/**
 * @} End of "addtogroup hooks".
 */
