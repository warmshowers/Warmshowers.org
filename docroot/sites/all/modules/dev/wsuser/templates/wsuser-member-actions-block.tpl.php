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
 */

if (!$is_self) {
  print theme('linkbutton',
    array(
      'link' => array(
        'title' => t('Provide Feedback'),
        'href' => url('node/add/trust-referral', array(
            'absolute' => TRUE,
            'query' => array(
              'edit[field_member_i_trust][und][0][uid]' => $account->name,
            ),
          )
        ),
        'classes' => 'rounded dark big',
      )
    )
  );
}
