<?php
/**
 * @file
 * notify_deletion_admin_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function notify_deletion_admin_rules_default_rules_configuration() {
  $items = array();
  $items['rules_notify_on_deletion_by_admin'] = entity_import('rules_config', '{ "rules_notify_on_deletion_by_admin" : {
      "LABEL" : "Notify on deletion by admin",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_delete" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : {
                "3" : "3",
                "27" : "27",
                "26" : "26",
                "8" : "8",
                "12" : "12",
                "17" : "17",
                "6" : "6",
                "13" : "13",
                "11" : "11",
                "16" : "16"
              }
            }
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org,jturner@warmshowers.org",
            "subject" : "[Warmshowers] Admin user deletion notification [account:fullname] ([account:name] deleted by [site:current-user:name])",
            "message" : "Admin [site:current-user:name] deleted [account:fullname] ([account:name]) Email: [account:mail]\\u003Cbr\\/\\u003E\\r\\nLocation: [account:city], [account:province] [account:country]\\u003Cbr\\/\\u003E\\r\\nMember since: [account:created]\\r\\n\\r\\nAbout me:\\r\\n[account:about_me]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
