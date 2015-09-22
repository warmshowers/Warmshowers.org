<?php
/**
 * Member donate block template
 *
 * Supported variables
 * - $account (not sanitized)
 * - $uid
 * - $username
 * - $fullname (User's full name from wsuser)
 * - $homephone
 * - $mobilephone
 * - $homephone
 * - $street
 * - $additional
 * - $city
 * - $province
 * - $country
 * - $postal_code
 * - $latitude
 * - $longitude
 * - $responsive_member: Count of times marked responsive
 * - $unresponsive_member : Count of times marked unresponsive
 * @see wsuser_preprocess_wsuser_member_contact_location()
 */

// If looking at own account or admin viewing:
if ($is_self || user_access('administer users')) {
  if (wsuser_is_current_donor_member($account)) {
    $role_desc = wsuser_highest_donation_level($account);
    $rid = key($role_desc);
    $expire_date = wsuser_uc_role_expiration($account, $rid);
    print "<ul><li>";
    print t("You are a current donor, Thanks!") . "</li>";
    print "<li>" . t("You're at the %role level, which expires on %expire", array(
        '%role' => $role_desc[$rid],
        '%expire' => date('Y-m-d', $expire_date)
      )) . "</li>";

    if (drupal_valid_path("user/{$account->uid}/orders")) {
      print "<li>" . t("View your !history",
          array(
            '!history' => l(t('donation history'), "user/{$account->uid}/orders"),
          )
        ) . "</li>";
      if (drupal_valid_path("user/{$account->uid}/recurring-fees")) {
        print "<li>" . t("View and adjust !recurring",
            array(
              '!recurring' => l(t('recurring donations'), "user/{$account->uid}/recurring-fees")
            )
          ) . "</li>";
      }
      print '</ul>';
      print '<p>' . t('(This information is displayed to you but not to other members.)') . '</p>';
    }
  }
  else {
    if (wsuser_is_nondonor_member($account)) {
      print t('Thanks for signing up for a free donation level. At any time you can upgrade on the !link page.', array('!link' => l(t('Donate'), 'donate')));
    }
    else {
      print theme('linkbutton',
      array('link' =>
        array(
          'title' => t('Donate Now'),
          'href' => url('donate', array(
              'absolute' => TRUE,
            )
          ),
          'classes' => 'rounded dark big',
        )
      )
      );
      print '<p>' . t('Your donation will help Warmshowers to continue to thrive and provide this great service connecting hosts and bike travelers.') . '</p>';

    }
  }

//  print t('You can choose to show or hide donation status by editing your profile.') . " ";
//  if ($account->hide_donation_status) {
//    print t('Your donation status is currently hidden from other members.');
//  } else {
//    print t('Your donation status is currently shown to other members.');
//  }
}

// If looking at someone else's account
//else {
//  if (!$account->hide_donation_status) {
//    if (wsuser_is_current_donor_member($account)) {
//      print t('%fullname is currently a donor member of Warmshowers.org.', array('%fullname' => $account->fullname));
//    }
//    else {
//      print t('%fullname is not yet a donor member of Warmshowers.org.', array('%fullname' => $account->fullname));
//    }
//  } else {
//    print t('Donation status is kept private for this member.');
//  }
//}

?>
