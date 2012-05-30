<?php
/**
 * @file user-profile.tpl.php
 *
 * warmshowers_zen version of user profile theming.
  */
?>

<div id="profile-container">
  <div id="profile-image"><?php print theme('user_picture', $account); ?></div>
  <div id="name-title">
    <h3><?php print $account->fullname; drupal_set_title($account->fullname); ?></h3>
    <br />
    <?php print l(format_plural($reference_count, '1 recommendation', '!count recommendations', array('!count' => $reference_count)), 'user/' . $uid . '/recommendations_of_me', array('html' => TRUE)); ?> -
    <?php print t('Member for') . ' ' . $account->content['summary']['member_for']['#value']; ?>
  </div>
  <div class="content">
    <h1><?php print t('About this Member'); ?></h1>
    <?php print check_markup($account->comments); ?>
    <div id="host-services">
      <h2><?php print t('This Host Offers'); ?></h2>
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
