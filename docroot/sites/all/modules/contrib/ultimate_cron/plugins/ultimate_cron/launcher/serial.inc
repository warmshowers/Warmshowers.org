<?php
/**
 * @file
 * Serial cron job launcher for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Serial'),
  'description' => t('Launches scheduled jobs in the same thread and runs them consecutively.'),

  'handler' => array(
    'file' => 'serial.class.php',
    'class' => 'UltimateCronSerialLauncher',
    'parent' => 'UltimateCronLauncher',
  ),
);
