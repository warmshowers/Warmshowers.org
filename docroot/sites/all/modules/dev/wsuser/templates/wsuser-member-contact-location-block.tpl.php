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
 * - $source
 *
 * @see wsuser_preprocess_wsuser_member_contact_location()
 */
?>

<div class="member-map">
	<a class="colorbox-load" href="/user/<?php print $uid; ?>/maponly/8?height=90%25&width=90%25&iframe=true" accesskey="" >
		<img title="<?php print t('Click for more detail and nearby hosts'); ?>" alt="<?php print t('Location map');?>" src="https://maps.googleapis.com/maps/api/staticmap?zoom=8&size=240x220&sensor=false&markers=color:blue%7Clabel:S%7C <?php print $latitude . ',' . $longitude; ?>" />
	</a>
</div>

<div class="extra_div_wrapper">
<div class="member-fullname"><?php print $fullname; ?></div>

<div class="member-address">
	<?php if ($notcurrentlyavailable) { ?>
	<div class="notcurrentlyavailable"><?php print t('Address information is not shown because this member is not currently available for hosting.'); ?></div>
	<?php } else { ?>

	<?php if ($street && !$notcurrentlyavailable): ?>
		<span class="member-street"><?php print $street; ?></span><br/>
	<?php endif; ?>
	<?php if ($additional && !$notcurrentlyavailable): ?>
		<span class="member-additional"><?php print $additional; ?></span><br/>
	<?php endif; ?>
	<?php if (!$notcurrentlyavailable): ?>
		<span class="member-city"><?php print $city . ', ' . $province . ' ' . $postal_code . ' ' . $country; ?></span><br/>
	<?php endif; ?>
  <?php if ($source <= 5 && !$notcurrentlyavailable): ?>
    <span class="member-latlon"><?php print t('Lat') . ": " .  $latitude . ", " . t('Lon') . ': ' . $longitude; ?> </span>
  <?php endif; ?>
<?php } ?>
</div>

<div class="member-phones">
	<?php if ($homephone) : ?>
		<div class="phone homephone"><span class="number"><?php print $homephone; ?></span><br /><?php print t('Home Number'); ?></div>
	<?php endif; ?>
	<?php if ($mobilephone) : ?>
		<div class="phone mobilephone"><span class="number"><?php print $mobilephone; ?></span><br /><?php print t('Mobile Number'); ?></div>
	<?php endif; ?>
	<?php if ($workphone) : ?>
	<div class="phone workphone"><span class="number"><?php print $workphone; ?></span><br /><?php print t('Work Number'); ?></div>
	<?php endif; ?>
</div>

<div class="member-actions"><?php
  if ($account->uid != $GLOBALS['user']->uid) {
    print theme('linkbutton', array(
        'link' => array(
          'title' => t('Send Message'),
          // Note that this href is dependent on the setting of BASE URL PATH
          // in the privatemsg settings. It needs to be set to user/%user/messages
          // for this to work.
          'href' => "user/{$GLOBALS['user']->uid}/messages/new/{$account->uid}",
          'classes'=> 'rounded light',
        )
      )
    );
  }
else {
	print theme('linkbutton', array('link' => array('title' => t('Update'), 'href' => 'user/' . $account->uid . '/edit','classes'=> 'rounded light',)));
	print theme('linkbutton', array('link' => array('title' => t('Set Location'), 'href' => 'user/' . $account->uid . '/location','classes'=> 'rounded light',)));
} ?>

</div>

  <div class="responsive-counts">
    <div class="responsive-count">
      <?php print t('Message responsiveness: @responsiveness', array('@responsiveness' => $pm_responsiveness)); ?>
    </div>
		<?php if ($pm_responsiveness_over_full_period) {
			$stmt = '@responses responses to @requests requests over the past year';
		} else {
			$stmt = '@responses responses to @requests requests since @start';
		}

    print '(' . t($stmt, array('@responses' => $pm_responses, '@requests' => $pm_requests, '@start' => $pm_start_date)) . ')';
		?>
  </div>

</div>
