<?php
/**
 * @file
 * Default template for an advanced poll bar - based on default Drupal Poll template.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $votes: The number of votes for this choice
 * - $percentage: The percentage of votes for this choice.
 * - $vote: The choice number of the current user's vote.
 * - $voted: Set to TRUE if the user voted for this choice.
 */

// add extra class to wrapper so that user's selected vote can have a different style.
$voted_class = '';
if ($voted) {
    $voted_class = ' voted';
}
?>
<div class="poll-bar<?php print $voted_class; ?>">
  <div class="text"><?php print $title; ?></div>
  <div class="bar">
    <div style="width: <?php print $percentage; ?>%;" class="foreground"></div>
  </div>
  <div class="percent">
    <?php print $percentage; ?>% (<?php print format_plural($votes, '1 vote', '@count votes'); ?>)
  </div>
</div>