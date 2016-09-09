<?php
/**
 * @file
 * General settings for Ultimate Cron.
 */

/**
 * General settings plugin class.
 */
class UltimateCronGeneralSettings extends UltimateCronSettings {
  /**
   * Handle kill signal.
   */
  public function signal($item, $signal) {
    switch ($signal) {
      case 'kill':
        $item->sendSignal('kill', TRUE);
        return;
    }
  }

  /**
   * Use ajax for run, since we're launching in the background.
   */
  public function build_operations_alter($job, &$allowed_operations) {
    if (empty($allowed_operations['run'])) {
      if (in_array('killable', $job->hook['tags']) && !$job->peekSignal('kill')) {
        $allowed_operations['kill'] = array(
          'title' => t('Kill'),
          'href' => 'admin/config/system/cron/jobs/list/' . $job->name . '/signal/' . $this->type . '/' . $this->name . '/kill',
          'attributes' => array('class' => array('use-ajax')),
          'query' => array('token' => drupal_get_token('signal')),
        );
      }
    }
  }

  /**
   * Implements hook_cron_post_schedule().
   */
  public function cron_post_schedule($job, &$result) {
    if (self::getGlobalOption('bypass_schedule')) {
      $result = $result || (empty($job->disabled) && !$job->isLocked());
    }
  }

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'nodejs' => module_exists('nodejs'),
    );
  }

  /**
   * Settings form.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    if (!$job) {
      $elements['nodejs'] = array(
        '#type' => 'checkbox',
        '#title' => t('nodejs'),
        '#default_value' => $values['nodejs'],
        '#description' => t('Enable nodejs integration (Live reload on jobs page. Requires the nodejs module to be installed and configured).'),
        '#fallback' => TRUE,
      );
    }
    else {
      $elements['no_settings'] = array(
        '#markup' => '<p>' . t('This plugin has no settings.') . '</p>',
      );
    }
  }

}
