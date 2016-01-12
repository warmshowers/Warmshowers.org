<?php
/**
 * @file
 * ws_rules_feature.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function ws_rules_feature_default_rules_configuration() {
  $items = array();
  $items['rules_admin_email_when_user_blocked'] = entity_import('rules_config', '{ "rules_admin_email_when_user_blocked" : {
      "LABEL" : "Admin email when user blocked",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "admin" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "AND" : [
            { "data_is" : { "data" : [ "account:status" ], "value" : "0" } },
            { "data_is" : { "data" : [ "account-unchanged:status" ], "value" : "1" } }
          ]
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org,jturner@warmshowers.org",
            "subject" : "[Warmshowers] Admin notification: [account-unchanged:fullname]([account-unchanged:name]) disabled by [site:current-user:fullname] ",
            "message" : " [account-unchanged:url] still exists, but is blocked.\\r\\n\\u003Ca href=\\u0022 [account-unchanged:url]\\u0022\\u003E[site:current-user:fullname] disabled\\/blocked\\u003C\\/a\\u003E:   [account-unchanged:fullname] ( [account-unchanged:name]) Email:  [account-unchanged:mail]\\u003Cbr\\/\\u003E\\r\\nLocation:  [account-unchanged:city],  [account-unchanged:province]  [account-unchanged:country]\\u003Cbr\\/\\u003E\\r\\nMember since:  [account-unchanged-unchanged:created]\\r\\n\\r\\nAbout me:\\r\\n [account-unchanged:about_me]",
            "language" : "en-working"
          }
        }
      ]
    }
  }');
  $items['rules_admin_user_deletion_notification'] = entity_import('rules_config', '{ "rules_admin_user_deletion_notification" : {
      "LABEL" : "Admin e-mail notification of admin user deletion",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "admin" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_delete" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "3" : "3", "6" : "6" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org,jturner@warmshowers.org",
            "subject" : "[Warmshowers] Admin notification: [account:fullname]([account:name]) DELETED by [site:current-user:fullname] ",
            "message" : " [account:url] was deleted, no longer exists.\\r\\n\\u003Ca href=\\u0022 [account:url]\\u0022\\u003E[site:current-user:fullname] disabled\\/blocked\\u003C\\/a\\u003E:   [account:fullname] ( [account:name]) Email:  [account:mail]\\u003Cbr\\/\\u003E\\r\\nLocation:  [account:city],  [account:province]  [account:country]\\u003Cbr\\/\\u003E\\r\\nMember since:  [account-unchanged:created]\\r\\n\\r\\nAbout me:\\r\\n [account:about_me]",
            "from" : "noreply@warmshowers.org",
            "language" : "en"
          }
        }
      ]
    }
  }');
  $items['rules_newmember_notification_email'] = entity_import('rules_config', '{ "rules_newmember_notification_email" : {
      "LABEL" : "New member notification to admin",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "-1",
      "OWNER" : "rules",
      "TAGS" : [ "admin" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org",
            "subject" : "WS new user: [account:name] - [account:fullname], [account:city], [account:province], [account:country]\\r\\n",
            "message" : "\\u003Cb\\u003ENew user: \\u003C\\/b\\u003E [account:name] \\r\\n\\u003Cb\\u003EUser email address: \\u003C\\/b\\u003E [account:mail]   \\r\\n\\u003Cb\\u003EFull Name: \\u003C\\/b\\u003E [account:fullname]\\r\\n\\u003Cb\\u003EAbout Me: \\u003C\\/b\\u003E [account:about_me]\\r\\n\\u003Cb\\u003ELocation: \\u003C\\/b\\u003E [account:city], [account:province], [account:country]\\r\\n\\u003Cb\\u003Enotcurrentlyavailable: \\u003C\\/b\\u003E [account:notcurrentlyavailable]\\r\\n\\u003Cb\\u003EHow did you hear about WS.org? \\u003C\\/b\\u003E [account:howdidyouhear]\\r\\n\\u003Cb\\u003EProfile Link: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:url]\\u0022\\u003E[account:url]\\u003C\\/a\\u003E\\r\\n\\u003Cb\\u003EEdit: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:edit-url]\\u0022\\u003E[account:edit-url]\\u003C\\/a\\u003E\\r\\n\\u003Cb\\u003EDelete: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[site:url]\\/user\\/[account:uid]\\/cancel\\u0022\\u003E[site:url]\\/user\\/[account:uid]\\/cancel\\u003C\\/a\\u003E \\r\\n[account:cancel-url]",
            "from" : "noreply@warmshowers.org",
            "language" : "en-working"
          }
        }
      ]
    }
  }');
  $items['rules_notify_about_ios_app_at_login'] = entity_import('rules_config', '{ "rules_notify_about_ios_app_at_login" : {
      "LABEL" : "Message to user at login",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "data_is" : {
            "data" : [ "account:language" ],
            "op" : "IN",
            "value" : { "value" : { "en" : "en", "en-working" : "en-working", "de" : "de" } }
          }
        }
      ],
      "DO" : [
        { "drupal_message" : {
            "message" : "The 5.0.1 version of the IOS app for Warmshowers is now available in the Apple App Store - works with IOS 8 and 9. If you have any trouble, remember to uninstall and reinstall.",
            "repeat" : "0"
          }
        }
      ]
    }
  }');
  $items['rules_notify_on_comment_needing_moderation'] = entity_import('rules_config', '{ "rules_notify_on_comment_needing_moderation" : {
      "LABEL" : "Notify on comment needing moderation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "admin" ],
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
            "to" : "martinmemartin@warmshowers.org",
            "subject" : "Comment posted by new user [comment:author:fullname] ([comment:author:name])- needs approval",
            "message" : "New user \\u003Ca href=\\u0022[comment:author:url]\\u0022\\u003E[comment:author:fullname] ([comment:author:name])\\u003C\\/a\\u003E has posted a comment:\\r\\n---\\r\\n[comment:subject]\\r\\n\\r\\n[comment:body]\\r\\n\\r\\n-------\\r\\nUser\\u0027s About Me:\\r\\n[comment:author:about_me]\\r\\n\\r\\n-------\\r\\nComment approvals: [site:url]admin\\/content\\/comment\\/approval\\r\\nUser profile: [comment:author:url]\\r\\nUser edit: [site:url]user\\/[comment:author:uid]\\/edit\\r\\nUser cancel: [site:url]user\\/[comment:author:uid]\\/cancel\\r\\n",
            "language" : "en"
          }
        }
      ]
    }
  }');
  $items['rules_preauth_user_login_redirect'] = entity_import('rules_config', '{ "rules_preauth_user_login_redirect" : {
      "LABEL" : "Preauth user login redirect",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } },
        { "NOT data_is" : {
            "data" : [ "site:current-page:url" ],
            "value" : "services\\/rest\\/user\\/login"
          }
        }
      ],
      "DO" : [ { "redirect" : { "url" : "unvalidated_user_login" } } ]
    }
  }');
  $items['rules_redirect_to_dashboard_on_login'] = entity_import('rules_config', '{ "rules_redirect_to_dashboard_on_login" : {
      "LABEL" : "Redirect to dashboard on login",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } }
      ],
      "DO" : [ { "redirect" : { "url" : "[site:url]", "force" : "0" } } ]
    }
  }');
  $items['rules_request_feedback_about_self_deletion'] = entity_import('rules_config', '{ "rules_request_feedback_about_self_deletion" : {
      "LABEL" : "E-mail to request feedback about self-deletion",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "member email" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_delete" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "account:uid" ], "value" : [ "site:current-user:uid" ] } },
        { "data_is" : {
            "data" : [ "account:language" ],
            "op" : "IN",
            "value" : { "value" : {
                "und" : "und",
                "en" : "en",
                "en-working" : "en-working",
                "es" : "es",
                "de" : "de"
              }
            }
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "account:mail" ],
            "subject" : "[Warmshowers] Could you please say why you deleted your account? ([account:name])",
            "message" : "Hi - We saw that you deleted your Warmshowers.org account and we\\u0027re sorry to see you go.\\r\\n\\r\\nCould you please take a moment to explain why you decided to delete your account?  Just reply to this email.\\r\\n\\r\\nThanks,\\r\\n-Randy Fay and Mark Martin\\r\\nWarmshowers.org Admins",
            "language" : [ "account:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_send_feedback_notifications_when_feedback_created'] = entity_import('rules_config', '{ "rules_send_feedback_notifications_when_feedback_created" : {
      "LABEL" : "Send feedback notifications when feedback created",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "member email" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--trust_referral" : { "bundle" : "trust_referral" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_rating" } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_member_i_trust" } },
        { "data_is" : { "data" : [ "node:field-rating" ], "value" : "Positive" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "node:field-member-i-trust:mail" ],
            "subject" : "[Warmshowers.org] You have received member feedback from [node:author:fullname]",
            "message" : "Hi [node:field-member-i-trust:fullname],\\r\\n\\r\\nWarmshowers member [node:author:fullname] has written feedback for you.\\r\\n\\r\\nTo view the feedback, please log into \\u003Ca href=\\u0022[site:url]\\u0022\\u003E[site:url]\\u003C\\/a\\u003E and the feedback now appears on your \\u003Ca href=\\u0022[node:field-member-i-trust:url]\\u0022\\u003Eprofile page\\u003C\\/a\\u003E.\\r\\n\\r\\nHave you left feedback for [node:author:fullname] yet? If not, please go to \\u003Ca href=\\u0022[node:author:url]\\u0022\\u003E[node:author:fullname]\\u0027s profile page\\u003C\\/a\\u003E and write about your experience with them. Just click the button that says \\u0022Leave Feedback\\u0022.\\r\\n\\r\\nFeedback is a fundamental part of our community. Only you can tell others how great somebody is or warn them if there is an issue.\\r\\n\\r\\nThanks!\\r\\n\\r\\nWarmshowers Administration",
            "language" : [ "node:field-member-i-trust:language" ]
          }
        },
        { "mail" : {
            "to" : [ "node:author:mail" ],
            "subject" : "[Warmshowers.org] Your member feedback for [node:field-member-i-trust:fullname] was successful",
            "message" : "An email has been sent to  [node:field-member-i-trust:fullname] letting them know about your feedback.\\r\\n\\r\\nThank you for contributing to our community by contributing feedback!\\r\\n\\r\\n- Warmshowers Team",
            "language" : [ "node:author:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_user_account_created'] = entity_import('rules_config', '{ "rules_user_account_created" : {
      "LABEL" : "Go to dashboard after saving account",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "site:current-user:uid" ], "value" : [ "account:uid" ] } }
      ],
      "DO" : [ { "redirect" : { "url" : "[site:url]\\/welcome", "force" : "0" } } ]
    }
  }');
  $items['rules_user_self_deletion_notification'] = entity_import('rules_config', '{ "rules_user_self_deletion_notification" : {
      "LABEL" : "User self-deletion e-mail notification to admin",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "admin" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_delete" : [] },
      "IF" : [
        { "NOT user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "3" : "3", "6" : "6" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org,jturner@warmshowers.org",
            "subject" : "[Warmshowers] User self-deletion notification [account:name]\\/[account:uid] created [account:created]",
            "message" : "[account:fullname] ([account:name]) - UID:[account:uid]), [account:mail]\\r\\n[account:city], [account:province], [account:country] \\r\\nMember since: [account:created]\\r\\n\\r\\nAbout me:\\r\\n[account:about_me]",
            "from" : "noreply@warmshowers.org",
            "language" : "en"
          }
        }
      ]
    }
  }');
  $items['uc_cart_e_mail_admin_checkout_notification_cloned_'] = entity_import('rules_config', '{ "uc_cart_e_mail_admin_checkout_notification_cloned_" : {
      "LABEL" : "E-mail admin donation notification",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "admin" ],
      "REQUIRES" : [ "rules", "uc_order", "uc_payment" ],
      "ON" : { "uc_payment_entered" : [] },
      "DO" : [
        { "uc_order_email_invoice" : {
            "order" : [ "order" ],
            "from" : "\\u0022Warmshowers.org Donations\\u0022 \\u003Crandy+wsl@warmshowers.org\\u003E",
            "addresses" : "chris.andrews.russo+donations@gmail.com",
            "subject" : "New Donation [order:total] [order:email] #newdonation",
            "template" : "admin",
            "view" : "admin-mail"
          }
        }
      ]
    }
  }');
  return $items;
}
