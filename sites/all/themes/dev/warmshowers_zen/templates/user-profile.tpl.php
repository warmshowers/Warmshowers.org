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
    <div class="member-history">
      <?php print t('Member for %years; Last visit !when ago.', array('%years' => $account->content['summary']['member_for']['#value'], '!when' => $last_login)); ?>
    </div>
    <div class="recommendation-summary">
      <?php print l(format_plural($reference_count, '1 recommendation', '!count recommendations', array('!count' => $reference_count)), 'user/' . $uid . '/recommendations_of_me', array('html' => TRUE)); ?>
    </div>
    <div class="personal-details">
      <?php if (!empty($URL)): ?>
        <div class="personal-website">
          <?php print t('Personal Website: !url', array('!url' => $URL)); ?>
        </div>
      <?php endif; ?>
      <?php if (!empty($languagesspoken)): ?>
        <div class="languagesspoken">
          <?php print t('Languages Spoken: %languages', array('%languages' => $languagesspoken)); ?>
        </div>
      <?php endif; ?>
    </div>
  </div>
  <div class="content">
    <h1><?php print t('About this Member'); ?></h1>
    <?php print check_markup($account->comments); ?>
    <div id="host-services">
      <h2><?php print t('Hosting information'); ?></h2>
      <?php if ($notcurrentlyavailable) : ?>
        <?php print t('This member has marked themselves as not currently available for hosting, so their hosting information is not displayed'); ?>
      <?php else: ?>
        <?php foreach (array('preferred_notice', 'maxcyclists', 'bikeshop', 'campground', 'motel') as $item) : ?>
           <?php if (!empty($$item)): ?>
             <div class="member-info-<?php print $item; ?>"><span class="item-title"><?php print $fieldlist[$item]['title'];?></span>: <span class="item-value"><?php print $$item; ?></span></div>
           <?php endif; ?>
        <?php endforeach; ?>
        <h4><?php print t('This host may offer'); ?></h4>
        <ul>
          <?php print $services; ?>
        </ul>
      <?php endif; ?>
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
