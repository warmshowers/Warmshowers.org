<?php

/**
 * @file
 * Callbacks for the default Ubercart checkout panes plus helper functions.
 *
 * Checkout panes are defined using hook_uc_checkout_pane() and use a callback
 * to handle the different processes involved in completing the checkout form.
 * The default checkout panes are defined in uc_cart_uc_checkout_pane() in
 * uc_cart.module.
 */

/**
 * Displays the cart contents for review during checkout.
 */
function uc_checkout_pane_cart($op, $order, $form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      $contents['cart_review_table'] = array(
        '#theme' => 'uc_cart_review_table',
        '#items' => $order->products,
        '#weight' => variable_get('uc_pane_cart_field_cart_weight', 2),
      );
      return array('contents' => $contents, 'next-button' => FALSE);

    case 'review':
      //$review[] = theme('uc_checkout_pane_cart_review', array('items' => $order->products));
      $review[] = theme('uc_cart_review_table', array('items' => $order->products, 'show_subtotal' => FALSE));
      return $review;
  }
}

/**
 * Gets the user's email address for login.
 */
function uc_checkout_pane_customer($op, $order, $form = NULL, &$form_state = NULL) {
  global $user;

  switch ($op) {
    case 'view':
      if ($user->uid) {
        $email = $user->mail;
        $description = t('Order information will be sent to your account e-mail listed below.');// . '<br />'
        $contents['primary_email'] = array('#type' => 'hidden', '#value' => $email);
        $contents['email_text'] = array(
          '#markup' => '<div>' . t('<b>E-mail address:</b> @email (<a href="!url">edit</a>)', array('@email' => $email, '!url' => url('user/' . $user->uid . '/edit', array('query' => drupal_get_destination())))) . '</div>',
        );
      }
      else {
        $email = $order->primary_email;
        $description = t('Enter a valid email address for this order or <a href="!url">click here</a> to login with an existing account and return to checkout.', array('!url' => url('user/login', array('query' => drupal_get_destination()))));
        $contents['primary_email'] = uc_textfield(t('E-mail address'), $email, TRUE, NULL, 64);
      }

      if (variable_get('uc_cart_email_validation', FALSE) && !$user->uid) {
        $contents['primary_email_confirm'] = uc_textfield(t('Confirm e-mail address'), $email, TRUE, NULL, 64);
      }

      if ($user->uid == 0) {
        $contents['new_account'] = array();

        if (variable_get('uc_cart_new_account_name', FALSE)) {
          $contents['new_account']['name'] = array(
            '#type' => 'textfield',
            '#title' => t('Username'),
            '#default_value' => isset($order->data['new_user']['name']) ? $order->data['new_user']['name'] : '',
            '#maxlength' => 60,
            '#size' => 32,
          );
        }
        if (variable_get('uc_cart_new_account_password', FALSE)) {
          $contents['new_account']['pass'] = array(
            '#type' => 'password',
            '#title' => t('Password'),
            '#maxlength' => 32,
            '#size' => 32,
          );
          $contents['new_account']['pass_confirm'] = array(
            '#type' => 'password',
            '#title' => t('Confirm password'),
            '#description' => t('Passwords must match to proceed.'),
            '#maxlength' => 32,
            '#size' => 32,
          );
        }

        if (!empty($contents['new_account'])) {
          $array = array(
            '#type' => 'fieldset',
            '#title' => t('New account details'),
            '#description' => variable_get('uc_cart_new_account_details', t('<b>Optional.</b> New customers may supply custom account details.<br />We will create these for you if no values are entered.')),
            '#collapsible' => FALSE,
          );
          $contents['new_account'] = array_merge($array, $contents['new_account']);
        }
      }

      return array('description' => $description, 'contents' => $contents);

    case 'process':
      $pane = $form_state['values']['panes']['customer'];

      if (!empty($pane['primary_email']) && !valid_email_address($pane['primary_email'])) {
        form_set_error('panes][customer][primary_email', t('You must enter a valid e-mail address.'));
      }

      $order->primary_email = $pane['primary_email'];

      if (variable_get('uc_cart_email_validation', FALSE) && !$user->uid &&
          $pane['primary_email'] !== $pane['primary_email_confirm']) {
        form_set_error('panes][customer][primary_email_confirm', t('The e-mail address did not match.'));
      }

      // Invalidate if an account already exists for this e-mail address, and the user is not logged into that account
      if (!variable_get('uc_cart_mail_existing', TRUE) && $user->uid == 0 && !empty($pane['primary_email'])) {
        if (db_query("SELECT uid FROM {users} WHERE mail LIKE :mail", array(':mail' => $pane['primary_email']))->fetchField() > 0) {
          form_set_error('panes][customer][primary_email', t('An account already exists for your e-mail address. You will either need to login with this e-mail address or use a different e-mail address.'));
        }
      }

      // If new users can specify names or passwords then...
      if ((variable_get('uc_cart_new_account_name', FALSE) ||
          variable_get('uc_cart_new_account_password', FALSE)) &&
          $user->uid == 0) {
        // Skip if an account already exists for this e-mail address.
        if (variable_get('uc_cart_mail_existing', TRUE) && db_query("SELECT uid FROM {users} WHERE mail LIKE :mail", array(':mail' => $pane['primary_email']))->fetchField() > 0) {
          drupal_set_message(t('An account already exists for your e-mail address. The new account details you entered will be disregarded.'));
        }
        else {
          // Validate the username.
          if (variable_get('uc_cart_new_account_name', FALSE) && !empty($pane['new_account']['name'])) {
            $message = user_validate_name($pane['new_account']['name']);
            if (!empty($message)) {
              form_set_error('panes][customer][new_account][name', $message);
            }
            elseif (db_query("SELECT uid FROM {users} WHERE name LIKE :name", array(':name' => $pane['new_account']['name']))->fetchField()) {
              form_set_error('panes][customer][new_account][name', t('The username %name is already taken. Please enter a different name or leave the field blank for your username to be your e-mail address.', array('%name' => $pane['new_account']['name'])));
            }
            else {
              $order->data['new_user']['name'] = $pane['new_account']['name'];
            }
          }
          // Validate the password.
          if (variable_get('uc_cart_new_account_password', FALSE)) {
            if (strcmp($pane['new_account']['pass'], $pane['new_account']['pass_confirm'])) {
              form_set_error('panes][customer][new_account][pass_confirm', t('The passwords you entered did not match. Please try again.'));
            }
            if (!empty($pane['new_account']['pass'])) {
              require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
              $order->data['new_user']['hash'] = user_hash_password(trim($pane['new_account']['pass']));
            }
          }
        }
      }

      if ($user->uid) {
        $order->uid = $user->uid;
      }
      return TRUE;

    case 'review':
      $review[] = array('title' => t('E-mail'), 'data' => check_plain($order->primary_email));
      return $review;

    case 'settings':
      $form['uc_cart_new_account_details'] = array(
        '#type' => 'textarea',
        '#title' => t('New account details help message'),
        '#description' => t('Enter the help message displayed in the new account details fieldset when shown.'),
        '#default_value' => variable_get('uc_cart_new_account_details', t('<b>Optional.</b> New customers may supply custom account details.<br />We will create these for you if no values are entered.')),
      );

      return $form;
  }
}

/**
 * Gets the delivery information.
 */
function uc_checkout_pane_delivery($op, $order, $form = NULL, &$form_state = NULL) {
  $description = t('Enter your delivery address and information here.');
  $copy = t('My delivery information is the same as my billing information.');
  return uc_checkout_pane_address('delivery', $op, $order, $form_state, $description, $copy);
}

/**
 * Gets the billing information.
 */
function uc_checkout_pane_billing($op, $order, $form = NULL, &$form_state = NULL) {
  $description = t('Enter your billing address and information here.');
  $copy = t('My billing information is the same as my delivery information.');
  return uc_checkout_pane_address('billing', $op, $order, $form_state, $description, $copy);
}

/**
 * Generic address pane handler.
 */
function uc_checkout_pane_address($pane, $op, $order, &$form_state, $description, $copy) {
  global $user;

  // Source pane for "copy address" checkbox.
  static $source;
  if (!isset($source)) {
    $source = $pane;
  }

  switch ($op) {
    case 'view':
      if ($source != $pane) {
        $contents['copy_address'] = array(
          '#type' => 'checkbox',
          '#title' => $copy,
          '#default_value' => variable_get('uc_cart_default_same_address', FALSE),
          '#ajax' => array(
            'callback' => 'uc_checkout_pane_address_render',
            'wrapper' => $pane . '-address-pane',
            'progress' => array(
              'type' => 'throbber',
            ),
          ),
        );
      }

      if ($user->uid && $addresses = uc_select_addresses($user->uid, $pane)) {
        $contents['select_address'] = array(
          '#type' => 'select',
          '#title' => t('Saved addresses'),
          '#options' => $addresses['#options'],
          '#ajax' => array(
            'callback' => 'uc_checkout_pane_address_render',
            'wrapper' => $pane . '-address-pane',
            'progress' => array(
              'type' => 'throbber',
            ),
          ),
          '#states' => array(
            'invisible' => array(
              'input[name="panes[' . $pane . '][copy_address]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }

      $contents['address'] = array(
        '#type' => 'uc_address',
        '#default_value' => $order,
        '#key_prefix' => $pane,
        '#prefix' => '<div id="' . $pane . '-address-pane">',
        '#suffix' => '</div>',
      );

      if (isset($form_state['values']['panes'][$pane]['copy_address'])) {
        $contents['address']['#hidden'] = !empty($form_state['values']['panes'][$pane]['copy_address']);
      }
      elseif (isset($contents['copy_address'])) {
        $contents['address']['#hidden'] = variable_get('uc_cart_default_same_address', FALSE);
      }

      // If this was an Ajax request, update form input values for the
      // copy and select address features.
      if (isset($form_state['triggering_element'])) {
        $element = &$form_state['triggering_element'];

        if ($element['#name'] == "panes[$pane][copy_address]") {
          $address = &$form_state['values']['panes'][$source];
          foreach ($address as $field => $value) {
            if (substr($field, 0, strlen($source)) == $source) {
              $field = str_replace($source, $pane, $field);
              $form_state['input']['panes'][$pane][$field] = $value;
            }
          }
        }

        if ($element['#name'] == "panes[$pane][select_address]") {
          $address = $addresses[$element['#value']];
          foreach ($address as $field => $value) {
            $form_state['input']['panes'][$pane][$pane . '_' . $field] = $value;
          }
        }

        // Forget any previous Ajax submissions, as we send new default values.
        unset($form_state['uc_address']);
      }

      return array('description' => $description, 'contents' => $contents);

    case 'process':
      $panes = &$form_state['values']['panes'];
      foreach ($panes[$pane] as $field => $value) {
        if (substr($field, 0, strlen($pane)) == $pane) {
          if (!empty($panes[$pane]['copy_address'])) {
            $value = $panes[$source][str_replace($pane, $source, $field)];
          }

          $order->$field = $value;
        }
      }
      if (isset($panes[$pane]['select_address']) && $panes[$pane]['select_address'] >= 0) {
        $addresses = uc_select_addresses($user->uid, $pane);
        $address = $addresses[$panes[$pane]['select_address']];
        foreach ($address as $field => $value) {
          $order->{$pane . '_' . $field} = $value;
        }
      }
      return TRUE;

    case 'review':
      $review[] = array('title' => t('Address'), 'data' => uc_order_address($order, $pane, FALSE));
      if (uc_address_field_enabled('phone') && !empty($order->{$pane . '_phone'})) {
        $review[] = array('title' => t('Phone'), 'data' => check_plain($order->{$pane . '_phone'}));
      }
      return $review;
  }
}

/**
 * Ajax callback to re-render the full address element.
 */
function uc_checkout_pane_address_render($form, &$form_state) {
  $element = &$form;
  foreach (array_slice($form_state['triggering_element']['#array_parents'], 0, -1) as $field) {
    $element = &$element[$field];
  }
  return $element['address'];
}

/**
 * Allows a customer to make comments on the order.
 */
function uc_checkout_pane_comments($op, $order, $form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      $description = t('Use this area for special instructions or questions regarding your order.');

      if (!empty($order->order_id)) {
        $default = db_query("SELECT message FROM {uc_order_comments} WHERE order_id = :id", array(':id' => $order->order_id))->fetchField();
      }
      else {
        $default = NULL;
      }
      $contents['comments'] = array(
        '#type' => 'textarea',
        '#title' => t('Order comments'),
        '#default_value' => $default,
      );

      return array('description' => $description, 'contents' => $contents);

    case 'process':
      db_delete('uc_order_comments')
        ->condition('order_id', $order->order_id)
        ->execute();
      if (strlen($form_state['values']['panes']['comments']['comments']) > 0) {
        uc_order_comment_save($order->order_id, 0, $form_state['values']['panes']['comments']['comments'], 'order', uc_order_state_default('post_checkout'), TRUE);
      }
      return TRUE;

    case 'review':
      $review = NULL;
      $result = db_query("SELECT message FROM {uc_order_comments} WHERE order_id = :id", array(':id' => $order->order_id));
      if ($comment = $result->fetchObject()) {
        $review[] = array('title' => t('Comment'), 'data' => check_plain($comment->message));
      }
      return $review;
  }
}

/**
 * Finds the collapsible pane displayed above the pane with an ID of $pane_id.
 */
function _uc_cart_checkout_prev_pane($panes, $pane_id = NULL) {
  if (is_null($pane_id)) {
    return FALSE;
  }

  $prev = FALSE;
  foreach ($panes as $target) {
    if ($target['id'] == $pane_id) {
      return $prev;
    }
    if ($target['collapsible'] && $target['enabled']) {
      $prev = $target['id'];
    }
  }

  return FALSE;
}

/**
 * Finds the pane that displays below the pane with an ID of $pane_id.
 */
function _uc_cart_checkout_next_pane($panes, $pane_id = NULL) {
  if (is_null($pane_id)) {
    return FALSE;
  }

  $next = FALSE;
  foreach ($panes as $target) {
    if ($next) {
      if ($target['collapsible'] && $target['enabled']) {
        return $target['id'];
      }
    }
    if ($target['id'] == $pane_id) {
      $next = TRUE;
    }
  }

  return FALSE;
}

/**
 * Builds a list of checkout panes defined in the enabled modules.
 */
function _uc_checkout_pane_list($action = NULL) {
  static $panes = array();

  if (count($panes) > 0 && $action !== 'rebuild') {
    return $panes;
  }

  foreach (module_invoke_all('uc_checkout_pane') as $id => $pane) {
    // Preserve backward compatibility for panes with no key specified.
    if (is_numeric($id)) {
      $id = $pane['id'];
    }

    // Set defaults.
    $pane += array(
      'id' => $id,
      'enabled' => TRUE,
      'weight' => 0,
      'review' => TRUE,
      'process' => TRUE,
      'collapsible' => TRUE,
    );

    $pane['enabled'] = variable_get('uc_pane_' . $id . '_enabled', $pane['enabled']);
    $pane['weight'] = variable_get('uc_pane_' . $id . '_weight', $pane['weight']);

    $panes[$id] = $pane;
  }

  // Allow other modules to alter the defaults.
  drupal_alter('uc_checkout_pane', $panes);

  uasort($panes, 'uc_weight_sort');

  return $panes;
}

/**
 * Returns data from a checkout pane by pane ID and the array key.
 */
function _uc_checkout_pane_data($pane_id, $key) {
  $panes = _uc_checkout_pane_list();
  return $panes[$pane_id][$key];
}

/**
 * Formats the cart contents table on the checkout page.
 *
 * @param $variables
 *   An associative array containing:
 *   - show_subtotal: TRUE or FALSE indicating if you want a subtotal row
 *     displayed in the table.
 *   - items: An associative array of cart item information containing:
 *     - qty: Quantity in cart.
 *     - title: Item title.
 *     - price: Item price.
 *     - desc: Item description.
 *
 * @return
 *   The HTML output for the cart review table.
 *
 * @ingroup themeable
 */
function theme_uc_cart_review_table($variables) {
  $items = $variables['items'];
  $show_subtotal = $variables['show_subtotal'];

  $subtotal = 0;

  // Set up table header.
  $header = array(
    array('data' => theme('uc_qty_label'), 'class' => array('qty')),
    array('data' => t('Products'), 'class' => array('products')),
    array('data' => t('Price'), 'class' => array('price')),
  );

  // Set up table rows.
  $display_items = uc_order_product_view_multiple($items);
  if (!empty($display_items['uc_order_product'])) {
    foreach (element_children($display_items['uc_order_product']) as $key) {
      $display_item = $display_items['uc_order_product'][$key];
      $subtotal += $display_item['total']['#price'];
      $rows[] = array(
        array('data' => $display_item['qty'], 'class' => array('qty')),
        array('data' => $display_item['product'], 'class' => array('products')),
        array('data' => $display_item['total'], 'class' => array('price')),
      );
    }
  }

  // Add the subtotal as the final row.
  if ($show_subtotal) {
    $rows[] = array(
      'data' => array(
        // One cell
        array(
          'data' => array(
            '#theme' => 'uc_price',
            '#prefix' => '<span id="subtotal-title">' . t('Subtotal:') . '</span> ',
            '#price' => $subtotal,
          ),
          // Cell attributes
          'colspan' => 3,
          'class' => array('subtotal'),
        ),
      ),
      // Row attributes
      'class' => array('subtotal'),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('cart-review'))));
}

/**
 * Themes cart items on the checkout review order page.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An associative array of cart item information containing:
 *     - qty: Quantity in cart.
 *     - title: Item title.
 *     - price: Item price.
 *     - desc: Item description.
 *
 * @return
 *   A string of HTML for the page contents.
 *
 * @ingroup themeable
 */
function theme_uc_checkout_pane_cart_review($variables) {
  $rows = array();
  $items = uc_order_product_view_multiple($variables['items']);

  foreach (element_children($items['uc_order_product']) as $key) {
    $item = $items['uc_order_product'][$key];
    $rows[] = array(
      array('data' => $item['qty'], 'class' => array('qty')),
      array('data' => $item['product'], 'class' => array('products')),
      array('data' => $item['total'], 'class' => array('price')),
    );
  }

  return theme('table', array('rows' => $rows, 'attributes' => array('class' => array('cart-review'))));;
}
