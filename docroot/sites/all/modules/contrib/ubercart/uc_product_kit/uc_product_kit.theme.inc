<?php

/**
 * @file
 * Theme functions for the uc_product_kit module.
 */

/**
 * Renders the list of kit components on the product kit edit form.
 *
 * @ingroup themeable
 */
function theme_uc_product_kit_items_form($variables) {
  $form = &$variables['form'];

  $header = array(t('Product'), t('Quantity'), t('List position'), t('Discount'));
  $rows = array();

  foreach (element_children($form) as $item) {
    $row = array(
      drupal_render($form[$item]['link']),
      drupal_render($form[$item]['qty']),
      drupal_render($form[$item]['ordering']),
      drupal_render($form[$item]['discount']),
    );
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  if (empty($rows)) {
    return '';
  }

  drupal_add_tabledrag('uc-product-kit-item-table', 'order', 'sibling', 'uc-product-kit-item-ordering');

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'uc-product-kit-item-table')));

  if (!empty($form['#description'])) {
    $output .= '<div class="description">' . $form['#description'] . "</div>\n";
  }

  return $output;
}

/**
 * Renders a product kit component.
 *
 * @ingroup themeable
 */
function theme_uc_product_kit_list_item($variables) {
  $product = $variables['product'];

  $node = node_load($product->nid);
  if (node_access('view', $node)) {
    $title = l($product->title, 'node/' . $product->nid);
  }
  else {
    $title = check_plain($product->title);
  }

  return theme('uc_qty', array('qty' => $product->qty)) . ' ' . $title;
}

/**
 * Wraps the "Add to Cart" form in a <div>.
 *
 * @ingroup themeable
 */
function theme_uc_product_kit_add_to_cart($variables) {
  $form = $variables['form'];

  $output = '<div class="add-to-cart" title="' . t('Click to add to cart.') . '">';
  $output .= drupal_render($form);
  $output .= '</div>';

  return $output;
}
