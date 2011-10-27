<?php

/**
 * Implementation of hook_rules_defaults().
 */
function member_recommendation_rule_rules_defaults() {
  return array(
    'rules' => array(
      'member_recommendation_rule_1' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Member_Recommendation_email',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'member_recommendation_rule' => 'member_recommendation_rule',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'trust_referral' => 'trust_referral',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Created content is Member Recommendation',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Send a mail to an arbitrary mail address',
              'module' => 'System',
              'eval input' => array(
                '0' => 'subject',
                '1' => 'message',
                '2' => 'from',
                '3' => 'to',
              ),
            ),
            '#name' => 'rules_action_mail',
            '#settings' => array(
              'to' => '[node:field_member_i_trust-mail]',
              'from' => '',
              'subject' => 'You have received a member recommendation',
              'message' => 'Hi [node:field_member_i_trust-name],

[node:author-name] has written a recommendation for you.

To view this recommendation, please login to the warmshowers.org website.  Then either go to your profile page to see the recommendation or click on this link [node:node-url].

Thanks!

Warmshowers Administration',
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'message' => array(
                    '0' => 'node',
                  ),
                  'to' => array(
                    '0' => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}
