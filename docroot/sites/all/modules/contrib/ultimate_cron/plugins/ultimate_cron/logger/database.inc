<?php
/**
 * @file
 * Database logger for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Database'),
  'description' => t('Stores logs in the database.'),

  'handler' => array(
    'file' => 'database.class.php',
    'class' => 'UltimateCronDatabaseLogger',
    'parent' => 'UltimateCronLogger',
  ),
);
