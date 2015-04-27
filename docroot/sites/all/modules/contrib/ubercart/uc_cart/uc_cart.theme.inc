<?php

/**
 * @file
 * Theme functions for the uc_cart module.
 */

/**
 * Themes the shopping cart block title.
 *
 * @param $variables
 *   An associative array containing:
 *   - title: The text to use for the title of the block.
 *   - icon_class: Class to use for the cart icon image or FALSE if the icon is
 *     disabled.
 *   - collapsible: TRUE or FALSE indicating whether or not the cart block is
 *     collapsible.
 *   - collapsed: TRUE or FALSE indicating whether or not the cart block is
 *     collapsed.
 *
 * @ingroup themeable
 */
function theme_uc_cart_block_title($variables) {
  $title = $variables['title'];
  $icon_class = $variables['icon_class'];
  $collapsible = $variables['collapsible'];
  $collapsed = $variables['collapsed'];

  $output = '';

  // Add in the cart image if specified.
  if ($icon_class) {
    $output .= theme('uc_cart_block_title_icon', array('icon_class' => $icon_class));
  }

  // Add the main title span and text, with or without the arrow based on the
  // cart block collapsibility settings.
  if ($collapsible) {
    $output .= '<span class="cart-block-title-bar" title="' . t('Show/hide shopping cart contents.') . '">' . $title;
    if ($collapsed) {
      $output .= '<span class="cart-block-arrow arrow-down"></span>';
    }
    else {
      $output .= '<span class="cart-block-arrow"></span>';
    }
    $output .= '</span>';
  }
  else {
    $output .= '<span class="cart-block-title-bar">' . $title . '</span>';
  }

  return $output;
}

/**
 * Themes the shopping cart icon.
 *
 * @param $variables
 *   An associative array containing:
 *   - icon_class: Class to use for the cart icon image, either cart-full or
 *     cart-empty.
 *
 * @ingroup themeable
 */
function theme_uc_cart_block_title_icon($variables) {
  $icon_class = $variables['icon_class'];

  return l('<span class="' . $icon_class . '" title="' . t('View your shopping cart.') . '"></span>', 'cart', array('html' => TRUE));
}

/**
 * Themes the shopping cart block content.
 *
 * @param $variables
 *   An associative array containing:
 *   - help_text: Text to place in the small help text area beneath the cart
 *     block title or FALSE if disabled.
 *   - items: An associative array of cart item information containing:
 *     - qty: Quantity in cart.
 *     - title: Item title.
 *     - price: Item price.
 *     - desc: Item description.
 *   - item_count: The number of items in the shopping cart.
 *   - item_text: A textual representation of the number of items in the
 *     shopping cart.
 *   - total: The unformatted total of all the products in the shopping cart.
 *   - summary_links: An array of links used in the cart summary.
 *   - collapsed: TRUE or FALSE indicating whether or not the cart block is
 *     collapsed.
 *
 * @ingroup themeable
 */
function theme_uc_cart_block_content($variables) {
  $help_text = $variables['help_text'];
  $items = $variables['items'];
  $item_count = $variables['item_count'];
  $item_text = $variables['item_text'];
  $total = $variables['total'];
  $summary_links = $variables['summary_links'];
  $collapsed = $variables['collapsed'];

  $output = '';

  // Add the help text if enabled.
  if ($help_text) {
    $output .= '<span class="cart-help-text">' . $help_text . '</span>';
  }

  // Add a table of items in the cart or the empty message.
  $output .= theme('uc_cart_block_items', array('items' => $items, 'collapsed' => $collapsed));

  // Add the summary section beneath the items table.
  $output .= theme('uc_cart_block_summary', array('item_count' => $item_count, 'item_text' => $item_text, 'total' => $total, 'summary_links' => $summary_links));

  return $output;
}

/**
 * Themes the table listing the items in the shopping cart block.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An associative array of cart item information containing:
 *     - qty: Quantity in cart.
 *     - title: Item title.
 *     - price: Item price.
 *     - desc: Item description.
 *   - collapsed: TRUE or FALSE indicating whether or not the cart block is
 *     collapsed.
 *
 * @ingroup themeable
 */
function theme_uc_cart_block_items($variables) {
  $items = $variables['items'];
  $class = $variables['collapsed'] ? 'cart-block-items collapsed' : 'cart-block-items';

  // If there are items in the shopping cart...
  if ($items) {
    $output = '<table class="' . $class . '"><tbody>';

    // Loop through each item.
    $row_class = 'odd';
    foreach ($items as $item) {
      // Add the basic row with quantity, title, and price.
      $output .= '<tr class="' . $row_class . '"><td class="cart-block-item-qty">' . $item['qty'] . '</td>'
                . '<td class="cart-block-item-title">' . $item['title'] . '</td>'
                . '<td class="cart-block-item-price">' . theme('uc_price', array('price' => $item['price'])) . '</td></tr>';

      // Add a row of description if necessary.
      if ($item['desc']) {
        $output .= '<tr class="' . $row_class . '"><td colspan="3" class="cart-block-item-desc">' . $item['desc'] . '</td></tr>';
      }

      // Alternate the class for the rows.
      $row_class = ($row_class == 'odd') ? 'even' : 'odd';
    }

    $output .= '</tbody></table>';
  }
  else {
    // Otherwise display an empty message.
    $output = '<p class="' . $class . ' uc-cart-empty">' . t('There are no products in your shopping cart.') . '</p>';
  }

  return $output;
}

/**
 * Themes the summary table at the bottom of the default shopping cart block.
 *
 * @param $variables
 *   An associative array containing:
 *   - item_count: The number of items in the shopping cart.
 *   - item_text: A textual representation of the number of items in the
 *      shopping cart.
 *   - total: The unformatted total of all the products in the shopping cart.
 *   - summary_links: An array of links used in the summary.
 *
 * @ingroup themeable
 */
function theme_uc_cart_block_summary($variables) {
  $item_count = $variables['item_count'];
  $item_text = $variables['item_text'];
  $total = $variables['total'];
  $summary_links = $variables['summary_links'];

  // Build the basic table with the number of items in the cart and total.
  $output = '<table class="cart-block-summary"><tbody><tr>'
           . '<td class="cart-block-summary-items">' . $item_text . '</td>'
           . '<td class="cart-block-summary-total"><label>' . t('Total:')
           . '</label> ' . theme('uc_price', array('price' => $total)) . '</td></tr>';

  // If there are products in the cart...
  if ($item_count > 0) {
    // Add a view cart link.
    $output .= '<tr class="cart-block-summary-links"><td colspan="2">'
             . theme('links', array('links' => $summary_links)) . '</td></tr>';
  }

  $output .= '</tbody></table>';

  return $output;
}

/**
 * Themes the uc_cart_view_form().
 *
 * Outputs a hidden copy of the update cart button first, so pressing Enter
 * updates the cart instead of removing an item.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see uc_cart_view_form()
 * @ingroup themeable
 */
function theme_uc_cart_view_form($variables) {
  $form = &$variables['form'];

  $output = '<div class="uc-default-submit">';
  $output .= drupal_render($form['actions']['update']);
  $output .= '</div>';
  $form['actions']['update']['#printed'] = FALSE;

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Themes the cart checkout button(s).
 *
 * @param $variables
 *   An associative array containing:
 *   - buttons: A render element representing the form buttons.
 *
 * @see uc_cart_view_form()
 * @ingroup themeable
 */
function theme_uc_cart_checkout_buttons($variables) {
  $output = '';

  if ($buttons = element_children($variables['buttons'])) {
    // Render the first button.
    $button = array_shift($buttons);
    $output = drupal_render($variables['buttons'][$button]);

    // Render any remaining buttons inside a separate container.
    if ($buttons) {
      $output .= '<div class="uc-cart-checkout-button-container clearfix">';

      // Render the second button.
      $output .= '<div class="uc-cart-checkout-button">';
      $output .= '<div class="uc-cart-checkout-button-separator">' . t('- or -') . '</div>';
      $button = array_shift($buttons);
      $output .= drupal_render($variables['buttons'][$button]);
      $output .= '</div>';

      // Render any remaining buttons.
      foreach ($buttons as $button) {
        $output .= '<div class="uc-cart-checkout-button">';
        $output .= drupal_render($variables['buttons'][$button]);
        $output .= '</div>';
      }

      $output .= '</div>';
    }
  }

  return $output;
}

/**
 * Returns the text displayed for an empty shopping cart.
 *
 * @ingroup themeable
 */
function theme_uc_empty_cart() {
  return '<p class="uc-cart-empty">' . t('There are no products in your shopping cart.') . '</p>';
}

/**
 * Themes the sale completion page.
 *
 * @param $variables
 *   An associative array containing:
 *   - message: Message containing order number info, account info, and link to
 *     continue shopping.
 *
 * @ingroup themeable
 */
function theme_uc_cart_complete_sale($variables) {
  return $variables['message'];
}
