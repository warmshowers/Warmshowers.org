<?php
/**
 * @file
 * Hooks provided by Ultimate Cron.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Inform Ultimate Cron about cron jobs.
 *
 * Note that the result of this hook is cached.
 *
 * @return array
 *   Array of cron jobs, keyed by name.
 *    - "title": (optional) The title of the cron job. If not provided, the
 *      name of the cron job will be used.
 *    - "file": (optional) The file where the callback lives.
 *    - "module": The module where this job lives.
 *    - "file path": (optional) The path to the directory containing the file
 *      specified in "file". This defaults to the path to the module
 *      implementing the hook.
 *    - "callback": (optional) The callback to call when running the job.
 *      Defaults to the job name.
 *    - "callback arguments": (optional) Arguments for the callback. Defaults
 *      to array().
 *    - "enabled": (optional) Initial state of the job. Defaults to TRUE.
 *    - "tags": (optional) Tags for the job. Defaults to array().
 *    - "settings": (optional) Default settings (plugin type) for this job.
 *      Example of a job declaring some default settings for a plugin called
 *      "some_plugin":
 *      'settings' => array(
 *        'some_plugin' => array(
 *          'some_value' => 60,
 *        ),
 *      ),
 *    - "scheduler": (optional) Default scheduler (plugin type) for this job.
 *      Example of a job using the crontab scheduler as default:
 *      'scheduler' => array(
 *        'name' => 'crontab',
 *        'crontab' => array(
 *          'rules' => array('* * * * *'),
 *        ),
 *      ),
 *    - "launcher": (optional) Default launcher (plugin type) for this job.
 *      Example of a job using the serial launcher as default:
 *      'launcher' => array(
 *        'name' => 'serial',
 *        'serial' => array(
 *          'thread' => 'any',
 *        ),
 *      ),
 *    - "logger": (optional) Default logger (plugin type) for this job.
 *      Example of a job using the cache logger as default:
 *      'logger' => array(
 *        'name' => 'cache',
 *        'cache' => array(
 *          'bin' => 'mycachebin',
 *        ),
 *      ),
 */
function hook_cronapi() {
  $items = array();

  $items['example_my_cron_job_1'] = array(
    'title' => t('This is my cron job #1'),
    'file' => 'example.jobs.inc',
    'file path' => drupal_get_path('module', 'example') . '/cron',
    'callback' => 'example_my_cron_job_callback',
    'callback arguments' => array('cronjob1'),
    'enabled' => FALSE,
    'tags' => array('example'),
    'settings' => array(
      'example_plugin' => array(
        'example_setting' => 'example_value',
      ),
    ),
    'scheduler' => array(
      'name' => 'crontab',
      'crontab' => array(
        'rules' => array('* * * * *'),
      ),
    ),
    'launcher' => array(
      'name' => 'serial',
      'serial' => array(
        'thread' => 'any',
      ),
    ),
    'logger' => array(
      'name' => 'cache',
      'cache' => array(
        'bin' => 'my_cache_bin',
      ),
    ),
  );

  return $items;
}

/**
 * Alter the output of hook_cronapi() and hook_cron().
 *
 * Note that the result of this hook is cached just like hook_cronapi().
 *
 * This can hook can also be implemented inside a plugin, but with a
 * slight difference. Inside the plugin, the hook is not cached and it operates
 * on an array of UltimateCronJob objects instead of hook definitions.
 *
 * @param array &$items
 *   Hooks defined in the system.
 */
function hook_cron_alter(&$items) {
  $items['example_my_cron_job_1']['title'] = 'NEW TITLE FOR EXAMPLE CRON JOB #1! HA!';
}

/**
 * Provide easy hooks for Ultimate Cron.
 *
 * Ultimate Cron has a built-in set of easy hooks:
 *  - hook_cron_hourly().
 *  - hook_cron_daily().
 *  - hook_cron_nightly().
 *  - hook_cron_weekly().
 *  - hook_cron_monthly().
 *  - hook_cron_yearly().
 *
 * This hook makes it possible to provide custom easy hooks.
 *
 * @return array
 *   Array of easy hook definitions.
 */
function hook_cron_easy_hooks() {
  return array(
    'cron_fullmoonly' => array(
      'title' => 'Run at full moon',
      'scheduler' => array(
        'name' => 'moonphase',
        'moonphase' => array(
          'phase' => 'full',
        ),
      ),
    ),
  );
}

/**
 * Alter easy hooks.
 *
 * @param array &$easy_hooks
 *   Easy hook definitions.
 */
function hook_cron_easy_hooks_alter(&$easy_hooks) {
  $easy_hooks['cron_fullmoonly']['scheduler']['moonphase']['phase'] = 'new';
}

/**
 * The following hooks are invoked during the jobs life cycle,
 * from schedule to finish. The chronological order is:
 *
 * cron_pre_schedule
 * cron_post_schedule
 * cron_pre_launch
 * cron_pre_launch(*)
 * cron_pre_run
 * cron_pre_invoke
 * cron_post_invoke
 * cron_post_run
 * cron_post_launch(*)
 *
 * Depending on how the launcher works, the hook_cron_post_launch() may be
 * invoked before or after hook_cron_post_run() or somewhere in between.
 * An example of this is the Background Process launcher, which launches
 * the job in a separate thread. After the launch, hook_cron_post_launch()
 * is invoked, but the run/invoke hooks are invoked simultaneously in a
 * separate thread.
 *
 * All of these hooks can also be implemented inside a plugin as a method.
 */

/**
 * Invoked just before a job is asked for its schedule.
 *
 * @param UltimateCronJob $job
 *   The job being queried.
 */
function hook_cron_pre_schedule($job) {
}

/**
 * Invoked after a job has been asked for its schedule.
 *
 * @param UltimateCronJob $job
 *   The job being queried.
 */
function hook_cron_post_schedule($job) {
}

/**
 * Invoked just before a job is launched.
 *
 * @param UltimateCronJob $job
 *   The job being launched.
 */
function hook_cron_pre_launch($job) {
}

/**
 * Invoked after a job has been launched.
 *
 * @param UltimateCronJob $job
 *   The job that was launched.
 */
function hook_cron_post_launch($job) {
}

/**
 * Invoked just before a job is being run.
 *
 * @param UltimateCronJob $job
 *   The job being run.
 */
function hook_cron_pre_run($job) {
}

/**
 * Invoked after a job has been run.
 *
 * @param UltimateCronJob $job
 *   The job that was run.
 */
function hook_cron_post_run($job) {
}

/**
 * Invoked just before a job is asked for its schedule.
 *
 * @param UltimateCronJob $job
 *   The job being invoked.
 */
function hook_cron_pre_invoke($job) {
}

/**
 * Invoked after a job has been invoked.
 *
 * @param UltimateCronJob $job
 *   The job that was invoked.
 */
function hook_cron_post_invoke($job) {
}

/**
 * Alter the allowed operations for a given job on the export UI page.
 *
 * This hook can also be implemented inside a plugin as a method:
 * build_operations_alter($job, &$allowed_operations). It will only be
 * run for the currently active plugin for the job.
 *
 * @param UltimateCronJob $job
 *   The job in question.
 * @param array &$allowed_operations
 *   Allowed operations for this job.
 */
function hook_ultimate_cron_plugin_build_operations_alter($job, &$allowed_operations) {
}
