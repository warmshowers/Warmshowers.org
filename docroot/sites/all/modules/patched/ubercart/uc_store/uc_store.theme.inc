<?php

/**
 * @file
 * Theme functions for the uc_store module.
 */

/**
 * Displays a price in the standard format and with consistent markup.
 *
 * @param $variables
 *   An associative array containing:
 *   - price: A numerical price value.
 *   - suffixes: An array of suffixes to be attached to this price.
 *
 * @ingroup themeable
 */
function theme_uc_price($variables) {
  $output = '<span class="uc-price">' . uc_currency_format($variables['price']) . '</span>';
  if (!empty($variables['suffixes'])) {
    $output .= '<span class="price-suffixes">' . implode(' ', $variables['suffixes']) . '</span>';
  }
  return $output;
}

/**
 * Displays "Quantity" abbreviated as "Qty".
 *
 * @ingroup themeable
 */
function theme_uc_qty_label() {
  return '<abbr title="' . t('Quantity') . '">' . t('Qty') . '</abbr>';
}

/**
 * Displays a quantity.
 *
 * @param $variables
 *   An associative array containing:
 *   - qty: The quantity to display.
 *
 * @ingroup themeable
 */
function theme_uc_qty($variables) {
  return $variables['qty'] . ' ×';
}

/**
 * Displays a username in the standard format and with consistent markup.
 *
 * @param $variables
 *   An associative array containing:
 *   - uid: A user ID value.
 *
 * @ingroup themeable
 */
function theme_uc_uid($variables) {
  if ($variables['uid']) {
    return theme('username', array('account' => user_load($variables['uid'])));
  }
  else {
    return '-';
  }
}

/**
 * Wraps the footer in a div so it can be re-styled.
 *
 * @param $variables
 *   An associative array containing:
 *   - message: String containing footer text.
 *
 * @ingroup themeable
 */
function theme_uc_store_footer($variables) {
  return '<div id="store-footer">' . $variables['message'] . '</div>';
}

/**
 * Themes a pane sorting form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_uc_pane_sort_table($variables) {
  $form = $variables['form'];
  $prefix = $form['#pane_prefix'];

  $attributes = array();
  if (isset($form['#draggable'])) {
    $attributes['id'] = $form['#draggable'] . '-table';
    drupal_add_tabledrag($form['#draggable'] . '-table', 'order', 'sibling', $form['#draggable']);
  }

  $header = array(t('Pane'), t('List position'));

  foreach (element_children($form) as $pane_id) {
    $rows[] = array(
      'data' => array(
        drupal_render($form[$pane_id][$prefix . '_' . $pane_id . '_enabled']),
        drupal_render($form[$pane_id][$prefix . '_' . $pane_id . '_weight']),
      ),
      'class' => array('draggable'),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $attributes)) . '<br />';
}

