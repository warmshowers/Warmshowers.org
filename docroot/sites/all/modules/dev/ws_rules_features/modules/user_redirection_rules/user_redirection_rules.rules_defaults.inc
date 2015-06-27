<?php
/**
 * @file
 * user_redirection_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function user_redirection_rules_default_rules_configuration() {
  $items = array();
  $items['rules_user_redirection'] = entity_import('rules_config', '{ "rules_user_redirection" : {
      "LABEL" : "User redirection",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [], "user_update" : [] },
      "DO" : [ { "redirect" : { "url" : "[account:url]", "force" : "0" } } ]
    }
  }');
  return $items;
}
