<?php
/**
 * @file user-profile.tpl.php
 *
 * warmshowers_zen version of user profile theming.
 *
 * Available variables
 * - $uid
 * - $username
 * - $fullname
 * - $homephone
 * - $mobilephone
 * - $workphone
 * - $street
 * - $additional
 * - $city
 * - $province
 * - $country
 * - $postal_code
 * - $latitude
 * - $longitude
 * - $reference_count
 * - $last_login
 * - $responsive_member
 * - $member_hosted_me
 * - $services
 * - $URL
 * - $motel, $bikeshop, $maxcyclists, $campground, $languagesspoken
  */
?>

<div id="profile-container">
  <div id="profile-image"><?php print theme('user_picture', $account); ?></div>
  <div id="name-title">
    <h3><?php print check_plain($account->fullname); drupal_set_title(check_plain($account->fullname)); ?></h3>
    <br />
    <?php print l(format_plural($reference_count, '1 recommendation', '!count recommendations', array('!count' => $reference_count)), 'user/' . $uid . '/recommendations_of_me', array('html' => TRUE)); ?> -
    <?php print t('Member for') . ' ' . $account->content['summary']['member_for']['#value']; ?>
  </div>
  <div class="content">
    <h1><?php print t('About this Member'); ?></h1>
    <?php print check_markup($account->comments); ?>
    <div id="host-services">
      <h2><?php print t('Detailed information'); ?></h2>
      <?php foreach (array('URL', 'preferred_notice', 'maxcyclists', 'bikeshop', 'campground', 'motel', 'languagesspoken', ) as $item) : ?>
         <?php if (!empty($$item)): ?>
           <div class="member-info-<?php print $item; ?>"><span class="item-title"><?php print $fieldlist[$item]['title'];?></span>: <span class="item-value"><?php print $$item; ?></span></div>
         <?php endif; ?>
      <?php endforeach; ?>
      <h3><?php print t('This host offers'); ?></h3>
      <ul>
        <?php print $services; ?>
      </ul>
    </div>
    <div id="recommendations">
      <h2><?php print t('Recommendations'); ?></h2>
      <?php print views_embed_view('user_referrals_by_referee', 'block_1', $account->uid); ?>
    </div>
  </div>
  <div id="right-sidebar">
    <div id="block-1">
      Woot
    </div>
    <div id="block-2">
      Woot

    </div>
  </div>
</div>
