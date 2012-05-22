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

<?php print drupal_get_form('wsuser_simple_link_button', t('Recommend'),
  url('node/add/trust-referral', array(
    'absolute' => TRUE,
    'query' => array(
      'edit[field_member_i_trust][0][uid][uid]' => $username,
    ),
  )));
?>

<div class="flags">
  <div class="flag-description">
    <?php print t('We count on members to say whether another member has been responsive or not. Please click below based on the responsiveness of this member'); ?>
  </div>
  <?php print flag_create_link('responsive_member', $uid); ?>
  <?php print flag_create_link('unresponsive_member', $uid); ?>

  <div class="responsive-counts">
    <?php if ($responsive_member) {
    print format_plural($responsive_member, '%count member has marked %fullname responsive', '%count members have marked %fullname responsive', array('%count' => $responsive_member, '%fullname' => $fullname));
  }
    ?>
    <?php if ($unresponsive_member) {
      print format_plural($unresponsive_member, '%count member has marked %fullname unresponsive', '%count members have marked %fullname unresponsive', array('%count' => $unresponsive_member, '%fullname' => $fullname));
    }
    ?>

  </div>


</div>
