<?php
/**
 * @file
 * Returns the HTML for the profile highlight block.
 *
 * Available variables:
 *  - $user_picture: An already rendered user picture or a scolding message if not present.
 *  - $stats: An already rendered list of custom user stats.
 *  - user_profile: Standard user profile fields variables.
 */
?>
<div class="profile-highlight">
  <div class="profile-image">
    <?php print $user_picture; ?>
  </div>

  <div class="name-title">
    <h3><?php print $fullname; ?></h3>

    <?php print $stats; ?>
  </div>
</div>