<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function redesign_system_level_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_rules_defaults().
 */
function redesign_system_level_rules_defaults() {
  return array(
    'rules' => array(
      'rules_user_logs_in' => array(
        '#type' => 'rule',
        '#set' => 'event_user_login',
        '#label' => 'User logs in',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Randy Fay is King!',
          'redesign_system_level' => 'redesign_system_level',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'path' => '<front>',
              'query' => '',
              'fragment' => '',
              'force' => 0,
              'immediate' => 0,
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'path' => array(
                    '0' => ':global',
                  ),
                  'query' => array(
                    '0' => ':global',
                  ),
                  'fragment' => array(
                    '0' => ':global',
                  ),
                ),
              ),
            ),
            '#name' => 'rules_action_drupal_goto',
            '#info' => array(
              'label' => 'Redirect to homepage',
              'label callback' => FALSE,
              'module' => 'System',
              'eval input' => array(
                '0' => 'path',
                '1' => 'query',
                '2' => 'fragment',
              ),
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}
