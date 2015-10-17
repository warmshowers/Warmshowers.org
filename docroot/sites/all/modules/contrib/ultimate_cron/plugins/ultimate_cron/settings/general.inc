<?php
/**
 * @file
 * General settings for Ultimate Cron.
 */

$plugin = array(
  'title' => t('General'),
  'description' => t('General cron settings.'),

  'handler' => array(
    'file' => 'general.class.php',
    'class' => 'UltimateCronGeneralSettings',
    'parent' => 'UltimateCronSettings',
  ),
);
