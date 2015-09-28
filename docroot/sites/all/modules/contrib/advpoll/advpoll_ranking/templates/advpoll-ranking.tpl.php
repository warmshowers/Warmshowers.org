<?php
/**
 * @file
 * Default template for displaying draggable ranking poll
 * 
 * Variables available:
 * - $node: 
 *   Full advanced poll node object
 * - $data:
 *   object containing the following fields.
 *   choices:
 *      array containing:
 *        choice_id = the unique hex id of the choice
 *        choice =    The text for a given choice.
 *        write_in =  a boolean value indicating whether or not the choice was a 
 *                    write in.
 *   start_date:      (int) Start date of the poll
 *   end_date:        (int) End date of the poll
 *   mode:            The mode used to store the vote: normal, cookie, unlimited
 *   cookie_duration: (int) If mode is cookie, the number of minutes to delay votes.
 *   state:           Is the poll 'open' or 'close'
 *   behavior:        approval or pool - determines how to treat multiple vote/user 
 *                    tally. When plugin is installed, voting will default to tabulating
 *                    values returned from voting API.
 *   max_choices:     (int) How many choices a user can select per vote.
 *   show_results:    When to display results - aftervote, afterclose or never.
 *   electoral:       Boolean - voting restricted to users in an electoral list.
 *   write_in:        Boolean - all write-in voting.
 *   block:           Boolean - Poll can be displayed as a block.
 */

$tableId = 'advpolltable-' . $node->nid;
drupal_add_tabledrag($tableId, 'match', 'sibling', 'advpoll-weight', NULL, NULL, FALSE);

$choices = $data->choices;


?>
<ul class="selectable-list">
  <?php $row = 0; ?>
  <?php foreach ($choices as $choice): ?>
  <li class="selectable">
    <span class="choice"><?php print $choice['choice']; ?></span>
    <a class="vote add" href="" style="display: none"><?php print t('Add'); ?> ></a>
    <a class="vote remove" href="" style="display: none">(x)</a>
    <select id="edit-choice-<?php print $row;?>" class="form-select" name="choice[<?php print $row; ?>]" >
      <option value="0">--</option>
      <?php for ($i = 0; $i < $data->max_choices; $i++): ?>
        <option value="<?php print $i + 1; ?>"><?php print $i + 1; ?></option>
      <?php endfor; ?>
    </select>
    <?php $row++; ?>
  </li>
  <?php endforeach; ?>
  
  <?php if ($data->write_in) : ?>
  <li class="selectable">
    <span class="choice"><?php print t('Other (Write-in)'); ?></span>
    <a class="vote add" href="" style="display: none"><?php print t('Add'); ?> ></a>
    <a class="vote remove" href="" style="display: none">(x)</a>    
    <input id="edit-write-in" class="form-text" type="text" maxlength="128" size="30" value="" name="write_in" />
  </li>
  <?php endif; ?>
</ul>

<table id="<?php print $tableId ?>" class="sticky-enabled advpoll-ranking-table-wrapper" data-nid="<?php print $node->nid; ?>">
  <thead>
    <tr>
      <th><?php print t('Your Vote'); ?></th>
    </tr>
  </thead>
  <tbody>
  <?php for ($i = 0; $i < $data->max_choices; $i++): ?>
      <tr class="draggable">
        <td class="advpoll-weight"></td>
      </tr>
  <?php endfor; ?>
  </tbody>
  <tfoot>
    <tr class="submit-row"><td></td></tr>
    <tr class="message"><td><p><?php print t ('Votes remaining: @votes', array('@votes' => $data->max_choices)); ?></p></td></tr>
  </tfoot>
</table>