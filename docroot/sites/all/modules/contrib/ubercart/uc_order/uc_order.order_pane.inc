<?php

/**
 * @file
 * This file contains the callbacks for the default order panes supplied with
 * Ubercart and their corresponding helper functions.
 *
 * Order panes are defined using hook_order_pane() and use a callback to handle
 * the different processes involved in order viewing/editing.  The default order
 * panes are defined in uc_order_order_pane() in uc_order.module.
 */

/**
 * Handles the "Ship to" order pane.
 */
function uc_order_pane_ship_to($op, $arg1) {
  switch ($op) {
    case 'customer':
      if (!uc_order_is_shippable($arg1)) {
        return;
      }
    case 'view':
      $output = uc_order_address($arg1, 'delivery') .'<br />'. check_plain($arg1->delivery_phone);
      return $output;

    case 'edit-form':
      $form['ship_to'] = array(
        '#type' => 'fieldset',
        '#title' => t("Modify 'Ship to' information"),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      if (uc_address_field_enabled('first_name')) {
        $form['ship_to']['delivery_first_name'] = uc_textfield(uc_get_field_name('first_name'), $arg1->delivery_first_name, FALSE);
      }
      if (uc_address_field_enabled('last_name')) {
        $form['ship_to']['delivery_last_name'] = uc_textfield(uc_get_field_name('last_name'), $arg1->delivery_last_name, FALSE);
      }
      if (uc_address_field_enabled('phone')) {
        $form['ship_to']['delivery_phone'] = uc_textfield(uc_get_field_name('phone'), $arg1->delivery_phone, FALSE, NULL, 32, 16);
      }
      if (uc_address_field_enabled('company')) {
        $form['ship_to']['delivery_company'] = uc_textfield(uc_get_field_name('company'), $arg1->delivery_company, FALSE, NULL, 64);
      }
      if (uc_address_field_enabled('street1')) {
        $form['ship_to']['delivery_street1'] = uc_textfield(uc_get_field_name('street1'), $arg1->delivery_street1, FALSE, NULL, 64);
      }
      if (uc_address_field_enabled('street2')) {
        $form['ship_to']['delivery_street2'] = uc_textfield(uc_get_field_name('street2'), $arg1->delivery_street2, FALSE, NULL, 64);
      }
      if (uc_address_field_enabled('city')) {
        $form['ship_to']['delivery_city'] = uc_textfield(uc_get_field_name('city'), $arg1->delivery_city, FALSE);
      }
      if (uc_address_field_enabled('country')) {
        $form['ship_to']['delivery_country'] = uc_country_select(uc_get_field_name('country'), $arg1->delivery_country);
      }
      if (uc_address_field_enabled('zone')) {
        if (isset($_POST['delivery_country'])) {
          $country_id = intval($_POST['delivery_country']);
        }
        else {
          $country_id = $arg1->delivery_country;
        }
        $form['ship_to']['delivery_zone'] = uc_zone_select(uc_get_field_name('zone'), $arg1->delivery_zone, NULL, $country_id);
      }
      if (uc_address_field_enabled('postal_code')) {
        $form['ship_to']['delivery_postal_code'] = uc_textfield(uc_get_field_name('postal_code'), $arg1->delivery_postal_code, FALSE, NULL, 10, 10);
      }
      return $form;

    case 'edit-title':
      $output = ' <img src="'. base_path() . drupal_get_path('module', 'uc_store')
                .'/images/address_book.gif" alt="'. t('Select from address book.') .'" '
                .'title="'. t('Select from address book.') .'" onclick="load_address_select('. $arg1['order_uid']['#value'] .', \'#delivery_address_select\', \'delivery\');" '
                .'style="position: relative; top: 2px; cursor: pointer;" />';

      $output .= ' <img src="'. base_path() . drupal_get_path('module', 'uc_store')
                 .'/images/copy.gif" alt="'. t('Copy billing information.') .'" title="'
                 . t('Copy billing information.') .'" onclick="uc_order_copy_billing_to_shipping();" '
                 .'style="position: relative; top: 2px; cursor: pointer;" />';

      return $output;

    case 'edit-theme':
      $output = '<div id="delivery_address_select"></div><table class="order-edit-table">';
      foreach (element_children($arg1['ship_to']) as $field) {
        $title = $arg1['ship_to'][$field]['#title'];
        $arg1['ship_to'][$field]['#title'] = NULL;
        $output .= '<tr><td class="oet-label">'. $title .':</td><td>'
                 . drupal_render($arg1['ship_to'][$field]) .'</td></tr>';
      }
      $output .= '</table>';
      return $output;

    case 'edit-process':
      $changes = array();
      foreach ($arg1 as $key => $value) {
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
function uc_order_pane_bill_to($op, $arg1) {
  switch ($op) {
    case 'view':
    case 'customer':
      $output = uc_order_address($arg1, 'billing') .'<br />'. check_plain($arg1->billing_phone);
      return $output;

    case 'edit-form':
      $form['bill_to'] = array(
        '#type' => 'fieldset',
        '#title' => t("Modify 'Bill to' information"),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      if (uc_address_field_enabled('first_name')) {
        $form['bill_to']['billing_first_name'] = uc_textfield(uc_get_field_name('first_name'), $arg1->billing_first_name, FALSE);
      }
      if (uc_address_field_enabled('last_name')) {
        $form['bill_to']['billing_last_name'] = uc_textfield(uc_get_field_name('last_name'), $arg1->billing_last_name, FALSE);
      }
      if (uc_address_field_enabled('phone')) {
        $form['bill_to']['billing_phone'] = uc_textfield(uc_get_field_name('phone'), $arg1->billing_phone, FALSE, NULL, 32, 16);
      }
      if (uc_address_field_enabled('company')) {
        $form['bill_to']['billing_company'] = uc_textfield(uc_get_field_name('company'), $arg1->billing_company, FALSE, NULL, 64);
      }
      if (uc_address_field_enabled('street1')) {
        $form['bill_to']['billing_street1'] = uc_textfield(uc_get_field_name('street1'), $arg1->billing_street1, FALSE, NULL, 64);
      }
      if (uc_address_field_enabled('street2')) {
        $form['bill_to']['billing_street2'] = uc_textfield(uc_get_field_name('street2'), $arg1->billing_street2, FALSE, NULL, 64);
      }
      if (uc_address_field_enabled('city')) {
        $form['bill_to']['billing_city'] = uc_textfield(uc_get_field_name('city'), $arg1->billing_city, FALSE);
      }
      if (uc_address_field_enabled('country')) {
        $form['bill_to']['billing_country'] = uc_country_select(uc_get_field_name('country'), $arg1->billing_country);
      }
      if (uc_address_field_enabled('zone')) {
        if (isset($_POST['billing_country'])) {
          $country_id = intval($_POST['billing_country']);
        }
        else {
          $country_id = $arg1->billing_country;
        }
        $form['bill_to']['billing_zone'] = uc_zone_select(uc_get_field_name('zone'), $arg1->billing_zone, NULL, $country_id);
      }
      if (uc_address_field_enabled('postal_code')) {
        $form['bill_to']['billing_postal_code'] = uc_textfield(uc_get_field_name('postal_code'), $arg1->billing_postal_code, FALSE, NULL, 10, 10);
      }
      return $form;

    case 'edit-title':
      $output = ' <img src="'. base_path() . drupal_get_path('module', 'uc_store')
                .'/images/address_book.gif" alt="'. t('Select from address book.') .'" '
                .'title="'. t('Select from address book.') .'" onclick="load_address_select('. $arg1['order_uid']['#value'] .', \'#billing_address_select\', \'billing\');" '
                .'style="position: relative; top: 2px; cursor: pointer;" />';
      $output .= ' <img src="'. base_path() . drupal_get_path('module', 'uc_store')
               .'/images/copy.gif" alt="'. t('Copy shipping information.') .'" title="'
               . t('Copy shipping information.') .'" onclick="uc_order_copy_shipping_to_billing();" '
               .'style="position: relative; top: 2px; cursor: pointer;" />';
      return $output;

    case 'edit-theme':
      $output = '<div id="billing_address_select"></div><table class="order-edit-table">';
      foreach (element_children($arg1['bill_to']) as $field) {
        $title = $arg1['bill_to'][$field]['#title'];
        $arg1['bill_to'][$field]['#title'] = NULL;
        $output .= '<tr><td class="oet-label">'. $title .':</td><td>'
                 . drupal_render($arg1['bill_to'][$field]) .'</td></tr>';
      }
      $output .= '</table>';
      return $output;

    case 'edit-process':
      $changes = array();
      foreach ($arg1 as $key => $value) {
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
function uc_order_pane_customer($op, $arg1) {
  switch ($op) {
    case 'view':
      $output = t('Customer number: !user_link', array('!user_link' => $arg1->uid ? l($arg1->uid, 'user/'. $arg1->uid) : '0'))
               .'<br />'. t('Primary e-mail:') .'<br />'
              . check_plain($arg1->primary_email);
      return $output;

    case 'edit-form':
      $form['customer'] = array(
        '#type' => 'fieldset',
        '#title' => t("Modify 'Customer info'"),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE
      );
      $form['customer']['uid'] = array(
        '#type' => 'hidden',
        '#default_value' => $arg1->uid,
      );
      $form['customer']['text']['uid_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Customer number'),
        '#default_value' => $arg1->uid,
        '#maxlength' => 10,
        '#size' => 10,
        '#disabled' => TRUE,
      );
      $form['customer']['primary_email'] = array(
        '#type' => 'hidden',
        '#default_value' => $arg1->primary_email,
      );
      $form['customer']['text']['primary_email_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Primary e-mail'),
        '#default_value' => $arg1->primary_email,
        '#maxlength' => 64,
        '#size' => 32,
        '#disabled' => TRUE,
      );
      return $form;

    case 'edit-title':
      $output = ' <img src="'. base_path() . drupal_get_path('module', 'uc_store')
                .'/images/order_view.gif" alt="'. t('Search for an existing customer.') .'" '
                .'title="'. t('Search for an existing customer.') .'" onclick="load_customer_search();" '
                .'style="position: relative; top: 2px; cursor: pointer;" />';
      $output .= ' <img src="'. base_path() . drupal_get_path('module', 'uc_store')
                .'/images/menu_customers_small.gif" alt="'. t('Create a new customer.') .'" '
                .'title="'. t('Create a new customer.') .'" onclick="load_new_customer_form();" '
                .'style="position: relative; top: 2px; cursor: pointer;" />';
      return $output;

    case 'edit-theme':
      $output = '<div id="customer-select"></div><table class="order-edit-table">';
      foreach (element_children($arg1['customer']['text']) as $field) {
        $title = $arg1['customer']['text'][$field]['#title'];
        $arg1['customer']['text'][$field]['#title'] = NULL;
        $output .= '<tr><td class="oet-label">'. $title .':</td><td>'
                 . drupal_render($arg1['customer']['text'][$field]) .'</td></tr>';
      }
      $output .= '</table>'. drupal_render($arg1['customer']['primary_email'])
               . drupal_render($arg1['customer']['uid']);
      return $output;

    case 'edit-process':
      $changes = array();
      $changes['uid'] = $arg1['uid'];
      $changes['primary_email'] = $arg1['primary_email'];
      return $changes;
  }
}

/**
 * Handles the "Products" order pane.
 */
function uc_order_pane_products($op, $arg1) {
  switch ($op) {
    case 'view':
      return tapir_get_table('op_products_view_table', $arg1);

    case 'customer':
      return tapir_get_table('op_products_customer_table', $arg1);

    case 'edit-theme':
      drupal_add_js('uc_order_load_product_edit_div('. $arg1['order_id']['#value'] .');', 'inline');

      $output = '<div id="products-selector"></div>'
               .'<div id="products-container">'. t('Loading product information... (<em>If nothing happens, make sure you have Javascript enabled.</em>)') .'</div>';
      return $output;

    case 'edit-title':
      $settings = array(
        'div' => '#products-selector',
        'class' => 'product-select-box',
        'vid' => variable_get('uc_catalog_vid', 0),
        'filter' => implode(',', uc_product_types()),
        'search' => 'true',
        'nids' => 'true',
        'close' => 'true',
        'nodesg' => 'product',
        'nodepl' => 'products',
        'select' => 'add_product_form();',
      );

      $output = '<input type="button" value="'. t('Add product')
               .'" onclick="load_product_select('. $arg1['order_id']['#value']
               .', false);" id="add-product-button" />';

      $output .= '<input type="button" value="'. t('Add blank line')
                .'" onclick="add_blank_line_button('. $arg1['order_id']['#value']
                .');" />';
      $output .= ' <span id="product-div-throbber"></span>';
      return $output;
  }
}

/**
 * Handles the "Line Items" order pane.
 */
function uc_order_pane_line_items($op, $order) {
  switch ($op) {
    case 'view':
    case 'customer':
      $line_items = $order->line_items;
      $items = _line_item_list();
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

      $context = array(
        'revision' => 'themed',
        'type' => 'line_item',
        'subject' => array(
          'order' => $order,
        ),
      );
      $output = '<table class="line-item-table">';
      foreach ($line_items as $item) {
        $context['subject']['line_item'] = $item;
        $output .= '<tr><td class="li-title">' .
                   check_plain($item['title']) .
                   ':</td><td class="li-amount">' .
                   uc_price($item['amount'], $context) .
                   '</td></tr>';
      }
      $output .= '</table>';
      return $output;

    case 'show-title':
      return FALSE;

    case 'edit-form':
      $options = array();
      $items = _line_item_list();
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

      $form['add_line_item'] = array(
        '#type' => 'fieldset',
        '#title' => t('Add line item'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['add_line_item']['li_type_select'] = array(
        '#type' => 'select',
        '#title' => t('Select a type'),
        '#options' => $options,
      );
      $form['add_line_item']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add line'),
        '#attributes' => array('class' => 'save-button'),
        '#disabled' => TRUE,
      );
      $form['li_delete_id'] = array(
        '#type' => 'hidden',
      );
      $form['line_items'] = array(
        '#type' => 'fieldset',
        '#title' => t("Modify 'line items'"),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
      );

      $context = array(
        'revision' => 'themed',
        'type' => 'line_item',
        'subject' => array(
          'order' => $order,
        ),
      );
      foreach ($line_items as $item) {
        $form['line_items'][$item['line_item_id']]['li_id'] = array(
          '#type' => 'hidden',
          '#value' => $item['line_item_id'],
        );
        if (isset($item['type']) && _line_item_data($item['type'], 'stored') == TRUE) {
          $form['line_items'][$item['line_item_id']]['title'] = array(
            '#type' => 'textfield',
            '#title' => t('Title'),
            '#default_value' => $item['title'],
            '#size' => 40,
            '#maxlength' => 128,
            '#prefix' => '<img id="li-' . $item['line_item_id'] . '" src="'
                       . base_path() . drupal_get_path('module', 'uc_store')
                       . '/images/error.gif" alt="' . t('Remove line item.')
                       . '" title="' . t('Remove line item.') . '" style="'
                       . 'cursor: pointer; position: relative; top: 3px;" '
                       . 'onclick="confirm_line_item_delete(\''
                       . addslashes(t('Remove this line item?')) . '\', this.id);" />',
          );
          $form['line_items'][$item['line_item_id']]['amount'] = array(
            '#type' => 'textfield',
            '#title' => t('Amount'),
            '#default_value' => uc_store_format_price_field_value($item['amount']),
            '#size' => 6,
            '#maxlength' => 13,
            '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
            '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
          );
        }
        else {
          $context['subject']['line_item'] = $item;
          $form['line_items'][$item['line_item_id']]['title'] = array(
            '#value' => check_plain($item['title']),
          );
          $form['line_items'][$item['line_item_id']]['amount'] = array(
            '#value' => uc_price($item['amount'], $context),
          );
        }
      }
      return $form;

    case 'edit-theme':
      $order['add_line_item']['li_type_select']['#title'] = '';
      $output = '<table class="full-width"><tr><td>';
      $output .= '<table><tr><td colspan="2"><b>' . t('Add a line item')
                . ': </b></td></tr><tr><td>' . drupal_render($order['add_line_item']['li_type_select'])
                . ' </td><td>' . drupal_render($order['add_line_item']['submit'])
                . '</td></tr></table>';
      $output .= '</td><td>' . drupal_render($order['li_delete_id'])
                . '<table class="line-item-table">';
      foreach (element_children($order['line_items']) as $field) {
        $order['line_items'][$field]['title']['#title'] = '';
        $order['line_items'][$field]['amount']['#title'] = '';
        $output .= '<tr><td class="li-title">'
                 . drupal_render($order['line_items'][$field]['li_id'])
                 . drupal_render($order['line_items'][$field]['title'])
                 . ':</td><td class="li-amount">'
                 . drupal_render($order['line_items'][$field]['amount'])
                 . '</td></tr>';
      }
      $output .= '</table></td></tr></table>';
      return $output;

    case 'edit-process':
      if (is_array($order['line_items'])) {
        foreach ($order['line_items'] as $line) {
          if (is_numeric($line['li_id']) && intval($line['li_id']) > 0) {
            uc_order_update_line_item($line['li_id'], $line['title'], $line['amount']);
          }
        }
      }
      if (intval($order['li_delete_id']) > 0) {
        uc_order_delete_line_item($order['li_delete_id']);
        drupal_set_message(t('Line item removed.'));
      }
      return;

    case 'edit-ops':
      return array(t('Add line'));

    case t('Add line'):
      drupal_goto('admin/store/orders/' . $order['order_id']
                 . '/add_line_item/' . $order['li_type_select']);
  }
}

/**
 * Handles the "Order Comments" order pane.
 */
function uc_order_pane_order_comments($op, $order) {
  switch ($op) {
    case 'view':
      $comments = uc_order_comments_load($order->order_id);
      return tapir_get_table('op_order_comments_view_table', $comments);

    case 'customer':
      $comments = uc_order_comments_load($order->order_id);
      $header = array(t('Date'), t('Status'), t('Message'));
      $rows[] = array(
        array('data' => format_date($order->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y')), 'class' => 'date'),
        array('data' => '-', 'class' => 'status'),
        array('data' => t('Order created.'), 'class' => 'message'),
      );
      if (count($comments) > 0) {
        foreach ($comments as $comment) {
          $rows[] = array(
            array('data' => format_date($comment->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y')), 'class' => 'date'),
            array('data' => $comment->title, 'class' => 'status'),
            array('data' => check_plain($comment->message), 'class' => 'message'),
          );
        }
      }
      $output = theme('table', $header, $rows, array('class' => 'uc-order-comments'));

      return $output;
  }
}

/**
 * Handles the "Admin Comments" order pane.
 */
function uc_order_pane_admin_comments($op, $arg1) {
  switch ($op) {
    case 'view':
      $comments = uc_order_comments_load($arg1->order_id, TRUE);
      return tapir_get_table('op_admin_comments_view_table', $comments);

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
        '#wysiwyg' => FALSE,
      );
      return $form;

    case 'edit-theme':
      $comments = uc_order_comments_load($arg1['order_id']['#value'], TRUE);
      if (is_array($comments) && count($comments) > 0) {
        foreach ($comments as $comment) {
          $items[] = '['. uc_get_initials($comment->uid) .'] '. filter_xss_admin($comment->message);
        }
      }
      else {
        $items = array(t('No admin comments have been entered for this order.'));
      }
      $output = theme('item_list', $items) . drupal_render($arg1['admin_comment_field']);
      return $output;

    case 'edit-process':
      if (!is_null($arg1['admin_comment']) && strlen(trim($arg1['admin_comment'])) > 0) {
        global $user;
        uc_order_comment_save($arg1['order_id'], $user->uid, $arg1['admin_comment']);
      }
      return;
  }
}

/**
 * Handles the "Update" order pane.
 */
function uc_order_pane_update($op, $arg1) {
  switch ($op) {
    case 'view':
      return drupal_get_form('uc_order_view_update_form', $arg1);
  }
}

/**
 * Form to save order comments and update the order status.
 *
 * @see theme_uc_order_view_update_controls()
 * @see uc_order_view_update_form_submit()
 * @ingroup forms
 */
function uc_order_view_update_form($form_state, $order) {
  $form['order_comment_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add an order comment'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['order_comment_field']['order_comment'] = array(
    '#type' => 'textarea',
    '#description' => t('Order comments are used primarily to communicate with the customer.'),
    '#wysiwyg' => FALSE,
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
    '#wysiwyg' => FALSE,
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
    '#theme' => 'uc_order_view_update_controls',
    '#weight' => 10,
  );

  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['controls']['status'] = array(
    '#type' => 'select',
    '#title' => t('Order status'),
    '#default_value' => $order->order_status,
    '#options' => $options,
    '#attributes' => array('style' => 'display: inline;'),
    '#weight' => -10,
  );

  $form['controls']['notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail notification on update.'),
    '#weight' => 0,
  );

  $form['controls']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * @see uc_order_view_update_form()
 * @ingroup themeable
 */
function theme_uc_order_view_update_controls($form) {
  return '<div class="update-controls">'. drupal_render($form) .'</div>';
}

/**
 * Form submit handler for uc_order_view_update_form().
 *
 * @see uc_order_view_update_form()
 */
function uc_order_view_update_form_submit($form, &$form_state) {
  global $user;

  if (!is_null($form_state['values']['order_comment']) && strlen(trim($form_state['values']['order_comment'])) > 0) {
    uc_order_comment_save($form_state['values']['order_id'], $user->uid, $form_state['values']['order_comment'], 'order', $form_state['values']['status'], $form_state['values']['notify']);
  }

  if (!is_null($form_state['values']['admin_comment']) && strlen(trim($form_state['values']['admin_comment'])) > 0) {
    uc_order_comment_save($form_state['values']['order_id'], $user->uid, $form_state['values']['admin_comment']);
  }

  if ($form_state['values']['status'] != $form_state['values']['current_status']) {
    if (uc_order_update_status($form_state['values']['order_id'], $form_state['values']['status'])) {
      if (is_null($form_state['values']['order_comment']) || strlen($form_state['values']['order_comment']) == 0) {
        uc_order_comment_save($form_state['values']['order_id'], $user->uid, '-', 'order', $form_state['values']['status'], $form_state['values']['notify']);
      }
    }
  }

  // Let conditional actions send email if requested.
  if ($form_state['values']['notify']) {
    $order = uc_order_load($form_state['values']['order_id']);
    ca_pull_trigger('uc_order_status_email_update', $order);
  }

  drupal_set_message(t('Order updated.'));
}

/**
 * Builds the order view products table.
 */
function op_products_view_table($order) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => 'order-pane-table'),
  );

  $table['#columns']['qty'] = array(
    'cell' => array(
      'data' => t('Qty'),
      'class' => 'qty',
    ),
    'weight' => 0,
  );
  $table['#columns']['product'] = array(
    'cell' => array(
      'data' => t('Product'),
      'class' => 'product',
    ),
    'weight' => 1,
  );
  $table['#columns']['model'] = array(
    'cell' => array(
      'data' => t('SKU'),
      'class' => 'sku',
    ),
    'weight' => 2,
  );
  if (user_access('administer products')) {
    $table['#columns']['cost'] = array(
      'cell' => array(
        'data' => t('Cost'),
        'class' => 'cost',
      ),
      'weight' => 3,
    );
  }
  $table['#columns']['price'] = array(
    'cell' => array(
      'data' => t('Price'),
      'class' => 'price',
    ),
    'weight' => 4,
  );
  $table['#columns']['total'] = array(
    'cell' => array(
      'data' => t('Total'),
      'class' => 'total',
    ),
    'weight' => 5,
  );
  drupal_alter('tapir_table_header', $table['#columns'], 'op_products_view_table');

  $context = array(
    'revision' => 'themed',
    'type' => 'order_product',
    'subject' => array(
      'order' => $order,
    ),
  );

  if (is_array($order->products)) {
    foreach ($order->products as $product) {
      $data = array();
      $data['qty'] = array(
        '#value' => $product->qty .'&times;',
        '#cell_attributes' => array('class' => 'qty'),
      );
      $node = node_load($product->nid);
      $title = node_access('view', $node) ? l($product->title, 'node/'. $node->nid) : check_plain($product->title);
      $data['product'] = array(
        '#value' => $title . uc_product_get_description($product),
        '#cell_attributes' => array('class' => 'product'),
      );
      $data['model'] = array(
        '#value' => check_plain($product->model),
        '#cell_attributes' => array('class' => 'sku'),
      );

      $context['subject']['product'] = $product;
      $context['subject']['node'] = node_load($product->nid);
      if (user_access('administer products')) {
        $context['field'] = 'cost';

        $data['cost'] = array(
          '#value' => uc_price($product->cost, $context),
          '#cell_attributes' => array('class' => 'cost'),
        );
      }

      $context['field'] = 'price';
      $price_info = array(
        'price' => $product->price,
        'qty' => $product->qty,
      );
      $data['price'] = array(
        '#value' => uc_price($product->price, $context),
        '#cell_attributes' => array('class' => 'price'),
      );
      $data['total'] = array(
        '#value' => uc_price($price_info, $context),
        '#cell_attributes' => array('class' => 'total'),
      );
      $table['#rows'][] = $data;
    }
  }
  else {
    $table['#rows'][]['product'] = array(
      '#value' => t('This order contains no products.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
  }

  return $table;
}

/**
 * Builds the order customer's view products table.
 */
function op_products_customer_table($order) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => 'order-pane-table'),
  );

  $table['#columns']['qty'] = array(
    'cell' => array(
      'data' => t('Qty'),
      'class' => 'qty',
    ),
    'weight' => 0,
  );
  $table['#columns']['product'] = array(
    'cell' => array(
      'data' => t('Product'),
      'class' => 'product',
    ),
    'weight' => 1,
  );
  $table['#columns']['model'] = array(
    'cell' => array(
      'data' => t('SKU'),
      'class' => 'sku',
    ),
    'weight' => 2,
  );
  if (user_access('administer products')) {
    $table['#columns']['cost'] = array(
      'cell' => array(
        'data' => t('Cost'),
        'class' => 'cost',
      ),
      'weight' => 3,
    );
  }
  $table['#columns']['price'] = array(
    'cell' => array(
      'data' => t('Price'),
      'class' => 'price',
    ),
    'weight' => 4,
  );
  $table['#columns']['total'] = array(
    'cell' => array(
      'data' => t('Total'),
      'class' => 'total',
    ),
    'weight' => 5,
  );
  drupal_alter('tapir_table_header', $table['#columns'], 'op_products_customer_table');

  $context = array(
    'revision' => 'themed',
    'type' => 'order_product',
    'subject' => array(
      'order' => $order,
    ),
  );

  if (is_array($order->products)) {
    foreach ($order->products as $product) {
      $data = array();
      $data['qty'] = array(
        '#value' => $product->qty .'&times;',
        '#cell_attributes' => array('class' => 'qty'),
      );
      $node = node_load($product->nid);
      $title = node_access('view', $node) ? l($product->title, 'node/'. $node->nid) : check_plain($product->title);
      $data['product'] = array(
        '#value' => $title . uc_product_get_description($product),
        '#cell_attributes' => array('class' => 'product'),
      );
      $data['model'] = array(
        '#value' => check_plain($product->model),
        '#cell_attributes' => array('class' => 'sku'),
      );

      $context['subject']['product'] = $product;
      $context['subject']['node'] = node_load($product->nid);
      if (user_access('administer products')) {
        $context['field'] = 'cost';
        $data['cost'] = array(
          '#value' => uc_price($product->cost, $context),
          '#cell_attributes' => array('class' => 'cost'),
        );
      }

      $context['field'] = 'price';
      $price_info = array(
        'price' => $product->price,
        'qty' => $product->qty,
      );
      $data['price'] = array(
        '#value' => uc_price($product->price, $context),
        '#cell_attributes' => array('class' => 'price'),
      );
      $data['total'] = array(
        '#value' => uc_price($price_info, $context),
        '#cell_attributes' => array('class' => 'total'),
      );
      $table['#rows'][] = $data;
    }
  }
  else {
    $table['#rows'][]['product'] = array(
      '#value' => t('This order contains no products.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
  }

  return $table;
}

/**
 * Builds the order comments table.
 */
function op_order_comments_view_table($comments) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => 'order-pane-table'),
  );

  $table['#columns']['date'] = array(
    'cell' => array('data' => t('Date'), 'class' => 'text-center'),
    'weight' => 0,
  );
  $table['#columns']['user'] = array(
    'cell' => t('User'),
    'weight' => 1,
  );
  $table['#columns']['notified'] = array(
    'cell' => t('Notified'),
    'weight' => 2,
  );
  $table['#columns']['status'] = array(
    'cell' => array('data' => t('Status'), 'class' => 'text-center'),
    'weight' => 3,
  );
  $table['#columns']['comment'] = array(
    'cell' => array('data' => t('Comment'), 'width' => '80%'),
    'weight' => 4,
  );
  drupal_alter('tapir_table_header', $table['#columns'], 'op_order_comments_view_table');

  if (is_array($comments)) {
    foreach ($comments as $comment) {
      $data = array();
      $data['date'] = array(
        '#value' => format_date($comment->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y') .'<\b\r />g:i:s A'),
        '#cell_attributes' => array('class' => 'date'),
      );
      $data['user'] = array(
        '#value' => uc_get_initials($comment->uid),
        '#cell_attributes' => array('align' => 'center'),
      );
      $data['notified'] = array(
        '#value' => (($comment->notified == 1)
          ? '<img src="'. base_path() . drupal_get_path('module', 'uc_order') .'/images/true-icon.gif" />'
          : '<img src="'. base_path() . drupal_get_path('module', 'uc_order') .'/images/false-icon.gif" />'),
        '#cell_attributes' => array('align' => 'center'),
      );
      $data['status'] = array(
        '#value' => $comment->title,
        '#cell_attributes' => array('align' => 'center'),
      );
      $data['comment'] = array(
        '#value' => check_plain($comment->message),
      );
      $table['#rows'][] = $data;
    }
  }
  else {
    $data['comment'] = array(
      '#value' => t('This order has no comments associated with it.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
    $table['#rows'][] = $data;
  }

  return $table;
}

/**
 * Builds the order admin comments table.
 */
function op_admin_comments_view_table($comments) {
  $table = array(
    '#type' => 'tapir_table',
    '#attributes' => array('class' => 'order-pane-table'),
  );

  $table['#columns']['date'] = array(
    'cell' => array('data' => t('Date'), 'class' => 'text-center'),
    'weight' => 0,
  );
  $table['#columns']['user'] = array(
    'cell' => array('data' => t('User'), 'class' => 'text-center'),
    'weight' => 1,
  );
  $table['#columns']['comment'] = array(
    'cell' => array('data' => t('Comment'), 'width' => '80%'),
    'weight' => 2,
  );
  drupal_alter('tapir_table_header', $table['#columns'], 'op_admin_comments_view_table');

  if (is_array($comments)) {
    foreach ($comments as $comment) {
      $data = array();
      $data['date'] = array(
        '#value' => format_date($comment->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y') .'<\b\r />g:i:s A'),
        '#cell_attributes' => array('align' => 'center', 'valign' => 'top'),
      );
      $data['user'] = array(
        '#value' => uc_get_initials($comment->uid),
        '#cell_attributes' => array('align' => 'center', 'valign' => 'top'),
      );
      $data['comment'] = array(
        '#value' => filter_xss_admin($comment->message),
        '#cell_attributes' => array('valign' => 'top'),
      );
      $table['#rows'][] = $data;
    }
  }
  else {
    $data['comment'] = array(
      '#value' => t('This order has no admin comments associated with it.'),
      '#cell_attributes' => array('colspan' => 'full'),
    );
    $table['#rows'][] = $data;
  }

  return $table;
}

/**
 * Builds a list of order panes defined in the enabled modules.
 */
function _order_pane_list($view = 'view') {
  static $panes;

  if (count($panes) > 0) {
    return $panes;
  }

  $panes = module_invoke_all('order_pane', NULL);
  foreach ($panes as $i => $value) {
    $panes[$i]['enabled'] = variable_get('uc_order_pane_'. $panes[$i]['id'] .'_enabled', (!isset($panes[$i]['enabled']) ? TRUE : $panes[$i]['enabled']));
    $panes[$i]['weight'] = variable_get('uc_order_pane_'. $panes[$i]['id'] .'_weight_'. $view, (!isset($panes[$i]['weight']) ? 0 : $panes[$i]['weight']));
  }

  // Allow other modules to alter the panes.
  drupal_alter('order_pane', $panes);

  // Make sure that all the required attributes are set
  foreach ($panes as $i => $value) {
    $panes[$i]['enabled'] = !isset($panes[$i]['enabled']) ? TRUE : $panes[$i]['enabled'];
    $panes[$i]['weight'] = !isset($panes[$i]['weight']) ? 0 : $panes[$i]['weight'];
  }
  usort($panes, 'uc_weight_sort');

  return $panes;
}

/**
 * Returns data from an order pane by pane ID and the array key.
 */
function _order_pane_data($pane_id, $key) {
  $panes = _order_pane_list();
  foreach ($panes as $pane) {
    if ($pane['id'] == $pane_id) {
      return $pane[$key];
    }
  }
}

/**
 * Calls an order pane function with the argument passed by reference.
 */
function _call_order_pane_byref($func, $op, &$form) {
  return $func($op, $form);
}
