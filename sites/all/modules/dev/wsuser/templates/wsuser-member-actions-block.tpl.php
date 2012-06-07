<?php
/**
 * Member actions block template
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

<?php if (!$is_self):
  print theme('linkbutton', array(
    'title' => t('Recommend'),
    'href' => url('node/add/trust-referral', array(
      'absolute' => TRUE,
      'query' => array(
        'edit[field_member_i_trust][0][uid][uid]' => $username,
      ),
    )))
);
endif;
?>

<div class="flags">
  <?php if (!$is_self) : ?>
    <div class="flag-description">
      <?php print t('We count on members to say whether another member has been responsive or not. Please click below based on the responsiveness of this member'); ?>
    </div>
    <?php print flag_create_link('responsive_member', $uid); ?>
    <?php print flag_create_link('unresponsive_member', $uid); ?>
  <?php endif; ?>

  <div class="responsive-counts">
    <?php if ($responsive_member): ?>
      <div class="responsive-count">
      <?php print format_plural($responsive_member, '%count member marked this member responsive', '%count members marked this member responsive', array('%count' => $responsive_member)); ?>
      </div>
    <?php endif; ?>
    <?php if ($unresponsive_member): ?>
      <div class="unresponsive-count">
      <?php print format_plural($unresponsive_member, '%count member marked this member unresponsive', '%count members marked this member unresponsive', array('%count' => $unresponsive_member)); ?>
      </div>
    <?php endif; ?>
  </div>


</div>
