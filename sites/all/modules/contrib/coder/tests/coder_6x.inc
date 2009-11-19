<?php
// $Id: coder_6x.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * This file implements tests for the Drupal 6.x upgrade.
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

function this_is_not_a_menu_hook() {
  $items = array();
  $items[] = array(); // This should not generate an error.
  return $items;
}

function _coder_6x_l_tests() {
  $link = l(t($display_vars['back_to_top']), 'faq', array('fragment' => '')); // this is ok
  $link = l(t($display_vars['back_to_top']), 'faq', NULL, NULL); // this is not ok
  $output = l($image, $item['filepath'], $link_attributes, NULL, NULL, FALSE, TRUE); // Not ok.
}

function _coder_6x_more_tests() {
  if (taxonomy_node_get_terms($nid)) { // Should generate an error.
  }
  if (taxonomy_node_get_terms($node->nid)) { // Should generate an error.
  }
  if (taxonomy_node_get_terms($node)) { // Not an error.
  }
  $output = strtr(format_plural($num, 'There is currently 1 %type post on your site.', 'There are currently @count %type posts on your site.'), array('%type' => theme('placeholder', $type))); // a valid error
  $output = notstrtr(format_plural($num, 'There is currently 1 %type post on your site.', 'There are currently @count %type posts on your site.'), array('%type' => theme('placeholder', $type))); // not an error

  watchdog('user', t('Removed %username user.', array('%username' => $user->name))); // An error.
  watchdog('user', 'Removed %username user.', array('%username' => $user->name)); // Not an error.
  watchdog('debug', 'My debug message here'); // Not ok.

  $complex = unserialize(cache_get('complex_cid')); // An error.
  cache_set('simple_cid', 'cache', $simple); // An error.
  $ip = $_SERVER['REMOTE_ADDR']; // An error.

  if ($file = file_check_upload('picture_upload')) { // An error.
    $file = file_save_upload('picture_upload', $destination, FILE_EXISTS_REPLACE); // An error.
    $file = file_save_upload('picture_upload', $dest); // An error.
    $file = file_save_upload('picture_upload', 'abc'); // An error.
    $file = file_save_upload('picture_upload', array('foo')); // Ok.
  }

  if ($file = file_save_upload('picture_upload', $validators, $dest)) { // Ok.
  }

  $sql = 'UPDATE {file_revisions} SET vid=1'; // An error.
  if (db_query('SELECT * FROM {file_revisions}')) { // An error.
  }

  // Add FAPI tests.
  $form = array(
    '#base' => 'my_shared_form_id', // An error.
    '#pre_render' => '<div>', // An error.
  );
  $form['#base'] = 'http://example.com';
  $form['#submit']['my_submit_function'] = array($param1, $param2); // An error.
  $form['#submit']['my_validate_function'] = array(); // An error.
  $form['#multistep'] = TRUE;
  $form['#DANGEROUS_SKIP_CHECK'] = TRUE;

  drupal_retrieve_form('testform'); // Missing second arg.
  drupal_retrieve_form($form_id, $form_state); // Ok.

  form_set_value($element, 'value', $form_status); // Ok.
  form_set_value($element, 'value'); // An error.

  confirm_form($form, t('Do you really want to delete this?'), "node/$nid", t("Don't do it unless you're really sure!"), t('Delete it'), t('Go back'), 'delete'); // Should fail.

  confirm_form($form, t('Do you really want to delete this?'), "node/$nid", $options); // Ok.
  confirm_form($form, t('Do you really want to delete this?'), "node/$nid", array()); // Ok.

  custom_url_rewrite($url); // An error.
  custom_url_rewrite_inbound($url); // Ok.

  $form_location = variable_get('comment_form_location', COMMENT_FORM_SEPARATE_PAGE); // An error.
  $form_location = variable_get('comment_form_location_page', COMMENT_FORM_SEPARATE_PAGE); // Ok.
  $variable_name = 'comment_form_location'.'_abc'; // Ok.

  $log_path = 'admin/logs/';

  $permission = user_access('administer access control');
  $user_access_path = 'admin/user/access';

  locale_refresh_cache();

  db_next_id();

  menu_set_location();

  taxonomy_get_vocabulary($vid);

  db_result($result, $row);
  db_result($result); // Ok.
  $number = db_result(db_query("SELECT COUNT(*) FROM {flood} WHERE event = '%s' AND hostname = '%s' AND timestamp > %d", $name, ip_address(), time() - 3600)); // Ok.


  // Book api tests.
  book_admin_orphan(); // Deprecated.
  book_content(); // Deprecated.
  book_form(); // Deprecated.
  book_insert(); // Deprecated.
  book_location(); // Deprecated.
  book_location_down(); // Deprecated.
  book_node_visitor_html_post(); // Deprecated.
  book_node_visitor_html_pre(); // Deprecated.
  book_recurse(); // Deprecated.
  book_toc_recurse(); // Deprecated.
  book_tree(); // Deprecated.
  book_tree_recurse(); // Deprecated.

  book_admin_edit($nid); // Not ok.
  book_admin_edit($form_state, $node); // Ok.

  book_toc(); // Not ok.
  book_toc($exclude); // Not ok.
  book_toc($bid, array(), $depth); // Ok.

  book_export_html($nid, $depth); // Not ok.
  book_export_html($nid); // Ok.

  book_export(); // Not ok.
  book_export($type); // Not ok.
  book_export($type, $nid); // Ok.

  book_outline($nid); // Not ok.
  book_outline($node->nid); // Not ok.
  book_outline($node); // Ok.

  book_prev($node); // Not ok.
  book_prev($book_link); // Ok.
  book_next($node); // Not ok.
  book_next($book_link); // Ok.
  // End book api tests.

  drupal_mail($action, $to, $subject, $body, $from); // Not ok.
  drupal_mail('action_email', $recipient, $subject, $body, $from); // Not ok.
  drupal_mail('action_email', $account->mail, $subject, $body, $from); // Not ok.
  drupal_mail('action_email', 'foo@foo.com', $subject, $body, $from); // Not ok.
  drupal_mail('system', 'action_email', $recipient, $language, $params); // Ok.

  user_authenticate($name, $pass); // Not ok.
  user_authenticate($array); // Ok.
  user_authenticate(array('name' => $name, 'pass' => $pass)); // Ok.

  $node = node_load(arg(1)); // Not ok.

}

function _coder_6x_test_link_alter($node, &$links) { // An error.
}

function _coder_6x_test2_link_alter(&$links, $node) { // Not an error.
}

function _coder_6x_test_mail_alter(&$mailkey, &$to, &$subject, &$body, &$from, &$headers) { // An error.
}

function _coder_6x_test2_mail_alter(&$message) { // An error.
}

function coder_node_get_name() {
  node_get_name('name');
}

function myform_validate($form_id, $form) { // This will fail.
}

function myform_prerender($form_id, $form) { // This will fail.
}

function myform_submit($form_id, $form) { // This will fail.
  $form['#submit']['newsubmit'] = array();
  $form['#submit'][] = "newsubmit";
  return $foo; // Not ok.
  return 'my/path'; // Not ok.
}

function mymodule_info($field = 0) {
}

function mymodule_auth($username, $password, $server) {
}

function mymodule_help($section) { // Not ok.
}

function mymodule_help($section, $arg) { // Ok.
}

function mymodule_enable() {
  node_access_rebuild(); // Not ok in node access modules.
}

function mymodule_disable() {
  node_access_rebuild(); // Not ok in node access modules.
}

function mymodule_access($op, $node) { // Not ok.
}

function coder_6x_tests_menu() {
  $items = array();

  $items['test_good'] = array(
    'title' => 'My title', // Ok.
    'description' => 'My description', // Ok.
  );

  $items['test_bad'] = array(
    'title' => t('My title'), // Not Ok.
    'description' => t('My description'), // Not Ok.
  );

}
