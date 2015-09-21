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
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "account:status" ], "value" : "0" } },
        { "data_is" : { "data" : [ "account-unchanged:status" ], "value" : "1" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org,jturner@warmshowers.org",
            "subject" : "[Warmshowers] Admin user disable notification [account:fullname] ([account:name] disabled by [site:current-user:name])",
            "message" : "[site:current-user:name] disabled [account-unchanged:fullname] ([account-unchanged:name]) Email: [account-unchanged:mail]\\u003Cbr\\/\\u003E\\r\\nLocation: [account-unchanged:city], [account-unchanged:province] [account-unchanged:country]\\u003Cbr\\/\\u003E\\r\\nMember since: [account-unchanged:created]\\r\\n\\r\\nAbout me:\\r\\n[account-unchanged:about_me]",
            "language" : [ "" ]
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
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "DO" : [
        { "mail" : {
            "to" : "userstatusemails@warmshowers.org",
            "subject" : "WS new user: [account:name]",
            "message" : "\\u003Cb\\u003ENew user: \\u003C\\/b\\u003E [account:name] \\r\\n\\u003Cb\\u003EUser email address: \\u003C\\/b\\u003E [account:mail]   \\r\\n\\u003Cb\\u003EFull Name: \\u003C\\/b\\u003E [account:fullname]\\r\\n\\u003Cb\\u003EAbout Me: \\u003C\\/b\\u003E [account:about_me]\\r\\n\\u003Cb\\u003ELocation: \\u003C\\/b\\u003E [account:city], [account:province], [account:country]\\r\\n\\u003Cb\\u003EGeocoding Accuracy: \\u003C\\/b\\u003E [account:source]\\r\\n\\u003Cb\\u003Enotcurrentlyavailable: \\u003C\\/b\\u003E [account:notcurrentlyavailable]\\r\\n\\u003Cb\\u003EHow did you hear about WS.org? \\u003C\\/b\\u003E [account:howdidyouhear]\\r\\n\\u003Cb\\u003EProfile Link: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:url]\\u0022\\u003E[account:url]\\u003C\\/a\\u003E\\r\\n\\u003Cb\\u003EEdit: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[account:account-edit]\\u0022\\u003E[account:edit-url]\\u003C\\/a\\u003E\\r\\n\\u003Cb\\u003EDelete: \\u003C\\/b\\u003E\\u003Ca href=\\u0022[site-url]\\/user\\/[account:uid]\\/delete\\u0022\\u003E[site-url]\\/user\\/[account:uid]\\/delete\\u003C\\/a\\u003E ",
            "from" : "noreply@warmshowers.org",
            "language" : [ "account:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_redirect_to_dashboard_on_login'] = entity_import('rules_config', '{ "rules_redirect_to_dashboard_on_login" : {
      "LABEL" : "Redirect to dashboard on login",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "DO" : [ { "redirect" : { "url" : "[site:url]", "force" : "0" } } ]
    }
  }');
  $items['rules_request_feedback_about_self_deletion'] = entity_import('rules_config', '{ "rules_request_feedback_about_self_deletion" : {
      "LABEL" : "Request feedback about self-deletion",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
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
  $items['rules_user_account_created'] = entity_import('rules_config', '{ "rules_user_account_created" : {
      "LABEL" : "User account creation tasks - Go to dashboard, send admin email",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "DO" : [
        { "redirect" : { "url" : "[site:url]", "force" : "0" } },
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
  $items['rules_user_redirection'] = entity_import('rules_config', '{ "rules_user_redirection" : {
      "LABEL" : "Redirect to profile on account save",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_update" : [] },
      "DO" : [ { "redirect" : { "url" : "[account:url]", "force" : "0" } } ]
    }
  }');
  $items['uc_cart_e_mail_admin_checkout_notification_cloned_'] = entity_import('rules_config', '{ "uc_cart_e_mail_admin_checkout_notification_cloned_" : {
      "LABEL" : "E-mail admin checkout notification (cloned)",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "uc_order", "uc_cart" ],
      "ON" : { "uc_checkout_complete" : [] },
      "DO" : [
        { "uc_order_email_invoice" : {
            "order" : [ "order" ],
            "from" : "\\u0022Warmshowers.org Donations\\u0022 \\u003Crandy+wsl@warmshowers.org\\u003E",
            "addresses" : "randy+wsl@warmshowers.org",
            "subject" : "New Donation [order:total] [order:email] #newdonation",
            "template" : "admin",
            "view" : "admin-mail"
          }
        }
      ]
    }
  }');
  $items['uc_cart_e_mail_customer_checkout_notification_cloned_'] = entity_import('rules_config', '{ "uc_cart_e_mail_customer_checkout_notification_cloned_" : {
      "LABEL" : "E-mail customer checkout notification (cloned)",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "uc_order", "uc_cart" ],
      "ON" : { "uc_checkout_complete" : [] },
      "DO" : [
        { "uc_order_email_invoice" : {
            "order" : [ "order" ],
            "from" : "\\u0022Warmshowers.org Donations\\u0022 \\u003Crandy+wsl@warmshowers.org\\u003E",
            "addresses" : "[order:email]",
            "subject" : "Thank you for your Warmshowers.org Donation!",
            "template" : "customer",
            "view" : "checkout-mail"
          }
        }
      ]
    }
  }');
  return $items;
}
