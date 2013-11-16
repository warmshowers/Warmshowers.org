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
  print theme('linkbutton',
    array(
      'title' => t('Provide Feedback'),
      'href' => url('node/add/trust-referral', array(
          'absolute' => TRUE,
          'query' => array(
            'edit[field_member_i_trust][0][uid][uid]' => $username,
          ),
        )
      ),
      'classes' => 'rounded dark big',
    )
  );
endif;
?>
