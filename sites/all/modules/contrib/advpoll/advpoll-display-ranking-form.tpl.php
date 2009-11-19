<?php
// $Id: advpoll-display-ranking-form.tpl.php,v 1.1.2.1 2009/04/11 16:02:47 chriskennedy Exp $

/**
 * @file advpoll-display-ranking-form.tpl.php
 * Default theme implementation to show voting form for ranked polls.
 *
 * $writein_choice - writein_choice element, if poll needs it.
 * $form_id
 * $form_submit
 * $choice_list - choices in the poll.
 * $tabledrag_group_class
 */
?>
<?php
  // Add table JavaScript.
  drupal_add_js(drupal_get_path('module', 'advpoll') .'/advpoll-vote.js');
  drupal_add_tabledrag($form_id . '-table', 'order', 'self', 'advpoll-choice-order', NULL, NULL, FALSE);
?>
<div class="poll">
<div class="advpoll-available-choices">
<div class="choice-header"><?php print t('Choices') ?></div>
<div class="vote-choices">
<?php print $choice_list ?>
</div>
<?php if (isset($writein_choice)) { ?>
<div class="writein-choice"><?php print $writein_choice ?></div>
<?php } ?>
</div>
<!-- table-drag re-ordering if JavaScript is enabled. -->
<div class="advpoll-drag-box">
<div class="advpoll-vote-header"><?php print t('Your Vote') ?></div>
<table cellspacing=0 id="<?php print $form_id ?>-table" class="advpoll-existing-choices-table">
</table>
<div class="vote-status"></div>
</div>
<?php print $form_submit ?>
<br clear="left" />
<?php if ($message) {?><p class="message"><?php print $message ?></p><?php } ?>
</div>
