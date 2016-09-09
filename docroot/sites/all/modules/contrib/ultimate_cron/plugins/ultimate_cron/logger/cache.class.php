<?php
/**
 * @file
 * Cache logger for Ultimate Cron.
 */

/**
 * Class for using cache as storage for logs.
 */
class UltimateCronCacheLogger extends UltimateCronLogger {
  public $log_entry_class = 'UltimateCronCacheLogEntry';

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'bin' => 'cache_ultimate_cron',
      'timeout' => 0,
    );
  }

  /**
   * Load log entry.
   */
  public function load($name, $lock_id = NULL, $log_types = array(ULTIMATE_CRON_LOG_TYPE_NORMAL)) {
    $log_entry = new $this->log_entry_class($name, $this);

    $job = _ultimate_cron_job_load($name);
    $settings = $job->getSettings('logger');

    if (!$lock_id) {
      $cache = cache_get('uc-name:' . $name, $settings['bin']);
      if (empty($cache) || empty($cache->data)) {
        return $log_entry;
      }
      $lock_id = $cache->data;
    }
    $cache = cache_get('uc-lid:' . $lock_id, $settings['bin']);

    if (!empty($cache->data)) {
      $log_entry->setData((array) $cache->data);
      $log_entry->finished = TRUE;
    }
    return $log_entry;
  }

  /**
   * Get log entries.
   */
  public function getLogEntries($name, $log_types, $limit = 10) {
    $log_entry = $this->load($name);
    return $log_entry->lid ? array($log_entry) : array();
  }

  /**
   * Settings form.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    $elements['bin'] = array(
      '#type' => 'textfield',
      '#title' => t('Cache bin'),
      '#description' => t('Select which cache bin to use for storing logs.'),
      '#default_value' => $values['bin'],
      '#fallback' => TRUE,
      '#required' => TRUE,
    );
    $elements['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Cache timeout'),
      '#description' => t('Seconds before cache entry expires (0 = never, -1 = on next general cache wipe).'),
      '#default_value' => $values['timeout'],
      '#fallback' => TRUE,
      '#required' => TRUE,
    );
  }

}

/**
 * Class for using cache as storage for log entries.
 */
class UltimateCronCacheLogEntry extends UltimateCronLogEntry {
  /**
   * Save log entry.
   */
  public function save() {
    if (!$this->lid) {
      return;
    }

    if ($this->log_type != ULTIMATE_CRON_LOG_TYPE_NORMAL) {
      return;
    }

    $job = _ultimate_cron_job_load($this->name);

    $settings = $job->getSettings('logger');

    $expire = $settings['timeout'] > 0 ? time() + $settings['timeout'] : $settings['timeout'];
    cache_set('uc-name:' . $this->name, $this->lid, $settings['bin'], $expire);
    cache_set('uc-lid:' . $this->lid, $this->getData(), $settings['bin'], $expire);
  }

}
