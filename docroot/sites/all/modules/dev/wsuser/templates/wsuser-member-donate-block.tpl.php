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

?>

<?php
// If looking at own account:
if ($is_self) {
  if (wsuser_is_current_donor_member($account)) {
    $role_desc = wsuser_highest_donation_level($account);
    $rid = key($role_desc);
    $expire_date = wsuser_uc_role_expiration($account, $rid);
    print "<ul><li>";
    print t("You are a current donor member, Thanks!") . "</li>";
    print "<li>" . t("Your membership is at the %role level and it expires on %expire", array('%role' => $role_desc[$rid], '%expire' => date( 'Y-m-d', $expire_date))) . "</li>";
    print "<li>" . t("View your !history or adjust !recurring",
        array(
          '!history' => l(t('donation history'), "user/{$account->uid}/orders"),
          '!recurring' => l(t('recurring donations'), "user/{$account->uid}/recurring-fees")
        )
      ) . "</li></ul>";
  }
  else {
    print theme('linkbutton',
      array(
        'title' => t('Donate Now'),
        'href' => url('donate', array(
            'absolute' => TRUE,
          )
        ),
        'classes' => 'rounded dark big',
      )
    );
  }
}
// If looking at someone else's account
else {
  if (wsuser_is_current_donor_member($account)) {
    print t('%fullname is currently a donor member of Warmshowers.org.', array('%fullname' => $account->fullname));
  }
  else {
    print t('%fullname is not yet a donor member of Warmshowers.org.', array('%fullname' => $account->fullname));
  }
}

?>
