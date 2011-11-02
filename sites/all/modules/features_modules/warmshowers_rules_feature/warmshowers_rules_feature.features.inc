<?php

/**
 * Implementation of hook_rules_defaults().
 */
function warmshowers_rules_feature_rules_defaults() {
  return array(
    'rules' => array(
      'warmshowers_rules_feature_1' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Member_Recommendation_email',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'member_recommendation_rule' => 'member_recommendation_rule',
          'warmshowers_rules_feature' => 'warmshowers_rules_feature',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '0' => array(
              '#weight' => 0,
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
              '#name' => 'rules_condition_content_is_type',
              '#settings' => array(
                'type' => array(
                  'trust_referral' => 'trust_referral',
                ),
                '#argument map' => array(
                  'node' => 'node',
                ),
              ),
              '#type' => 'condition',
            ),
            '#type' => 'OR',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#type' => 'action',
            '#settings' => array(
              'to' => '[node:field_member_i_trust-mail]',
              'from' => '',
              'subject' => 'You have received a member recommendation',
              'message' => 'Hi [node:field_member_i_trust-name],

Warmshowers member [node:author-name] has written a recommendation for you.

To view this recommendation, please log into <a href="http://www.warmshowers.org">warmshowers.org</a> and the recommendation now appears on your <a href="[node:field_member_i_trust-url]">profile page</a>.

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
            '#name' => 'rules_action_mail',
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
          ),
        ),
        '#version' => 6003,
      ),
      'warmshowers_rules_feature_2' => array(
        '#type' => 'rule',
        '#set' => 'event_user_insert',
        '#label' => 'newmember_notification_email',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'warmshowers_rules_feature' => 'warmshowers_rules_feature',
        ),
        '#status' => 'default',
        '#conditions' => array(),
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
              'to' => 'wsl@warmshowers.org',
              'from' => '',
              'subject' => 'WS new user: [user:user]',
              'message' => '<b>New user [user:user] ([user:mail]):  </b>
<br/>
<b>Full Name:</b> [user:fullname]
<br/>
<b>About Me:</b> [user:about_me]
<br/>
<b>Location:</b> [user:city], [user:province], [user:country]
<br/>
<b>Geocoding Accuracy:</b> [user:source]
<br/>
<b>notcurrentlyavailable:</b> [user:notcurrentlyavailable]
<br/>
<b>How did you hear about WS.org?</b>: [user:howdidyouhear]
<br/>
<b>Profile Link</b>[user:account-url]
<br/>
<b>Edit:</b>[user:account-edit]
<br/>
<b>Delete:</b>[user:account-url]/delete',
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'subject' => array(
                    '0' => 'user',
                  ),
                  'message' => array(
                    '0' => 'user',
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
