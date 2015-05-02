<?php
/**
 * Provide a member counts block.
 *
 * Todo ideas: Add link to a page that shows counts by continent, country, search, and recent members. Kind of a
 * member summary page.
 *
 * Note that this block is set to be cached globally, so counts on block caching being turned on.
 *
 * Variables:
 * - $numusers: Total number of users currently on the site
 * - $numhosts: Total number of users currently hosting.
 */
?>
<div class="numusers">
  <span class="big_number"><?php print number_format($numusers); ?></span>
  <br />
  <?php print t('Active Members'); ?>
</div>
<div class="numhosts">
  <span class="big_number"><?php print number_format($numhosts); ?></span>
  <br />
  <?php print t('Active Hosts'); ?>
</div>

