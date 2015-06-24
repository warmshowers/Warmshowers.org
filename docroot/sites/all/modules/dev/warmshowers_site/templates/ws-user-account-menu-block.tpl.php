<?php
/**
 * Provide a menu for authenticated user actions
 *
 * Note that this block is set to be cached per user, so counts on block caching being turned on.
 *
 * Variables:
 * - $account: A Drupal user account object.
 * - $profile_link: The formatted name of the user linked to their profile page.
 * - $logout_link: The logout link.
 */
?>
<div class="account-menu">
  <?php print t("Logged in as !name | !logout", array('!name' => $profile_link, '!logout' => $logout_link)); ?>
</div>

