<?php
// $Id: reg_with_pic.admin.inc,v 1.1.2.3 2010/05/24 01:19:20 mmilano Exp $

/**
 * @file Administratino pages for the Reg With Pic module
 */

/**
 * Admin handler for reg with pic settings.
 */
function reg_with_pic_admin_settings() {
  $form = array();
  $form['reg_with_pic_required'] = array(
    '#type' => 'select',
    '#title' => t('Requre Picture on Registration Form?'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => variable_get('reg_with_pic_required', 0)
  );
  return system_settings_form($form);
}