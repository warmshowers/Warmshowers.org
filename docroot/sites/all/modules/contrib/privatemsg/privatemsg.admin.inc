<?php

/**
 * @file
 * Administration menu callbacks for Privatemsg.
 */

function privatemsg_admin_settings() {
  $form = array();

  $form['display_settings'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Display settings'),
    '#weight'      => 5,
  );
  $form['display_settings']['private_message_view_template'] = array(
    '#type'          => 'radios',
    '#title'         => t('Private message display template'),
    '#default_value' => variable_get('private_message_view_template', 'privatemsg-view'),
    '#options'       => private_message_view_options(),
    '#weight'        => -10,
  );
  $form['display_settings']['privatemsg_display_loginmessage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inform the user about new messages on login'),
    '#default_value' => variable_get('privatemsg_display_loginmessage', TRUE),
    '#description' => t('This option can safely be disabled if the "New message indication" block is used instead.'),
    '#weight'      => -5,
  );
  $form['display_settings']['privatemsg_display_disabled_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inform the user on /messages pages that he can not write new messages when privatemsg is disabled.'),
    '#default_value' => variable_get('privatemsg_display_disabled_message', TRUE),
    '#description' => t('Users can (if given the permission) disable Privatemsg which disallows writing messages to them and they can not write messages themself. If enabled, those users are informed on the relevant pages why they are not allowed to write messages.'),
    '#weight'      => -8,
  );
  $form['display_settings']['privatemsg_default_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Default redirection URL'),
    '#default_value' => variable_get('privatemsg_default_redirect', '<new-message>'),
    '#description' => t('Defines to which page users are sent after sending a new message. &lt;front&gt; can be used for the defined front page and &lt;new-message&gt; to redirect to the new message. Leave empty to disable the redirect.'),
    '#weight'      => 0,
  );
  $form['display_settings']['privatemsg_default_redirect_reply'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect replies too'),
    '#default_value' => variable_get('privatemsg_default_redirect_reply', FALSE),
    '#description' => t('Also redirect to the defined page when replying.'),
    '#weight'      => 1,
  );
  $form['display_settings']['privatemsg_display_preview_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show preview button on compose form'),
    '#description' => t('If checked, displays a preview button when sending new messages.'),
    '#default_value' => variable_get('privatemsg_display_preview_button', FALSE),
    '#weight' => 5,
  );

  $form['integration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Integration settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings allow to configure the integration with other modules.'),
    '#weight' => 15,
  );
  $form['integration']['privatemsg_popups'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open "send private message" links in a popup'),
    '#default_value' => variable_get('privatemsg_popups', TRUE),
    '#disabled' => !module_exists('popups'),
    '#description' => t('This requires the <a href="http://drupal.org/project/popups">Popups API module</a>. Also, the "Scan all pages for popup links." option needs to be enabled at <a href="!popups_admin_url">Popups Settings</a>.', array('!popups_admin_url' => url('admin/settings/popups'))),
  );

  $form['flush_deleted'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Flush deleted messages'),
    '#description' => t('By default, deleted messages are only hidden from the user but still stored in the database. These settings control if and when messages should be removed.'),
    '#weight'    => 20,
  );

  $form['flush_deleted']['privatemsg_flush_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Flush deleted messages'),
    '#default_value' => variable_get('privatemsg_flush_enabled', FALSE),
    '#description'   => t('Enable the flushing of deleted messages. Requires that cron is enabled'),
  );

  $form['flush_deleted']['privatemsg_flush_days'] = array(
    '#type' => 'select',
    '#title' => t('Flush messages after they have been deleted for more days than'),
    '#default_value' => variable_get('privatemsg_flush_days', 30),
    '#options' => drupal_map_assoc(array(0, 1, 2, 5, 10, 30, 100)),
  );

  $form['flush_deleted']['privatemsg_flush_max'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of messages to flush per cron run'),
    '#default_value' => variable_get('privatemsg_flush_max', 200),
    '#options' => drupal_map_assoc(array(50, 100, 200, 500, 1000)),
  );

  $form['privatemsg_listing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure listings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -5,
  );

  $form['privatemsg_listing']['privatemsg_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Threads per page'),
    '#default_value' => variable_get('privatemsg_per_page', 25),
    '#options' => drupal_map_assoc(array(10, 25, 50, 75, 100)),
    '#description' => t('Choose the number of conversations that should be listed per page.'),
  );

  $form['privatemsg_listing']['privatemsg_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure fields'),
    '#description' => t('Select which columns/fields should be displayed in the message listings.'),
    '#collapsible' => FALSE,
    '#theme' => 'privatemsg_admin_settings_display_fields',
    'privatemsg_display_fields' => array(
      '#tree' => TRUE,
    ),
    'privatemsg_display_fields_weights' => array(
      '#tree' => TRUE,
    ),
  );

  foreach (privatemsg_get_headers() as $key => $header) {
    $form['privatemsg_listing']['privatemsg_fields']['privatemsg_display_fields'][$key] = array(
      '#type' => 'checkbox',
      '#title' => $header['#title'],
      '#return_value' => $key,
      '#default_value' => $header['#enabled'],
      '#disabled' => $header['#locked'],
      '#weight' => $header['#weight'],
    );

    $form['privatemsg_listing']['privatemsg_fields']['privatemsg_display_fields_weights'][$key] = array(
      '#type' => 'weight',
      '#title' => t('!field weight', array('!field' => $header['#title'])),
      '#default_value' => $header['#weight'],
      '#delta' => 20,
    );
  }

  $form['privatemsg_listing']['privatemsg_url_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL path'),
    '#required' => TRUE,
    '#description' => t("Specify the base URL path to be used for all Private Message pages. Note that %user can be used as a placeholder for the current user's ID. Possible paths include 'user/%user/messages' (pages appear as children of the user account page) or 'messages' (the default path)."),
    '#default_value' => variable_get('privatemsg_url_prefix', 'messages'),
    '#weight' => 20,
  );

  $amounts = drupal_map_assoc(array(5, 10, 20, 30, 50, 70, 90, 150, 200, 250, 300));
  $form['privatemsg_listing']['privatemsg_view_max_amount'] = array(
    '#type'          => 'select',
    '#title'         => t('Number of messages on thread pages'),
    '#options'       => $amounts + array(PRIVATEMSG_UNLIMITED => t('Unlimited')),
    '#default_value' => variable_get('privatemsg_view_max_amount', 20),
    '#description'   => t('Threads will not show more than this number of messages on a single page.'),
    '#weight'        => 10,
  );

  $form['privatemsg_listing']['privatemsg_view_use_max_as_default'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display different amount of messages on first thread page'),
    '#default_value' => variable_get('privatemsg_view_use_max_as_default', FALSE),
    '#description'   => t('By default, the first thread page shows the maximally allowed amount of messages. Enable this checkbox to set a different value.'),
    '#weight'        => 15,
  );

  $form['privatemsg_listing']['privatemsg_view_default_amount'] = array(
    '#prefix'        => '<div id="privatemsg-view-default-button">',
    '#suffix'        => '</div>',
    '#type'          => 'select',
    '#title'         => t('Number of messages on first thread page'),
    '#default_value' => variable_get('privatemsg_view_default_amount', 10),
    '#description'   => t('The number of messages to be displayed on first thread page. Displays the newest messages.'),
    '#options'       => $amounts,
    '#weight'        => 20,
  );

  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('"Send private message" link settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
  );

  $form['links']['privatemsg_display_link_self'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "Send this user a message" links for themself'),
    '#description' => t('If enabled, each users sees that link on their own profile, comments and similiar places.'),
    '#default_value' => variable_get('privatemsg_display_link_self', TRUE),
    '#weight'        => -10,
  );

  $form['links']['privatemsg_display_profile_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link on profile pages.'),
    '#description' => t('If this setting is enabled, a link to send a private message will be displayed.'),
    '#default_value' => variable_get('privatemsg_display_profile_links', 1),
  );

  $node_types = node_get_types('names');
  $form['links']['privatemsg_link_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display link on the selected content types'),
    '#description' => t('Select which content types should display a link to send a private message to the author. By default, the link is not displayed below teasers.'),
    '#default_value' => variable_get('privatemsg_link_node_types', array()),
    '#options' => $node_types,
  );
  $form['links']['privatemsg_display_on_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link on teasers of the selected content types.'),
    '#default_value' => variable_get('privatemsg_display_on_teaser', 1),
  );
  $form['links']['privatemsg_display_on_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display links on comments of selected content types.'),
    '#description' => t('Also display a link to send a private message to the authors of the comments of the selected content types.'),
    '#default_value' => variable_get('privatemsg_display_on_comments', 0),
  );
  drupal_add_js(drupal_get_path('module', 'privatemsg') .'/privatemsg-admin.js');

  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 50;
  $form['#submit'][] = 'privatemsg_admin_settings_submit';
  return $form;
}

/**
 * Submit handler to check if URL prefixes have changes and rebuild the menu
 * cache if this is true.
 */
function privatemsg_admin_settings_submit($form, &$form_state) {
  // Only rebuild menu if url prefix has changed.
  if ($form_state['values']['privatemsg_url_prefix'] != $form['privatemsg_listing']['privatemsg_url_prefix']['#default_value']) {
    menu_rebuild();
  }
}
