<?php
/**
 * @file
 * d7_theme_upgrade.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function d7_theme_upgrade_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_forum_image'
  $field_bases['field_forum_image'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_forum_image',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => NULL,
      'description_field' => 1,
      'list_default' => 0,
      'list_field' => 1,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'image',
    'type_name' => 'forum',
  );

  // Exported field_base: 'taxonomy_forums'
  $field_bases['taxonomy_forums'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'taxonomy_forums',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_4',
          'parent' => 0,
        ),
      ),
      'required' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'taxonomyextra'
  $field_bases['taxonomyextra'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'taxonomyextra',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_4',
          'parent' => 0,
        ),
        1 => array(
          'parent' => 0,
          'vocabulary' => 'vocabulary_1',
        ),
        2 => array(
          'parent' => 0,
          'vocabulary' => 'vocabulary_6',
        ),
        3 => array(
          'parent' => 0,
          'vocabulary' => 'vocabulary_2',
        ),
        4 => array(
          'parent' => 0,
          'vocabulary' => 'vocabulary_5',
        ),
        5 => array(
          'parent' => 0,
          'vocabulary' => 'vocabulary_3',
        ),
      ),
      'required' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  return $field_bases;
}
