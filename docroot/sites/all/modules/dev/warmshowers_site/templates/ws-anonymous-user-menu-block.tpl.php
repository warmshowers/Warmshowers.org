<?php
/**
 * Provide a menu for anonymous user actions.
 *
 * Note that this block is set to be cached globally, so counts on block caching being turned on.
 *
 * Variables:
 * - $signup_link: A link to the useer registration page.
 * - $login_link: A link to the user login page.
 */
?>
<div class="anon-account-menu">
  <?php print $signup_link; ?>
  <?php print $login_link; ?>
</div>

