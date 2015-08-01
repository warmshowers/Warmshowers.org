<?php
/**
 * @file
 * d7_theme_upgrade.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function d7_theme_upgrade_taxonomy_default_vocabularies() {
  return array(
    'vocabulary_1' => array(
      'name' => 'base',
      'machine_name' => 'vocabulary_1',
      'description' => '',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'vocabulary_2' => array(
      'name' => 'Image Galleries',
      'machine_name' => 'vocabulary_2',
      'description' => '',
      'hierarchy' => 1,
      'module' => 'image',
      'weight' => 0,
    ),
    'vocabulary_3' => array(
      'name' => 'Topics',
      'machine_name' => 'vocabulary_3',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'vocabulary_4' => array(
      'name' => 'Forums',
      'machine_name' => 'vocabulary_4',
      'description' => '',
      'hierarchy' => 1,
      'module' => 'forum',
      'weight' => -10,
    ),
    'vocabulary_5' => array(
      'name' => 'Newsletter',
      'machine_name' => 'vocabulary_5',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'simplenews',
      'weight' => 0,
    ),
    'vocabulary_6' => array(
      'name' => 'FAQ Category',
      'machine_name' => 'vocabulary_6',
      'description' => 'There are a number of questions/answers in the FAQ under this category name',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
