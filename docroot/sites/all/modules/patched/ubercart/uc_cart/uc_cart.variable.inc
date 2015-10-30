<?php

/**
 * @file
 * Variable module hook implementations.
 */


/**
 * Implements hook_variable_group_info().
 */
function uc_cart_variable_group_info() {
  $groups['uc_cart_checkout'] = array(
    'title' => t('Ubercart checkout settings'),
    'access' => 'administer store',
    'path' => array('admin/store/settings/checkout'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function uc_cart_variable_info($options) {
  $variables['uc_msg_order_submit'] = array(
    'type' => 'text',
    'title' => t('Completion message header', array(), $options),
    'description' => t('Header for message displayed after a user checks out.', array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => uc_get_message('completion_message'),
  );
  $variables['uc_msg_order_logged_in'] = array(
    'type' => 'text',
    'title' => t('Completion message for logged in users', array(), $options),
    'description' => t('Message displayed upon checkout for a user who is logged in.', array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => uc_get_message('completion_logged_in'),
  );
  $variables['uc_msg_order_existing_user'] = array(
    'type' => 'text',
    'title' => t('Completion message for existing users', array(), $options),
    'description' => t("Message displayed upon checkout for a user who has an account but wasn't logged in.", array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => uc_get_message('completion_existing_user'),
  );
  $variables['uc_msg_order_new_user'] = array(
    'type' => 'text',
    'title' => t('Completion message for new users', array(), $options),
    'description' => t("Message displayed upon checkout for a new user whose account was just created. You may use the special tokens !new_username for the username of a newly created account and !new_password for that account's password.", array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => uc_get_message('completion_new_user'),
  );
  $variables['uc_msg_order_new_user_logged_in'] = array(
    'type' => 'text',
    'title' => t('Completion message for new logged in users', array(), $options),
    'description' => t('Message displayed upon checkout for a new user whose account was just created and also <em>"Login users when new customer accounts are created at checkout."</em> is set on the <a href="!user_login_setting_ur">checkout settings</a>.', array('!user_login_setting_ur' => 'admin/store/settings/checkout'), $options),
    'group' => 'uc_cart_checkout',
    'default' => uc_get_message('completion_new_user_logged_in'),
  );
  $variables['uc_msg_continue_shopping'] = array(
    'type' => 'text',
    'title' => t('Continue shopping message', array(), $options),
    'description' => t('Message displayed upon checkout to direct customers to another part of your site.', array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => uc_get_message('continue_shopping'),
  );
  $variables['uc_cart_new_account_details'] = array(
    'type' => 'text',
    'title' => t('New account details help message', array(), $options),
    'description' => t('Enter the help message displayed in the new account details fieldset when shown.', array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => t('<b>Optional.</b> New customers may supply custom account details.<br />We will create these for you if no values are entered.', array(), $options),
  );
  $variables['uc_checkout_instructions'] = array(
    'type' => 'text',
    'title' => t('Checkout instructions', array(), $options),
    'description' => t('Provide instructions for customers at the top of the checkout screen.', array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => '',
  );
  $variables['uc_checkout_review_instructions'] = array(
    'type' => 'text',
    'title' => t('Checkout review instructions', array(), $options),
    'description' => t('Provide instructions for customers at the top of the checkout review screen.', array(), $options),
    'group' => 'uc_cart_checkout',
    'default' => t("Your order is almost complete. Please review the details below and click 'Submit order' if all the information is correct.  You may use the 'Back' button to make changes to your order if necessary.", array(), $options),
  );
  return $variables;
}
