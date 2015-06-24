<?php
/**
 * Provide a block containing basic member stats.
 *
 * Todo ideas: Add link to a page that shows counts by continent, country, search, and recent members. Kind of a
 * member summary page.
 *
 * Note that this block is set to be cached globally, so counts on block caching being turned on.
 *
 * Variables:
 * - $account: A Drupal user account object.
 * - $user_count: Total number of users currently on the site.
 * - $host_count: Total number of users currently hosting.
 */
?>
<div class="numusers">
  <span class="big_number"><?php print $user_count; ?></span>
  <br />
  <?php print t('Active Members'); ?>
</div>
<div class="numhosts">
  <span class="big_number"><?php print $host_count; ?></span>
  <br />
  <?php print t('Active Hosts'); ?>
</div>

