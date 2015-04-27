<?php

/**
 * @file
 * Shipping administration menu items.
 */

/**
 * Displays a list of an order's packaged products.
 */
function uc_shipping_order_packages($order) {
  $shipping_type_options = uc_quote_shipping_type_options();
  $header = array(t('Package ID'), t('Products'), t('Shipping type'), t('Package type'), t('Shipment ID'), t('Tracking number'), t('Labels'), array('data' => t('Actions'), 'colspan' => 4));
  $rows = array();
  $result = db_query("SELECT * FROM {uc_packages} WHERE order_id = :id", array(':id' => $order->order_id));
  foreach ($result as $package) {
    $row = array();

    $row[] = $package->package_id;

    $product_list = array();
    $result2 = db_query("SELECT op.order_product_id, pp.qty, op.title, op.model FROM {uc_packaged_products} pp LEFT JOIN {uc_order_products} op ON op.order_product_id = pp.order_product_id WHERE pp.package_id = :id", array(':id' => $package->package_id));
    foreach ($result2 as $product) {
      $product_list[] = $product->qty . ' x ' . check_plain($product->model);
    }

    $row[] = '<ul><li>' . implode('</li><li>', $product_list) . '</li></ul>';
    $row[] = isset($shipping_type_options[$package->shipping_type]) ? $shipping_type_options[$package->shipping_type] : strtr($package->shipping_type, '_', ' ');
    $row[] = check_plain($package->pkg_type);
    $row[] = isset($package->sid) ? l($package->sid, 'admin/store/orders/' . $order->order_id . '/shipments/' . $package->sid . '/view') : '';
    $row[] = isset($package->tracking_number) ? check_plain($package->tracking_number) : '';

    if ($package->label_image && $image = file_load($package->label_image)) {
      $package->label_image = $image;
    }
    else {
      unset($package->label_image);
    }

    if (isset($package->sid) && isset($package->label_image)) {
      $method = db_query("SELECT shipping_method FROM {uc_shipments} WHERE sid = :sid", array(':sid' => $package->sid))->fetchField();
      $row[] = l(
        theme('image_style', array(
          'style_name' => 'uc_thumbnail',
          'path' => $package->label_image->uri,
          'alt' => t('Shipping label'),
          'title' => t('Shipping label'),
        )),
        'admin/store/orders/' . $order->order_id . '/shipments/labels/' . $method . '/' . $package->label_image->uri,
        array('html' => TRUE)
      );
    }
    else {
      $row[] = '';
    }

    $row[] = l(t('edit'), 'admin/store/orders/' . $order->order_id . '/packages/' . $package->package_id . '/edit');
    $row[] = l(t('ship'), 'admin/store/orders/' . $order->order_id . '/shipments/new', array('query' => array('pkgs' => array($package->package_id))));
    $row[] = l(t('delete'), 'admin/store/orders/' . $order->order_id . '/packages/' . $package->package_id . '/delete');

    if ($package->sid) {
      $row[] = l(t('cancel shipment'), 'admin/store/orders/' . $order->order_id . '/packages/' . $package->package_id . '/cancel');
    }
    else {
      $row[] = '';
    }

    $rows[] = $row;
  }

  if (empty($rows)) {
    drupal_set_message(t("This order's products have not been organized into packages."));
    drupal_goto('admin/store/orders/' . $order->order_id . '/packages/new');
  }

  $build['packages'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Puts ordered products into a package.
 *
 * @see uc_shipping_new_package_validate()
 * @see uc_shipping_new_package_submit()
 * @see theme_uc_shipping_new_package_fieldset()
 * @ingroup forms
 */
function uc_shipping_new_package($form, &$form_state, $order) {
  $form['#tree'] = TRUE;
  $shipping_types_products = array();
  foreach ($order->products as $product) {
    if ($product->data['shippable']) {
      $product->shipping_type = uc_product_get_shipping_type($product);
      $shipping_types_products[$product->shipping_type][] = $product;
    }
  }

  $shipping_type_weights = variable_get('uc_quote_type_weight', array());

  $result = db_query("SELECT op.order_product_id, SUM(pp.qty) AS quantity FROM {uc_packaged_products} pp LEFT JOIN {uc_packages} p ON pp.package_id = p.package_id LEFT JOIN {uc_order_products} op ON op.order_product_id = pp.order_product_id WHERE p.order_id = :id GROUP BY op.order_product_id", array(':id' => $order->order_id));
  $packaged_products = $result->fetchAllKeyed();

  $form['shipping_types'] = array();
  $shipping_type_options = uc_quote_shipping_type_options();
  foreach ($shipping_types_products as $shipping_type => $products) {
    $form['shipping_types'][$shipping_type] = array(
      '#type'        => 'fieldset',
      '#title'       => isset($shipping_type_options[$shipping_type]) ?
                        $shipping_type_options[$shipping_type]        :
                        ucwords(str_replace('_', ' ', $shipping_type)),
      '#collapsible' => TRUE,
      '#collapsed'   => FALSE,
      '#weight'      => isset($shipping_type_weights[$shipping_type]) ? $shipping_type_weights[$shipping_type] : 0,
    );

    foreach ($products as $product) {
      $unboxed_qty = $product->qty;
      if (isset($packaged_products[$product->order_product_id])) {
        $unboxed_qty -= $packaged_products[$product->order_product_id];
      }

      if ($unboxed_qty > 0) {
        $product_row = array();
        $product_row['checked'] = array(
          '#type'          => 'checkbox',
          '#default_value' => 0,
        );
        $product_row['model'] = array(
          '#markup' => check_plain($product->model)
        );
        $product_row['name'] = array(
          '#markup' => filter_xss_admin($product->title)
        );
        $product_row['qty'] = array(
          '#type'          => 'select',
          '#title'         => theme('uc_qty_label'),
          '#title_display' => 'invisible',
          '#options'       => drupal_map_assoc(range(1, $unboxed_qty)),
          '#default_value' => $unboxed_qty,
        );

        $options = drupal_map_assoc(range(0, count($order->products)));
        $options[0] = t('Sep.');
        $product_row['package'] = array(
          '#type' => 'select',
          '#title' => t('Package'),
          '#title_display' => 'invisible',
          '#options' => $options,
          '#default_value' => 0,
        );

        $form['shipping_types'][$shipping_type][$product->order_product_id] = $product_row;
      }
    }

    $form['shipping_types'][$shipping_type]['#theme'] = 'uc_shipping_new_package_fieldset';
  }

  $form['order_id'] = array(
    '#type'  => 'hidden',
    '#value' => $order->order_id,
  );
  $form['actions'] = array(
    '#type'  => 'actions',
  );
  $form['actions']['create'] = array(
    '#type'  => 'submit',
    '#value' => t('Make packages'),
  );
  $form['actions']['combine'] = array(
    '#type'  => 'submit',
    '#value' => t('Create one package'),
  );
  $form['actions']['cancel'] = array(
    '#type'  => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Formats and displays the products in a shipping type fieldset.
 *
 * @see uc_shipping_new_package()
 * @ingroup themeable
 */
function theme_uc_shipping_new_package_fieldset($variables) {
  $fieldset = $variables['fieldset'];
  drupal_add_js('misc/tableselect.js');

  $output = '';
  $header = array(array('class' => array('select-all')), t('SKU'), t('Title'), theme('uc_qty_label'), t('Package'));

  $rows = array();
  foreach (element_children($fieldset) as $op_id) {
    $row = array();
    $row[] = drupal_render($fieldset[$op_id]['checked']);
    $row[] = drupal_render($fieldset[$op_id]['model']);
    $row[] = drupal_render($fieldset[$op_id]['name']);
    $row[] = drupal_render($fieldset[$op_id]['qty']);
    $row[] = drupal_render($fieldset[$op_id]['package']);
    $rows[] = $row;
  }

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('There are no products available for this type of package.'),
  ));
  $output .= drupal_render_children($fieldset);

  return $output;
}

/**
 * Validation handler for uc_shipping_new_package().
 *
 * Do not allow empty packages.
 *
 * @see uc_shipping_new_package()
 * @see uc_shipping_new_package_submit()
 */
function uc_shipping_new_package_validate($form, &$form_state) {
  if ($form_state['values']['op'] != t('Cancel')) {
    $empty = TRUE;

    foreach ($form_state['values']['shipping_types'] as $shipping_type => $products) {
      foreach ($products as $product) {
        if ($product['checked'] != 0) {
          $empty = FALSE;
          break 2;
        }
      }
    }

    if ($empty) {
      form_set_error($shipping_type, t('Packages should have at least one product in them.'));
    }
  }
}

/**
 * Submit handler for uc_shipping_new_package().
 *
 * @see uc_shipping_new_package()
 * @see uc_shipping_new_package_validate()
 */
function uc_shipping_new_package_submit($form, &$form_state) {
  if ($form_state['values']['op'] != t('Cancel')) {
    $packages = array(0 => array());

    foreach ($form_state['values']['shipping_types'] as $shipping_type => $products) {
      foreach ($products as $id => $product) {
        if ($product['checked']) {
          if ($form_state['values']['op'] == t('Create one package')) {
            $product['package'] = 1;
          }

          if ($product['package'] != 0) {
            $packages[$product['package']]['products'][$id] = (object)$product;

            if (!isset($packages[$product['package']]['shipping_type'])) {
              $packages[$product['package']]['shipping_type'] = $shipping_type;
            }
          }
          else {
            $packages[0][$shipping_type][$id] = (object)$product;
          }
        }
      }
      if (isset($packages[0][$shipping_type])) {
        foreach ($packages[0][$shipping_type] as $id => $product) {
          $qty = $product->qty;
          $product->qty = 1;
          for ($i = 0; $i < $qty; $i++) {
            $packages[] = array('products' => array($id => $product), 'shipping_type' => $shipping_type);
          }
        }
      }

      unset($packages[0][$shipping_type]);
    }

    if (empty($packages[0])) {
      unset($packages[0]);
    }

    foreach ($packages as $package) {
      $package['order_id'] = $form_state['values']['order_id'];
      uc_shipping_package_save($package);
    }
  }

  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/packages';
}

/**
 * Rearranges the products in or out of a package.
 *
 * @see uc_shipping_package_edit_submit()
 * @see theme_uc_shipping_edit_package_fieldset()
 * @ingroup forms
 */
function uc_shipping_package_edit($form, &$form_state, $order, $package) {
  $products = array();
  $shipping_types_products = array();
  foreach ($order->products as $product) {
    if ($product->data['shippable']) {
      $product->shipping_type = uc_product_get_shipping_type($product);
      $shipping_types_products[$product->shipping_type][$product->order_product_id] = $product;
      $products[$product->order_product_id] = $product;
    }
  }

  $result = db_query("SELECT order_product_id, SUM(qty) AS quantity FROM {uc_packaged_products} pp LEFT JOIN {uc_packages} p ON pp.package_id = p.package_id WHERE p.order_id = :id GROUP BY order_product_id", array(':id' => $order->order_id));
  foreach ($result as $packaged_product) {
    // Make already packaged products unavailable, except those in this package.
    $products[$packaged_product->order_product_id]->qty -= $packaged_product->quantity;
    if (isset($package->products[$packaged_product->order_product_id])) {
      $products[$packaged_product->order_product_id]->qty += $package->products[$packaged_product->order_product_id]->qty;
    }
  }

  $form['#tree'] = TRUE;
  $form['package_id'] = array('#type' => 'hidden', '#value' => $package->package_id);
  $form['products'] = array();
  foreach ($products as $product) {
    if ($product->qty > 0) {
      $product_row = array();
      $product_row['checked'] = array('#type' => 'checkbox', '#default_value' => isset($package->products[$product->order_product_id]));
      $product_row['model'] = array('#markup' => check_plain($product->model));
      $product_row['name'] = array('#markup' => filter_xss_admin($product->title));
      $product_row['qty'] = array(
        '#type' => 'select',
        '#options' => drupal_map_assoc(range(1, $product->qty)),
        '#default_value' => isset($package->products[$product->order_product_id]) ? $package->products[$product->order_product_id]->qty : 1,
      );

      $form['products'][$product->order_product_id] = $product_row;
    }
  }
  $form['products']['#theme'] = 'uc_shipping_edit_package_fieldset';
  $options = array();
  $shipping_type_options = uc_quote_shipping_type_options();
  foreach (array_keys($shipping_types_products) as $type) {
    $options[$type] = isset($shipping_type_options[$type]) ? $shipping_type_options[$type] : ucwords(str_replace('_', ' ', $type));
  }
  $form['shipping_type'] = array(
    '#type' => 'select',
    '#title' => t('Shipping type'),
    '#options' => $options,
    '#default_value' => $package->shipping_type,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Displays a formatted shipping type fieldset.
 *
 * @ingroup themeable
 */
function theme_uc_shipping_edit_package_fieldset($variables) {
  $fieldset = $variables['fieldset'];
  drupal_add_js('misc/tableselect.js');

  $output = '';
  $header = array(array('class' => array('select-all')), t('SKU'), t('Title'), theme('uc_qty_label'));
  $rows = array();
  foreach (element_children($fieldset) as $op_id) {
    $row = array();
    $row[] = drupal_render($fieldset[$op_id]['checked']);
    $row[] = drupal_render($fieldset[$op_id]['model']);
    $row[] = drupal_render($fieldset[$op_id]['name']);
    $row[] = drupal_render($fieldset[$op_id]['qty']);
    $rows[] = $row;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($fieldset);

  return $output;
}

/**
 * Submit handler for uc_shipping_package_edit().
 *
 * @see uc_shipping_package_edit()
 */
function uc_shipping_package_edit_submit($form, &$form_state) {
  $package = uc_shipping_package_load($form_state['values']['package_id']);
  foreach ($form_state['values']['products'] as $id => $product) {
    if ($product['checked']) {
      $package->products[$id] = (object)$product;
    }
    else {
      unset($package->products[$id]);
    }
  }
  $package->shipping_type = $form_state['values']['shipping_type'];
  uc_shipping_package_save($package);

  $form_state['redirect'] = 'admin/store/orders/' . $package->order_id . '/packages';
}

/**
 * Confirms cancellation of a package's shipment.
 *
 * @see uc_shipping_package_cancel_confirm_submit()
 * @ingroup forms
 */
function uc_shipping_package_cancel_confirm($form, &$form_state, $order, $package) {
  $form['order_id'] = array('#type' => 'value', '#value' => $order->order_id);
  $form['package_id'] = array('#type' => 'value', '#value' => $package->package_id);
  $output = confirm_form($form, t('Are you sure you want to cancel the shipment of this package?'), 'admin/store/orders/' . $order->order_id . '/packages',
    t('It will be available for reshipment.'), t('Cancel shipment'), t('Nevermind'));
  return $output;
}

/**
 * Form submission handler for uc_shipping_package_cancel_confirm().
 *
 * @see uc_shipping_package_cancel_confirm()
 */
function uc_shipping_package_cancel_confirm_submit($form, &$form_state) {
  $package = uc_shipping_package_load($form_state['values']['package_id']);
  $shipment = uc_shipping_shipment_load($package->sid);
  $methods = module_invoke_all('uc_shipping_method');
  if (isset($methods[$shipment->shipping_method]['cancel']) &&
      function_exists($methods[$shipment->shipping_method]['cancel'])) {
    $result = call_user_func($methods[$shipment->shipping_method]['cancel'], $shipment->tracking_number, array($package->tracking_number));
    if ($result) {
      db_update('uc_packages')
        ->fields(array(
          'sid' => NULL,
          'label_image' => NULL,
          'tracking_number' => NULL,
        ))
        ->condition('package_id', $package->package_id)
        ->execute();

      if (isset($package->label_image)) {
        file_usage_delete($package->label_image, 'uc_shipping', 'package', $package->package_id);
        file_delete($package->label_image);
        unset($package->label_image);
      }

      unset($shipment->packages[$package->package_id]);
      if (!count($shipment->packages)) {
        uc_shipping_shipment_delete($shipment->sid);
      }
    }
  }
  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/packages';
}

/**
 * Decides to unpackage products.
 *
 * @see uc_shipping_package_delete_confirm_submit()
 * @ingroup forms
 */
function uc_shipping_package_delete_confirm($form, &$form_state, $order, $package) {
  $form['order_id'] = array('#type' => 'value', '#value' => $order->order_id);
  $form['package_id'] = array('#type' => 'value', '#value' => $package->package_id);
  $output = confirm_form($form, t('Are you sure you want to delete this package?'), 'admin/store/orders/' . $order->order_id . '/packages',
    t('The products it contains will be available for repackaging.'), t('Delete'), t('Cancel'));
  return $output;
}

/**
 * Submit handler for uc_shipping_package_delete_confirm().
 *
 * @see uc_shipping_package_delete_confirm()
 */
function uc_shipping_package_delete_confirm_submit($form, &$form_state) {
  uc_shipping_package_delete($form_state['values']['package_id']);
  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/packages';
}

/**
 * Displays a list of shipments for an order.
 *
 * @param $order
 *   The order object.
 */
function uc_shipping_order_shipments($order) {
  $result = db_query("SELECT * FROM {uc_shipments} WHERE order_id = :id", array(':id' => $order->order_id));
  $header = array(t('Shipment ID'), t('Name'), t('Company'), t('Destination'), t('Ship date'), t('Estimated delivery'), t('Tracking number'), array('data' => t('Actions'), 'colspan' => 5));
  $rows = array();

  foreach ($result as $shipment) {
    $row = array();
    $row[] = $shipment->sid;
    $row[] = check_plain($shipment->d_first_name) . ' ' . check_plain($shipment->d_last_name);
    $row[] = check_plain($shipment->d_company);
    $row[] = check_plain($shipment->d_city) . ', ' . uc_get_zone_code($shipment->d_zone) . ' ' . check_plain($shipment->d_postal_code);
    $row[] = format_date($shipment->ship_date, 'uc_store');
    $row[] = format_date($shipment->expected_delivery, 'uc_store');
    $row[] = is_null($shipment->tracking_number) ? t('n/a') : check_plain($shipment->tracking_number);
    $row[] = l(t('view'), 'admin/store/orders/' . $order->order_id . '/shipments/' . $shipment->sid . '/view');
    $row[] = l(t('edit'), 'admin/store/orders/' . $order->order_id . '/shipments/' . $shipment->sid . '/edit');
    $row[] = l(t('print'), 'admin/store/orders/' . $order->order_id . '/shipments/' . $shipment->sid . '/print');
    $row[] = l(t('packing slip'), 'admin/store/orders/' . $order->order_id . '/shipments/' . $shipment->sid . '/packing_slip');
    $row[] = l(t('delete'), 'admin/store/orders/' . $order->order_id . '/shipments/' . $shipment->sid . '/delete');
    $rows[] = $row;
  }

  if (empty($rows)) {
    if (!db_query("SELECT COUNT(*) FROM {uc_packages} WHERE order_id = :id", array(':id' => $order->order_id))->fetchField()) {
      drupal_set_message(t("This order's products have not been organized into packages."));
      drupal_goto('admin/store/orders/' . $order->order_id . '/packages/new');
    }
    else {
      drupal_set_message(t('No shipments have been made for this order.'));
      drupal_goto('admin/store/orders/' . $order->order_id . '/shipments/new');
    }
  }

  $build['shipments'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Sets up a new shipment with the chosen packages.
 *
 * @see uc_shipping_new_shipment_submit()
 * @see theme_uc_shipping_new_shipment()
 * @ingroup forms
 */
function uc_shipping_new_shipment($form, &$form_state, $order) {
  $checked_pkgs = isset($_GET['pkgs']) ? $_GET['pkgs'] : array();

  $form['#tree'] = TRUE;
  $form['order_id'] = array('#type' => 'hidden', '#value' => $order->order_id);

  $packages_by_type = array();
  $units = variable_get('uc_weight_unit', 'lb');
  $result = db_query("SELECT * FROM {uc_packages} WHERE order_id = :id AND sid IS NULL", array(':id' => $order->order_id));
  foreach ($result as $package) {
    $products = array();
    $weight = 0;
    $result2 = db_query("SELECT pp.order_product_id, pp.qty, pp.qty * op.weight AS weight, op.weight_units, op.title, op.model FROM {uc_packaged_products} pp LEFT JOIN {uc_order_products} op ON op.order_product_id = pp.order_product_id WHERE pp.package_id = :id", array(':id' => $package->package_id));
    foreach ($result2 as $product) {
      $units_conversion = uc_weight_conversion($product->weight_units, $units);
      $weight += $product->weight * $units_conversion;
      $products[$product->order_product_id] = $product;
    }
    $package->weight = $weight;
    $package->products = $products;
    $packages_by_type[$package->shipping_type][$package->package_id] = $package;
  }
  $option_methods = array();
  $shipping_types = uc_quote_get_shipping_types();
  $shipping_methods = module_invoke_all('uc_shipping_method');
  $shipping_methods_by_type = array();
  foreach ($shipping_methods as $method) {
    if (isset($method['ship'])) {
      $shipping_methods_by_type[$method['ship']['type']][] = $method;
    }
  }
  $pkgs_exist = FALSE;
  foreach ($packages_by_type as $shipping_type => $packages) {
    $form['shipping_types'][$shipping_type] = array(
      '#type' => 'fieldset',
      '#title' => $shipping_types[$shipping_type]['title'],
    );
    $form['shipping_types'][$shipping_type]['packages'] = array();
    foreach ($packages as $package) {
      $pkgs_exist = TRUE;
      $package_row = array();
      $package_row['checked'] = array('#type' => 'checkbox', '#default_value' => (in_array($package->package_id, $checked_pkgs) ? 1 : 0));
      $package_row['package_id'] = array('#markup' => $package->package_id);
      $product_list = array();
      foreach ($package->products as $product) {
        $product_list[] = $product->qty . ' x ' . check_plain($product->model);
      }
      $package_row['products'] = array('#markup' => '<ul><li>' . implode('</li><li>', $product_list) . '</li></ul>');
      $package_row['weight'] = array('#markup' => uc_weight_format($package->weight, $units));
      $form['shipping_types'][$shipping_type]['packages'][$package->package_id] = $package_row;
    }

    if (isset($shipping_methods_by_type[$shipping_type])) {
      foreach ($shipping_methods_by_type[$shipping_type] as $method) {
        $option_methods += array($method['id'] => $method['title']);
      }
    }
  }
  if ($pkgs_exist) {
    $option_methods = array('all' => t('Ship Manually')) + $option_methods;
    $form['method'] = array(
      '#type' => 'select',
      '#title' => t('Shipping method'),
      '#options' => $option_methods,
      '#default_value' => 'all',
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['ship'] = array(
      '#type' => 'submit',
      '#value' => t('Ship packages'),
    );
  }
  return $form;
}

/**
 * Formats and displays the new shipment form.
 *
 * @see uc_shipping_new_shipment()
 * @ingroup themeable
 */
function theme_uc_shipping_new_shipment($variables) {
  $form = $variables['form'];
  drupal_add_js('misc/tableselect.js');

  $output = '';
  $header = array(array('class' => array('select-all')), t('Package'), t('Products'), t('Weight'));
  foreach (element_children($form['shipping_types']) as $shipping_type) {
    $rows = array();
    foreach (element_children($form['shipping_types'][$shipping_type]['packages']) as $package_id) {
      $row = array();
      $row[] = drupal_render($form['shipping_types'][$shipping_type]['packages'][$package_id]['checked']);
      $row[] = drupal_render($form['shipping_types'][$shipping_type]['packages'][$package_id]['package_id']);
      $row[] = drupal_render($form['shipping_types'][$shipping_type]['packages'][$package_id]['products']);
      $row[] = drupal_render($form['shipping_types'][$shipping_type]['packages'][$package_id]['weight']);
      $rows[] = $row;
    }
    if (count($rows)) {
      $form['shipping_types'][$shipping_type]['packages']['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
      );
    }
  }

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit handler for uc_shipping_new_shipment().
 *
 * Sends package information to the chosen method.
 *
 * @see uc_shipping_new_shipment()
 */
function uc_shipping_new_shipment_submit($form, &$form_state) {
  $packages = array();
  foreach ($form_state['values']['shipping_types'] as $shipping_type) {
    if (is_array($shipping_type['packages'])) {
      foreach ($shipping_type['packages'] as $id => $input) {
        if ($input['checked']) {
          $packages[] = $id;
        }
      }
    }
  }
  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/ship/' . $form_state['values']['method'] . '/' . implode('/', $packages);
}

/**
 * Displays shipment details.
 */
function uc_shipping_shipment_view($order, $shipment) {
  $build = array();

  $origin = uc_order_address($shipment, 'o');
  $destination = uc_order_address($shipment, 'd');

  $build['pickup_address'] = array('#markup' => '<div class="order-pane pos-left"><div class="order-pane-title">' . t('Pickup Address:') . '</div>' . $origin . '</div>');
  $build['delivery_address'] = array('#markup' => '<div class="order-pane pos-left"><div class="order-pane-title">' . t('Delivery Address:') . '</div>' . $destination . '</div>');

  $rows = array();
  $rows[] = array(t('Ship date:'), format_date($shipment->ship_date, 'uc_store'));
  $rows[] = array(t('Expected delivery:'), format_date($shipment->expected_delivery, 'uc_store'));

  $build['schedule'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#attributes' => array('style' => 'width: auto'),
    '#prefix' => '<div class="order-pane abs-left"><div class="order-pane-title">' . t('Schedule:') . '</div>',
    '#suffix' => '</div>',
  );

  $rows = array();
  $rows[] = array(t('Carrier:'), check_plain($shipment->carrier));

  if ($shipment->transaction_id) {
    $rows[] = array(t('Transaction ID:'), check_plain($shipment->transaction_id));
  }

  if ($shipment->tracking_number) {
    $rows[] = array(t('Tracking number:'), check_plain($shipment->tracking_number));
  }

  $methods = module_invoke_all('uc_shipping_method');

  if (isset($methods[$shipment->shipping_method]['quote']['accessorials'][$shipment->accessorials])) {
    $rows[] = array(t('Services:'), $methods[$shipment->shipping_method]['quote']['accessorials'][$shipment->accessorials]);
  }
  else {
    $rows[] = array(t('Services:'), $shipment->accessorials);
  }

  $rows[] = array(t('Cost:'), array('data' => array('#theme' => 'uc_price', '#price' => $shipment->cost)));

  $build['details'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#attributes' => array('style' => 'width:auto'),
    '#prefix' => '<div class="order-pane abs-left"><div class="order-pane-title">' . t('Shipment Details:') . '</div>',
    '#suffix' => '</div>',
  );

  foreach ($shipment->packages as $package) {
    $build['packages'][] = uc_shipping_package_view($package);
  }

  return $build;
}

/**
 * Creates or edits a shipment.
 *
 * @see uc_shipping_shipment_edit_validate()
 * @see uc_shipping_shipment_edit_submit()
 * @ingroup forms
 */
function uc_shipping_shipment_edit($form, &$form_state, $order, $shipment) {
  $form['order_id'] = array('#type' => 'value', '#value' => $order->order_id);
  if (isset($shipment->sid)) {
    $form['sid'] = array('#type' => 'value', '#value' => $shipment->sid);
    $methods = module_invoke_all('uc_shipping_method');
    if (isset($methods[$shipment->shipping_method])) {
      $method = $methods[$shipment->shipping_method];
    }
  }
  $addresses = array();
  $form['packages'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Packages'),
    '#collapsible' => TRUE,
    '#tree'        => TRUE,
  );
  if (isset($shipment->o_street1)) {
    $o_address = new stdClass();
    foreach ($shipment as $field => $value) {
      if (substr($field, 0, 2) == 'o_') {
        $o_address->{substr($field, 2)} = $value;
      }
    }
    $addresses[] = $o_address;
  }
  foreach ($shipment->packages as $id => $package) {
    foreach ($package->addresses as $address) {
      if (!in_array($address, $addresses)) {
        $addresses[] = $address;
      }
    }

    // Create list of products and get a representative product (last one in
    // the loop) to use for some default values
    $product_list = array();
    $declared_value = 0;
    foreach ($package->products as $product) {
      $product_list[]  = $product->qty . ' x ' . check_plain($product->model);
      $declared_value += $product->qty * $product->price;
    }
    $pkg_form = array(
      '#type'  => 'fieldset',
      '#title' => t('Package @id', array('@id' => $id)),
    );
    $pkg_form['products'] = array(
      '#theme' => 'item_list',
      '#items' => $product_list,
    );
    $pkg_form['package_id'] = array(
      '#type'  => 'hidden',
      '#value' => $id,
    );
    $pkg_form['pkg_type'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Package type'),
      '#default_value' => $package->pkg_type,
      '#description'   => t('For example: Box, pallet, tube, envelope, etc.'),
    );
    if (isset($method) && is_array($method['ship']['pkg_types'])) {
      $pkg_form['pkg_type']['#type']        = 'select';
      $pkg_form['pkg_type']['#options']     = $method['ship']['pkg_types'];
      $pkg_form['pkg_type']['#description'] = '';
    }
    $pkg_form['declared_value'] = array(
      '#type'          => 'uc_price',
      '#title'         => t('Declared value'),
      '#default_value' => isset($package->value) ? $package->value : $declared_value,
    );
    $pkg_form['weight'] = array(
      '#type'        => 'container',
      '#attributes'  => array('class' => array('uc-inline-form', 'clearfix')),
      '#description' => t('Weight of the package. Default value is sum of product weights in the package.'),
      '#weight'      => 15,
    );
    $pkg_form['weight']['weight'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Weight'),
      '#default_value' => isset($package->weight) ? $package->weight : 0,
      '#size'          => 10,
    );
    $pkg_form['weight']['units'] = array(
      '#type'          => 'select',
      '#title'         => t('Units'),
      '#options'       => array(
        'lb' => t('Pounds'),
        'kg' => t('Kilograms'),
        'oz' => t('Ounces'),
        'g'  => t('Grams'),
      ),
      '#default_value' => isset($package->weight_units) ?
                          $package->weight_units        :
                          variable_get('uc_weight_unit', 'lb'),
    );
    $pkg_form['dimensions'] = array(
      '#type'        => 'container',
      '#attributes'  => array('class' => array('uc-inline-form', 'clearfix')),
      '#title'       => t('Dimensions'),
      '#description' => t('Physical dimensions of the packaged product.'),
      '#weight'      => 20,
    );
    $pkg_form['dimensions']['length'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Length'),
      '#default_value' => isset($package->length) ? $package->length : 1,
      '#size'          => 8,
    );
    $pkg_form['dimensions']['width'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Width'),
      '#default_value' => isset($package->width) ? $package->width : 1,
      '#size'          => 8,
    );
    $pkg_form['dimensions']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => isset($package->height) ? $package->height : 1,
      '#size' => 8,
    );
    $pkg_form['dimensions']['units'] = array(
      '#type'    => 'select',
      '#title'   => t('Units of measurement'),
      '#options' => array(
        'in' => t('Inches'),
        'ft' => t('Feet'),
        'cm' => t('Centimeters'),
        'mm' => t('Millimeters'),
      ),
      '#default_value' => isset($package->length_units) ?
                          $package->length_units :
                          variable_get('uc_length_unit', 'in'),
    );
    $pkg_form['tracking_number'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Tracking number'),
      '#default_value' => isset($package->tracking_number) ? $package->tracking_number : '',
    );

    $form['packages'][$id] = $pkg_form;
  }

  if (!empty($shipment->d_street1)) {
    foreach ($shipment as $field => $value) {
      if (substr($field, 0, 2) == 'd_') {
        $order->{'delivery_' . substr($field, 2)} = $value;
      }
    }
  }
  $form = uc_shipping_address_form($form, $form_state, $addresses, $order);

  $form['shipment'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Shipment data'),
    '#collapsible' => TRUE,
  );

  // Determine shipping option chosen by the customer.
  $message = '';
  if (isset($order->quote['method'])) {
    // Order has a quote attached.
    $method  = $order->quote['method'];
    $methods = module_invoke_all('uc_shipping_method');
    if (isset($methods[$method])) {
      // Quote is from a currently-active shipping method.
      $services = $methods[$method]['quote']['accessorials'];
      $method   = $services[$order->quote['accessorials']];
    }
    $message = t('Customer selected "@method" as the shipping method and paid @rate', array('@method' => $method, '@rate' => uc_currency_format($order->quote['rate'])));
  }
  else {
    // No quotes for this order.
    $message = t('There are no shipping quotes attached to this order. Customer was not charged for shipping.');
  }

  // Inform administrator of customer's shipping choice.
  $form['shipment']['shipping_choice'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div>',
    '#markup' => $message,
    '#suffix' => '</div>',
  );

  $form['shipment']['shipping_method'] = array(
    '#type'  => 'hidden',
    '#value' => isset($shipment->shipping_method) ? $shipment->shipping_method : 'manual',
  );
  $form['shipment']['carrier'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Carrier'),
    '#default_value' => isset($shipment->carrier) ? $shipment->carrier : '',
  );
  $form['shipment']['accessorials'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shipment options'),
    '#default_value' => isset($shipment->accessorials) ? $shipment->accessorials : '',
    '#description'   => t('Short notes about the shipment, e.g. residential, overnight, etc.'),
  );
  $form['shipment']['transaction_id'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Transaction ID'),
    '#default_value' => isset($shipment->transaction_id) ? $shipment->transaction_id : '',
  );
  $form['shipment']['tracking_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Tracking number'),
    '#default_value' => isset($shipment->tracking_number) ? $shipment->tracking_number : '',
  );

  if (isset($shipment->ship_date)) {
    $ship_date = getdate($shipment->ship_date);
  }
  else {
    $ship_date = getdate();
  }
  if (isset($shipment->expected_delivery)) {
    $exp_delivery = getdate($shipment->expected_delivery);
  }
  else {
    $exp_delivery = getdate();
  }
  $form['shipment']['ship_date'] = array(
    '#type'          => 'date',
    '#title'         => t('Ship date'),
    '#default_value' => array(
      'year'  => $ship_date['year'],
      'month' => $ship_date['mon'],
      'day'   => $ship_date['mday']
    ),
  );
  $form['shipment']['expected_delivery'] = array(
    '#type'          => 'date',
    '#title'         => t('Expected delivery'),
    '#default_value' => array(
      'year'  => $exp_delivery['year'],
      'month' => $exp_delivery['mon'],
      'day'   => $exp_delivery['mday']
    ),
  );
  $form['shipment']['cost'] = array(
    '#type'          => 'uc_price',
    '#title'         => t('Shipping cost'),
    '#default_value' => isset($shipment->cost) ? $shipment->cost : 0,
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save shipment'),
    '#weight' => 10
  );

  return $form;
}

/**
 * Ensures the package dimensions are positive numbers.
 *
 * @see uc_shipping_shipment_edit()
 * @see uc_shipping_shipment_submit()
 */
function uc_shipping_shipment_edit_validate($form, &$form_state) {
  foreach ($form_state['values']['packages'] as $key => $package) {
    foreach (array('length', 'width', 'height') as $property) {
      if (!empty($package['dimensions'][$property]) && (!is_numeric($package['dimensions'][$property]) || $package['dimensions'][$property] < 0)) {
        form_set_error('packages][' . $key . '][dimensions][' . $property, t('@property must be a positive number. No commas and only one decimal point.', array('@property' => drupal_ucfirst($property))));
      }
    }
  }
}

/**
 * Submit handler for uc_shipping_shipment_edit().
 *
 * @see uc_shipping_shipment_edit()
 * @see uc_shipping_shipment_validate()
 */
function uc_shipping_shipment_edit_submit($form, &$form_state) {
  $shipment = new stdClass();
  $shipment->order_id = $form_state['values']['order_id'];
  if (isset($form_state['values']['sid'])) {
    $shipment->sid = $form_state['values']['sid'];
  }
  $shipment->origin = (object) $form_state['values']['pickup_address'];
  $shipment->destination = new stdClass();
  foreach ($form_state['values'] as $key => $value) {
    if (substr($key, 0, 9) == 'delivery_') {
      $field = substr($key, 9);
      $shipment->destination->$field = $value;
    }
  }
  $shipment->packages = array();
  foreach ($form_state['values']['packages'] as $id => $pkg_form) {
    $package = uc_shipping_package_load($id);
    $package->pkg_type = $pkg_form['pkg_type'];
    $package->value = $pkg_form['declared_value'];
    $package->length = $pkg_form['dimensions']['length'];
    $package->width = $pkg_form['dimensions']['width'];
    $package->height = $pkg_form['dimensions']['height'];
    $package->length_units = $pkg_form['dimensions']['units'];
    $package->tracking_number = $pkg_form['tracking_number'];
    $package->qty = 1;
    $shipment->packages[$id] = $package;
  }

  $shipment->shipping_method = $form_state['values']['shipping_method'];
  $shipment->accessorials = $form_state['values']['accessorials'];
  $shipment->carrier = $form_state['values']['carrier'];
  $shipment->transaction_id = $form_state['values']['transaction_id'];
  $shipment->tracking_number = $form_state['values']['tracking_number'];
  $shipment->ship_date = gmmktime(12, 0, 0, $form_state['values']['ship_date']['month'], $form_state['values']['ship_date']['day'], $form_state['values']['ship_date']['year']);
  $shipment->expected_delivery = gmmktime(12, 0, 0, $form_state['values']['expected_delivery']['month'], $form_state['values']['expected_delivery']['day'], $form_state['values']['expected_delivery']['year']);
  $shipment->cost = $form_state['values']['cost'];

  uc_shipping_shipment_save($shipment);

  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/shipments';
}

/**
 * Shows a printer-friendly version of a shipment.
 */
function uc_shipping_shipment_print($order, $shipment, $labels = TRUE) {
  $build = array(
    '#theme' => 'uc_shipping_shipment_print',
    '#order' => $order,
    '#shipment' => $shipment,
    '#labels' => $labels,
  );

  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  print theme('uc_packing_slip_page', array('content' => drupal_render($build)));
  exit();
}

/**
 * Displays the packing slip and shipping labels for printing.
 *
 * @ingroup themeable
 */
function theme_uc_shipping_shipment_print($variables) {
  $order = $variables['order'];
  $shipment = $variables['shipment'];
  $labels = $variables['labels'];

  $output = theme('uc_packing_slip', array('order' => $order, 'shipment' => $shipment));

  if ($labels) {
    foreach ($shipment->packages as $id => $package) {
      if (isset($package->label_image) &&
          file_exists($package->label_image->uri)) {
        // TODO: Find a way to store these magic numbers specifically for UPS.
        list($width, $height) = array(672, 392);
        $output .= '<br class="page-break" />' . "\n";;
        $output .= theme('image', array(
          'path' => $package->label_image->uri,
          'attributes' => array('width' => $width, 'height' => $height),
          'getsize' => FALSE,
        )) . "\n";
      }
    }
  }

  return $output;
}

/**
 * Decides to release packages to be put on another shipment.
 *
 * @see uc_shipping_shipment_delete_confirm_submit()
 * @ingroup forms
 */
function uc_shipping_shipment_delete_confirm($form, &$form_state, $order, $shipment) {
  $form['order_id'] = array('#type' => 'value', '#value' => $order->order_id);
  $form['sid'] = array('#type' => 'value', '#value' => $shipment->sid);
  $output = confirm_form($form, t('Are you sure you want to delete this shipment?'), 'admin/store/orders/' . $order->order_id . '/shipments',
    t('The shipment will be canceled and the packages it contains will be available for reshipment.'), t('Delete'), t('Cancel'));
  return $output;
}

/**
 * Submit handler for uc_shipping_shipment_delete_confirm().
 *
 * @see uc_shipping_shipment_delete_confirm()
 */
function uc_shipping_shipment_delete_confirm_submit($form, &$form_state) {
  $shipment = uc_shipping_shipment_load($form_state['values']['sid']);
  $methods = module_invoke_all('uc_shipping_method');
  if ($shipment->tracking_number &&
      isset($methods[$shipment->shipping_method]['cancel']) &&
      function_exists($methods[$shipment->shipping_method]['cancel'])) {
    $result = call_user_func($methods[$shipment->shipping_method]['cancel'], $shipment->tracking_number);
    if ($result) {
      uc_shipping_shipment_delete($form_state['values']['sid']);
    }
    else {
      drupal_set_message(t('The shipment %tracking could not be canceled with %carrier. To delete it anyway, remove the tracking number and try again.', array('%tracking' => $shipment->tracking_number, '%carrier' => $shipment->carrier)));
    }
  }
  else {
    uc_shipping_shipment_delete($form_state['values']['sid']);
  }

  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/shipments';
}

/**
 * Default method to send packages on a shipment.
 */
function uc_shipping_make_shipment($order) {
  $args = func_get_args();
  if (count($args) > 2) {
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l(t('Shipments'), 'admin/store/orders/' . $order->order_id . '/shipments');
    drupal_set_breadcrumb($breadcrumb);

    $order = array_shift($args);
    $method_id = array_shift($args);
    $package_ids = $args;
    $methods = module_invoke_all('uc_shipping_method');
    if (isset($methods[$method_id])) {
      $method = $methods[$method_id];
      if (isset($method['ship']['file'])) {
        $inc_file = drupal_get_path('module', $method['module']) . '/' . $method['ship']['file'];
        if (is_file($inc_file)) {
          require_once($inc_file);
        }
      }
      return drupal_get_form($method['ship']['callback'], $order, $package_ids);
    }
    else {
      $shipment = new stdClass();
      $shipment->order_id = $order->order_id;
      $shipment->packages = array();
      foreach ($package_ids as $id) {
        $package = uc_shipping_package_load($id);
        $shipment->packages[$id] = $package;
      }
      return drupal_get_form('uc_shipping_shipment_edit', $order, $shipment);
    }
  }
  else {
    drupal_set_message(t('There is no sense in making a shipment with no packages on it, right?'));
    drupal_goto('admin/store/orders/' . $args[0]->order_id . '/shipments/new');
  }
}
