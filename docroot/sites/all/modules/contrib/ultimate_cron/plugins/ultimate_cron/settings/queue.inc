<?php
/**
 * @file
 * Queue settings for Ultimate Cron.
 */

$plugin = array(
  'title' => t('Queue'),
  'description' => t('Cron queue settings.'),

  'handler' => array(
    'file' => 'queue.class.php',
    'class' => 'UltimateCronQueueSettings',
    'parent' => 'UltimateCronTaggedSettings',
  ),
);
