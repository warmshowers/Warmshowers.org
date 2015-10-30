<?php

/**
 * @file
 * This file contains the callbacks for the default order panes supplied with
 * Ubercart and their corresponding helper functions.
 *
 * Order panes are defined using hook_uc_order_pane() and use a callback to
 * handle the different processes involved in order viewing/editing. The
 * default order panes are defined in uc_order_order_pane() in uc_order.module.
 */

/**
 * Handles the "Print button" order pane.
 */
function uc_order_pane_print_button($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'customer':
      if (user_access('view own invoices')) {
        $link = t('Click to open a window with a printable invoice.');
        $build = array(
          '#markup' => l($link, 'user/' . $order->uid . '/orders/' . $order->order_id . '/print', array(
            'html' => TRUE,
            'attributes' => array(
              'onclick' => "window.open(this.href, '" . t('Invoice') . "', 'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=1,width=600,height=480,left=50,top=50'); return false;",
            ),
          )),
        );
        return $build;
      }
  }
}

/**
 * Handles the "Ship to" order pane.
 */
function uc_order_pane_ship_to($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'customer':
      if (!uc_order_is_shippable($order)) {
        return;
      }
    case 'view':
      $build = array('#markup' => uc_order_address($order, 'delivery') . '<br />' . check_plain($order->delivery_phone));
      return $build;

    case 'edit-form':
      $form['ship_to'] = array(
        '#type' => 'uc_address',
        '#default_value' => $order,
        '#required' => FALSE,
        '#attributes' => array('class' => array('uc-store-address-field')),
        '#key_prefix' => 'delivery',
      );
      return $form;

    case 'edit-theme':
      $output = '<div class="order-pane-icons">';
      $output .= ' <img src="' . base_path() . drupal_get_path('module', 'uc_store')
                . '/images/address_book.gif" alt="' . t('Select from address book.') . '" '
                . 'title="' . t('Select from address book.') . '" onclick="load_address_select(' . $form['order_uid']['#value'] . ', \'#delivery_address_select\', \'delivery\');" />';
      $output .= ' <img src="' . base_path() . drupal_get_path('module', 'uc_store')
                 . '/images/copy.gif" alt="' . t('Copy billing information.') . '" title="'
                 . t('Copy billing information.') . '" onclick="uc_order_copy_billing_to_shipping();" />';
      $output .= '</div>';
      $output .= '<div id="delivery_address_select"></div>';
      return $output . drupal_render($form['ship_to']);

    case 'edit-process':
      foreach ($form_state['values'] as $key => $value) {
        if (substr($key, 0, 9) == 'delivery_') {
          if (uc_address_field_enabled(substr($key, 9))) {
            $changes[$key] = $value;
          }
        }
      }
      return $changes;
  }
}

/**
 * Handles the "Bill to" order pane.
 */
function uc_order_pane_bill_to($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
    case 'customer':
      $build = array('#markup' => uc_order_address($order, 'billing') . '<br />' . check_plain($order->billing_phone));
      return $build;

    case 'edit-form':
      $form['bill_to'] = array(
        '#type' => 'uc_address',
        '#default_value' => $order,
        '#required' => FALSE,
        '#attributes' => array('class' => array('uc-store-address-field')),
        '#key_prefix' => 'billing',
      );
      return $form;

    case 'edit-theme':
      $output = '<div class="order-pane-icons">';
      $output .= ' <img src="' . base_path() . drupal_get_path('module', 'uc_store')
                . '/images/address_book.gif" alt="' . t('Select from address book.') . '" '
                . 'title="' . t('Select from address book.') . '" onclick="load_address_select(' . $form['order_uid']['#value'] . ', \'#billing_address_select\', \'billing\');" />';
      $output .= ' <img src="' . base_path() . drupal_get_path('module', 'uc_store')
               . '/images/copy.gif" alt="' . t('Copy shipping information.') . '" title="'
               . t('Copy shipping information.') . '" onclick="uc_order_copy_shipping_to_billing();" />';
      $output .= '</div>';
      $output .= '<div id="billing_address_select"></div>';
      return $output . drupal_render($form['bill_to']);

    case 'edit-process':
      foreach ($form_state['values'] as $key => $value) {
        if (substr($key, 0, 8) == 'billing_') {
          if (uc_address_field_enabled(substr($key, 8))) {
            $changes[$key] = $value;
          }
        }
      }
      return $changes;
  }
}

/**
 * Handles the "Customer Info" order pane.
 */
function uc_order_pane_customer($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      $build['uid'] = array('#markup' => t('Customer number: !user_link', array('!user_link' => $order->uid ? l($order->uid, 'user/' . $order->uid) : '0')));
      $build['primary_email'] = array('#markup' => '<br />' . t('Primary e-mail:') . '<br />' . check_plain($order->primary_email));

      return $build;

    case 'edit-form':
      $form['customer']['uid'] = array(
        '#type' => 'hidden',
        '#default_value' => $order->uid,
      );
      $form['customer']['uid_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Customer number'),
        '#default_value' => $order->uid,
        '#maxlength' => 10,
        '#size' => 10,
        '#disabled' => TRUE,
      );
      $form['customer']['primary_email'] = array(
        '#type' => 'textfield',
        '#title' => t('E-mail address'),
        '#default_value' => $order->primary_email,
        '#maxlength' => 64,
        '#size' => 32,
      );
      return $form;

    case 'edit-theme':
      $output = '<div class="order-pane-icons">';
      $output .= ' <img src="' . base_path() . drupal_get_path('module', 'uc_store')
                . '/images/order_view.gif" alt="' . t('Search for an existing customer.') . '" '
                . 'title="' . t('Search for an existing customer.') . '" onclick="load_customer_search();" />';
      $output .= ' <img src="' . base_path() . drupal_get_path('module', 'uc_store')
                . '/images/menu_customers_small.gif" alt="' . t('Create a new customer.') . '" '
                . 'title="' . t('Create a new customer.') . '" onclick="load_new_customer_form();" />';
      $output .= '</div>';
      $output .= '<div id="customer-select"></div>';
      $output .= drupal_render($form['customer']);
      return $output;

    case 'edit-process':
      $changes['uid'] = $form_state['values']['uid'];
      $changes['primary_email'] = $form_state['values']['primary_email'];
      return $changes;
  }
}

/**
 * Handles the "Products" order pane.
 */
function uc_order_pane_products($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      return tapir_get_table('uc_op_products_view_table', $order);

    case 'customer':
      return tapir_get_table('uc_op_products_customer_table', $order);

    case 'edit-form':
      $form['add_product_button'] = array(
        '#type' => 'submit',
        '#value' => t('Add product'),
        '#submit' => array('uc_order_pane_products_select'),
        '#ajax' => array(
          'callback' => 'uc_order_pane_products_ajax_callback',
          'wrapper' => 'product-controls',
        ),
      );
      $form['add_blank_line_button'] = array(
        '#type' => 'submit',
        '#value' => t('Add blank line'),
        '#submit' => array('uc_order_edit_products_add_blank'),
        '#ajax' => array(
          'callback' => 'uc_order_pane_products_ajax_callback',
          'wrapper' => 'product-controls',
        ),
      );

      $form['product_controls'] = array(
        '#tree' => TRUE,
        '#prefix' => '<div id="product-controls">',
        '#suffix' => '</div>',
      );

      $controls = array();

      if (isset($form_state['products_action'])) {
        switch ($form_state['products_action']) {
          case 'select':
            $controls = uc_order_product_select_form($form['product_controls'], $form_state, $order);
            break;
          case 'add_product':
            $controls = uc_order_add_product_form($form['product_controls'], $form_state, $order, $form_state['node']);
            break;
        }
      }

      $form['product_controls'] += $controls;

      $form += uc_order_edit_products_form($form, $form_state, $order->products);

      return $form;

    case 'edit-theme':
      $output = drupal_render($form['add_product_button']);
      $output .= drupal_render($form['add_blank_line_button']);
      $output .= drupal_render($form['product_controls']);
      $output .= drupal_render($form['products']);

      return $output;

    case 'edit-process':
      if (isset($form_state['values']['products'])) {
        foreach ($form_state['values']['products'] as $key => $product) {
          $product['data'] = unserialize($product['data']);
          uc_order_product_save($order->order_id, (object) $product);
        }
      }

      break;
  }
}

/**
 * Form to choose a product to add to the order.
 *
 * @ingroup forms
 */
function uc_order_product_select_form($form, &$form_state, $order) {
  $options = $form_state['product_select_options'];
  $ajax = array(
    'callback' => 'uc_order_pane_products_ajax_callback',
    'wrapper' => 'product-controls',
  );

  $form['nid'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#size' => 7,
    '#ajax' => $ajax + array(
      'event' => 'dblclick',
      'trigger_as' => array(
        'name' => 'op',
        'value' => t('Select'),
      ),
    ),
  );
  $form['product_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search by name or model/SKU (* is the wildcard)'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['select'] = array(
    '#type' => 'submit',
    '#value' => t('Select'),
    '#validate' => array('uc_order_product_select_form_validate'),
    '#submit' => array('uc_order_pane_products_add'),
    '#ajax' => $ajax,
    '#weight' => 0,
  );
  $form['actions']['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#submit' => array('uc_order_pane_products_select'),
    '#ajax' => $ajax,
    '#weight' => 1,
  );
  $form['actions']['close'] = array(
    '#type' => 'submit',
    '#value' => t('Close'),
    '#submit' => array('uc_order_pane_products_close'),
    '#ajax' => $ajax,
    '#weight' => 2,
  );

  return $form;
}

/**
 * Validation handler for uc_order_product_select_form().
 */
function uc_order_product_select_form_validate($form, &$form_state) {
  if (empty($form_state['values']['product_controls']['nid'])) {
    form_set_error('product_controls][nid', t('Please select a product.'));
  }
}

/**
 * Sets the quantity and attributes of a product added to the order.
 *
 * @see uc_order_add_product_form()
 * @ingroup forms
 */
function uc_order_add_product_form($form, &$form_state, $order, $node) {
  $data = array();
  if (isset($form_state['values']['product_controls']['qty'])) {
    $data += module_invoke_all('uc_add_to_cart_data', $form_state['values']['product_controls']);
  }
  if (!empty($node->data) && is_array($node->data)) {
    $data += $node->data;
  }
  $node = uc_product_load_variant(intval($form_state['values']['product_controls']['nid']), $data);
  $form['title'] = array(
    '#markup' => '<h3>' . check_plain($node->title) . '</h3>',
  );
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['qty'] = array(
    '#type' => 'uc_quantity',
    '#title' => theme('uc_qty_label'),
    '#default_value' => 1,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add to order'),
    '#submit' => array('uc_order_edit_products_add'),
    '#ajax' =>  array(
      'callback' => 'uc_order_pane_products_ajax_callback',
      'wrapper' => 'product-controls',
    ),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('uc_order_pane_products_select'),
    '#ajax' =>  array(
      'callback' => 'uc_order_pane_products_ajax_callback',
      'wrapper' => 'product-controls',
    ),
    '#limit_validation_errors' => array(),
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  uc_form_alter($form, $form_state, __FUNCTION__);

  return $form;
}

/**
 * Form to allow ordered products' data to be changed.
 *
 * @see uc_op_products_edit_table()
 * @see theme_uc_order_edit_products_form()
 */
function uc_order_edit_products_form($form, &$form_state, $products) {
  if (($product_count = count($products)) > 0) {
    $form['products'] = tapir_get_table('uc_op_products_edit_table');
    for ($i=0, $product = reset($products); $i < $product_count; $i++, $product = next($products)) {
      $form['products'][$i]['remove'] = array(
        '#type' => 'image_button',
        '#title' => t('Remove this product.'),
        '#src' => drupal_get_path('module', 'uc_store') . '/images/error.gif',
        '#button_type' => 'remove',
        '#submit' => array('uc_order_edit_products_remove', 'uc_order_edit_form_submit'),
        '#return_value' => $product->order_product_id,
      );
      $form['products'][$i]['order_product_id'] = array(
        '#type' => 'hidden',
        '#value' => $product->order_product_id,
      );
      $form['products'][$i]['nid'] = array(
        '#type' => 'hidden',
        '#value' => $product->nid,
      );
      $form['products'][$i]['qty'] = array(
        '#type' => 'uc_quantity',
        '#title' => theme('uc_qty_label'),
        '#title_display' => 'invisible',
        '#default_value' => $product->qty,
      );
      $form['products'][$i]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#title_display' => 'invisible',
        '#default_value' => $product->title,
        '#size' => 30,
        '#maxlength' => 255,
      );
      $form['products'][$i]['model'] = array(
        '#type' => 'textfield',
        '#title' => t('SKU'),
        '#title_display' => 'invisible',
        '#default_value' => $product->model,
        '#size' => 6,
      );
      $form['products'][$i]['weight'] = array(
        '#type' => 'textfield',
        '#title' => t('Weight'),
        '#title_display' => 'invisible',
        '#default_value' => $product->weight,
        '#size' => 3,
      );
      $units = array(
        'lb' => t('Pounds'),
        'kg' => t('Kilograms'),
        'oz' => t('Ounces'),
        'g'  => t('Grams'),
      );
      $form['products'][$i]['weight_units'] = array(
        '#type' => 'select',
        '#title' => t('Units'),
        '#title_display' => 'invisible',
        '#default_value' => $product->weight_units,
        '#options' => $units,
      );
      $form['products'][$i]['cost'] = array(
        '#type' => 'uc_price',
        '#title' => t('Cost'),
        '#title_display' => 'invisible',
        '#default_value' => $product->cost,
        '#size' => 5,
      );
      $form['products'][$i]['price'] = array(
        '#type' => 'uc_price',
        '#title' => t('Price'),
        '#title_display' => 'invisible',
        '#default_value' => $product->price,
        '#size' => 5,
      );
      $form['products'][$i]['data'] = array(
        '#type' => 'hidden',
        '#value' => serialize($product->data),
      );
    }
  }
  else {
    $form['products'] = array(
      '#markup' => t('This order contains no products.'),
      '#prefix' => '<div id="order-edit-products">',
      '#suffix' => '</div>',
    );
  }

  return $form;
}

/**
 * Sets the order pane to show the product selection form.
 */
function uc_order_pane_products_select($form, &$form_state) {
  $types = uc_product_types();
  $options = array();

  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', $types, 'IN')
    ->orderBy('n.title')
    ->addTag('node_access');

  if (!empty($form_state['values']['product_controls']['product_search'])) {
    $search = strtolower(str_replace('*', '%', $form_state['values']['product_controls']['product_search']));
    $query->leftJoin('uc_products', 'p', 'n.nid = p.nid');
    $query->condition(db_or()
            ->condition('n.title', $search, 'LIKE')
            ->condition('p.model', $search, 'LIKE')
          );
  }

  $result = $query->execute();
  foreach ($result as $row) {
    $options[$row->nid] = $row->title;
  }

  if (count($options) == 0) {
    $options[0] = t('No products found.');
  }

  $form_state['products_action'] = 'select';
  $form_state['product_select_options'] = $options;
  unset($form_state['refresh_products']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Sets the order pane to show the add product to order form.
 */
function uc_order_pane_products_add($form, &$form_state) {
  $form_state['products_action'] = 'add_product';
  $form_state['node'] = node_load($form_state['values']['product_controls']['nid']);
  unset($form_state['refresh_products']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Hides the form to add another product to the order.
 */
function uc_order_pane_products_close($form, &$form_state) {
  unset($form_state['products_action']);
  unset($form_state['refresh_products']);
  unset($form_state['product_select_options']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Form submit callback: add a blank line product to an order.
 */
function uc_order_edit_products_add_blank($form, &$form_state) {
  $form_state['refresh_products'] = TRUE;
  $form_state['rebuild'] = TRUE;

  $order = $form_state['build_info']['args'][0];

  $product = new stdClass();
  $product->qty = 1;
  $product->order_id = $order->order_id;
  uc_order_product_save($order->order_id, $product);

  $order->products[] = $product;

  uc_order_log_changes($order->order_id, array('add' => t('Added new product line to order.')));
}

/**
 * Form submit callback: add a product to an order.
 */
function uc_order_edit_products_add($form, &$form_state) {
  $form_state['products_action'] = 'products_select';
  $form_state['refresh_products'] = TRUE;
  $form_state['rebuild'] = TRUE;
  $order = $form_state['build_info']['args'][0];

  $data = module_invoke_all('uc_add_to_cart_data', $form_state['values']['product_controls']);
  $product = uc_product_load_variant(intval($form_state['values']['product_controls']['nid']), $data);
  $product->qty = isset($form_state['values']['product_controls']['qty']) ? $form_state['values']['product_controls']['qty'] : $product->default_qty;

  drupal_alter('uc_order_product', $product, $order);
  uc_order_product_save($order->order_id, $product);
  $order->products[] = $product;

  uc_order_log_changes($order->order_id, array('add' => t('Added (@qty) @title to order.', array('@qty' => $product->qty, '@title' => $product->title))));

  // Decrement stock.
  if (module_exists('uc_stock')) {
    uc_stock_adjust_product_stock($product, 0, $order);
  }

  // Add this product to the form values for accurate tax calculations.
  $form_state['values']['products'][] = (array) $product;
}

/**
 * Form submit callback: remove a product from an order.
 */
function uc_order_edit_products_remove($form, &$form_state) {
  $form_state['refresh_products'] = TRUE;

  $order_product_id = intval($form_state['triggering_element']['#return_value']);

  if (module_exists('uc_stock')) {
    // Replace stock immediately.
    $product = uc_order_product_load($order_product_id);
    uc_stock_adjust($product->model, $product->qty);
  }

  uc_order_product_delete($order_product_id);

  $order = $form_state['build_info']['args'][0];
  $matches = array();
  preg_match('/products\[(\d+)\]/', $form_state['triggering_element']['#name'], $matches);
  $key = $matches[1];

  unset($order->products[$key]);
  $order->products = array_values($order->products);
}

/**
 * AJAX callback to render the order product controls.
 */
function uc_order_pane_products_ajax_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#product-controls', trim(drupal_render($form['product_controls'])));
  $commands[] = ajax_command_prepend('#product-controls', trim(theme('status_messages')));

  if (isset($form_state['refresh_products']) && $form_state['refresh_products']) {
    $commands[] = ajax_command_replace('#order-edit-products', trim(drupal_render($form['products'])));
    $commands[] = ajax_command_replace('#order-line-items', trim(drupal_render($form['line_items'])));
    $commands[] = ajax_command_prepend('#order-edit-products', trim(theme('status_messages')));
  }

  // Remove the field so we only refresh the admin comments item-list.
  unset($form['admin_comment_field']);
  $commands[] = ajax_command_replace('#order-pane-admin_comments .item-list', uc_order_pane_admin_comments('edit-theme', $form['#order'], $form, $form_state));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Handles the "Line Items" order pane.
 */
function uc_order_pane_line_items($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
    case 'customer':
      $line_items = $order->line_items;
      $items = _uc_line_item_list();
      foreach ($items as $item) {
        if (isset($item['display_only']) && $item['display_only'] == TRUE) {
          $result = $item['callback']('display', $order);
          if (is_array($result)) {
            foreach ($result as $line) {
              $line_items[] = array(
                'title' => $line['title'],
                'amount' => $line['amount'],
                'weight' => $item['weight']
              );
            }
          }
        }
      }
      usort($line_items, 'uc_weight_sort');

      $build['line_items'] = array(
        '#prefix' => '<table class="line-item-table">',
        '#suffix' => '</table>',
      );
      foreach ($line_items as $item) {
        $table_row = array(
          '#prefix' => '<tr>',
          '#suffix' => '</tr>',
        );

        $table_row['title'] = array(
          '#markup' => check_plain($item['title']),
          '#prefix' => '<td class="li-title">',
          '#suffix' => '</td>',
        );

        $table_row['amount'] = array(
          '#theme' => 'uc_price',
          '#price' => $item['amount'],
          '#prefix' => '<td class="li-amount">',
          '#suffix' => '</td>',
        );

        $build['line_items'][] = $table_row;
      }

      return $build;

    case 'edit-form':
      $options = array();
      $items = _uc_line_item_list();
      $line_items = $order->line_items;
      foreach ($items as $item) {
        if (isset($item['add_list']) && $item['add_list'] === TRUE) {
          $options[$item['id']] = check_plain($item['title']);
        }
        if (isset($item['display_only']) && $item['display_only'] == TRUE) {
          $result = $item['callback']('display', $order);
          if (is_array($result)) {
            foreach ($result as $line) {
              $line_items[] = array(
                'line_item_id' => $line['id'],
                'title' => $line['title'],
                'amount' => $line['amount'],
                'weight' => $item['weight'],
              );
            }
          }
        }
      }
      usort($line_items, 'uc_weight_sort');

      $form['add_line_item'] = array('#type' => 'container');

      $form['add_line_item']['li_type_select'] = array(
        '#type' => 'select',
        '#title' => t('Add a line item'),
        '#options' => $options,
      );
      $form['add_line_item']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add line'),
        '#submit' => array('uc_order_pane_line_items_submit', 'uc_order_pane_line_items_add'),
        '#ajax' => array(
          'callback' => 'uc_order_pane_line_items_update',
        ),
      );
      $form['line_items'] = array(
        '#tree' => TRUE,
        '#theme' => 'uc_order_pane_line_items',
        '#prefix' => '<div id="order-line-items">',
        '#suffix' => '</div>',
      );

      foreach ($line_items as $item) {
        $form['line_items'][$item['line_item_id']]['li_id'] = array(
          '#type' => 'hidden',
          '#value' => $item['line_item_id'],
        );
        if (isset($item['type']) && _uc_line_item_data($item['type'], 'stored') == TRUE) {
          $form['line_items'][$item['line_item_id']]['remove'] = array(
            '#type' => 'image_button',
            '#title' => t('Remove line item.'),
            '#src' => drupal_get_path('module', 'uc_store') . '/images/error.gif',
            '#button_type' => 'remove',
            '#submit' => array('uc_order_pane_line_items_submit', 'uc_order_pane_line_items_remove'),
            '#ajax' => array(
              'callback' => 'uc_order_pane_line_items_update',
            ),
            '#return_value' => $item['line_item_id'],
          );
          $form['line_items'][$item['line_item_id']]['title'] = array(
            '#type' => 'textfield',
            '#title' => t('Title'),
            '#default_value' => $item['title'],
            '#size' => 40,
            '#maxlength' => 128,
          );
          $form['line_items'][$item['line_item_id']]['amount'] = array(
            '#type' => 'uc_price',
            '#title' => t('Amount'),
            '#default_value' => $item['amount'],
            '#size' => 6,
            '#allow_negative' => TRUE,
          );
        }
        else {
          $form['line_items'][$item['line_item_id']]['title'] = array(
            '#markup' => check_plain($item['title']),
          );
          $form['line_items'][$item['line_item_id']]['amount'] = array(
            '#theme' => 'uc_price',
            '#price' => $item['amount'],
          );
        }
      }
      return $form;

    case 'edit-theme':
      return drupal_render($form['add_line_item'])
           . drupal_render($form['line_items']);

    case 'edit-process':
      uc_order_pane_line_items_submit($form, $form_state);
      return;
  }
}

/**
 * @ingroup themeable
 */
function theme_uc_order_pane_line_items($variables) {
  $form = $variables['form'];

  $output = '<table class="line-item-table">';
  foreach (element_children($form) as $field) {
    $form[$field]['title']['#title'] = '';
    $form[$field]['amount']['#title'] = '';
    $output .= '<tr><td class="li-title">'
      . drupal_render($form[$field]['li_id'])
      . drupal_render($form[$field]['remove'])
      . drupal_render($form[$field]['title'])
      . ':</td><td class="li-amount">'
      . drupal_render($form[$field]['amount'])
      . '</td></tr>';
  }
  $output .= '</table>' . drupal_render_children($form);

  return $output;
}

/**
 * Form submit callback: Update line items titles and amounts in an order.
 */
function uc_order_pane_line_items_submit($form, &$form_state) {
  $values = $form_state['values'];

  if (is_array($values['line_items'])) {
    foreach ($values['line_items'] as $line) {
      if (is_numeric($line['li_id']) && intval($line['li_id']) > 0 && isset($line['title']) && isset($line['amount'])) {
        uc_order_update_line_item($line['li_id'], $line['title'], $line['amount']);
      }
    }
  }
}

/**
 * Order pane submit callback: Add a line item to an order.
 */
function uc_order_pane_line_items_add($form, &$form_state) {
  $order = &$form_state['order'];
  $type = $form_state['values']['li_type_select'];

  uc_order_line_item_add($order->order_id, $type, _uc_line_item_data($type, 'title'), 0);
  $order->line_items = uc_order_load_line_items($order);

  $form_state['rebuild'] = TRUE;
}

/**
 * Order pane submit callback: Remove a line item from an order.
 */
function uc_order_pane_line_items_remove($form, &$form_state) {
  $order = &$form_state['order'];
  $line_item_id = intval($form_state['triggering_element']['#return_value']);

  uc_order_delete_line_item($line_item_id);
  $order->line_items = uc_order_load_line_items($order);

  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback to render the line items.
 */
function uc_order_pane_line_items_update($form, &$form_state) {
  $commands[] = ajax_command_replace('#order-line-items', trim(drupal_render($form['line_items'])));
  $commands[] = ajax_command_prepend('#order-line-items', theme('status_messages'));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Handles the "Order Comments" order pane.
 */
function uc_order_pane_order_comments($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      $comments = uc_order_comments_load($order->order_id);
      return tapir_get_table('uc_op_order_comments_view_table', $comments);

    case 'customer':
      $comments = uc_order_comments_load($order->order_id);
      $header = array(t('Date'), t('Status'), t('Message'));
      $rows[] = array(
        array('data' => format_date($order->created, 'uc_store'), 'class' => array('date')),
        array('data' => '-', 'class' => array('status')),
        array('data' => t('Order created.'), 'class' => array('message')),
      );
      if (count($comments) > 0) {
        foreach ($comments as $comment) {
          $rows[] = array(
            array('data' => format_date($comment->created, 'uc_store'), 'class' => array('date')),
            array('data' => $comment->title, 'class' => array('status')),
            array('data' => check_plain($comment->message), 'class' => array('message')),
          );
        }
      }
      $build = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('class' => array('uc-order-comments')),
      );

      return $build;
  }
}

/**
 * Handles the "Admin Comments" order pane.
 */
function uc_order_pane_admin_comments($op, $order, &$form = NULL, &$form_state = NULL) {
  global $user;

  switch ($op) {
    case 'view':
      $comments = uc_order_comments_load($order->order_id, TRUE);
      return tapir_get_table('uc_op_admin_comments_view_table', $comments);

    case 'edit-form':
      $form['admin_comment_field'] = array(
        '#type' => 'fieldset',
        '#title' => t('Add an admin comment'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['admin_comment_field']['admin_comment'] = array(
        '#type' => 'textarea',
        '#description' => t('Admin comments are only seen by store administrators.'),
      );
      return $form;

    case 'edit-theme':
      $comments = uc_order_comments_load($form['order_id']['#value'], TRUE);
      if (is_array($comments) && count($comments) > 0) {
        foreach ($comments as $comment) {
          $items[] = '[' . theme('uc_uid', array('uid' => $comment->uid)) . '] ' . filter_xss_admin($comment->message);
        }
      }
      else {
        $items = array(t('No admin comments have been entered for this order.'));
      }
      $output = theme('item_list', array('items' => $items)) . drupal_render($form['admin_comment_field']);
      return $output;

    case 'edit-process':
      if (!empty($form_state['values']['admin_comment'])) {
        uc_order_comment_save($form_state['values']['order_id'], $user->uid, $form_state['values']['admin_comment']);
      }
      return;
  }
}

/**
 * Handles the "Update" order pane.
 */
function uc_order_pane_update($op, $order, &$form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      return drupal_get_form('uc_order_view_update_form', $order);
  }
}

/**
 * Form to save order comments and update the order status.
 *
 * @see uc_order_view_update_form_submit()
 * @ingroup forms
 */
function uc_order_view_update_form($form, &$form_state, $order) {
  $form['order_comment_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add an order comment'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['order_comment_field']['order_comment'] = array(
    '#type' => 'textarea',
    '#description' => t('Order comments are used primarily to communicate with the customer.'),
  );

  $form['admin_comment_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add an admin comment'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['admin_comment_field']['admin_comment'] = array(
    '#type' => 'textarea',
    '#description' => t('Admin comments are only seen by store administrators.'),
  );

  $form['current_status'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_status,
  );

  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );

  $form['controls'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('uc-inline-form', 'clearfix')),
    '#weight' => 10,
  );

  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['controls']['status'] = array(
    '#type' => 'select',
    '#title' => t('Order status'),
    '#default_value' => $order->order_status,
    '#options' => $options,
  );

  $form['controls']['notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail notification on update.'),
  );

  $form['controls']['actions'] = array('#type' => 'actions');
  $form['controls']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Form submit handler for uc_order_view_update_form().
 *
 * @see uc_order_view_update_form()
 */
function uc_order_view_update_form_submit($form, &$form_state) {
  global $user;

  if (!empty($form_state['values']['order_comment'])) {
    uc_order_comment_save($form_state['values']['order_id'], $user->uid, $form_state['values']['order_comment'], 'order', $form_state['values']['status'], $form_state['values']['notify']);
  }

  if (!empty($form_state['values']['admin_comment'])) {
    uc_order_comment_save($form_state['values']['order_id'], $user->uid, $form_state['values']['admin_comment']);
  }

  if ($form_state['values']['status'] != $form_state['values']['current_status']) {
    if (uc_order_update_status($form_state['values']['order_id'], $form_state['values']['status'])) {
      if (empty($form_state['values']['order_comment'])) {
        uc_order_comment_save($form_state['values']['order_id'], $user->uid, '-', 'order', $form_state['values']['status'], $form_state['values']['notify']);
      }
    }
  }

  // Let Rules send email if requested.
  if ($form_state['values']['notify']) {
    $order = uc_order_load($form_state['values']['order_id']);
    rules_invoke_event('uc_order_status_email_update', $order);
  }

  drupal_set_message(t('Order updated.'));
}

/**
 * Builds the order view products table.
 */
function uc_op_products_view_table($order) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => array('order-pane-table')),
  );

  $table['#columns']['qty'] = array(
    'cell' => array(
      'data' => theme('uc_qty_label'),
      'class' => array('qty'),
    ),
    'weight' => 0,
  );
  $table['#columns']['product'] = array(
    'cell' => array(
      'data' => t('Product'),
      'class' => array('product'),
    ),
    'weight' => 1,
  );
  $table['#columns']['model'] = array(
    'cell' => array(
      'data' => t('SKU'),
      'class' => array('sku'),
    ),
    'weight' => 2,
  );
  if (user_access('administer products')) {
    $table['#columns']['cost'] = array(
      'cell' => array(
        'data' => t('Cost'),
        'class' => array('cost'),
      ),
      'weight' => 3,
    );
  }
  $table['#columns']['price'] = array(
    'cell' => array(
      'data' => t('Price'),
      'class' => array('price'),
    ),
    'weight' => 4,
  );
  $table['#columns']['total'] = array(
    'cell' => array(
      'data' => t('Total'),
      'class' => array('total'),
    ),
    'weight' => 5,
  );

  if (!empty($order->products)) {
    $build = uc_order_product_view_multiple($order->products);
    $table['#rows'] = $build['uc_order_product'];
  }
  else {
    $table['#rows'][]['product'] = array(
      '#markup' => t('This order contains no products.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
  }

  return $table;
}

/**
 * Builds the order customer's view products table.
 */
function uc_op_products_customer_table($order) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => array('order-pane-table')),
  );

  $table['#columns']['qty'] = array(
    'cell' => array(
      'data' => theme('uc_qty_label'),
      'class' => array('qty'),
    ),
    'weight' => 0,
  );
  $table['#columns']['product'] = array(
    'cell' => array(
      'data' => t('Product'),
      'class' => array('product'),
    ),
    'weight' => 1,
  );
  $table['#columns']['model'] = array(
    'cell' => array(
      'data' => t('SKU'),
      'class' => array('sku'),
    ),
    'weight' => 2,
  );
  if (user_access('administer products')) {
    $table['#columns']['cost'] = array(
      'cell' => array(
        'data' => t('Cost'),
        'class' => array('cost'),
      ),
      'weight' => 3,
    );
  }
  $table['#columns']['price'] = array(
    'cell' => array(
      'data' => t('Price'),
      'class' => array('price'),
    ),
    'weight' => 4,
  );
  $table['#columns']['total'] = array(
    'cell' => array(
      'data' => t('Total'),
      'class' => array('total'),
    ),
    'weight' => 5,
  );

  if (!empty($order->products)) {
    $build = uc_order_product_view_multiple($order->products);
    $table['#rows'] = $build['uc_order_product'];
  }
  else {
    $table['#rows'][]['product'] = array(
      '#markup' => t('This order contains no products.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
  }

  return $table;
}

/**
 * TAPIr table for products pane on the order edit page.
 */
function uc_op_products_edit_table() {
  $table = array(
    '#type' => 'tapir_table',
    '#tree' => TRUE,
    '#attributes' => array('id' => 'order-edit-products', 'class' => array('order-pane-table')),
  );

  $table['#columns']['remove'] = array(
    'cell' => t('Remove'),
    'weight' => 0,
  );
  $table['#columns']['qty'] = array(
    'cell' => theme('uc_qty_label'),
    'weight' => 1,
  );
  $table['#columns']['title'] = array(
    'cell' => t('Name'),
    'weight' => 2,
  );
  $table['#columns']['model'] = array(
    'cell' => t('SKU'),
    'weight' => 3,
  );
  $table['#columns']['weight'] = array(
    'cell' => t('Weight'),
    'weight' => 4,
  );
  $table['#columns']['weight_units'] = array(
    'cell' => t('Units'),
    'weight' => 5,
  );
  $table['#columns']['cost'] = array(
    'cell' => t('Cost'),
    'weight' => 6,
  );
  $table['#columns']['price'] = array(
    'cell' => t('Price'),
    'weight' => 7,
  );

  return $table;
}

/**
 * Builds the order comments table.
 */
function uc_op_order_comments_view_table($comments) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => array('order-pane-table uc-order-comments')),
  );

  $table['#columns']['date'] = array(
    'cell' => array('data' => t('Date'), 'class' => array('date')),
    'weight' => 0,
  );
  $table['#columns']['user'] = array(
    'cell' => array('data' => t('User'), 'class' => array('user')),
    'weight' => 1,
  );
  $table['#columns']['notified'] = array(
    'cell' => array('data' => t('Notified'), 'class' => array('notified')),
    'weight' => 2,
  );
  $table['#columns']['status'] = array(
    'cell' => array('data' => t('Status'), 'class' => array('status')),
    'weight' => 3,
  );
  $table['#columns']['comment'] = array(
    'cell' => array('data' => t('Comment'), 'class' => array('message')),
    'weight' => 4,
  );

  if (is_array($comments) && !empty($comments)) {
    foreach ($comments as $comment) {
      $data = array();
      $data['date'] = array(
        '#markup' => format_date($comment->created, 'short'),
        '#cell_attributes' => array('class' => 'date'),
      );
      $data['user'] = array(
        '#markup' => theme('uc_uid', array('uid' => $comment->uid)),
        '#cell_attributes' => array('class' => 'user'),
      );
      $icon = $comment->notified ? 'true-icon.gif' : 'false-icon.gif';
      $data['notified'] = array(
        '#markup' => theme('image', array('path' => drupal_get_path('module', 'uc_order') . '/images/' . $icon)),
        '#cell_attributes' => array('class' => 'notified'),
      );
      $data['status'] = array(
        '#markup' => $comment->title,
        '#cell_attributes' => array('class' => 'status'),
      );
      $data['comment'] = array(
        '#markup' => check_plain($comment->message),
        '#cell_attributes' => array('class' => 'message'),
      );
      $table['#rows'][] = $data;
    }
  }
  else {
    $data['comment'] = array(
      '#markup' => t('This order has no comments associated with it.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
    $table['#rows'][] = $data;
  }

  return $table;
}

/**
 * Builds the order admin comments table.
 */
function uc_op_admin_comments_view_table($comments) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => array('order-pane-table uc-order-comments')),
  );

  $table['#columns']['date'] = array(
    'cell' => array('data' => t('Date'), 'class' => array('date')),
    'weight' => 0,
  );
  $table['#columns']['user'] = array(
    'cell' => array('data' => t('User'), 'class' => array('user')),
    'weight' => 1,
  );
  $table['#columns']['comment'] = array(
    'cell' => array('data' => t('Comment'), 'class' => array('message')),
    'weight' => 2,
  );

  if (is_array($comments) && !empty($comments)) {
    foreach ($comments as $comment) {
      $data = array();
      $data['date'] = array(
        '#markup' => format_date($comment->created, 'short'),
        '#cell_attributes' => array('class' => 'date'),
      );
      $data['user'] = array(
        '#markup' => theme('uc_uid', array('uid' => $comment->uid)),
        '#cell_attributes' => array('class' => 'user'),
      );
      $data['comment'] = array(
        '#markup' => filter_xss_admin($comment->message),
        '#cell_attributes' => array('class' => 'message'),
      );
      $table['#rows'][] = $data;
    }
  }
  else {
    $data['comment'] = array(
      '#markup' => t('This order has no admin comments associated with it.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
    $table['#rows'][] = $data;
  }

  return $table;
}

/**
 * Builds a list of order panes defined in the enabled modules.
 */
function _uc_order_pane_list($view = 'view') {
  static $panes = array();

  if (count($panes) > 0) {
    return $panes;
  }

  foreach (module_invoke_all('uc_order_pane') as $id => $pane) {
    // Preserve backward compatibility for panes with no key specified.
    if (is_numeric($id)) {
      $id = $pane['id'];
    }

    // Set defaults.
    $pane += array(
      'id' => $id,
      'enabled' => TRUE,
      'weight' => 0,
    );

    $panes[$id] = $pane;
  }

  // Allow other modules to alter the defaults.
  drupal_alter('uc_order_pane', $panes);

  uasort($panes, 'uc_weight_sort');

  return $panes;
}

/**
 * Returns data from an order pane by pane ID and the array key.
 */
function _uc_order_pane_data($pane_id, $key) {
  $panes = _uc_order_pane_list();
  return $panes[$pane_id][$key];
}
