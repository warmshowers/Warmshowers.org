<?php
// $Id: user_delete.admin.inc,v 1.1.2.4 2010/01/12 07:44:29 sanduhrs Exp $

/**
 * @file
 * User delete - Administration page
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Administrative settings page
 *
 * @return array
 *   a form array
 */
function user_delete_settings() {
  //TODO: add additional settings based on http://drupal.org/node/8#comment-628434
  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Defaults'),
  );
  $form['defaults']['user_delete_default_action'] = array(
    '#type' => 'select',
    '#title' => t('Default action when deleting'),
    '#default_value' => variable_get('user_delete_default_action', 0),
    '#options' => array(
        0 => t('Let users choose action'),
        'user_delete_block' => t('Disable the account and keep all content.'),
        'user_delete_block_unpublish' => t('Disable the account and unpublish all content.'),
        'user_delete_reassign' => t('Delete the account and make all content belong to the <em>Anonymous user</em>.'),
        'user_delete_delete' => t('Delete the account and all content.'),
      ),
  );
  $form['redirect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirect'),
  );
  $form['redirect']['user_delete_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirection page'),
    '#default_value' => variable_get('user_delete_redirect', ''),
    '#description' => t('Choose where to redirect your users after account deletion. Any valid Drupal path will do, e.g. %front or %node', array('%front' => '<front>', '%node' => 'node/1')),
  );
  $form['backup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backup'),
  );
  $form['backup']['user_delete_backup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Backup data'),
    '#default_value' => variable_get('user_delete_backup', 0),
    '#description' => t('Backup data that is being deleted to the filesystem.'),
  );
  $options = array(
    60*60*24*7 => format_interval(60*60*24*7, 2),
    60*60*24*7*2 => format_interval(60*60*24*7*2, 2),
    60*60*24*7*4 => format_interval(60*60*24*7*4, 2),
    60*60*24*7*8 => format_interval(60*60*24*7*8, 2),
    60*60*24*7*12 => format_interval(60*60*24*7*12, 2),
    60*60*24*7*16 => format_interval(60*60*24*7*16, 2),
    60*60*24*7*24 => format_interval(60*60*24*7*24, 2),
  );
  $form['backup']['user_delete_backup_period'] = array(
    '#type' => 'select',
    '#title' => t('Keep backup time'),
    '#default_value' => variable_get('user_delete_backup_period', 60*60*24*7*12),
    '#options' => $options,
    '#description' => t('The time frame after which the backup should be deleted from the filesystem.'),
  );
  return system_settings_form($form);
}
