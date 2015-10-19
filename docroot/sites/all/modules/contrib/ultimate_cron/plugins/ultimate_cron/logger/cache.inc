<?php
/**
 * @file
 * Cache logger for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Cache'),
  'description' => t('Stores the last log entry (and only the last) in the cache.'),

  'handler' => array(
    'file' => 'cache.class.php',
    'class' => 'UltimateCronCacheLogger',
    'parent' => 'UltimateCronLogger',
  ),
);
