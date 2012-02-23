<?php

/**
 * Implementation of hook_drush_command().
 */
function nagios_drush_command() {
  $items['nagios-list'] = array(
    'description' => dt('List all available nagios commands.'),
    'arguments' => array(),
    'aliases' => array('nagios-ls'),
  );
  $items['nagios-check'] = array(
    'description' => dt('Run a nagios command.'),
    'arguments' => array('command' => 'The command to run'),
    'aliases' => array('nagios'),
  );
  return $items;
}

/**
 * Drush command callback: nagios-list.
 */
function drush_nagios_list() {
  // Invoke hook_nagios_checks() here so people can add in their own checks.
  $commands = array_merge(module_invoke_all('nagios_checks'), drush_command_invoke_all('nagios_checks'));
  foreach ($commands as $cmd => $desc) {
    $rows[] = array($cmd, $desc);
  }
  return drush_print_table($rows);
}

/**
 * Drush command callback: nagios-check.
 */
function drush_nagios_check($check = null) {
  if (isset($check)) {
    foreach (module_implements('nagios_checks') as $module) {
      if (in_array($check, array_keys(module_invoke($module, 'nagios_checks')))) {
        return nagios_check($module, $check);
      }
    }
    foreach (drush_command_implements('nagios_checks') as $command) {
      $function = $command . '_nagios_checks';
      if (in_array($check, array_keys($function()))) {
        return nagios_check($command, $check);
      }
    }
    drush_set_error(dt('The requested nagios check does not exist: !check.', array('!check' => $check)));
  }
  else {
    drush_set_error(dt('Missing argument for nagios-check'));
  }
}

/**
 * Helper function for drush_nagios_check().
 */
function nagios_check($module, $check) {
  $function = $module . '_nagios_check_' . $check;
  if (function_exists($function)) {
    $nagios_data = $function();
  }
  else {
    $function = $module . '_nagios_check';
    $nagios_data = $function($check);
  }
  $key = key($nagios_data);
  $status = nagios_status();
  $message = $key . ' ' . $status[$nagios_data[$key]['status']] . ' - ' . $nagios_data[$key]['text'];
  if ($nagios_data[$key]['type'] == 'perf') {
    $message .= '|' . $key . '=' . $nagios_data[$key]['text'];
  }
  // We want to exit with the correct exit status so Nagios knows the outcome
  // of our drush nagios check so set DRUSH_EXECUTION_COMPLETED to true.
  drush_set_context('DRUSH_EXECUTION_COMPLETED', TRUE);
  drush_print($message);
  exit((int) $nagios_data[$key]['status']);
}
