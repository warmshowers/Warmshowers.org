<?php

/**
 * @file
 * Implementation of i18n hooks for Ubercart orders.
 */


/**
 * Implements hook_i18n_string_info().
 */
function uc_order_i18n_string_info() {
  $groups['uc_order_status'] = array(
    'title' => t('Ubercart order status'),
    'description' => t('Translatable Ubercart order statuses'),
    'format' => FALSE,
    'list' => TRUE,
  );
  return $groups;
}

/**
 * Refreshes translated order statuses.
 */
function uc_order_i18n_string_refresh() {
  $statuses = db_query("SELECT order_status_id, title FROM {uc_order_statuses}");
  foreach ($statuses as $status) {
    i18n_string_update('uc_order_status:status:' . $status->order_status_id . ':title', $status->title);
  }

  return TRUE;
}
