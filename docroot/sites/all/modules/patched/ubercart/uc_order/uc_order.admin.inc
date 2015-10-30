<?php

/**
 * @file
 * Order administration menu items.
 */

/**
 * Redirects to the customers listing page.
 */
function uc_order_customers() {
  drupal_goto('admin/store/customers/view');
}

/**
 * Redirects to the orders listing page.
 */
function uc_order_orders() {
  drupal_goto('admin/store/orders/view');
}

/**
 * Generates the settings form for orders.
 *
 * @ingroup forms
 */
function uc_order_settings_form($form, &$form_state) {
  $form['invoice']['uc_cust_order_invoice_template'] = array(
    '#type' => 'select',
    '#title' => t('On-site invoice template'),
    '#description' => t('Select the invoice template to use when invoices are viewed on the site.'),
    '#options' => uc_order_template_options(),
    '#default_value' => variable_get('uc_cust_order_invoice_template', 'customer'),
  );
  if (module_exists('uc_cart')) {
    $form['invoice']['uc_cust_order_invoice_template']['#description'] .= '<br />' . t('This is separate from the template used to e-mail invoices to customers which is configured through <a href="!url">Rules</a>.', array('!url' => url('admin/store/settings/checkout/rules')));
  }

  return system_settings_form($form);
}

/**
 * Displays the order workflow form for order state and status customization.
 *
 * @see uc_order_workflow_form_submit()
 * @see theme_uc_order_state_table()
 * @see theme_uc_order_status_table()
 * @ingroup forms
 */
function uc_order_workflow_form($form, &$form_state) {
  $states = uc_order_state_list();
  $statuses = uc_order_status_list();

  $form['order_states'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order states'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#theme' => 'uc_order_state_table',
    '#tree' => TRUE,
  );

  foreach ($states as $state_id => $state) {
    $form['order_states'][$state_id]['title'] = array(
      '#markup' => $state['title'],
    );

    // Create the select box for specifying a default status per order state.
    $options = array();
    foreach ($statuses as $status) {
      if ($status['state'] == $state_id) {
        $options[$status['id']] = $status['title'];
      }
    }
    if (empty($options)) {
      $form['order_states'][$state_id]['default'] = array(
        '#markup' => t('- N/A -'),
      );
    }
    else {
      $form['order_states'][$state_id]['default'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => uc_order_state_default($state_id),
      );
    }
  }

  $form['order_statuses'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order statuses'),
    '#collapsible' => FALSE,
    '#theme' => 'uc_order_status_table',
    '#tree' => TRUE,
  );

  // Build the state option array for the order status table.
  $options = array();
  foreach ($states as $state_id => $state) {
    $options[$state_id] = $state['title'];
  }

  foreach ($statuses as $status) {
    $form['order_statuses'][$status['id']]['id'] = array(
      '#markup' => $status['id'],
    );
    $form['order_statuses'][$status['id']]['title'] = array(
      '#type' => 'textfield',
      '#default_value' => $status['title'],
      '#size' => 32,
      '#required' => TRUE,
    );
    $form['order_statuses'][$status['id']]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 20,
      '#default_value' => $status['weight'],
    );
    $form['order_statuses'][$status['id']]['locked'] = array(
      '#type' => 'value',
      '#value' => $status['locked'],
    );
    if ($status['locked']) {
      $form['order_statuses'][$status['id']]['state'] = array(
        '#markup' => uc_order_state_data($status['state'], 'title'),
      );
    }
    else {
      $form['order_statuses'][$status['id']]['state'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $status['state'],
      );
      $form['order_statuses'][$status['id']]['remove'] = array(
        '#type' => 'checkbox',
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes'),
  );

  return $form;
}

/**
 * Form submission handler for uc_order_workflow_form().
 *
 * @see uc_order_workflow_form()
 */
function uc_order_workflow_form_submit($form, &$form_state) {
  foreach ($form_state['values']['order_states'] as $key => $value) {
    variable_set('uc_state_' . $key . '_default', $value['default']);
  }

  foreach ($form_state['values']['order_statuses'] as $key => $value) {
    if ($value['locked'] != TRUE && $value['remove'] == TRUE) {
      db_delete('uc_order_statuses')
        ->condition('order_status_id', $key)
        ->execute();
      drupal_set_message(t('Order status %status removed.', array('%status' => $key)));
    }
    else {
      $fields = array(
        'title' => $value['title'],
        'weight' => $value['weight'],
      );

      // The state cannot be changed if the status is locked.
      if ($value['locked'] == FALSE) {
        $fields['state'] = $value['state'];
      }

      $query = db_update('uc_order_statuses')
        ->fields($fields)
        ->condition('order_status_id', $key)
        ->execute();
    }
  }

  drupal_set_message(t('Order workflow information saved.'));
}

/**
 * Themes the order state table in the order workflow settings.
 *
 * @see uc_order_workflow_form()
 * @ingroup themeable
 */
function theme_uc_order_state_table($variables) {
  $form = $variables['form'];

  $header = array(t('State'), t('Default order status'));

  foreach (element_children($form) as $state_id) {
    $rows[] = array(
      drupal_render($form[$state_id]['title']),
      drupal_render($form[$state_id]['default']),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Themes the order status table in the order workflow settings.
 *
 * @see uc_order_workflow_form()
 * @ingroup themeable
 */
function theme_uc_order_status_table($variables) {
  $form = $variables['form'];

  $header = array(t('ID'), t('Title'), t('List position'), t('State'), t('Remove'));

  foreach (element_children($form) as $state_id) {
    $rows[] = array(
      drupal_render($form[$state_id]['id']),
      drupal_render($form[$state_id]['title']),
      drupal_render($form[$state_id]['weight']),
      drupal_render($form[$state_id]['state']),
      array('data' => drupal_render($form[$state_id]['remove']), 'align' => 'center'),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Presents the form to create a custom order status.
 *
 * @see uc_order_status_create_form_validate()
 * @see uc_order_status_create_form_submit()
 * @ingroup forms
 */
function uc_order_status_create_form($form, &$form_state) {
  $form['status_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Order status ID'),
    '#description' => t('Must be a unique ID with no spaces.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['status_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The order status title displayed to users.'),
    '#size' => 32,
    '#maxlength' => 48,
    '#required' => TRUE,
  );

  // Build the state option array for the order status table.
  $options = array();
  foreach (uc_order_state_list() as $state) {
    $options[$state['id']] = $state['title'];
  }
  $form['status_state'] = array(
    '#type' => 'select',
    '#title' => t('Order state'),
    '#description' => t('Set which order state this status is for.'),
    '#options' => $options,
    '#default_value' => 'post_checkout',
  );

  $form['status_weight'] = array(
    '#type' => 'weight',
    '#title' => t('List position'),
    '#delta' => 20,
    '#default_value' => 0,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/store/settings/orders/workflow'),
  );

  return $form;
}

/**
 * Ensures the new status id is unique and has no spaces.
 *
 * @see uc_order_status_create_form()
 * @see uc_order_status_create_form_submit()
 */
function uc_order_status_create_form_validate($form, &$form_state) {
  $new_status = strtolower(trim($form_state['values']['status_id']));
  if (strpos($new_status, ' ') !== FALSE || $new_status == 'all') {
    form_set_error('status_id', t('You have entered an invalid status ID.'));
  }

  $statuses = uc_order_status_list();
  foreach ($statuses as $status) {
    if ($new_status == $status['id']) {
      form_set_error('status_id', t('This ID is already in use.  Please specify a unique ID.'));
    }
  }
}

/**
 * Form submission handler for uc_order_status_create_form_submit().
 *
 * @see uc_order_status_create_form()
 * @see uc_order_status_create_form_validate()
 */
function uc_order_status_create_form_submit($form, &$form_state) {
  db_insert('uc_order_statuses')
    ->fields(array(
      'order_status_id' => $form_state['values']['status_id'],
      'title' => $form_state['values']['status_title'],
      'state' => $form_state['values']['status_state'],
      'weight' => $form_state['values']['status_weight'],
      'locked' => 0,
    ))
    ->execute();

  drupal_set_message(t('Custom order status created.'));

  $form_state['redirect'] = 'admin/store/settings/orders/workflow';
}

/**
 * Creates a new order and redirect to its edit screen.
 *
 * @see uc_order_create_form_create_validate()
 * @see uc_order_create_form_create_submit()
 * @ingroup forms
 */
function uc_order_create_form($form, &$form_state) {
  $form['customer_type'] = array(
    '#type' => 'radios',
    '#options' => array(
      'search' => t('Search for an existing customer.'),
      'create' => t('Create a new customer account.'),
      'none'   => t('No customer account required.'),
    ),
    '#required' => TRUE,
    '#default_value' => 'search',
    '#ajax' => array(
      'callback' => 'uc_order_create_form_customer',
      'wrapper'  => 'uc-order-customer',
      'progress' => array('type' => 'throbber'),
    ),
  );

  $form['customer'] = array(
    '#prefix' => '<div id="uc-order-customer">',
    '#suffix' => '</div>',
    '#tree'   => TRUE,
  );

  // Create form elements needed for customer search.
  // Shown only when the 'Search for an existing customer.' radio is selected.
  if (!isset($form_state['values']['customer_type']) ||
      $form_state['values']['customer_type'] == 'search') {

    // Container for customer search fields.
    $form['customer'] += array(
      '#type' => 'fieldset',
      '#title' => t('Customer search'),
      '#description' => t('Enter full or partial information in one or more of the following fields, then press the "Search" button. Search results will match all the provided information.'),
    );
    // Customer first name.
    $form['customer']['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#size' => 24,
      '#maxlength' => 32,
    );
    // Customer last name.
    $form['customer']['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#size' => 24,
      '#maxlength' => 32,
    );
    // Customer e-mail address.
    $form['customer']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 24,
      '#maxlength' => 96,
    );
    // Customer username.
    $form['customer']['username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#size' => 24,
      '#maxlength' => 96,
    );
    $form['customer']['search'] = array(
      '#type' => 'button',
      '#value' => t('Search'),
      '#ajax' => array(
        'callback' => 'uc_order_create_form_customer_search',
        'wrapper' => 'uc-order-customer-results',
        'progress' => array('type' => 'throbber'),
      ),
    );
    $form['customer']['uid'] = array(
      '#prefix' => '<div id="uc-order-customer-results">',
      '#suffix' => '</div>',
    );

    // Search for existing customer by e-mail address.
    if (isset($form_state['values']['customer']['email'])) {
      $query = db_select('users', 'u')->distinct();
      $query->leftJoin('uc_orders', 'o', 'u.uid = o.uid');
      $query->fields('u', array('uid', 'name', 'mail'))
        ->fields('o', array('billing_first_name', 'billing_last_name'))
        ->condition('u.uid', 0, '>')
        ->condition(db_or()
          ->isNull('o.billing_first_name')
          ->condition('o.billing_first_name', db_like(trim($form_state['values']['customer']['first_name'])) . '%', 'LIKE')
        )
        ->condition(db_or()
          ->isNull('o.billing_last_name')
          ->condition('o.billing_last_name', db_like(trim($form_state['values']['customer']['last_name'])) . '%', 'LIKE')
        )
        ->condition(db_or()
          ->condition('o.primary_email', db_like(trim($form_state['values']['customer']['email'])) . '%', 'LIKE')
          ->condition('u.mail', db_like(trim($form_state['values']['customer']['email'])) . '%', 'LIKE')
        )
        ->condition('u.name', db_like(trim($form_state['values']['customer']['username'])) . '%', 'LIKE')
        ->orderBy('o.created', 'DESC')
        ->range(0, $limit = 11);

      $result = $query->execute();

      $options = array();
      foreach ($result as $user) {
        $name = '';
        if (!empty($user->billing_first_name) && !empty($user->billing_last_name)) {
          $name = $user->billing_first_name . ' ' . $user->billing_last_name . ' ';
        }
        // Options formated as "First Last <email@example.com> (username)".
        $options[$user->uid] = $name . '&lt;' . $user->mail . '&gt;' . ' (' . $user->name . ')';
      }

      $max = FALSE;
      if (count($options) == $limit) {
        array_pop($options);
        $max = TRUE;
      }

      if (!empty($options)) {
        // Display search results.
        $form['customer']['uid'] += array(
          '#type' => 'radios',
          '#title' => t('Select customer'),
          '#description' => $max ? t('More than !limit results found. Refine your search to find other customers.', array('!limit' => $limit - 1)) : '',
          '#options' => $options,
          '#default_value' => key($options),
        );
      }
      else {
        // No search results found.
        $form['customer']['uid'] += array(
          '#markup' => '<p>' . t('Search returned no results.') . '</p>',
        );
      }
    }
  }
  // Create form elements needed for new customer creation.
  // Shown only when the 'Create a new customer account.' radio is selected.
  elseif ($form_state['values']['customer_type'] == 'create') {
    // Container for new customer information.
    $form['customer'] += array(
      '#type'  => 'fieldset',
      '#title' => t('New customer details'),
    );
    // Customer e-mail address.
    $form['customer']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Customer e-mail address'),
      '#size' => 24,
      '#maxlength' => 96,
    );
    // Option to notify customer.
    $form['customer']['sendmail'] = array(
      '#type'  => 'checkbox',
      '#title' => t('E-mail account details to customer.'),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create order'),
    '#validate' => array('uc_order_create_form_create_validate'),
    '#submit' => array('uc_order_create_form_create_submit'),
  );

  return $form;
}

/**
 * Ajax callback: updates the customer selection fields.
 */
function uc_order_create_form_customer($form, &$form_state) {
  return $form['customer'];
}

/**
 * Ajax callback: updates the customer search results.
 */
function uc_order_create_form_customer_search($form, &$form_state) {
  return $form['customer']['uid'];
}

/**
 * Form validation handler for customer search.
 *
 * @see uc_order_create_form()
 * @ingroup forms
 */
function uc_order_create_form_create_validate($form, &$form_state) {
  switch ($form_state['values']['customer_type']) {
    case 'search':
      if (empty($form_state['values']['customer']['uid'])) {
        form_set_error('customer][uid', t('Please select a customer.'));
      }
      break;

    case 'create':
      $email = trim($form_state['values']['customer']['email']);
      if (!valid_email_address($email)) {
        form_set_error('customer][mail', t('Invalid e-mail address.'));
      }

      $uid = db_query('SELECT uid FROM {users} WHERE mail LIKE :mail', array(':mail' => $email))->fetchField();
      if ($uid) {
        form_set_error('customer][mail', t('An account already exists for that e-mail.'));
      }
      break;
  }
}

/**
 * Form submission handler for customer search.
 *
 * @see uc_order_create_form()
 * @ingroup forms
 */
function uc_order_create_form_create_submit($form, &$form_state) {
  global $user;

  switch ($form_state['values']['customer_type']) {
    case 'search':
      $uid = $form_state['values']['customer']['uid'];
      break;

    case 'create':
      // Create new account.
      $email = trim($form_state['values']['customer']['email']);
      $fields = array(
        'name' => uc_store_email_to_username($email),
        'mail' => $email,
        'pass' => user_password(),
        'status' => variable_get('uc_new_customer_status_active', TRUE) ? 1 : 0,
      );
      $account = user_save(NULL, $fields);
      $uid = $account->uid;

      if ($form_state['values']['customer']['sendmail']) {
        // Manually set the password so it appears in the e-mail.
        $account->password = $fields['pass'];
        drupal_mail('user', 'register_admin_created', $email, uc_store_mail_recipient_language($email), array('account' => $account), uc_store_email_from());
        drupal_set_message(t('A welcome message has been e-mailed to the new user.'));
      }
      break;

    default:
      $uid = 0;
  }

  $order = uc_order_new($uid, 'post_checkout');
  uc_order_comment_save($order->order_id, $user->uid, t('Order created by the administration.'), 'admin');

  $form_state['redirect'] = 'admin/store/orders/' . $order->order_id . '/edit';
}

/**
 * Creates a new order for the specified customer, ready for editing.
 */
function uc_order_create_for_user($account) {
  global $user;

  $order = uc_order_new($account->uid, 'post_checkout');
  uc_order_comment_save($order->order_id, $user->uid, t('Order created by the administration.'), 'admin');

  drupal_goto('admin/store/orders/' . $order->order_id . '/edit');
}

/**
 * Displays a form to select a previously entered address.
 *
 * @see uc_order_address_book_form()
 */
function uc_order_address_book() {
  $uid = intval($_POST['uid']);
  $type = $_POST['type'];
  $func = $_POST['func'];

  $form = drupal_get_form('uc_order_address_book_form', $uid, $type, $func);
  print drupal_render($form);
  exit();
}

/**
 * Presents previously entered addresses as selectable options.
 *
 * @see uc_order_address_book()
 * @ingroup forms
 */
function uc_order_address_book_form($form, &$form_state, $uid = 0, $type = 'billing', $func = '') {
  $select = uc_select_address($uid, $type, $func);

  if ($uid == 0) {
    $form['desc'] = array('#markup' => '<br />' . t('You must select a customer before address<br />information is available.<br />') . '<br />');
  }
  elseif (is_null($select)) {
    $form['desc'] = array('#markup' => '<br />' . t('No addresses found for customer.') . '<br />');
  }
  else {
    $form['addresses'] = uc_select_address($uid, $type, $func, t('Select an address'));
    $form['addresses']['#prefix'] = '<div style="float: left; margin-right: 1em;">';
    $form['addresses']['#suffix'] = '</div>';
  }

  $form['close'] = array(
    '#type' => 'button',
    '#value' => t('Close'),
    '#attributes' => array('onclick' => "return close_address_select('#" . $type . "_address_select');"),
  );

  return $form;
}

/**
 * Presents the customer search results and let one of them be chosen.
 *
 * @see uc_order_select_customer_form()
 */
function uc_order_select_customer($email = NULL) {
  $build = array();
  $options = NULL;

  // Return the search results and let them pick one!
  if (arg(4) == 'search') {
    $first_name = str_replace('*', '%', db_like($_POST['first_name']));
    $last_name = str_replace('*', '%', db_like($_POST['last_name']));
    $email = str_replace('*', '%', db_like($_POST['email']));

    $query = db_select('users', 'u')->distinct();
    $query->leftJoin('uc_orders', 'o', 'u.uid = o.uid');
    $query->fields('u', array('uid', 'mail'))
      ->fields('o', array('billing_first_name', 'billing_last_name'))
      ->condition('u.uid', 0, '>')
      ->orderBy('o.billing_last_name');

    if ($first_name && $first_name !== '%') {
      $query->condition('o.billing_first_name', $first_name, 'LIKE');
    }
    if ($last_name && $last_name !== '%') {
      $query->condition('o.billing_last_name', $last_name, 'LIKE');
    }
    if ($email && $email !== '%') {
      $query->condition(db_or()
        ->condition('o.primary_email', $email, 'LIKE')
        ->condition('u.mail', $email, 'LIKE')
      );
    }

    $result = $query->execute();

    $options = array();
    foreach ($result as $user) {
      if (empty($user->billing_first_name) && empty($user->billing_last_name)) {
        $name = '';
      }
      else {
        $name = $user->billing_last_name . ', ' . $user->billing_first_name . ' ';
      }
      $options[$user->uid . ':' . $user->mail] = $name . '(' . $user->mail . ')';
    }

    if (count($options) == 0) {
      $build['description'] = array('#markup' => '<p>' . t('Search returned no results.') . '</p>');
      $options = NULL;
    }
    else {
      $build['description'] = array('<p>' . t('Search returned the following:') . '</p>');
    }
  }

  // Check to see if the e-mail address for a new user is unique.
  if (arg(5) == 'check') {
    $email = check_plain($_POST['email']);
    $build['email'] = array('#markup' => '');
    if (!valid_email_address($email)) {
      $build['email']['#markup'] .= t('Invalid e-mail address.') . '<br />';
    }
    $result = db_query("SELECT uid, mail FROM {users} WHERE mail = :mail", array(':mail' => $email));
    if ($user = $result->fetchObject()) {
      $build['email']['#markup'] .= t('An account already exists for that e-mail.') . '<br /><br />';
      $build['email']['#markup'] .= '<b>' . t('Use this account now?') . '</b><br />'
        . t('User @uid - @mail', array('@uid' => $user->uid, '@mail' => $user->mail)) . ' <input type="button" '
        . 'onclick="select_existing_customer(' . $user->uid . ', \''
        . $user->mail . '\');" value="' . t('Apply') . '" /><br /><br /><hr /><br/>';
    }
    else {
      $name = uc_store_email_to_username($email);

      $fields = array(
        'name' => $name,
        'mail' => $email,
        'pass' => user_password(6),
        'status' => variable_get('uc_new_customer_status_active', TRUE) ? 1 : 0,
      );

      $account = user_save('', $fields);

      if ($_POST['sendmail'] == 'true') {
        // Manually set the password so it appears in the e-mail.
        $account->password = $fields['pass'];

        // Send the e-mail through the user module.
        drupal_mail('user', 'register_admin_created', $email, uc_store_mail_recipient_language($email), array('account' => $account), uc_store_email_from());

        $build['email']['#markup'] .= t('Account details sent to e-mail provided.<br /><br /><strong>Username:</strong> @username<br /><strong>Password:</strong> @password', array('@username' => $fields['name'], '@password' => $fields['pass'])) . '<br /><br />';
      }

      $build['result'] = array(
        '#markup' => '<strong>' . t('Use this account now?') . '</strong><br />'
          . t('User @uid - @mail', array('@uid' => $account->uid, '@mail' => $account->mail)) . ' <input type="button" '
          . 'onclick="select_existing_customer(' . $account->uid . ', \''
          . $account->mail . '\');" value="' . t('Apply') . '" /><br /><br /><hr /><br/>',
      );
    }
  }

  $build['customer_select_form'] = drupal_get_form('uc_order_select_customer_form', $options);

  print drupal_render($build);
  exit();
}

/**
 * Form to choose a customer from a list.
 *
 * @see uc_order_select_customer()
 * @ingroup forms
 */
function uc_order_select_customer_form($form, &$form_state, $options = NULL) {
  if (is_null(arg(4))) {
    $form['desc'] = array(
      '#markup' => '<div>' . t('Search for a customer based on these fields.')
                 . '<br />' . t('Use * as a wildcard to match any character.') . '<br />'
                 . '(<em>' . t('Leave a field empty to ignore it in the search.')
                 . '</em>)</div>',
    );

    $form['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#size' => 24,
      '#maxlength' => 32,
    );

    $form['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#size' => 24,
      '#maxlength' => 32,
    );

    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 24,
      '#maxlength' => 96,
    );
  }
  elseif (arg(4) == 'search' && !is_null($options)) {
    $form['cust_select'] = array(
      '#type' => 'select',
      '#title' => t('Select a customer'),
      '#size' => 7,
      '#options' => $options,
      '#default_value' => key($options),
      '#attributes' => array('ondblclick' => 'return select_customer_search();'),
    );
  }
  elseif (arg(4) == 'new') {
    $form['desc'] = array(
      '#markup' => '<div>' . t('Enter an e-mail address for the new customer.') . '</div>',
    );

    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 24,
      '#maxlength' => 96,
    );
  }

  $form['actions'] = array('#type' => 'actions');
  if (is_null(arg(4))) {
    $form['actions']['search'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#attributes' => array('onclick' => 'return load_customer_search_results();'),
    );
  }
  elseif (arg(4) == 'search') {
    if (!is_null($options)) {
      $form['actions']['select'] = array(
        '#type' => 'submit',
        '#value' => t('Select'),
        '#attributes' => array('onclick' => 'return select_customer_search();'),
      );
    }
    $form['actions']['back'] = array(
      '#type' => 'submit',
      '#value' => t('Back'),
      '#attributes' => array('onclick' => 'return load_customer_search();'),
    );
  }
  elseif (arg(4) == 'new') {
    $form['sendmail'] = array(
      '#type' => 'checkbox',
      '#title' => t('E-mail customer account details.'),
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#attributes' => array('onclick' => 'return check_new_customer_address();'),
    );
  }

  $form['actions']['close'] = array(
    '#type' => 'submit',
    '#value' => t('Close'),
    '#attributes' => array('onclick' => 'return close_customer_select();'),
  );

  return $form;
}

/**
 * Returns the sortable table listing of a customer's orders.
 *
 * This function is deprecated; this listing is now provided by Views.
 *
 * @param $uid
 *   The user ID whose orders you wish to list.
 */
function uc_order_history($user) {
  drupal_set_title(t('My order history'));

  $header = array(
    array('data' => t('Date'), 'field' => 'o.created', 'sort' => 'desc'),
    array('data' => t('Order #'), 'field' => 'o.order_id'),
    array('data' => t('Status'), 'field' => 'os.title'),
    array('data' => t('Products'), 'field' => 'products'),
    array('data' => t('Total'), 'field' => 'o.order_total')
  );

  $rows = array();

  $query = db_select('uc_orders', 'o');
  $o_order_id = $query->addField('o', 'order_id');
  $o_created = $query->addField('o', 'created');
  $o_status = $query->addField('o', 'order_status');
  $o_total = $query->addField('o', 'order_total');
  $o_uid = $query->addField('o', 'uid');

  $query->condition($o_uid, $user->uid)
    ->condition($o_status, uc_order_status_list('general', TRUE), 'IN');

  $count_query = $query->countQuery();

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $os = $query->leftJoin('uc_order_statuses', 'os', 'o.order_status = os.order_status_id');
  $op = $query->leftJoin('uc_order_products', 'op', 'o.order_id = op.order_id');

  $os_title = $query->addField('os', 'title');
  $op_products = $query->addExpression('SUM(op.qty)', 'products');

  $query->groupBy('o.order_id')
    ->groupBy('o.created')
    ->groupBy('os.title')
    ->groupBy('o.order_total')
    ->groupBy('o.order_status')
    ->groupBy('o.uid')
    ->orderByHeader($header)
    ->limit(20);
  $query->setCountQuery($count_query);

  $result = $query->execute();

  // Build a table based on the customer's orders.
  foreach ($result as $order) {
    $link = l($order->order_id, 'user/' . $user->uid . '/orders/' . $order->order_id);

    if (user_access('view all orders')) {
      $link .= '<span class="order-admin-icons">' . uc_order_actions($order, TRUE) . '</span>';
    }

    $rows[] = array(
      array('data' => format_date($order->created, 'uc_store')),
      array('data' => $link),
      array('data' => check_plain($order->title)),
      array('data' => (!is_null($order->products) ? $order->products : 0), 'align' => 'center'),
      array('data' => array('#theme' => 'uc_price', '#price' => $order->order_total), 'align' => 'right'),
    );
  }

  $build = array();
  $build['orders'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('uc-order-history')),
    '#empty' => t('No orders available.'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#element' => 0,
    '#weight' => 5,
  );

  return $build;
}

/**
 * Displays the order edit screen, constructed via hook_uc_order_pane().
 *
 * @see uc_order_edit_form_validate()
 * @see uc_order_edit_form_submit()
 * @see theme_uc_order_edit_form()
 * @see uc_order_edit_form_delete()
 * @ingroup forms
 */
function uc_order_edit_form($form, &$form_state, $order) {
  if (isset($form_state['order'])) {
    $order = $form_state['order'];
  }
  else {
    $form_state['order'] = $order;
  }
  $form['#order'] = $order;
  $form['order_id'] = array('#type' => 'hidden', '#value' => $order->order_id);
  $form['order_uid'] = array('#type' => 'hidden', '#value' => $order->uid);

  $modified = isset($form_state['values']['order_modified']) ? $form_state['values']['order_modified'] : $order->modified;
  $form['order_modified'] = array('#type' => 'hidden', '#value' => $modified);

  $panes = _uc_order_pane_list('edit');
  foreach ($panes as $id => $pane) {
    if (in_array('edit', $pane['show'])) {
      $func = $pane['callback'];
      if (function_exists($func)) {
        $func('edit-form', $order, $form, $form_state);
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit-changes'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes'),
    '#attributes' => array('class' => array('save-button')),
  );

  if (uc_order_can_delete($order)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('uc_order_edit_form_delete'),
    );
  }

  field_attach_form('uc_order', $order, $form, $form_state);

  form_load_include($form_state, 'inc', 'uc_store', 'includes/uc_ajax_attach');
  $form['#process'][] = 'uc_ajax_process_form';

  return $form;
}

/**
 * Prevents order edits from colliding.
 *
 * @see uc_order_edit_form()
 * @see uc_order_edit_form_submit()
 * @see theme_uc_order_edit_form()
 */
function uc_order_edit_form_validate($form, &$form_state) {
  $order = uc_order_load($form_state['values']['order_id']);
  if ($form_state['values']['order_modified'] != $order->modified) {
    form_set_error('order_modified', t('This order has been modified by another user, changes cannot be saved.'));
  }

  entity_form_field_validate('uc_order', $form, $form_state);

  // Build list of changes to be applied.
  $panes = _uc_order_pane_list();
  foreach ($panes as $id => $pane) {
    if (in_array('edit', $pane['show'])) {
      $func = $pane['callback'];
      if (function_exists($func)) {
        if (($changes = $func('edit-process', $order, $form, $form_state)) != NULL) {
          foreach ($changes as $key => $value) {
            $form_state['order']->$key = $value;
          }
        }
      }
    }
  }
}

/**
 * Form submission handler for uc_order_edit_form().
 *
 * @see uc_order_edit_form()
 * @see uc_order_edit_form_validate()
 * @see theme_uc_order_edit_form()
 */
function uc_order_edit_form_submit($form, &$form_state) {
  $order = uc_order_load($form_state['values']['order_id']);
  $log = array();

  foreach ($form_state['order'] as $key => $value) {
    if (!isset($order->$key) || $order->$key !== $value) {
      if (!is_array($value)) {
        $log[$key] = array('old' => $order->$key, 'new' => $value);
      }
      $order->$key = $value;
    }
  }

  if (module_exists('uc_stock')) {
    $qtys = array();
    foreach ($order->products as $product) {
      $qtys[$product->order_product_id] = $product->qty;
    }
  }

  $order->products = array();
  if (isset($form_state['values']['products']) && is_array($form_state['values']['products'])) {
    foreach ($form_state['values']['products'] as $product) {
      if (!isset($product['remove']) && intval($product['qty']) > 0) {
        $product['data'] = unserialize($product['data']);
        $product = (object)$product;
        $order->products[] = $product;

        if (module_exists('uc_stock')) {
          $temp = $product->qty;
          $product->qty = $product->qty - $qtys[$product->order_product_id];
          uc_stock_adjust_product_stock($product, 0, $order);
          $product->qty = $temp;
        }
      }
      else {
        $log['remove_' . $product['nid']] = $product['title'] . ' removed from order.';
      }
    }
  }

  // Load line items again, since some may have been updated by the form.
  $order->line_items = uc_order_load_line_items($order);

  uc_order_log_changes($order->order_id, $log);

  field_attach_submit('uc_order', $order, $form, $form_state);

  uc_order_save($order);

  drupal_set_message(t('Order changes saved.'));
}

/**
 * Formats the uc_order_edit_form().
 *
 * @see uc_order_edit_form()
 * @see uc_order_edit_form_validate()
 * @see uc_order_edit_form_submit()
 * @ingroup themeable
 */
function theme_uc_order_edit_form($variables) {
  $form = $variables['form'];
  $output = '';

  $panes = _uc_order_pane_list();
  foreach ($panes as $id => $pane) {
    if (in_array('edit', $pane['show'])) {
      $func = $pane['callback'];
      if (function_exists($func) && ($contents = $func('edit-theme', $form['#order'], $form)) != NULL) {
        $output .= '<div class="order-pane ' . $pane['class'] . '" id="order-pane-' . $id . '">';
        $title = isset($pane['display title']) ? $pane['display title'] : $pane['title'];
        if ($title) {
          $output .= '<div class="order-pane-title">' . $title . ':' . '</div>';
        }
        $output .= $contents . '</div>';
      }
    }
  }

  $last = '<div class="order-pane abs-left">' . drupal_render($form['order_id']) . drupal_render($form['order_modified'])
    . drupal_render($form['form_id']) . drupal_render($form['form_token'])
    . drupal_render($form['form_build_id'])
    . drupal_render($form['actions'])
    . '</div>';

  $output .= drupal_render_children($form) . $last;

  return $output;
}

/**
 * Handles order delete button action.
 *
 * @see uc_order_edit_form()
 */
function uc_order_edit_form_delete($form, &$form_state) {
  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/delete';
}

/**
 * Form to add a line item to an order.
 *
 * @see uc_order_add_line_item_form_validate()
 * @see uc_order_add_line_item_submit()
 */
function uc_order_add_line_item_form($form, &$form_state, $order, $line_item_id) {
  $func = _uc_line_item_data($line_item_id, 'callback');

  if (!function_exists($func) || ($form = $func('form', $order->order_id)) == NULL) {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Line item title'),
      '#description' => t('Display title of the line item.'),
      '#size' => 32,
      '#maxlength' => 128,
      '#default_value' => _uc_line_item_data($line_item_id, 'title'),
    );
    $form['amount'] = array(
      '#type' => 'uc_price',
      '#title' => t('Line item amount'),
      '#allow_negative' => TRUE,
    );
  }

  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['line_item_id'] = array(
    '#type' => 'hidden',
    '#value' => $line_item_id,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add line item'),
    '#suffix' => l(t('Cancel'), 'admin/store/orders/' . $order->order_id . '/edit'),
  );

  return $form;
}

/**
 * Validates new line item data.
 *
 * @see uc_order_add_line_item_form()
 */
function uc_order_add_line_item_form_validate($form, &$form_state) {
  $func = _uc_line_item_data($form_state['values']['line_item_id'], 'callback');
  if (function_exists($func) && ($form = $func('form', $form_state['values']['order_id'])) != NULL) {
    $func('validate', $form, $form_state);
  }
}

/**
 * Form submission handler for uc_order_add_line_item_form().
 *
 * @see uc_order_add_line_item_form()
 */
function uc_order_add_line_item_form_submit($form, &$form_state) {
  $func = _uc_line_item_data($form_state['values']['line_item_id'], 'callback');
  if (function_exists($func) && ($form = $func('form', $form_state['values']['order_id'])) != NULL) {
    $func('submit', $form, $form_state);
  }
  else {
    uc_order_line_item_add($form_state['values']['order_id'], $form_state['values']['line_item_id'], $form_state['values']['title'], $form_state['values']['amount']);
    drupal_set_message(t('Line item added to order.'));
  }

  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/edit';
}

/**
 * Sets recipients of an invoice, then mails it.
 *
 * @see uc_order_mail_invoice_form_validate()
 * @see uc_order_mail_invoice_form_submit()
 * @ingroup forms
 */
function uc_order_mail_invoice_form($form, &$form_state, $order) {
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient e-mail address'),
    '#default_value' => $order->primary_email,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit' ] = array(
    '#type' => 'submit',
    '#value' => t('Mail invoice'),
  );

  return $form;
}

/**
 * Only mail invoices to valid email addresses.
 *
 * @see uc_order_mail_invoice_form()
 */
function uc_order_mail_invoice_form_validate($form, &$form_state) {
  $recipient = check_plain($form_state['values']['email']);
  if (empty($recipient) || !valid_email_address($recipient)) {
    form_set_error('email', t('Invalid e-mail address.'));
  }
}

/**
 * Form submission handler for uc_order_mail_invoice_form().
 *
 * @see uc_order_mail_invoice_form()
 */
function uc_order_mail_invoice_form_submit($form, &$form_state) {
  $order = uc_order_load($form_state['values']['order_id']);
  $recipient = check_plain($form_state['values']['email']);

  $params = array('order' => $order);
  $sent = drupal_mail('uc_order', 'invoice', $recipient, uc_store_mail_recipient_language($recipient), $params, uc_store_email_from());

  if (!$sent) {
    drupal_set_message(t('E-mail failed.'));
  }
  else {
    $message = t('Invoice e-mailed to @email.', array('@email' => $recipient));
    drupal_set_message($message);
    uc_order_log_changes($order->order_id, array($message));
  }
}

/**
 * Displays a log of changes made to an order.
 */
function uc_order_log($order) {
  $result = db_query("SELECT * FROM {uc_order_log} WHERE order_id = :id ORDER BY created, order_log_id", array(':id' => $order->order_id));

  $header = array(t('Time'), t('User'), t('Changes'));
  $rows = array();
  foreach ($result as $change) {
    $rows[] = array(
      format_date($change->created, 'short'),
      theme('uc_uid', array('uid' => $change->uid)),
      $change->changes,
    );
  }

  $build['log'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No changes have been logged for this order.'),
  );

  return $build;
}

/**
 * Confirmation form to delete an order.
 *
 * @see uc_order_delete_confirm_form_submit()
 * @ingroup forms
 */
function uc_order_delete_confirm_form($form, &$form_state, $order) {
  if (!uc_order_can_delete($order)) {
    drupal_set_message(t('It is not possible to delete order @id.', array('@id' => $order->order_id)));
    drupal_goto('admin/store/orders');
  }

  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $order->order_id
  );

  return confirm_form($form, t('Are you sure you want to delete order @order_id?', array('@order_id' => $order->order_id)), 'admin/store/orders', NULL, t('Delete'));
}

/**
 * Form submission handler for uc_order_delete_confirm_form().
 *
 * @see uc_order_delete_confirm_form()
 */
function uc_order_delete_confirm_form_submit($form, &$form_state) {
  // Delete the specified order.
  uc_order_delete($form_state['values']['order_id']);

  // Display a message to the user and return to the order admin page.
  drupal_set_message(t('Order @order_id completely removed from the database.', array('@order_id' => $form_state['values']['order_id'])));

  $form_state['redirect'] = 'admin/store/orders';
}
