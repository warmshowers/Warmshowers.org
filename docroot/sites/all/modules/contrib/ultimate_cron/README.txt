
Credits
-------

Thanks to Mark James for the icons
  http://www.famfamfam.com/lab/icons/silk/


Example code:


// Default cron-function, configurable through /admin/config/system/cron
function mymodule_cron() {
  // Do some stuff ...
}


// Define custom cron functions.
function mymodule_cronapi($op, $job = NULL) {
  return array(
    'mymodule_cronjob_1' => array(
      'title' => 'Cron-1 Handler',
      'scheduler' => array(
        'name' => 'crontab',
        'crontab' => array(
          'rules' => array('*/13 * * * *'),
        ),
      ),
    ),
    'mymodule_cronjob_2' => array(
      'title' => 'Cron-2 Handler',
      'callback' => 'mymodule_somefunction',
      'scheduler' => array(
        'name' => 'crontab',
        'crontab' => array(
          'rules' => array('0 0 1 * *'),
        ),
      ),
    ),
    'mymodule_cronjob_3' => array(
      'title' => 'Cron-3 Handler',
    ),
  );
}

// Custom cron-function
function mymodule_cronjob_1($job) {
  // Do some stuff ...
}

// Custom cron-function
function mymodule_somefunction($job) {
  // Do some stuff ...
}

// Custom cron-function
function mymodule_cronjob_3($job) {
  // Do some stuff ...
}

// Easy-hook, uses rule: 0+@ * * * *
function mymodule_cron_hourly($job) {
  // Do some stuff
}

// Easy-hook, uses rule: 0+@ 12 * * *
function mymodule_cron_daily($job) {
  // Do some stuff
}

// Easy-hook, uses rule: 0+@ 0 * * *
function mymodule_cron_nightly($job) {
  // Do some stuff
}

// Easy-hook, uses rule: 0+@ 0 * * 1
function mymodule_cron_weekly($job) {
  // Do some stuff
}

// Easy-hook, uses rule: 0+@ 0 1 * *
function mymodule_cron_monthly($job) {
  // Do some stuff
}

// Easy-hook, uses rule: 0+@ 0 1 1 *
function mymodule_cron_yearly($job) {
  // Do some stuff
}


