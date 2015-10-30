<?php

/**
 * @file
 * Cart administration menu items.
 */

/**
 * General settings for the shopping cart.
 *
 * @see uc_cart_cart_settings_form_validate()
 * @ingroup forms
 */
function uc_cart_cart_settings_form($form, &$form_state) {

  // Put fieldsets into vertical tabs
  $form['cart-settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'uc_cart') . '/uc_cart.admin.js',
      ),
    ),
  );

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#group' => 'cart-settings',
  );

  $panes = uc_cart_cart_pane_list(NULL);
  $form['general']['panes'] = array(
    '#theme' => 'uc_pane_sort_table',
    '#pane_prefix' => 'uc_cap',
    '#draggable' => 'uc-cart-pane-weight',
  );
  foreach ($panes as $id => $pane) {
    $form['general']['panes'][$id]['uc_cap_' . $id . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($pane['title']),
      '#default_value' => variable_get('uc_cap_'. $pane['id'] .'_enabled', $pane['enabled']),
    );
    $form['general']['panes'][$id]['uc_cap_' . $id . '_weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => variable_get('uc_cap_'. $pane['id'] .'_weight', $pane['weight']),
      '#attributes' => array('class' => array('uc-cart-pane-weight')),
    );
  }

  $form['general']['uc_cart_add_item_msg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a message when a customer adds an item to their cart.'),
    '#default_value' => variable_get('uc_cart_add_item_msg', TRUE),
  );
  $form['general']['uc_add_item_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Add to cart redirect'),
    '#description' => t('Enter the page to redirect to when a customer adds an item to their cart, or &lt;none&gt; for no redirect.'),
    '#default_value' => variable_get('uc_add_item_redirect', 'cart'),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $form['general']['uc_cart_empty_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show an "Empty cart" button on the cart page.'),
    '#default_value' => variable_get('uc_cart_empty_button', FALSE),
  );

  $form['general']['uc_minimum_subtotal'] = array(
    '#type' => 'uc_price',
    '#title' => t('Minimum order subtotal'),
    '#description' => t('Customers will not be allowed to check out if the subtotal of items in their cart is less than this amount.'),
    '#default_value' => variable_get('uc_minimum_subtotal', 0),
  );

  $form['lifetime'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cart lifetime'),
    '#description' => t('Set the length of time that products remain in the cart. Cron must be running for this feature to work.'),
    '#group' => 'cart-settings',
  );

  $durations = array(
    'singular' => array(
      'minutes' => t('minute'),
      'hours' => t('hour'),
      'days' => t('day'),
      'weeks' => t('week'),
      'years' => t('year'),
    ),
    'plural' => array(
      'minutes' => t('minutes'),
      'hours' => t('hours'),
      'days' => t('days'),
      'weeks' => t('weeks'),
      'years' => t('years'),
    ),
  );

  $form['lifetime']['anonymous'] = array(
    '#type' => 'fieldset',
    '#title' => t('Anonymous users'),
    '#attributes' => array('class' => array('uc-inline-form', 'clearfix')),
  );
  $form['lifetime']['anonymous']['uc_cart_anon_duration'] = array(
    '#type' => 'select',
    '#title' => t('Duration'),
    '#options' => drupal_map_assoc(range(1, 60)),
    '#default_value' => variable_get('uc_cart_anon_duration', '4'),
  );
  $form['lifetime']['anonymous']['uc_cart_anon_unit'] = array(
    '#type' => 'select',
    '#title' => t('Units'),
    '#options' => array(
      'minutes' => t('Minute(s)'),
      'hours' => t('Hour(s)'),
      'days' => t('Day(s)'),
      'weeks' => t('Week(s)'),
      'years' => t('Year(s)'),
    ),
    '#default_value' => variable_get('uc_cart_anon_unit', 'hours'),
  );

  $form['lifetime']['authenticated'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authenticated users'),
    '#attributes' => array('class' => array('uc-inline-form', 'clearfix')),
  );
  $form['lifetime']['authenticated']['uc_cart_auth_duration'] = array(
    '#type' => 'select',
    '#title' => t('Duration'),
    '#options' => drupal_map_assoc(range(1, 60)),
    '#default_value' => variable_get('uc_cart_auth_duration', '1'),
  );
  $form['lifetime']['authenticated']['uc_cart_auth_unit'] = array(
    '#type' => 'select',
    '#title' => t('Units'),
    '#options' => array(
      'hours' => t('Hour(s)'),
      'days' => t('Day(s)'),
      'weeks' => t('Week(s)'),
      'years' => t('Year(s)'),
    ),
    '#default_value' => variable_get('uc_cart_auth_unit', 'years'),
  );

  $form['continue_shopping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Continue shopping element'),
    '#description' => t('These settings control the <em>continue shopping</em> option on the cart page.'),
    '#group' => 'cart-settings',
  );
  $form['continue_shopping']['uc_continue_shopping_type'] = array(
    '#type' => 'radios',
    '#title' => t('<em>Continue shopping</em> element'),
    '#options' => array(
      'link' => t('Text link'),
      'button' => t('Button'),
      'none' => t('Do not display'),
    ),
    '#default_value' => variable_get('uc_continue_shopping_type', 'link'),
  );
  $form['continue_shopping']['uc_continue_shopping_use_last_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make <em>continue shopping</em> go back to the last item that was added to the cart.'),
    '#description' => t('If this is disabled or the item is unavailable, the URL specified below will be used.'),
    '#default_value' => variable_get('uc_continue_shopping_use_last_url', TRUE),
  );
  $form['continue_shopping']['uc_continue_shopping_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Default <em>continue shopping</em> destination'),
    '#default_value' => variable_get('uc_continue_shopping_url', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $form['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cart breadcrumb'),
    '#description' => t('Drupal automatically adds a <em>Home</em> breadcrumb to the cart page, or you can use these settings to specify a custom breadcrumb.'),
    '#group' => 'cart-settings',
  );
  $form['breadcrumb']['uc_cart_breadcrumb_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Cart page breadcrumb text'),
    '#description' => t('Leave blank to use the default <em>Home</em> breadcrumb.'),
    '#default_value' => variable_get('uc_cart_breadcrumb_text', ''),
  );
  $form['breadcrumb']['uc_cart_breadcrumb_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Cart page breadcrumb destination'),
    '#default_value' => variable_get('uc_cart_breadcrumb_url', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  return system_settings_form($form);
}

/**
 * Form validation for uc_cart_cart_settings_form().
 *
 * @see uc_cart_cart_settings_form()
 */
function uc_cart_cart_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['uc_minimum_subtotal']) || $form_state['values']['uc_minimum_subtotal'] < 0 || $form_state['values']['uc_minimum_subtotal'] === '-0') {
    form_set_error('uc_minimum_subtotal', t('Minimum order subtotal should be a non-negative number.'));
  }
}

/**
 * General checkout settings.
 *
 * @ingroup forms
 */
function uc_cart_checkout_settings_form($form, &$form_state) {

  // Put fieldsets into vertical tabs
  $form['checkout-settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'uc_cart') . '/uc_cart.admin.js',
      ),
    ),
  );

  $form['checkout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#group' => 'checkout-settings',
  );
  $form['checkout']['uc_checkout_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable checkout.'),
    '#description' => t('Disable this to use only third party checkout services, such as PayPal Express Checkout.'),
    '#default_value' => variable_get('uc_checkout_enabled', TRUE),
  );

  $form['anonymous'] = array(
    '#type' => 'fieldset',
    '#title' => t('Anonymous checkout'),
    '#group' => 'checkout-settings',
  );
  $form['anonymous']['uc_checkout_anonymous'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable anonymous checkout.'),
    '#description' => t('Disable this to force users to log in before the checkout page.'),
    '#default_value' => variable_get('uc_checkout_anonymous', TRUE),
  );
  $anon_state = array('visible' => array('input[name="uc_checkout_anonymous"]' => array('checked' => TRUE)));
  $form['anonymous']['uc_cart_mail_existing'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow anonymous customers to use an existing account's email address."),
    '#default_value' => variable_get('uc_cart_mail_existing', TRUE),
    '#description' => t('If enabled, orders will be attached to the account matching the email address. If disabled, anonymous users using a registered email address must log in or use a different email address.'),
    '#states' => $anon_state,
  );
  $form['anonymous']['uc_cart_email_validation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require e-mail confirmation for anonymous customers.'),
    '#default_value' => variable_get('uc_cart_email_validation', FALSE),
    '#states' => $anon_state,
  );
  $form['anonymous']['uc_cart_new_account_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow new customers to specify a username.'),
    '#default_value' => variable_get('uc_cart_new_account_name', FALSE),
    '#states' => $anon_state,
  );
  $form['anonymous']['uc_cart_new_account_password'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow new customers to specify a password.'),
    '#default_value' => variable_get('uc_cart_new_account_password', FALSE),
    '#states' => $anon_state,
  );
  $form['anonymous']['uc_new_customer_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send new customers a separate e-mail with their account details.'),
    '#default_value' => variable_get('uc_new_customer_email', TRUE),
    '#states' => $anon_state,
  );
  $form['anonymous']['uc_new_customer_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log in new customers after checkout.'),
    '#default_value' => variable_get('uc_new_customer_login', FALSE),
    '#states' => $anon_state,
  );
  $form['anonymous']['uc_new_customer_status_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set new customer accounts to active.'),
    '#description' => t('Uncheck to create new accounts but make them blocked.'),
    '#default_value' => variable_get('uc_new_customer_status_active', TRUE),
    '#states' => $anon_state,
  );

  $panes = _uc_checkout_pane_list();
  $form['checkout']['panes'] = array(
    '#theme' => 'uc_pane_sort_table',
    '#pane_prefix' => 'uc_pane',
    '#draggable' => 'uc-checkout-pane-weight',
  );
  foreach ($panes as $id => $pane) {
    $form['checkout']['panes'][$id]['uc_pane_' . $id . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($pane['title']),
      '#default_value' => variable_get('uc_pane_' . $id . '_enabled', $pane['enabled']),
    );
    $form['checkout']['panes'][$id]['uc_pane_' . $id . '_weight'] = array(
      '#type' => 'weight',
      '#default_value' => variable_get('uc_pane_' . $id . '_weight', $pane['weight']),
      '#attributes' => array('class' => array('uc-checkout-pane-weight')),
    );
    $form['checkout']['panes'][$id]['#weight'] = variable_get('uc_pane_' . $id . '_weight', $pane['weight']);

    $null = NULL;
    $pane_settings = $pane['callback']('settings', $null, array());
    if (is_array($pane_settings)) {
      $form['pane_' . $id] = $pane_settings + array(
        '#type' => 'fieldset',
        '#title' => t('@pane pane', array('@pane' => $pane['title'])),
        '#group' => 'checkout-settings',
      );
    }
  }

  $form['checkout']['uc_cart_default_same_address'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the same address for billing and delivery by default.'),
    '#default_value' => variable_get('uc_cart_default_same_address', FALSE),
  );
  $form['checkout']['uc_cart_delivery_not_shippable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide delivery information when carts have no shippable items.'),
    '#default_value' => variable_get('uc_cart_delivery_not_shippable', TRUE),
  );
  $form['checkout']['uc_use_next_buttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use collapsing checkout panes with <em>Next</em> buttons.'),
    '#default_value' => variable_get('uc_use_next_buttons', FALSE),
  );
  $form['checkout']['uc_collapse_current_pane'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse pane after its <em>Next</em> button is clicked.'),
    '#default_value' => variable_get('uc_collapse_current_pane', TRUE),
    '#states' => array(
      'visible' => array(
        'input[name="uc_checkout_enabled"]' => array('checked' => TRUE),
        'input[name="uc_use_next_buttons"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instruction messages'),
    '#group' => 'checkout-settings',
  );
  $form['instructions']['uc_checkout_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout instructions'),
    '#description' => t('Provide instructions for customers at the top of the checkout screen.'),
    '#default_value' => variable_get('uc_checkout_instructions', ''),
    '#rows' => 3,
  );
  $form['instructions']['uc_checkout_review_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout review instructions'),
    '#description' => t('Provide instructions for customers at the top of the checkout review screen.'),
    '#default_value' => variable_get('uc_checkout_review_instructions', uc_get_message('review_instructions')),
    '#rows' => 3,
  );

  $form['completion_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Completion messages'),
    '#group' => 'checkout-settings',
  );
  $form['completion_messages']['uc_cart_checkout_complete_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate checkout completion page'),
    '#description' => t('Leave blank to use the default completion page (recommended).'),
    '#default_value' => variable_get('uc_cart_checkout_complete_page', ''),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    '#size' => 16,
  );
  $form['completion_messages']['uc_msg_order_submit'] = array(
    '#type' => 'textarea',
    '#title' => t('Message header'),
    '#description' => t('Header for message displayed after a user checks out.'),
    '#default_value' => variable_get('uc_msg_order_submit', uc_get_message('completion_message')),
    '#rows' => 3,
  );
  $form['completion_messages']['uc_msg_order_logged_in'] = array(
    '#type' => 'textarea',
    '#title' => t('Logged in users'),
    '#description' => t('Message displayed upon checkout for a user who is logged in.'),
    '#default_value' => variable_get('uc_msg_order_logged_in', uc_get_message('completion_logged_in')),
    '#rows' => 3,
  );
  $form['completion_messages']['uc_msg_order_existing_user'] = array(
    '#type' => 'textarea',
    '#title' => t('Existing users'),
    '#description' => t("Message displayed upon checkout for a user who has an account but wasn't logged in."),
    '#default_value' => variable_get('uc_msg_order_existing_user', uc_get_message('completion_existing_user')),
    '#rows' => 3,
    '#states' => $anon_state,
  );
  $form['completion_messages']['uc_msg_order_new_user'] = array(
    '#type' => 'textarea',
    '#title' => t('New users'),
    '#description' => t("Message displayed upon checkout for a new user whose account was just created. You may use the special tokens !new_username for the username of a newly created account and !new_password for that account's password."),
    '#default_value' => variable_get('uc_msg_order_new_user', uc_get_message('completion_new_user')),
    '#rows' => 3,
    '#states' => $anon_state,
  );
  $form['completion_messages']['uc_msg_order_new_user_logged_in'] = array(
    '#type' => 'textarea',
    '#title' => t('New logged in users'),
    '#description' => t('Message displayed upon checkout for a new user whose account was just created and also <em>"Login users when new customer accounts are created at checkout."</em> is set on the <a href="!user_login_setting_ur">checkout settings</a>.', array('!user_login_setting_ur' => 'admin/store/settings/checkout')),
    '#default_value' => variable_get('uc_msg_order_new_user_logged_in', uc_get_message('completion_new_user_logged_in')),
    '#rows' => 3,
    '#states' => $anon_state,
  );
  $form['completion_messages']['uc_msg_continue_shopping'] = array(
    '#type' => 'textarea',
    '#title' => t('Continue shopping message'),
    '#description' => t('Message displayed upon checkout to direct customers to another part of your site.'),
    '#default_value' => variable_get('uc_msg_continue_shopping', uc_get_message('continue_shopping')),
    '#rows' => 3,
  );

  if (module_exists('token')) {
    $form['completion_messages']['token_tree'] = array(
      '#markup' => theme('token_tree', array('token_types' => array('uc_order', 'site', 'store'))),
    );
  }

  return system_settings_form($form);
}

/**
 * Checkout rules configuration.
 */
function uc_cart_checkout_rules() {
  $conditions = array(
    'event' => 'uc_checkout_complete',
    'plugin' => 'reaction rule',
  );
  $options = array(
    'base path' => 'admin/store/settings/checkout/rules',
    'show plugin' => FALSE,
  );
  $content['rules'] = rules_ui()->overviewTable($conditions, $options);
  return $content;
}
