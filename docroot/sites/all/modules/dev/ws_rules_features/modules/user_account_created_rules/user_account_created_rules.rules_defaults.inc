<?php
/**
 * @file
 * user_account_created_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function user_account_created_rules_default_rules_configuration() {
  $items = array();
  $items['rules_user_account_created'] = entity_import('rules_config', '{ "rules_user_account_created" : {
      "LABEL" : "User account created",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org",
            "subject" : "WS new user: [account:name]",
            "message" : "\\u003Cb\\u003ENew user: \\u003C\\/b\\u003E [account:name] \\r\\n\\u003Cb\\u003EUser email address: \\u003C\\/b\\u003E [account:mail]   \\r\\n\\u003Cb\\u003EFull Name: \\u003C\\/b\\u003E [account:fullname]\\r\\n\\u003Cb\\u003EAbout Me: \\u003C\\/b\\u003E [account:about_me]\\r\\n\\u003Cb\\u003ELocation: \\u003C\\/b\\u003E [account:city], [account:province], [account:country]\\r\\n\\u003Cb\\u003EGeocoding Accuracy: \\u003C\\/b\\u003E [account:source]\\r\\n\\u003Cb\\u003Enotcurrentlyavailable: \\u003C\\/b\\u003E [account:notcurrentlyavailable]\\r\\n\\u003Cb\\u003EHow did you hear about WS.org? \\u003C\\/b\\u003E [account:howdidyouhear]\\r\\n\\u003Cb\\u003EProfile Link: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:account-url]\\u0022\\u003E[account:url]\\u003C\\/a\\u003E\\r\\n\\u003Cb\\u003EEdit: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:edit-url]\\u0022\\u003E[account:edit-url]\\u003C\\/a\\u003E\\r\\n\\u003Cb\\u003EDelete: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:cancel-url]\\u0022\\u003E[account:cancel-url]\\u003C\\/a\\u003E ",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
