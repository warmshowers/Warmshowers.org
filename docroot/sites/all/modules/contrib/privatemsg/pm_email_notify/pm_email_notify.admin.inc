<?php

/**
 * @file
 * Administration menu callbacks for pm_email_notify.module.
 */


/**
 * Menu callback for administration settings.
 */
function pm_email_notify_admin_settings_form() {
  $form['pm_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privatemsg e-mail notification'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['pm_email']['privatemsg_setting_email_notify_level'] = array(
    '#type' => 'radios',
    '#title' => t('Default e-mail notification level'),
    '#options' => array(
      PM_EMAIL_NOTIFY_LEVEL_DISABLED => t('Never send a notification'),
      PM_EMAIL_NOTIFY_LEVEL_THREAD => t('Only send a notification when a new discussion thread is created'),
      PM_EMAIL_NOTIFY_LEVEL_UNREAD_ONCE => t("Only send a notification when the user has not already received a notification since he last viewed a discussion thread."),
      PM_EMAIL_NOTIFY_LEVEL_ALL => t('Always send a notification'),
    ),
    '#default_value' => variable_get('privatemsg_setting_email_notify_level', PM_EMAIL_NOTIFY_LEVEL_ALL),
    '#description' => t('Choose when e-mail notifications will be sent by the system. Users with the appropriate permission may override this setting on the account edit page.'),
    '#weight' => 0,
  );
  $form['pm_email']['privatemsg_setting_email_notify_only_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only send e-mail notifications to individual users who are addressed directly'),
    '#default_value' => variable_get('privatemsg_setting_email_only_user', FALSE),
    '#weight' => 5,
  );


  $form['pm_email']['pm_email_template'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail customizations'),
    '#weight' => 15,
    '#description' => t('Customize the e-mail notification sent to users. Available variables are: !author, !author_uid, !pm_subject, !pm_body, !thread, !site, !login_url, !uri, !uri_brief, !message (URL) and !settings (URL).</p><p>To translate these messages use the i18nstrings.module and translate them trough the <a href="!translate_url">Translate interface</a> ("Privatemsg Email Notification" text group).</p>', array('!translate_url' => url('admin/build/translate/search'))),
  );

  $form['pm_email']['pm_email_template']['pm_email_notify_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From e-mail address for notifications'),
    '#default_value' => variable_get('pm_email_notify_from',''),
    '#weight' => 10,
    '#description' => t('This is the e-mail address that notifications will come from. Leave blank to use the site default.'),
  );

  $form['pm_email']['pm_email_template']['pm_email_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _pm_email_notify_text('subject', NULL, FALSE),
    '#weight' => 5,
  );

  $form['pm_email']['pm_email_template']['pm_email_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _pm_email_notify_text('body', NULL, FALSE),
    '#weight' => 10,
  );
  $form = system_settings_form($form);
  if (module_exists('i18nstrings')) {
    $form['#submit'][] = 'pm_email_notify_admin_settings_update_strings';
  }
  return $form;
}

/**
 * Form submit callback to update the translation string sources.
 */
function pm_email_notify_admin_settings_update_strings($form, &$form_state) {
  pm_email_notify_locale_refresh();
}