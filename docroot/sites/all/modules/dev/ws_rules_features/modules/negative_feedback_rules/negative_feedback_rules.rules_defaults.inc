<?php
/**
 * @file
 * negative_feedback_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function negative_feedback_rules_default_rules_configuration() {
  $items = array();
  $items['rules_negative_feedback_received'] = entity_import('rules_config', '{ "rules_negative_feedback_received" : {
      "LABEL" : "Negative feedback received",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Feedback" ],
      "REQUIRES" : [ "rules", "slack" ],
      "ON" : { "node_insert--trust_referral" : { "bundle" : "trust_referral" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_rating" } },
        { "data_is" : { "data" : [ "node:field-rating" ], "value" : "Negative" } }
      ],
      "DO" : [
        { "slack_send_message" : { "message" : "Negative feedback has been created for user [node:field-member-i-trust:url]\\r\\nPlease review the feedback here - [node:url]" } }
      ]
    }
  }');
  return $items;
}
