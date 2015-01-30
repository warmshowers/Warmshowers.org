<?php

/**
 * @file
 * Roles administration menu items.
 */

/**
 * Creates the header for the table/pager.
 */
function _uc_roles_expiration_header() {
  return array(
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Role'), 'field' => 'e.rid'),
    array('data' => t('Expiration date'), 'field' => 'e.expiration', 'sort' => 'asc'),
    t('Operations')
  );
}

/**
 * Menu callback for viewing expirations.
 */
function uc_roles_expiration() {
  // Create the header for the pager.
  $header = _uc_roles_expiration_header();

  // Grab all the info to build the pager.
  $sql = 'SELECT * FROM {uc_roles_expirations} AS e INNER JOIN {users} AS u ON e.uid = u.uid';
  $sql .= tablesort_sql($header);
  $result = pager_query($sql, 50, 0, NULL);

  // Stick the expirations into the form.
  while ($row = db_fetch_object($result)) {
    $form['name'][$row->uid .' '. $row->rid] = array('#value' => theme('username', $row));
    $form['role'][$row->uid .' '. $row->rid] = array('#value' => check_plain(_uc_roles_get_name($row->rid)));
    $form['expiration'][$row->uid .' '. $row->rid] = array('#value' => format_date($row->expiration, 'small'));
    $form['operations'][$row->uid .' '. $row->rid] = array('#value' => l(t('delete'), 'admin/user/user/expiration/delete/'. $row->uid .'/'. $row->rid) .' '. l(t('edit'), 'user/'. $row->uid .'/edit', array('fragment' => 'role-expiration-'. $row->rid, 'query' => 'destination=admin%2Fuser%2Fuser%2Fexpiration')));
  }

  return $form;
}

/**
 * Themes user role expiration page.
 *
 * @ingroup themeable
 */
function theme_uc_roles_expiration($form) {
  $header = _uc_roles_expiration_header();

  if (is_array($form['name'])) {
    foreach (element_children($form['name']) as $key) {
      $rows[] = array(
        drupal_render($form['name'][$key]),
        drupal_render($form['role'][$key]),
        drupal_render($form['expiration'][$key]),
        drupal_render($form['operations'][$key]),
      );
    }
  }
  else  {
    $rows[] = array(array('data' => t('No expirations set to occur'), 'colspan' => '4'));
  }

  // Render everything.
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, 50, 0);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Form builder for role expirations.
 *
 * @see uc_roles_deletion_form_submit()
 * @ingroup forms
 */
function uc_roles_deletion_form($form_id, $account, $rid) {
  $expiration = db_result(db_query("SELECT expiration FROM {uc_roles_expirations} WHERE uid = %d AND rid = %d", $account->uid, $rid));
  if ($expiration) {

    $role_name = _uc_roles_get_name($rid);

    $form['user'] = array('#type' => 'value', '#value' => $account->name);
    $form['uid'] = array('#type' => 'value', '#value' => $account->uid);
    $form['role'] = array('#type' => 'value', '#value' => $role_name);
    $form['rid'] = array('#type' => 'value', '#value' => $rid);

    $form = confirm_form(
      $form, t('Delete expiration of %role_name role for the user %user_name?', array('%user_name' => $account->name, '%role_name' => $role_name)),
      'admin/user/user/expiration',
      t('Deleting the expiration will give %user_name privileges set by the %role_name role indefinitely unless manually removed.', array('%user_name' => $account->name, '%role_name' => $role_name)),
      t('Yes'), t('No')
    );
  }
  else {
    $form['error'] = array(
      '#type' => 'markup',
      '#value' => t('Invalid user id or role id.'),
    );
  }

  return $form;
}

/**
 * Submission handler for uc_roles_deletion_form().
 *
 * @see uc_roles_deletion_form()
 */
function uc_roles_deletion_form_submit($form, &$form_state) {
  uc_roles_delete(user_load($form_state['values']['uid']), $form_state['values']['rid']);

  drupal_goto('admin/user/user/expiration');
}
