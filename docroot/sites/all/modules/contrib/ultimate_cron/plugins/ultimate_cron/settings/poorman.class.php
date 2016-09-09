<?php
/**
 * @file
 * Poormans cron settings for Ultimate Cron.
 */

/**
 * Poormans cron settings plugin class.
 */
class UltimateCronPoormanSettings extends UltimateCronSettings {
  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'launcher' => 'serial',
      'early_page_flush' => TRUE,
      'user_agent' => 'Ultimate Cron',
    );
  }

  /**
   * Settings form.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    if (!$job) {
      $launchers = _ultimate_cron_plugin_load_all('launcher');
      $options = array('' => '-- ' . t('Disabled') . ' --');
      foreach ($launchers as $name => $launcher) {
        if ($launcher->isValid() && method_exists($launcher, 'launchPoorman')) {
          $options[$name] = $launcher->title;
        }
      }
      $elements['launcher'] = array(
        '#type' => 'select',
        '#title' => t('Launcher'),
        '#options' => $options,
        '#default_value' => $values['launcher'],
        '#description' => t('Select the launcher to use for handling poormans cron.'),
        '#fallback' => TRUE,
      );
      $elements['early_page_flush'] = array(
        '#type' => 'checkbox',
        '#title' => t('Early page flush'),
        '#default_value' => $values['early_page_flush'],
        '#description' => t('If not checked, Ultimate Cron will postpone the poormans cron execution until every shutdown function has run.'),
        '#fallback' => TRUE,
        '#states' => array(
          'invisible' => array(':input[name="settings[' . $this->type . '][' . $this->name . '][launcher]"]' => array('value' => '')),
        ),
      );
      $elements['user_agent'] = array(
        '#type' => 'textfield',
        '#title' => t('User Agent'),
        '#default_value' => $values['user_agent'],
        '#description' => t('The User Agent to use for poormans cron triggering (used by the Serial launcher).'),
        '#fallback' => TRUE,
        '#states' => array(
          'invisible' => array(':input[name="settings[' . $this->type . '][' . $this->name . '][launcher]"]' => array('value' => '')),
        ),
      );
    }
    else {
      $elements['no_settings'] = array(
        '#markup' => '<p>' . t('This plugin has no settings.') . '</p>',
      );
    }
  }

}
