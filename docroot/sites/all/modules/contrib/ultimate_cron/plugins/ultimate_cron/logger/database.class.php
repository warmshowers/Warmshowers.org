<?php
/**
 * @file
 * Database logger for Ultimate Cron.
 */

define('ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_DISABLED', 1);
define('ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE', 2);
define('ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN', 3);

/**
 * Class for using database as log storage.
 */
class UltimateCronDatabaseLogger extends UltimateCronLogger {
  public $options = array();
  public $log_entry_class = '\UltimateCronDatabaseLogEntry';

  /**
   * Constructor.
   */
  public function __construct($name, $plugin, $log_type = ULTIMATE_CRON_LOG_TYPE_NORMAL) {
    parent::__construct($name, $plugin, $log_type);
    $this->options['method'] = array(
      ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_DISABLED => t('Disabled'),
      ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE => t('Remove logs older than a specified age'),
      ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN => t('Retain only a specific amount of log entries'),
    );
  }

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'method' => ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN,
      'expire' => 86400 * 14,
      'retain' => 1000,
    );
  }

  /**
   * Cleanup logs.
   */
  public function cleanup() {
    $jobs = _ultimate_cron_job_load_all();
    $current = 1;
    $max = 0;
    foreach ($jobs as $job) {
      if ($job->getPlugin($this->type)->name === $this->name) {
        $max++;
      }
    }
    foreach ($jobs as $job) {
      if ($job->getPlugin($this->type)->name === $this->name) {
        $this->cleanupJob($job);
        $class = _ultimate_cron_get_class('job');
        if ($class::$currentJob) {
          $class::$currentJob->setProgress($current / $max);
          $current++;
        }
      }
    }
  }

  /**
   * Cleanup logs for a single job.
   */
  public function cleanupJob($job) {
    $settings = $job->getSettings('logger');

    switch ($settings['method']) {
      case ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_DISABLED:
        return;

      case ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE:
        $expire = $settings['expire'];
        // Let's not delete more than ONE BILLION log entries :-o.
        $max = 10000000000;
        $chunk = 100;
        break;

      case ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN:
        $expire = 0;
        $max = db_query("SELECT COUNT(lid) FROM {ultimate_cron_log} WHERE name = :name", array(
          ':name' => $job->name,
        ))->fetchField();
        $max -= $settings['retain'];
        if ($max <= 0) {
          return;
        }
        $chunk = min($max, 100);
        break;

      default:
        watchdog('ultimate_cron', 'Invalid cleanup method: @method', array(
          '@method' => $settings['method'],
        ));
        return;
    }

    // Chunked delete.
    $count = 0;
    do {
      $lids = db_select('ultimate_cron_log', 'l')
        ->fields('l', array('lid'))
        ->condition('l.name', $job->name)
        ->condition('l.start_time', microtime(TRUE) - $expire, '<')
        ->range(0, $chunk)
        ->orderBy('l.start_time', 'ASC')
        ->orderBy('l.end_time', 'ASC')
        ->execute()
        ->fetchAll(PDO::FETCH_COLUMN);
      if ($lids) {
        $count += count($lids);
        $max -= count($lids);
        $chunk = min($max, 100);
        db_delete('ultimate_cron_log')
          ->condition('lid', $lids, 'IN')
          ->execute();
      }
    } while ($lids && $max > 0);
    if ($count) {
      watchdog('database_logger', '@count log entries removed for job @name', array(
        '@count' => $count,
        '@name' => $job->name,
      ), WATCHDOG_INFO);
    }
  }

  /**
   * Label for setting.
   */
  public function settingsLabel($name, $value) {
    switch ($name) {
      case 'method':
        return $this->options[$name][$value];
    }
    return parent::settingsLabel($name, $value);

  }

  /**
   * Settings form.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $defaults = &$form_state['default_values']['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    $elements['method'] = array(
      '#type' => 'select',
      '#title' => t('Log entry cleanup method'),
      '#description' => t('Select which method to use for cleaning up logs.'),
      '#options' => $this->options['method'],
      '#default_value' => $values['method'],
      '#fallback' => TRUE,
      '#required' => TRUE,
    );

    $states = array('expire' => array(), 'retain' => array());

    if ($job) {
      $states['expire'] = array(
        '#states' => array(
          'visible' => array(
            ':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]' => array(
              'value' => ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE,
            ),
          ),
          'enabled' => array(
            ':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]' => array(
              'value' => ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE,
            ),
          ),
        ),
      );

      $states['retain'] = array(
        '#states' => array(
          'visible' => array(
            ':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]' => array(
              'value' => ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN,
            ),
          ),
          'enabled' => array(
            ':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]' => array(
              'value' => ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN,
            ),
          ),
        ),
      );
    }

    $elements['method_expire'] = array(
      '#type' => 'fieldset',
      '#title' => t('Remove logs older than a specified age'),
    ) + $states['expire'];
    $elements['method_expire']['expire'] = array(
      '#parents' => array('settings', $this->type, $this->name, 'expire'),
      '#type' => 'textfield',
      '#title' => t('Log entry expiration'),
      '#description' => t('Remove log entries older than X seconds.'),
      '#default_value' => $values['expire'],
      '#fallback' => TRUE,
      '#required' => TRUE,
    ) + $states['expire'];

    $elements['method_retain'] = array(
      '#type' => 'fieldset',
      '#title' => t('Retain only a specific amount of log entries'),
    ) + $states['retain'];
    $elements['method_retain']['retain'] = array(
      '#parents' => array('settings', $this->type, $this->name, 'retain'),
      '#type' => 'textfield',
      '#title' => t('Retain logs'),
      '#description' => t('Retain X amount of log entries; this value is per cron job. Setting this to 1000 on sites with 15 cron jobs will store a total of 15000 entries. High values can result in slower log performance.'),
      '#default_value' => $values['retain'],
      '#fallback' => TRUE,
      '#required' => TRUE,
    ) + $states['retain'];

    if ($job) {
      if ($defaults['method'] == ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE) {
        $elements['method_default'] = $elements['method_expire'];
        $elements['method_default']['#states']['visible'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
        $elements['method_default']['#states']['enabled'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
        $elements['method_default']['expire']['#states']['visible'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
        $elements['method_default']['expire']['#states']['enabled'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
      }
      if ($defaults['method'] == ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN) {
        $elements['method_default'] = $elements['method_retain'];
        $elements['method_default']['#states']['visible'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
        $elements['method_default']['#states']['enabled'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
        $elements['method_default']['retain']['#states']['visible'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
        $elements['method_default']['retain']['#states']['enabled'][':input[name="settings[' . $this->type . '][' . $this->name . '][method]"]']['value'] = '';
      }
    }
  }

  /**
   * Submit handler.
   */
  public function settingsFormSubmit(&$form, &$form_state, $job = NULL) {
    $values = &$form_state['values']['settings'][$this->type][$this->name];
    $defaults = &$form_state['default_values']['settings'][$this->type][$this->name];
    if (!$job) {
      return;
    }

    $method = $values['method'] ? $values['method'] : $defaults['method'];

    // Cleanup form (can this be done elsewhere?)
    switch ($method) {
      case ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_DISABLED:
        unset($values['expire']);
        unset($values['retain']);
        break;

      case ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_EXPIRE:
        unset($values['retain']);
        break;

      case ULTIMATE_CRON_DATABASE_LOGGER_CLEANUP_METHOD_RETAIN:
        unset($values['expire']);
        break;
    }
  }

  /**
   * Load log entry.
   */
  public function load($name, $lock_id = NULL, $log_types = array(ULTIMATE_CRON_LOG_TYPE_NORMAL)) {
    if ($lock_id) {
      $log_entry = db_select('ultimate_cron_log', 'l')
        ->fields('l')
        ->condition('l.lid', $lock_id)
        ->execute()
        ->fetchObject($this->log_entry_class, array($name, $this));
    }
    else {
      $subquery = db_select('ultimate_cron_log', 'l3')
        ->fields('l3', array('name', 'log_type'))
        ->groupBy('name')
        ->groupBy('log_type');
      $subquery->addExpression('MAX(l3.start_time)', 'start_time');

      $query = db_select('ultimate_cron_log', 'l1')
        ->fields('l1');
      $query->join($subquery, 'l2', 'l1.name = l2.name AND l1.start_time = l2.start_time AND l1.log_type = l2.log_type');
      $query->condition('l2.name', $name);
      $query->condition('l2.log_type', $log_types, 'IN');

      $log_entry = $query->execute()->fetchObject($this->log_entry_class, array($name, $this));
    }
    if ($log_entry) {
      $log_entry->finished = TRUE;
    }
    else {
      $log_entry = new $this->log_entry_class($name, $this);
    }
    return $log_entry;
  }

  /**
   * Load latest log entry.
   */
  public function loadLatestLogEntries($jobs, $log_types) {
    if (Database::getConnection()->databaseType() !== 'mysql') {
      return parent::loadLatestLogEntries($jobs, $log_types);
    }

    $subquery = db_select('ultimate_cron_log', 'l3')
      ->fields('l3', array('name', 'log_type'))
      ->groupBy('name')
      ->groupBy('log_type');
    $subquery->addExpression('MAX(l3.start_time)', 'start_time');

    $query = db_select('ultimate_cron_log', 'l1')
      ->fields('l1');
    $query->join($subquery, 'l2', 'l1.name = l2.name AND l1.start_time = l2.start_time AND l1.log_type = l2.log_type');
    $query->condition('l2.log_type', $log_types, 'IN');

    $result = $query->execute();

    $log_entries = array();
    while ($object = $result->fetchObject()) {
      if (isset($jobs[$object->name])) {
        $log_entries[$object->name] = new $this->log_entry_class($object->name, $this);
        $log_entries[$object->name]->setData((array) $object);
      }
    }
    foreach ($jobs as $name => $job) {
      if (!isset($log_entries[$name])) {
        $log_entries[$name] = new $this->log_entry_class($name, $this);
      }
    }

    return $log_entries;
  }

  /**
   * Get log entries.
   */
  public function getLogEntries($name, $log_types, $limit = 10) {
    $result = db_select('ultimate_cron_log', 'l')
      ->fields('l')
      ->extend('PagerDefault')
      ->condition('l.name', $name)
      ->condition('l.log_type', $log_types, 'IN')
      ->limit($limit)
      ->orderBy('l.start_time', 'DESC')
      ->execute();

    $log_entries = array();
    while ($object = $result->fetchObject($this->log_entry_class, array($name, $this))) {
      $log_entries[$object->lid] = $object;
    }

    return $log_entries;
  }

}

/**
 * Class for Ultimate Cron log entries.
 */
class UltimateCronDatabaseLogEntry extends UltimateCronLogEntry {
  /**
   * Save log entry.
   */
  public function save() {
    if (!$this->lid) {
      return;
    }

    static $retry = 0;

    try {
      db_insert('ultimate_cron_log')
        ->fields(array(
          'lid' => $this->lid,
          'name' => $this->name,
          'log_type' => $this->log_type,
          'start_time' => $this->start_time,
          'end_time' => $this->end_time,
          'uid' => $this->uid,
          'init_message' => $this->init_message,
          'message' => $this->message,
          'severity' => $this->severity,
        ))
        ->execute();
    }
    catch (PDOException $e) {
      // Row already exists. Let's update it, if we can.
      $updated = db_update('ultimate_cron_log')
        ->fields(array(
          'name' => $this->name,
          'log_type' => $this->log_type,
          'start_time' => $this->start_time,
          'end_time' => $this->end_time,
          'init_message' => $this->init_message,
          'message' => $this->message,
          'severity' => $this->severity,
        ))
        ->condition('lid', $this->lid)
        ->condition('end_time', 0)
        ->execute();
      if (!$updated) {
        // Row was not updated, someone must have beaten us to it.
        // Let's create a new log entry.
        $lid = $this->lid . '-' . uniqid('', TRUE);
        $this->message = t('Lock #@original_lid was already closed and logged. Creating a new log entry #@lid', array(
          '@original_lid' => $this->lid,
          '@lid' => $lid,
        )) . "\n" . $this->message;
        $this->severity = $this->severity >= 0 && $this->severity < WATCHDOG_ERROR ? $this->severity : WATCHDOG_ERROR;
        $this->lid = $lid;
        $retry++;
        if ($retry > 3) {
          $retry = 0;
          watchdog('database_logger', (string) $e, array(), WATCHDOG_CRITICAL);
          return;
        }

        $this->save();
        $retry--;
      }
    }
  }

}
