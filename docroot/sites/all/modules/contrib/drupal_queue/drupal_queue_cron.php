<?php
// $Id: drupal_queue_cron.php,v 1.2 2010/03/19 15:27:41 alexb Exp $

/**
 * @file
 * Entry point for worker calls.
 */
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
if (function_exists('drupal_queue_cron_run')) {
  drupal_queue_cron_run();
}
