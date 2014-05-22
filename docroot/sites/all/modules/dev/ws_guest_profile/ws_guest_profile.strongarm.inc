<?php

/**
 * Implementation of hook_strongarm().
 */
function ws_guest_profile_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_hosting_request';
  $strongarm->value = 0;
  $export['comment_anonymous_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_traveler_trip';
  $strongarm->value = 0;
  $export['comment_anonymous_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_hosting_request';
  $strongarm->value = '3';
  $export['comment_controls_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_traveler_trip';
  $strongarm->value = '3';
  $export['comment_controls_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_hosting_request';
  $strongarm->value = '4';
  $export['comment_default_mode_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_traveler_trip';
  $strongarm->value = '4';
  $export['comment_default_mode_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_hosting_request';
  $strongarm->value = '1';
  $export['comment_default_order_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_traveler_trip';
  $strongarm->value = '1';
  $export['comment_default_order_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_hosting_request';
  $strongarm->value = '50';
  $export['comment_default_per_page_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_traveler_trip';
  $strongarm->value = '50';
  $export['comment_default_per_page_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_hosting_request';
  $strongarm->value = '0';
  $export['comment_form_location_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_traveler_trip';
  $strongarm->value = '0';
  $export['comment_form_location_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_hosting_request';
  $strongarm->value = '0';
  $export['comment_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_hosting_request';
  $strongarm->value = '1';
  $export['comment_preview_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_traveler_trip';
  $strongarm->value = '1';
  $export['comment_preview_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_hosting_request';
  $strongarm->value = '1';
  $export['comment_subject_field_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_traveler_trip';
  $strongarm->value = '1';
  $export['comment_subject_field_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_traveler_trip';
  $strongarm->value = '0';
  $export['comment_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_hosting_request';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '7',
    'revision_information' => '-2',
    'author' => '-1',
    'options' => '0',
    'comment_settings' => '2',
    'menu' => '-4',
    'path' => '3',
    'attachments' => '1',
    'nodewords' => '-3',
  );
  $export['content_extra_weights_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_traveler_trip';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '11',
    'revision_information' => '-2',
    'author' => '-1',
    'options' => '0',
    'comment_settings' => '2',
    'menu' => '-4',
    'path' => '3',
    'attachments' => '1',
    'nodewords' => '-3',
  );
  $export['content_extra_weights_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_hosting_request';
  $strongarm->value = '0';
  $export['language_content_type_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_traveler_trip';
  $strongarm->value = '0';
  $export['language_content_type_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_hosting_request';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_traveler_trip';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_traveler_trip'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_hosting_request';
  $strongarm->value = '1';
  $export['upload_hosting_request'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_traveler_trip';
  $strongarm->value = '1';
  $export['upload_traveler_trip'] = $strongarm;

  return $export;
}
