<?php
/**
 * @file
 * Cron hook implementations for Ultimate Cron.
 */

/**
 * Implements hook_cronapi().
 *
 * Adds clean up jobs for plugins.
 */
function ultimate_cron_cronapi() {
  $items = array();

  ctools_include('plugins');
  $plugin_types = ctools_plugin_get_plugin_type_info();
  foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
    foreach (_ultimate_cron_plugin_load_all($plugin_type) as $name => $plugin) {
      if ($plugin->isValid() && method_exists($plugin, 'cleanup')) {
        $items["ultimate_cron_plugin_{$plugin_type}_{$name}_cleanup"] = array(
          'title' => t('Ultimate Cron @type @title cleanup', array(
            '@type' => $info['defaults']['static']['title singular proper'],
            '@title' => $plugin->title,
          )),
          'callback' => 'ultimate_cron_plugin_cleanup',
          'callback arguments' => array(
            'type' => $plugin_type,
            'name' => $name,
          ),
        );
      }
    }
  }

  return $items;
}

// ---------- FIXUPS FOR CORE  ----------

/**
 * Implements hook_cron_alter().
 *
 * Add better description to core modules.
 */
function ultimate_cron_cron_alter(&$hooks) {
  $update['dblog_cron']['title'] = t('Remove expired log messages and flood control events');
  $update['field_cron']['title'] = t('Purges deleted Field API data');
  $update['filter_cron']['title'] = t('Expire outdated filter cache entries');
  $update['node_cron']['title'] = t('Mark old nodes as read');
  $update['search_cron']['title'] = t('Update indexes');
  $update['system_cron']['title'] = t('Cleanup (caches, batch, flood, temp-files, etc.)');
  $update['aggregator_cron']['title'] = t('Refresh feeds');
  $update['openid_cron']['title'] = t('Remove expired nonces from the database');
  $update['ping_cron']['title'] = t('Notify remote sites');
  $update['poll_cron']['title'] = t('Close expired polls');
  $update['statistics_cron']['title'] = t('Reset counts and clean up');
  $update['trigger_cron']['title'] = t('Run actions for cron triggers');
  $update['tracker_cron']['title'] = t('Update tracker index');
  $update['update_cron']['title'] = t('Check system for updates');
  $update['dblog_cron']['configure'] = 'admin/config/development/logging';
  $update['ctools_cron']['title'] = t('Clean up old caches');

  $update['system_cron']['scheduler']['crontab'] = array('rules' => array('0+@ */3 * * *'));
  $update['system_cron']['scheduler']['simple'] = array('rules' => array('0+@ */3 * * *'));

  foreach ($update as $name => $data) {
    if (isset($hooks[$name])) {
      $hooks[$name] = array_replace_recursive($hooks[$name], $data);
    }
  }
}
