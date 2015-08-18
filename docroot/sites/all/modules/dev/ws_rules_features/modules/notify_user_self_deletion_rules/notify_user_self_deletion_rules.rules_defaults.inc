<?php
/**
 * @file
 * notify_user_self_deletion_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function notify_user_self_deletion_rules_default_rules_configuration() {
  $items = array();
  $items['rules_notify_on_user_self_deletion'] = entity_import('rules_config', '{ "rules_notify_on_user_self_deletion" : {
      "LABEL" : "Notify on user self-deletion",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_delete" : [] },
      "IF" : [
        { "NOT user_has_role" : {
            "account" : [ "account" ],
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
        },
        { "data_is" : { "data" : [ "site:current-user:uid" ], "value" : [ "account:uid" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org,jturner@warmshowers.org",
            "subject" : "[Warmshowers] User self-deletion notification [account:name]\\/[account:uid] member [account:created]",
            "message" : "[account:fullname] ([account:name] - UID:[account:uid]), [account:mail]\\u003Cbr\\/\\u003E\\r\\nMember since: [account:created]\\r\\n\\r\\nAbout me:\\r\\n[account:about_me]",
            "from" : "noreply@warmshowers.org",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
