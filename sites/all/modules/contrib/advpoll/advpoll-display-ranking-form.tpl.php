<?php
// $Id: advpoll-display-ranking-form.tpl.php,v 1.1.2.3 2010/07/31 23:19:55 mirodietiker Exp $

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
<div class="poll">
  <div class="advpoll-available-choices">
    <div class="choice-header"><?php print t('Choices'); ?></div>
    <div class="vote-choices">
      <?php print $choice_list; ?>
    </div>
    <?php if (isset($writein_choice)): ?>
    <div class="writein-choice"><?php print $writein_choice; ?></div>
    <?php endif; ?>
  </div>
  <!-- table-drag re-ordering if JavaScript is enabled. -->
  <div class="advpoll-drag-box">
    <div class="advpoll-vote-header"><?php print t('Your Vote'); ?></div>
    <table cellspacing="0" id="<?php print $form_id ?>-table" class="advpoll-existing-choices-table">
    </table>
    <div class="vote-status"></div>
  </div>
  <?php print $form_submit; ?>
  <br />
  <?php if ($message): ?><p class="message"><?php print $message; ?></p><?php endif; ?>
</div>
