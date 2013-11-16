<?php

/**
 * Implementation of hook_content_default_fields().
 */
function redesign_content_types_content_default_fields() {
  $fields = array();

  // Exported field: field_contest_photo
  $fields['sitebuilding-field_contest_photo'] = array(
    'field_name' => 'field_contest_photo',
    'type_name' => 'sitebuilding',
    'display_settings' => array(
      'weight' => '-2',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'rightcolumn][thickbox',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'rightcolumn][thickbox',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '1',
    'widget' => array(
      'file_extensions' => 'jpg jpeg gif png',
      'file_path' => 'recommendation_photo/[uid]',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '10MB',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 0,
      'title' => '',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'label' => 'Images',
      'weight' => '-2',
      'description' => 'You can add as many photos of your host or guest as you like.',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Images');

  return $fields;
}
