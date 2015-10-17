<?php
/**
 * @file
 * Poormans cron settings for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Poormans Cron'),
  'description' => t('Poormans cron settings.'),

  'handler' => array(
    'file' => 'poorman.class.php',
    'class' => 'UltimateCronPoormanSettings',
    'parent' => 'UltimateCronSettings',
  ),
);
