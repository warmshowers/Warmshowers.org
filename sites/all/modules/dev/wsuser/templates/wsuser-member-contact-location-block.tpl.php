<?php
/**
 * Member contact location block template
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
 *
 * @see wsuser_preprocess_wsuser_member_contact_location()
 */
?>

<div class="member-map">
  <a class="thickbox" href="/maponly/uid=<?php print $uid; ?>?KeepThis=true&TB_iframe=true&height=600&width=800" accesskey="" >
    <img src="http://maps.googleapis.com/maps/api/staticmap?zoom=8&size=200x200&sensor=false&markers=color:blue%7Clabel:S%7C <?php print $latitude . ',' . $longitude; ?>" />
  </a>
</div>


<div class="member-address">
  <span class="member-fullname"><?php print $fullname; ?></span><br/>
  <?php if ($homephone) : ?>
    <span class="phone homephone"><?php print t('Home Phone Number') . ': ' . $homephone; ?></span><br/>
  <?php endif; ?>
  <?php if ($mobilephone) : ?>
    <span class="phone mobilephone"><?php print t('Mobile Number') . ': ' . $mobilephone; ?></span><br/>
  <?php endif; ?>
  <?php if ($workphone) : ?>
  <span class="phone workphone"><?php print t('Work Phone Number') . ': ' . $workphone; ?></span><br/>
  <?php endif; ?>

  <?php if ($notcurrentlyavailable): ?>
  <div class="notcurrentlyavailable"><?php print t('Address information is not shown because this member is not currently available for hosting.'); ?></div>
  <?php endif; ?>

  <?php if ($street && !$notcurrentlyavailable): ?>
    <span class="member-street"><?php print $street; ?></span><br/>
  <?php endif; ?>
  <?php if ($additional && !$notcurrentlyavailable): ?>
    <span class="member-additional"><?php print $additional; ?></span><br/>
  <?php endif; ?>
  <?php if (!$notcurrentlyavailable): ?>
    <span class="member-city"><?php print $city . ', ' . $province . ' ' . $postal_code . ' ' . $country; ?></span>
  <?php endif; ?>
</div>

<?php
if ($account->uid != $GLOBALS['user']->uid) {
  print theme('linkbutton', array('title' => t('Send Message'), 'href' => 'user/' .  $account->uid . '/contact'));
}
else {
  print theme('linkbutton', array('title' => t('Update'), 'href' => 'user/' . $account->uid . '/edit'));
  print theme('linkbutton', array('title' => t('Set Location'), 'href' => 'user/' . $account->uid . '/location'));
} ?>
