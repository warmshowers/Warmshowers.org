<?php
/**
 * Provide a member counts block.
 *
 * Todo ideas: Add link to a page that shows counts by continent, country, search, and recent members. Kind of a
 * member summary page.
 *
 * Note that this block is set to be cached globally, so counts on block caching being turned on.
 */

  $numusers = db_result(db_query("SELECT COUNT(*) numusers FROM {users} u,{wsuser} w
          WHERE u.uid = w.uid AND u.uid>1 AND u.status AND !isunreachable AND !isstale"));
  $numhosts = db_result(db_query("SELECT COUNT(*) numusers FROM {users} u,{wsuser} w
          WHERE u.uid = w.uid AND u.uid>1 AND u.status AND !isunreachable AND !isstale AND !notcurrentlyavailable"));
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

