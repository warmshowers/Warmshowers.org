<?php
/**
 * @file
 * Background Process legacy launcher for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Background Process'),
  'description' => t('Launches jobs through Background Process 1.x.'),

  'handler' => array(
    'file' => 'background_process_legacy.class.php',
    'class' => 'UltimateCronBackgroundProcessLegacyLauncher',
    'parent' => 'UltimateCronLauncher',
  ),
);
