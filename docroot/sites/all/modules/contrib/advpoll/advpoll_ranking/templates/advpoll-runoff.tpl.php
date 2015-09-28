<?php

/**
 * @file
 * Default template for wrapping bar results - includes count of votes.
 *
 * Variables available:
 * - $total: Total number of votes.
 * - $rows: An ordered array with the results of each candidate/choice.
 * - $nid: Node id of poll
 * - $cancel_form: Cancel button for users eligibile to clear their own vote.
 *      
 * - $percentage: percentage of votes received by top candidate
 *   
 */
?>
<div class="run-off-poll" id="advpoll-<?php print $nid; ?>">
    <ol>
        <?php for($i = 0; $i < count($rows); $i++): ?>
        <li><?php print $rows[$i]['choice']; ?>
        <?php if ($i == 0): ?>
            (<?php print $percentage; ?>%)
        <?php endif; ?>
        </li>
        
        <?php endfor; ?>
    </ol>
    <?php if (user_access('inspect all votes')): ?>
    <table class="sticky-enabled">
        <caption><?php print t('Per-round breakdown of votes for each choice.'); ?></caption>
        <thead><tr><th><?php print t('Rounds'); ?></th><th><?php print t('Final'); ?></th></tr></thead>
        <tbody>
        <?php for($i = 0; $i < count($rows); $i++): ?>
            <?php $i%2 ? $stripe = 'even': $stripe = 'odd'; ?>
        <tr class="<?php print $stripe; ?>">
            <td><?php print $rows[$i]['choice']; ?></td>
            <td><?php print $rows[$i]['votes']; ?></td>
        </tr>
        <?php endfor; ?>
        </tbody>
        <tfoot>
            <tr><td></td><td><?php print t('Total votes: @total', array('@total' => $total)); ?></td></tr>
        </tfoot>
        
    </table>
    <?php endif; ?>

        <?php print $cancel_form; ?>
</div>
