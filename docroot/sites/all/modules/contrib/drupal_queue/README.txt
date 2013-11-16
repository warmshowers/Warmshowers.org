$Id: README.txt,v 1.7 2010/05/04 20:51:05 alexb Exp $

DRUPAL QUEUE
------------

Queues jobs for asynchronous execution.

Drupal 6 Backport of Drupal 7 Queue API by chx, dww, neclimdul et. al.

Related Drupal 7 issues

Job Queue API http://drupal.org/node/391340

Use queue for cron http://drupal.org/node/578676

INSTALLATION
------------

- Install module
- Schedule workers: To provide full functionality without requiring additional
  cron configuration, jobs added to the queue are being worked off in subsequent
  cron runs. If this is not sufficient, it is recommended to add a separate
  process for workers: If using drush, add "drush queue-cron" to your
  crontab. Otherwise copy drupal_queue_cron.php to your site's root directory
  and add it to your crontab just like cron.php
- You can schedule as many workers concurrently as your server resources allow
  for.
- Note: modules that use Drupal Queue may still require cron to be configured
  http://drupal.org/cron

USING DRUPAL QUEUE
------------------

If your module uses the Drupal Queue API, note that jobs being queued need to be
concurrency-safe. For an example look at Drupal 7 aggregator module or Drupal 6
Feeds module.

http://cvs.drupal.org/viewvc/drupal/drupal/modules/aggregator/
http://drupal.org/project/feeds

API
---

- Disable dequeueing (working off) items on Drupal cron runs by setting the
  Drupal variable 'drupal_queue_on_cron' to FALSE.
- Configure a custom handler class for handling queues by specifying a class
  for the Drupal variable 'queue_module_[queue_name]'.

Drupal variables can be set by adding a key/value pair to the $conf variable in
settings.php.
