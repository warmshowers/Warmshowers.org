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
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [], "user_update" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "3" : "3", "8" : "8", "6" : "6", "13" : "13" } }
          }
        }
      ],
      "DO" : [ { "redirect" : { "url" : "[account:url]", "force" : "0" } } ]
    }
  }');
  return $items;
}
