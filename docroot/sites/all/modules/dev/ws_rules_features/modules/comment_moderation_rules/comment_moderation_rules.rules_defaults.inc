<?php
/**
 * @file
 * comment_moderation_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function comment_moderation_rules_default_rules_configuration() {
  $items = array();
  $items['rules_notify_on_comment_needing_moderation'] = entity_import('rules_config', '{ "rules_notify_on_comment_needing_moderation" : {
      "LABEL" : "Notify on comment needing moderation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Moderation" ],
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : { "comment_insert" : [] },
      "IF" : [
        { "NOT user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "15" : "15" } }
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "randy+wslreports@warmshowers.org, martinmemartin@warmshowers.org",
            "subject" : "Comment posted by new user [comment:author:fullname] ([comment:author:name]) - needs approval",
            "message" : "New user \\u003Ca href=\\u0022[comment:author:url]\\u0022\\u003E[comment:author:fullname] ([comment:author:name])\\u003C\\/a\\u003E has posted a comment:\\r\\n---\\r\\n[comment:title]\\r\\n\\r\\n[comment:body]\\r\\n\\r\\nUser\\u0027s About Me:\\r\\n[comment:author:about_me]\\r\\n\\r\\n-------\\r\\nComment approvals: http:\\/\\/www.warmshowers.org\\/admin\\/content\\/comment\\/approval\\r\\nUser profile: [comment:author:url]\\r\\nUser edit: [comment:author:edit-url]\\r\\nUser delete: [comment:author:cancel-url]",
            "from" : "noreply@warmshowers.org",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
