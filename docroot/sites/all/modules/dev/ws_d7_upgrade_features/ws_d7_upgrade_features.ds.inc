<?php
/**
 * @file
 * ws_d7_upgrade_features.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function ws_d7_upgrade_features_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|trust_referral|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'trust_referral';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'author' => array(
      'weight' => '-4',
      'label' => 'inline',
      'format' => 'author_linked',
    ),
    'submitted_by' => array(
      'weight' => '-5',
      'label' => 'inline',
      'format' => 'ds_post_date_medium',
    ),
    'ds_user_picture' => array(
      'weight' => '-10',
      'label' => 'hidden',
      'format' => 'ds_picture_thumbnail',
    ),
  );
  $export['node|trust_referral|default'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function ws_d7_upgrade_features_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|trust_referral|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'trust_referral';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ws_default';
  $ds_layout->settings = array(
    'regions' => array(
      'main' => array(
        0 => 'body',
        4 => 'field_guest_or_host',
        6 => 'field_hosting_date',
      ),
      'header' => array(
        1 => 'ds_user_picture',
        2 => 'submitted_by',
        3 => 'author',
        5 => 'field_member_i_trust',
        7 => 'field_rating',
      ),
    ),
    'fields' => array(
      'body' => 'main',
      'ds_user_picture' => 'header',
      'submitted_by' => 'header',
      'author' => 'header',
      'field_guest_or_host' => 'main',
      'field_member_i_trust' => 'header',
      'field_hosting_date' => 'main',
      'field_rating' => 'header',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'main' => 'div',
      'footer' => 'div',
      'comments' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|trust_referral|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|trust_referral|highlight';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'trust_referral';
  $ds_layout->view_mode = 'highlight';
  $ds_layout->layout = 'ws_default';
  $ds_layout->settings = array(
    'regions' => array(
      'main' => array(
        0 => 'field_member_i_trust',
        1 => 'body',
      ),
    ),
    'fields' => array(
      'field_member_i_trust' => 'main',
      'body' => 'main',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'main' => 'div',
      'footer' => 'div',
      'comments' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|trust_referral|highlight'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|trust_referral|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'trust_referral';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ws_default';
  $ds_layout->settings = array(
    'regions' => array(
      'main' => array(
        0 => 'field_member_i_trust',
        1 => 'field_rating',
        2 => 'field_hosting_date',
        3 => 'body',
      ),
    ),
    'fields' => array(
      'field_member_i_trust' => 'main',
      'field_rating' => 'main',
      'field_hosting_date' => 'main',
      'body' => 'main',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header' => 'div',
      'main' => 'div',
      'footer' => 'div',
      'comments' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|trust_referral|teaser'] = $ds_layout;

  return $export;
}

/**
 * Implements hook_ds_view_modes_info().
 */
function ws_d7_upgrade_features_ds_view_modes_info() {
  $export = array();

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'highlight';
  $ds_view_mode->label = 'Highlight';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['highlight'] = $ds_view_mode;

  return $export;
}
