<?php
/**
 * @file
 * Returns the HTML for the member welcome block.
 *
 * Available variables:
 *   - $menu: An array of profile items. Use render() to print them.
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