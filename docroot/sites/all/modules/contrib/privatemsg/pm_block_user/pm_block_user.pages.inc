<?php

/**
 * @file
 * User menu callbacks for pm_block_user.module.
 */

function pm_block_user_form($form_state, $author) {
  global $user;

  $form['author'] = array(
    '#type' => 'value',
    '#value' => $author,
  );
    $form['recipient'] = array(
    '#type' => 'value',
    '#value' => $user,
  );
  $form['destination'] = array(
    '#type' => 'value',
    '#value' => isset($_GET['destination']) ? $_GET['destination'] : privatemsg_get_dynamic_url_prefix(),
  );

  if (pm_block_user_has_blocked($author, $user)) {
    $form['block_action'] = array(
      '#type' => 'value',
      '#value' => 'unblock_user',
    );
    return confirm_form($form,
      t('You have previously blocked "@author" from sending you any more messages. Are you sure you want to unblock this user?', array('@author' => privatemsg_recipient_format($author, array('plain' => TRUE)))),
      isset($_GET['destination']) ? $_GET['destination'] : privatemsg_get_dynamic_url_prefix(),
      '',
      t('Unblock @author', array('@author' => privatemsg_recipient_format($author, array('plain' => TRUE)))),
      t('Cancel')
    );
  }
  else {
    $form['block_action'] = array(
      '#type' => 'value',
      '#value' => 'block_user',
    );
    return confirm_form($form,
      t('Are you sure you want to block "@author" from sending you any more messages?', array('@author' => privatemsg_recipient_format($author, array('plain' => TRUE)))),
      isset($_GET['destination']) ? $_GET['destination'] : privatemsg_get_dynamic_url_prefix(),
      '',
      t('Block @author', array('@author' => privatemsg_recipient_format($author, array('plain' => TRUE)))),
      t('Cancel')
    );
  }
}

/**
 * Submit callback for block user confirmation form.
 */
function pm_block_user_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    switch ($form_state['values']['block_action']) {
      case 'block_user':
        db_query('INSERT INTO {pm_block_user} (author, recipient) VALUES (%d, %d)', $form_state['values']['author']->uid, $form_state['values']['recipient']->uid);
        drupal_set_message(t('@author has been blocked from sending you any further messages.', array('@author' => privatemsg_recipient_format($form_state['values']['author'], array('plain' => TRUE)))));
      break;
      case 'unblock_user':
        db_query('DELETE FROM {pm_block_user} WHERE author = %d AND recipient = %d', $form_state['values']['author']->uid, $form_state['values']['recipient']->uid);
        drupal_set_message(t('@author is now allowed to send you new messages.', array('@author' => privatemsg_recipient_format($form_state['values']['author'], array('plain' => TRUE)))));
      break;
    }
  }
  $form_state['redirect'] = $form_state['values']['destination'];
}

/**
 * Formbuilder function to build a simple form to block users.
 */
function pm_block_user_list() {
  global $user;

  $form['new'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Block a user'),
  );
  $form['new']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#autocomplete_path' => 'messages/user/autocomplete',
    '#description'        => t('Separate multiple names with commas.'),
    '#required'          => TRUE,
  );
  $form['new']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Block user'),
    '#validate' => array('pm_block_user_block_validate'),
    '#submit' => array('pm_block_user_block_submit'),
  );

  $header = array(
    array(
      'data' => t('Username'),
      'field' => 'u.name',
      'sort'  => 'asc',
      ),
    array(
      'data' => t('Operations'),
      ),
  );

  $result = pager_query('SELECT * FROM {pm_block_user} pmb INNER JOIN {users} u ON (u.uid = pmb.author) WHERE pmb.recipient = %d ' . tablesort_sql($header), 20, 0, NULL, $user->uid);
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      theme('username', privatemsg_user_load($row->author)),
      l(t('unblock'), 'messages/block/' . $row->author, array('query' => drupal_get_destination())),
    );
  }
  $form['#header'] = $header;
  $form['#rows'] = $rows;
  return $form;
}

/**
 * Validate user names.
 */
function pm_block_user_block_validate($form, &$form_state) {
  global $user;
  list($accounts, $invalid) = _privatemsg_parse_userstring($form_state['values']['name'], array('user'));
  // Remove acocunts that can not be blocked.
  if (!empty($accounts)) {
    foreach ($accounts as $id => $account) {

      // Only authors can be blocked.
      if ($account->type != 'user') {
        drupal_set_message(t('Only users can be blocked.'));
        unset($accounts[$id]);
        continue;
      }

      // Check if the user can not be blocked because of a rule.
      if (_pm_block_user_rule_exists($account, $user, PM_BLOCK_USER_DISALLOW_BLOCKING)) {
        drupal_set_message(t('You are not allowed to block !account.', array('!account' => theme('username', $account))), 'warning');
        unset($accounts[$id]);
        continue;
      }
      // Check if the user is already blocked.
      if (pm_block_user_has_blocked($account, $user)) {
        drupal_set_message(t('You have already blocked !account.', array('!account' => theme('username', $account))), 'warning');
        unset($accounts[$id]);
        continue;
      }
      // Do not allow users to block themself.
      if ($user->uid == $account->uid) {
        drupal_set_message(t('You can not block yourself.'), 'warning');
        unset($accounts[$id]);
        continue;
      }
    }
  }
  // Display warning about invalid user names.
  if (!empty($invalid)) {
    drupal_set_message(t('The following users do not exist: @invalid.', array('@invalid' => implode(", ", $invalid))), 'warning');
  }
  // If there are no accounts left, display error.
  if (empty($accounts)) {
    form_set_error('name', t('You are either not allowed to block these users or the users do not exist.'));
  }
  else {
    $form_state['valid_accounts'] = $accounts;
  }
}

/**
 * Submit callback for block user form.
 */
function pm_block_user_block_submit($form, &$form_state) {
  global $user;
  foreach ($form_state['valid_accounts'] as $account) {
    db_query('INSERT INTO {pm_block_user} (author, recipient) VALUES (%d, %d)', $account->uid, $user->uid);
    drupal_set_message(t('!author has been blocked from sending you any further messages.', array('!author' => theme('username', $account))));
  }
}

/**
 * Theme function to theme the blocked user listing.
 */
function theme_pm_block_user_list($form) {
  return drupal_render($form) . theme('table', $form['#header'], $form['#rows']) . theme('pager');
}