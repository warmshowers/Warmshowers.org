<?php
/**
 * @file
 * positive_feedback_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function positive_feedback_rules_default_rules_configuration() {
  $items = array();
  $items['rules_positive_feedback_received'] = entity_import('rules_config', '{ "rules_positive_feedback_received" : {
      "LABEL" : "Positive feedback received",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Feedback" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--trust_referral" : { "bundle" : "trust_referral" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_rating" } },
        { "data_is" : { "data" : [ "node:field-rating" ], "value" : "Positive" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[node:field_member_i_trust:mail]",
            "subject" : "[Warmshowers.org] You have received member feedback",
            "message" : "Hi [node:field_member_i_trust:fullname],\\r\\n\\r\\nWarmshowers member [node:author:fullname] has written feedback for you.\\r\\n\\r\\nTo view the feedback, please log into \\u003Ca href=\\u0022[site:url]\\u0022\\u003Ewarmshowers.org\\u003C\\/a\\u003E and the feedback now appears on your \\u003Ca href=\\u0022[node:field_member_i_trust:url]\\u0022\\u003Eprofile page\\u003C\\/a\\u003E.\\r\\n\\r\\nHave you left feedback for [node:author:fullname] yet? If not, please go to \\u003Ca href=\\u0022[node:author:url]\\u0022\\u003E[node:author:fullname]\\u0027s profile page\\u003C\\/a\\u003E and write about your experience with them. Just click the button that says \\u0022Leave Feedback\\u0022.\\r\\n\\r\\nFeedback is a fundamental part of our community. Only you can tell others how great somebody is (or in some cases that there might be an issue...)\\r\\n\\r\\nThanks!\\r\\n\\r\\nWarmshowers Administration",
            "language" : [ "node:field-member-i-trust:language" ]
          }
        },
        { "mail" : {
            "to" : "[node:author:mail]",
            "subject" : "[Warmshowers.org] Your member feedback was successful",
            "message" : "An email has been sent to [node:field_member_i_trust:fullname] letting them know about your feedback.",
            "language" : [ "node:author:language" ]
          }
        }
      ]
    }
  }');
  return $items;
}
