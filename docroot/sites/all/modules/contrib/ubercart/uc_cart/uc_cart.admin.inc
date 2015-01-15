<?php

/**
 * @file
 * Cart administration menu items.
 */

/**
 * Display an overview of the cart settings.
 */
function uc_cart_cart_settings_overview() {
  // Load the form summaries for pages beneath this path.
  $summaries = summarize_child_form_pages('admin/store/settings/cart/edit');

  // Build the block summary since it comes from another form.
  if (variable_get('uc_cart_show_help_text', FALSE)) {
    $help_text = t('Help text is shown in block:') .'<br /><i>'
               . variable_get('uc_cart_help_text', t('Click title to display cart contents.')) .'</i>';
  }
  else {
    $help_text = t('Help text is not shown in block.');
  }
  $summaries[] = array(
    'path' => url('admin/store/settings/cart/edit/block'),
    'href' => 'admin/store/settings/cart/edit/block',
    'title' => t('Cart block'),
    'items' => array(
      t('Cart block is !option when empty.', array('!option' => variable_get('uc_cart_block_empty_hide', FALSE) ? t('hidden') : t('shown'))),
      t('Cart block is !option by default.', array('!option' => variable_get('uc_cart_expanded', FALSE) ? t('expanded') : t('collapsed'))),
      $help_text,
    ),
  );

  // Theme it all up in a summaries overview.
  return theme('summary_overview', $summaries);
}

/**
 * General settings for the shopping cart.
 *
 * @ingroup forms
 * @see uc_cart_cart_settings_form_validate()
 */
function uc_cart_cart_settings_form() {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General cart settings'),
    '#summary callback' => 'summarize_form',
  );
  $form['general']['uc_cart_add_item_msg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display an update message when an item is added to the cart through an add to cart form.'),
    '#default_value' => variable_get('uc_cart_add_item_msg', TRUE),
  );
  $form['general']['uc_add_item_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Add to cart redirect'),
    '#description' => t('Enter the Drupal page to redirect to when a customer adds an item to their cart.<br />Enter &lt;none&gt; for no redirect.'),
    '#summary' => t('Add to cart is redirecting to <br /><a href="!url">!url</a>', array('!url' => url(variable_get('uc_add_item_redirect', 'cart'), array('absolute' => TRUE)))),
    '#default_value' => variable_get('uc_add_item_redirect', 'cart'),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $context = array(
    'revision' => 'formatted',
    'type' => 'amount',
  );
  $form['general']['uc_minimum_subtotal'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum order subtotal'),
    '#description' => t('Optionally specify a minimum allowed subtotal for a cart to proceed to checkout.'),
    '#summary' => t('The minimum subtotal is @subtotal', array('@subtotal' => uc_price(variable_get('uc_minimum_subtotal', 0), $context))),
    '#default_value' => variable_get('uc_minimum_subtotal', 0),
    '#size' => 16,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
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

  $form['anonymous'] = array(
    '#type' => 'fieldset',
    '#title' => t('Anonymous cart duration'),
    '#description' => t('Set the length of time products remain in the cart for customers who <strong>have not</strong> logged in. Cron must be running for this feature to work.'),
    '#summary' => format_plural(variable_get('uc_cart_anon_duration', '4'), 'Anonymous cart duration is 1 @unit.', 'Anonymous cart duration is @count @units.', array('@unit' => $durations['singular'][variable_get('uc_cart_anon_unit', 'hours')], '@units' => $durations['plural'][variable_get('uc_cart_anon_unit', 'hours')])),
  );
  $form['anonymous']['uc_cart_anon_duration'] = array(
    '#type' => 'select',
    '#title' => t('Duration'),
    '#options' => drupal_map_assoc(uc_range(1, 60)),
    '#default_value' => variable_get('uc_cart_anon_duration', '4'),
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );
  $form['anonymous']['uc_cart_anon_unit'] = array(
    '#type' => 'select',
    '#title' => t('Unit of time'),
    '#options' => array(
      'minutes' => t('Minute(s)'),
      'hours' => t('Hour(s)'),
      'days' => t('Day(s)'),
      'weeks' => t('Week(s)'),
      'years' => t('Year(s)'),
    ),
    '#default_value' => variable_get('uc_cart_anon_unit', 'hours'),
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );

  $form['authenticated'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authenticated cart duration'),
    '#description' => t('Set the length of time products remain in the cart for customers who <strong>have</strong> logged in. Cron must be running for this feature to work.'),
    '#summary' => format_plural(variable_get('uc_cart_auth_duration', '1'), 'Authenticated cart duration is 1 @unit.', 'Authenticated cart duration is @count @units.', array('@unit' => $durations['singular'][variable_get('uc_cart_auth_unit', 'years')], '@units' => $durations['plural'][variable_get('uc_cart_auth_unit', 'years')])),
  );
  $form['authenticated']['uc_cart_auth_duration'] = array(
    '#type' => 'select',
    '#title' => t('Duration'),
    '#options' => drupal_map_assoc(uc_range(1, 24)),
    '#default_value' => variable_get('uc_cart_auth_duration', '1'),
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );
  $form['authenticated']['uc_cart_auth_unit'] = array(
    '#type' => 'select',
    '#title' => t('Unit of time'),
    '#options' => array(
      'hours' => t('Hour(s)'),
      'days' => t('Day(s)'),
      'weeks' => t('Week(s)'),
      'years' => t('Year(s)'),
    ),
    '#default_value' => variable_get('uc_cart_auth_unit', 'years'),
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );

  $form['continue_shopping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Continue shopping element'),
    '#description' => t('Ubercart will add a continue shopping to the cart page based on the settings below.'),
    '#summary' => t('Continue shopping !type is: <br />!link', array('!type' => variable_get('uc_continue_shopping_type', 'link') == 'link' ? t('link') : t('button'), '!link' => l(variable_get('uc_continue_shopping_text', '') ? variable_get('uc_continue_shopping_text', '') : t('Continue shopping'), variable_get('uc_continue_shopping_url', '')))),
  );
  $form['continue_shopping']['uc_continue_shopping_type'] = array(
    '#type' => 'radios',
    '#title' => t('Continue shopping element display'),
    '#options' => array(
      'link' => t('Text link'),
      'button' => t('Button'),
      'none' => t('None'),
    ),
    '#default_value' => variable_get('uc_continue_shopping_type', 'link'),
  );
  $form['continue_shopping']['uc_continue_shopping_use_last_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make the continue shopping link send customers back to product pages after adding items to the cart.'),
    '#description' => t('If this is disabled, the URL specified below will always be used.'),
    '#default_value' => variable_get('uc_continue_shopping_use_last_url', TRUE),
  );
  $form['continue_shopping']['uc_continue_shopping_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Default continue shopping link URL'),
    '#description' => t('Enter the Drupal page for the link to continue shopping from the cart view page.'),
    '#default_value' => variable_get('uc_continue_shopping_url', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );
  $form['continue_shopping']['uc_continue_shopping_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom continue shopping link text'),
    '#description' => t('Enter custom text for the continue shopping link.'),
    '#default_value' => variable_get('uc_continue_shopping_text', ''),
  );

  $form['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cart breadcrumb'),
    '#description' => t('Drupal automatically adds a Home breadcrumb to the cart page.  You can use these settings to specify a custom breadcrumb to use on the cart page instead.  If a custom text is not supplied, the default Home breadcrumb will be displayed.'),
    '#summary' => variable_get('uc_cart_breadcrumb_text', '') ? t('Custom cart breadcrumb is: <br />!breadcrumb', array('!breadcrumb' => l(variable_get('uc_cart_breadcrumb_text', ''), variable_get('uc_cart_breadcrumb_url', '')))) : t('Not using a custom cart breadcrumb.'),
  );
  $form['breadcrumb']['uc_cart_breadcrumb_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom cart breadcrumb text'),
    '#description' => t('Enter the text for the custom breadcrumb on the cart page.'),
    '#default_value' => variable_get('uc_cart_breadcrumb_text', ''),
  );
  $form['breadcrumb']['uc_cart_breadcrumb_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom cart breadcrumb URL'),
    '#description' => t('Enter the Drupal page linked to in the custom breadcrumb on the cart view page.'),
    '#default_value' => variable_get('uc_cart_breadcrumb_url', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  return system_settings_form($form);
}

/**
 * @see uc_cart_cart_settings_form()
 */
function uc_cart_cart_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['uc_minimum_subtotal']) || $form_state['values']['uc_minimum_subtotal'] < 0 || $form_state['values']['uc_minimum_subtotal'] === '-0') {
    form_set_error('uc_minimum_subtotal', t('Minimum order subtotal should be a non-negative number.'));
  }
}

/**
 * Settings for panes on the cart page.
 *
 * @ingroup forms
 */
function uc_cart_cart_panes_form() {
  $panes = uc_cart_cart_pane_list(NULL);

  $form['panes'] = array(
    '#theme' => 'uc_pane_sort_table',
    '#pane_prefix' => 'uc_cap',
    '#summary callback' => '_uc_cart_panes_summarize',
    '#summary arguments' => array($panes),
    '#draggable' => 'uc-cart-pane-weight',
  );
  foreach ($panes as $pane) {
    $form['panes'][$pane['id']]['uc_cap_'. $pane['id'] .'_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => $pane['title'],
      '#default_value' => variable_get('uc_cap_'. $pane['id'] .'_enabled', $pane['enabled']),
    );
    $form['panes'][$pane['id']]['uc_cap_'. $pane['id'] .'_weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => variable_get('uc_cap_'. $pane['id'] .'_weight', $pane['weight']),
      '#attributes' => array('class' => 'uc-cart-pane-weight'),
    );
  }

  return system_settings_form($form);
}

/**
 * Helpful message redirecting users to the block settings.
 */
function uc_cart_block_edit_info() {
  $output = t('Drupal handles all the block settings forms automatically.') .'<p>'
          . t('<a href="!url">Click here</a> to go to the shopping cart block configuration page.',
              array('!url' => url('admin/build/block/configure/uc_cart/0')));

  return $output;
}

/**
 * Display an overview of the checkout settings.
 */
function uc_cart_checkout_settings_overview() {
  // Theme all the pages beneath this path into summary overviews.
  return theme('summary_overview', summarize_child_form_pages('admin/store/settings/checkout/edit'));
}

/**
 * General checkout settings.
 *
 * @ingroup forms
 */
function uc_cart_checkout_settings_form() {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General checkout settings'),
    '#summary callback' => 'summarize_form',
  );
  $form['general']['uc_checkout_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable checkout (disable to only use third party checkout service like PayPal Express Checkout).'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Ubercart checkout is enabled.'),
      t('Ubercart checkout is disabled.'),
    ),
    '#default_value' => variable_get('uc_checkout_enabled', TRUE),
  );
  $form['general']['uc_checkout_anonymous'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable anonymous checkout (users can checkout without logging in).'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Anonymous checkout is enabled.'),
      t('Anonymous checkout is disabled.'),
    ),
    '#default_value' => variable_get('uc_checkout_anonymous', TRUE),
  );
  $form['general']['uc_cart_delivery_not_shippable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide shipping information when possible for carts with no shippable items.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Shipping fields are hidden when applicable.'),
      t('Shipping fields are always shown.'),
    ),
    '#default_value' => variable_get('uc_cart_delivery_not_shippable', TRUE),
  );

  $form['pane_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checkout pane display options'),
    '#summary callback' => 'summarize_form',
  );
  $form['pane_settings']['uc_use_next_buttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use collapsing checkout panes with next buttons during checkout.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Checkout panes are collapsed with next buttons.'),
      t('Checkout panes are expanded by default.'),
    ),
    '#default_value' => variable_get('uc_use_next_buttons', FALSE),
  );
  $form['pane_settings']['uc_collapse_current_pane'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse a pane when its next button is clicked.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Collapsible panes will collapse when their next buttons are clicked.'),
      t('Collapsible panes will not collapse when their next buttons are clicked.'),
    ),
    '#default_value' => variable_get('uc_collapse_current_pane', TRUE),
  );

  $form['completion'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checkout completion settings'),
    '#summary callback' => 'summarize_form',
  );
  $form['completion']['uc_new_customer_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send new customers a separate e-mail with their account details.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('New customers receive an e-mail with their account details.'),
      t('New customers will only see their details in their initial order e-mail.')
    ),
    '#default_value' => variable_get('uc_new_customer_email', TRUE),
  );
  $form['completion']['uc_new_customer_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Login users when new customer accounts are created at checkout.'),
    '#summary callback' => 'summarize_null',
    '#default_value' => variable_get('uc_new_customer_login', FALSE),
  );
  $form['completion']['uc_new_customer_status_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('New customer accounts will be set to active.'),
    '#description' => t('Uncheck to create new accounts but make them blocked.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('New customer account status will be active.'),
      t('New customer account status will be blocked.'),
    ),
    '#default_value' => variable_get('uc_new_customer_status_active', TRUE),
  );
  $form['completion']['uc_cart_checkout_complete_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate checkout completion page'),
    '#description' => t('Leave blank to use the default completion page (recommended).'),
    '#summary' => variable_get('uc_cart_checkout_complete_page', '') ? t('Checkout completion page has been set to <a href="!url">!url</a>.', array('!url' => variable_get('uc_cart_checkout_complete_page', ''))) : t('Checkout completion page will be the default page.'),
    '#default_value' => variable_get('uc_cart_checkout_complete_page', ''),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    '#size' => 16,
  );

  return system_settings_form($form);
}

/**
 * Summarize cart and checkout panes.
 *
 * @param $form
 *   The form passed from the summarizer
 * @param $panes
 *   Any array of cart/checkout panes
 * @return
 *   An array of summary information
 *
 * Although this function is called 'cart_panes', it also operates on
 * checkout panes, which are structured the same way.
 */
function _uc_cart_panes_summarize($form, $panes) {
  $items = array();

  foreach ($panes as $pane) {
    $items[] = t('@title is @enabled.', array('@title' => $pane['title'], '@enabled' => $pane['enabled'] ? t('enabled') : t('disabled')));
  }

  return $items;
}

/**
 * Settings for panes on the checkout page.
 *
 * @ingroup forms
 */
function uc_cart_checkout_panes_form() {
  $panes = _checkout_pane_list();

  $form['panes'] = array(
    '#theme' => 'uc_pane_sort_table',
    '#pane_prefix' => 'uc_pane',
    '#summary callback' => '_uc_cart_panes_summarize',
    '#summary arguments' => array($panes),
    '#draggable' => 'uc-checkout-pane-weight',
  );
  foreach ($panes as $pane) {
    $form['panes'][$pane['id']]['uc_pane_'. $pane['id'] .'_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => $pane['title'],
      '#default_value' => variable_get('uc_pane_'. $pane['id'] .'_enabled', TRUE),
    );
    $form['panes'][$pane['id']]['uc_pane_'. $pane['id'] .'_weight'] = array(
      '#type' => 'weight',
      '#default_value' => variable_get('uc_pane_'. $pane['id'] .'_weight', $pane['weight']),
      '#attributes' => array('class' => 'uc-checkout-pane-weight'),
    );
    $form['panes'][$pane['id']]['#weight'] = variable_get('uc_pane_'. $pane['id'] .'_weight', $pane['weight']);

    $null = NULL;
    $pane_settings = $pane['callback']('settings', $null, NULL);
    if (is_array($pane_settings)) {
      $form['pane_'. $pane['id']] = array(
        '#type' => 'fieldset',
        '#summary callback' => 'summarize_null',
        '#title' => t('!pane settings', array('!pane' => $pane['title'])),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['pane_'. $pane['id']] = array_merge($form['pane_'. $pane['id']], $pane_settings);
    }
  }

  return system_settings_form($form);
}

/**
 * Settings for help messages displayed on the checkout page.
 *
 * @ingroup forms
 */
function uc_cart_checkout_messages_form() {
  $form['uc_checkout_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout instructions'),
    '#description' => t('Provide instructions for customers at the top of the checkout screen.'),
    '#summary' => variable_get('uc_checkout_instructions', '') ? t('Checkout instructions are set.') : t('Checkout instructions are not set.'),
    '#default_value' => variable_get('uc_checkout_instructions', ''),
    '#rows' => 3,
  );
  $form['uc_checkout_instructions_format'] = filter_form(variable_get('uc_checkout_instructions_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_checkout_instructions_format'));

  $form['uc_checkout_review_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout review instructions'),
    '#description' => t('Provide instructions for customers at the top of the checkout review screen.'),
    '#summary' => variable_get('uc_checkout_review_instructions', uc_get_message('review_instructions'))
               ? t('Review instructions are set.')
               : t('Review instructions are not set.'),
    '#default_value' => variable_get('uc_checkout_review_instructions', uc_get_message('review_instructions')),
    '#rows' => 3,
  );
  $form['uc_checkout_review_instructions_format'] = filter_form(variable_get('uc_checkout_review_instructions_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_checkout_review_instructions_format'));

  $form['uc_msg_order_submit'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout completion message header'),
    '#description' => t('Header for message displayed after a user checks out. <a href="!url">Uses order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'))),
    '#summary' => variable_get('uc_msg_order_submit', uc_get_message('completion_message'))
               ? t('Completion message is set.')
               : t('Completion message is not set.'),
    '#default_value' => variable_get('uc_msg_order_submit', uc_get_message('completion_message')),
    '#rows' => 3,
  );
  $form['uc_msg_order_submit_format'] = filter_form(variable_get('uc_msg_order_submit_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_msg_order_submit_format'));

  $form['checkout_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checkout completion message body'),
    '#description' => t("In the following three boxes, you may use the special tokens !new_username for the username of a newly created account and !new_password for that account's password."),
    '#summary callback' => 'summarize_form',
    '#collapsible' => FALSE,
  );
  $form['checkout_messages']['uc_msg_order_logged_in'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout completion for logged-in users'),
    '#description' => t('Message displayed upon checkout for a user who has logged in. <a href="!url">Uses order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'))),
    '#summary' => variable_get('uc_msg_order_logged_in', uc_get_message('completion_logged_in'))
               ? t('Completion text for logged in users is set.')
               : t('Completion text for logged in users is not set.'),
    '#default_value' => variable_get('uc_msg_order_logged_in', uc_get_message('completion_logged_in')),
    '#rows' => 3,
  );
  $form['checkout_messages']['uc_msg_order_logged_in_format'] = filter_form(variable_get('uc_msg_order_logged_in_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_msg_order_logged_in_format'));

  $form['checkout_messages']['uc_msg_order_existing_user'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout completion for existing users'),
    '#description' => t("Message displayed upon checkout for a user who has an account but wasn't logged in. <a href=\"!url\">Uses order and global tokens</a>.", array('!url' => url('admin/store/help/tokens'))),
    '#summary' => variable_get('uc_msg_order_existing_user', uc_get_message('completion_existing_user'))
               ? t("Completion text for users who aren't logged in is set.")
               : t("Completion text for users who aren't logged in is not set."),

    '#default_value' => variable_get('uc_msg_order_existing_user', uc_get_message('completion_existing_user')),
    '#rows' => 3,
  );
  $form['checkout_messages']['uc_msg_order_existing_user_format'] = filter_form(variable_get('uc_msg_order_existing_user_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_msg_order_existing_user_format'));

  $form['checkout_messages']['uc_msg_order_new_user'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout completion for new users'),
    '#description' => t('Message displayed upon checkout for a new user whose account was just created. <a href="!url">Uses order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'))),
    '#summary' => variable_get('uc_msg_order_new_user', uc_get_message('completion_new_user'))
               ? t('Completion text for new users is set.')
               : t('Completion text for new users is not set.'),
    '#default_value' => variable_get('uc_msg_order_new_user', uc_get_message('completion_new_user')),
    '#rows' => 3,
  );
  $form['checkout_messages']['uc_msg_order_new_user_format'] = filter_form(variable_get('uc_msg_order_new_user_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_msg_order_new_user_format'));

  $form['checkout_messages']['uc_msg_order_new_user_logged_in'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout completion for new logged in users'),
    '#description' => t('Message displayed upon checkout for a new user whose account was just created and also <em>"Login users when new customer accounts are created at checkout."</em> is set on the <a href="!user_login_setting_ur">checkout settings</a>. <a href="!url">Uses order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'), '!user_login_setting_ur' => 'admin/store/settings/checkout/edit/basic')),
    '#summary' => variable_get('uc_msg_order_new_user_logged_in', uc_get_message('completion_new_user_logged_in'))
               ? t('Completion text for new logged in users is set.')
               : t('Completion text for new logged in users is not set.'),
    '#default_value' => variable_get('uc_msg_order_new_user_logged_in', uc_get_message('completion_new_user_logged_in')),
    '#rows' => 3,
  );
  $form['checkout_messages']['uc_msg_order_new_user_logged_in_format'] = filter_form(variable_get('uc_msg_order_new_user_logged_in_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_msg_order_new_user_logged_in_format'));


  $form['uc_msg_continue_shopping'] = array(
    '#type' => 'textarea',
    '#title' => t('Continue shopping message'),
    '#description' => t('Message displayed upon checkout to direct customers to another part of your site. <a href="!url">Uses order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'))),
    '#summary' => variable_get('uc_msg_continue_shopping', uc_get_message('continue_shopping'))
               ? t('Continue shopping text is set.')
               : t('Continue shopping text is not set.'),
    '#default_value' => variable_get('uc_msg_continue_shopping', uc_get_message('continue_shopping')),
    '#rows' => 3,
  );
  $form['uc_msg_continue_shopping_format'] = filter_form(variable_get('uc_msg_continue_shopping_format', FILTER_FORMAT_DEFAULT), NULL, array('uc_msg_continue_shopping_format'));

  return system_settings_form($form);
}
