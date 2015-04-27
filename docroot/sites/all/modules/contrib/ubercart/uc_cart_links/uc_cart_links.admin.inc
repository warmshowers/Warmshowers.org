<?php

/**
 * @file
 * Cart Links administration menu items.
 */

/**
 * Defines a form to configure the Cart Links settings.
 *
 * @ingroup forms
 */
function uc_cart_links_settings_form($form, &$form_state) {
  $form['uc_cart_links_add_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the cart link product action when you add a product to your cart.'),
    '#default_value' => variable_get('uc_cart_links_add_show', FALSE),
  );
  $form['uc_cart_links_track'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track clicks through Cart Links that specify tracking IDs.'),
    '#default_value' => variable_get('uc_cart_links_track', TRUE),
  );
  $form['uc_cart_links_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Cart Links to empty customer carts.'),
    '#default_value' => variable_get('uc_cart_links_empty', TRUE),
  );
  $form['uc_cart_links_messages'] = array(
    '#type' => 'textarea',
    '#title' => t('Cart Links messages'),
    '#description' => t('Enter messages available to the Cart Links API for display through a link. Separate messages with a line break. Each message should have a numeric key and text value, separated by "|". For example: 1337|Message text.'),
    '#default_value' => variable_get('uc_cart_links_messages', ''),
  );
  $form['uc_cart_links_restrictions'] = array(
    '#type' => 'textarea',
    '#title' => t('Cart Links restrictions'),
    '#description' => t('To restrict what Cart Links may be used on your site, enter all valid Cart Links in this textbox.  Separate links with a line break. Leave blank to permit any cart link.'),
    '#default_value' => variable_get('uc_cart_links_restrictions', ''),
  );
  $form['uc_cart_links_invalid_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Invalid link redirect page'),
    '#description' => t('Enter the URL to redirect to when an invalid cart link is used.'),
    '#default_value' => variable_get('uc_cart_links_invalid_page', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  return system_settings_form($form);
}

/**
 * Displays the Cart Links report.
 *
 * @return
 *   HTML output.
 */
function uc_cart_links_report() {
  $header = array(
    array('data' => t('ID'), 'field' => 'cart_link_id'),
    array('data' => t('Clicks'), 'field' => 'clicks'),
    array('data' => t('Last click'), 'field' => 'last_click', 'sort' => 'desc'),
  );

  $query = db_select('uc_cart_link_clicks')->extend('PagerDefault')->extend('TableSort')
    ->fields('uc_cart_link_clicks')
    ->limit(25)
    ->element(1)
    ->orderByHeader($header);

  $rows = array();
  $result = $query->execute();
  foreach ($result as $data) {
    $rows[] = array(
      check_plain($data->cart_link_id),
      $data->clicks,
      format_date($data->last_click, 'short'),
    );
  }

  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Cart Links have been tracked yet.'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#element' => 1,
  );

  return $build;
}

/**
 * Provides instructions on how to create Cart Links.
 *
 * @return
 *   Form API array with help text.
 */
function uc_cart_links_creation_help() {
  $build = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  $build['introduction'] = array(
    '#prefix' => '<p>',
    '#markup' => t("Cart Links allow you to craft links that add products to customer shopping carts and redirect customers to any page on the site. A store owner might use a Cart Link as a 'Buy it now' link in an e-mail, in a blog post, or on any page, either on or off site. These links may be identified with a unique ID, and clicks on these links may be reported to the administrator in order to track the effectiveness of each unique ID. You may track affiliate sales, see basic reports, and make sure malicious users don't create unapproved links."),
    '#suffix' => '</p>',
  );
  $build['uses'] = array(
    '#prefix' => t('The following actions may be configured to occur when a link is clicked:'),
    '#theme' => 'item_list',
    '#items' => array(
      t("Add any quantity of any number of products to the customer's cart, with specific attributes and options for each added product, if applicable."),
      t('Display a custom message to the user.'),
      t('Track the click for display on a store report.'),
      t("Empty the customer's shopping cart."),
      t('Redirect to any page on the site.'),
    ),
  );

  $build['suggestions'] = array(
    '#prefix' => '<p>',
    '#markup' => t('A Cart Link URL looks like:<blockquote><code>/cart/add/<em>&lt;cart_link_content&gt;</em></code></blockquote>where <code><em>&lt;cart_link_content&gt;</em></code> consists of one or more actions separated by a dash. Absolute URLs may also be used, e.g.:<blockquote><code>http://www.example.com/cart/add/<em>&lt;cart_link_content&gt;</em></code></blockquote>'),
    '#suffix' => '</p>',
  );

//  t('Specify the redirection by adding ?destination=url where url is the page to go to.'),

  $header = array(t('Action'), t('Description'), t('Argument'));
  $rows = array(
    array('p', t('Adds a product to the cart.'), t('A product node number, followed by optional arguments described in the table below.')),
    array('i', t('Sets the ID of the cart link.'), t('An alphanumeric string (32 characters max) to identify the link.')),
    array('m', t('Displays a message to the customer when the link is clicked.'), t('A <a href="!url">numeric message ID</a> to identify which message to display.', array('!url' => url('admin/store/settings/cart-links')))),
    array('e', t('Empties the cart. If used, this should be the first action.'), t('None.')),
  );

  $build['commands'] = array(
    '#prefix' => t('Allowed actions are:'),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $build['required'] = array(
    '#prefix' => '<p>',
    '#markup' => t('The only required part of the <code><em>&lt;cart_link_content&gt;</em></code> is the "p" action, which must be immediately followed by a product node number.  For example, to add product node 23 to a cart, use the following:<blockquote><code>/cart/add/p23</code></blockquote>To use this on your site, simply create an HTML anchor tag referencing your Cart Link URL:<blockquote><code>&lt;a href="http://www.example.com/cart/add/p23"&gt;Link text.&lt;/a&gt;</code></blockquote>'),
    '#suffix' => '</p>',
  );

  $header = array(t('Argument'), t('Description'), t('Values'));
  $rows = array(
    array('q', t('Specifies quantity of this product to add.'), t('A positive integer.')),
    array('a&lt;aid&gt;o&lt;oid&gt;', t('Specifies attribute/option for this product.'), t('aid is the integer attribute ID. oid is the integer option ID for radio, checkbox, and select options, or a url-escaped text string for textfield options.')),
    array('s', t('Silent.  Suppresses add-to-cart message for this product.
    (The add-to-cart message may be enabled on the <a href="!url">cart settings page</a>).', array('!url' => url('admin/store/settings/cart'))), t('None.')),
  );
  $build['args'] = array(
    '#prefix' => t('Optional arguments for "p" allow you to control the quantity, set product attributes and options, and suppress the default product action message normally shown when a product is added to a cart. These optional arguments are appended to the "p" action and separated with an underscore.  Allowed arguments for "p" are:'),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $build['quantity'] = array(
    '#prefix' => '<p>',
    '#markup' => t('For example, you may set the product quantity by appending the "q" argument to the "p" action. To add 5 items of product 23 you would use the link:<blockquote><code>/cart/add/p23_q5</code></blockquote>'),
    '#suffix' => '</p>',
  );

  $build['optional'] = array(
    '#prefix' => '<p>',
    '#markup' => t('Product attributes and options may be set with the <code>a&lt;aid&gt;o&lt;oid&gt;</code> argument.  For example, if product 23 has an attribute named "Size" with attribute ID = 12, and if there are three options defined for this attribute ("Small", "Medium", and "Large", with option IDs 4, 5, and 6 respectively), then to add a "Medium" to the cart you would use the link:<blockquote><code>/cart/add/p23_a12o5</code></blockquote>To add two products, one "Medium" and one "Small", you would use two actions:<blockquote><code>/cart/add/p23_a12o5-p23_a12o4</code></blockquote>Or, to just add two "Medium" products:<blockquote><code>/cart/add/p23_q2_a12o5</code></blockquote>'),
    '#suffix' => '</p>',
  );

  $build['example'] = array(
    '#prefix' => '<p>',
    '#markup' => t('A Cart Link that uses all of the available actions and arguments might look something like this:<blockquote><code>/cart/add/e-p23_q5_a12o5_a19o9_a1oA%20Text%20String_s-ispecialoffer-m77?destination=cart/checkout</code></blockquote>Note that the "e", "p", "i", and "m" actions are separated by dashes, while the optional arguments within the "p" action are separated by underscores. This example will first empty the shopping cart, then add 5 items of product 23 to the cart, track clicks with the ID "specialoffer", display a custom message with the ID "77", then redirect the user to the checkout page. In this case product 23 has three attributes which are set (aid = 12, 19, and 1), one of which is a textfield attribute (aid = 1).'),
    '#suffix' => '</p>',
  );

  $build['help'] = array(
    '#prefix' => '<p>',
    '#markup' => t('<a href="!url">Visit the settings page</a> to set preferences, define messages, and restrict links that may be used.', array('!url' => url('admin/store/settings/cart-links'))),
    '#suffix' => '</p>',
  );

  return $build;
}
