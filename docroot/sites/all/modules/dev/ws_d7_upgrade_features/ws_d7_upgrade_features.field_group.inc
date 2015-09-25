<?php
/**
 * @file
 * ws_d7_upgrade_features.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function ws_d7_upgrade_features_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_feedback_photos|node|trust_referral|form';
  $field_group->group_name = 'group_feedback_photos';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'trust_referral';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Optional Photos',
    'weight' => '9',
    'children' => array(
      0 => 'field_contest_photo',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Optional Photos',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-feedback-photos field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_feedback_photos|node|trust_referral|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_pm_extras|privatemsg_message|privatemsg_message|form';
  $field_group->group_name = 'group_pm_extras';
  $field_group->entity_type = 'privatemsg_message';
  $field_group->bundle = 'privatemsg_message';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Optional Attachments',
    'weight' => '4',
    'children' => array(
      0 => 'field_pm_attachment',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Optional Attachments',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-pm-extras field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_pm_extras|privatemsg_message|privatemsg_message|form'] = $field_group;

  return $export;
}
