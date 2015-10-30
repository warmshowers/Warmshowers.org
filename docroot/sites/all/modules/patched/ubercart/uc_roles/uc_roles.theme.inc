<?php

/**
 * @file
 * Theme functions for the uc_roles module.
 */

/**
 * Themes the roles dialog on the account edit page.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_uc_roles_user_new($variables) {
  $form = $variables['form'];

  // Render the expiration tables first.
  $output = drupal_render($form['expirations']);

  $output .= '<div class="expiration">';
  $output .=   drupal_render($form['new_role']);
  $output .=   drupal_render($form['new_role_add']);
  $output .=   drupal_render($form['new_role_add_for']);
  $output .=   drupal_render($form['new_role_add_qty']);
  $output .=   drupal_render($form['new_role_add_granularity']);
  $output .= '</div>';

  return $output;
}

/**
 * Themes the role expiration table on the account edit page.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_uc_roles_user_expiration($variables) {
  $form = $variables['form'];

  $header = array(
    array('data' => t('Make permanent')),
    array('data' => t('Role')),
    array('data' => t('Expiration')),
    array('data' => t('Add/remove time')),
  );

  $rows = array();
  // The expiration table.
  foreach ((array)$form['table'] as $rid => $expiration) {
    // We only want numeric rid's
    if (!is_numeric($rid)) {
      continue;
    }

    // Make sure the renders actually touch the elements.
    $data = &$form['table'][$rid];

    $rows[] = array(
      array('data' => drupal_render($data['remove'])),
      array('data' => check_plain($data['name']['#value'])),
      array('data' => format_date($data['expiration']['#value'], 'short')),

      // Options to adjust the expiration.
      array('data' => '<a name="role-expiration-' . $rid . '">' .
                      '<div class="expiration">' .
                        drupal_render($data['polarity']) . drupal_render($data['qty']) . drupal_render($data['granularity']) .
                      '</div></a>'),
    );
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'caption' => t('Below you can add or remove time to the expiration dates of the following roles.'),
    'empty' => t('There are no pending expirations for roles this user.'),
  ));
  $output .= drupal_render_children($form);

  return $output;
}
