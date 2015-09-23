<?php
/**
 * @file
 * Returns the HTML for the profile highlight block.
 *
 * Available variables:
 *  - $user_picture: An already rendered user picture or a scolding message if not present.
 *  - $stats: An already rendered list of custom user stats.
 *  - $fullname: and other variables from wsuser
 */
?>
<div class="profile-highlight clearfix">
  <div class="profile-image">
    <?php
    if (($account->uid != $GLOBALS['user']->uid) || !empty($account->picture)) {
      print theme('user_picture', array('account' => $account, 'user_picture_style' => variable_get('user_picture_style_profiles', 'profile_picture')));
    } else {
      print '<p class="photo-scolding">' . t('You have not uploaded a picture yet. Please upload a picture to improve your chances to find hosts or guests. !link', array('!link' => l(t('Upload your picture by editing your profile.'), 'user/' . $account->uid . '/edit'))) . '</p>';

    }
    ?>
  </div>

  <div class="name-title">
    <h3><?php print $fullname; ?></h3>

    <?php print $stats; ?>
  </div>
</div>
