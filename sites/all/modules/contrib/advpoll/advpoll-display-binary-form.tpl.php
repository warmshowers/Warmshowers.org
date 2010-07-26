<?php
// $Id: advpoll-display-binary-form.tpl.php,v 1.1.2.1 2010/06/25 23:17:14 mirodietiker Exp $

/**
 * @file advpoll-display-binary-form.tpl.php
 * Default theme implementation to show voting form for binary polls.
 *
 * $writein_choice - writein_choice element, if poll needs it.
 * $form_id
 * $form_submit
 * $choice_list - choices in the poll.
 */
?>
<div class="poll">
  <div class="advpoll-available-choices">
    <div class="choice-header"><?php print t('Choices') ?></div>
    <div class="vote-choices">
      <?php print $choice_list ?>
    </div>
    <?php if (isset($writein_choice)): ?>
    <div class="writein-choice"><?php print $writein_choice ?></div>
    <?php endif; ?>
  </div>
  <?php print $form_submit ?>
  <br/>
  <?php if ($message): ?><p class="message"><?php print $message ?></p><?php endif; ?>
</div>
