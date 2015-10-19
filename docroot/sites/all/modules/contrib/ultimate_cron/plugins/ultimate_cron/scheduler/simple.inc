<?php
/**
 * @file
 * Simple cron job scheduler for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Simple'),
  'description' => t('Provides a set of predefined intervals for scheduling.'),

  'handler' => array(
    'file' => 'simple.class.php',
    'class' => 'UltimateCronSimpleScheduler',
    'parent' => 'UltimateCronCrontabScheduler',
  ),
);
