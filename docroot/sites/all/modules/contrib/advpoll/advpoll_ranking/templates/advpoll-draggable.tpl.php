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

$tableId = 'advpoll-ranking-draggable-table-' . $node->nid;
drupal_add_tabledrag($tableId, 'match', 'sibling', 'advpoll-draggable-weight', NULL, NULL, FALSE);

?>
<table id="<?php print $tableId ?>" class="sticky-enabled advpoll-ranking-draggable" data-nid="<?php print $node->nid; ?>">
  <thead>
    <tr>
      <th><?php print t('Order your choices'); ?></th>
    </tr>
  </thead>
  <tbody>
  <?php
    $row = 0;
    $choices = $data->choices;
    $choice_count = count($choices);
  ?>
  <?php foreach ($choices as $choice): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
        <td class="advpoll-draggable-weight">
          <span class="choice"><?php print $choice['choice']; ?></span>
          <select id="edit-draggable-choice-<?php print $row;?>" class="form-select" name="choice[<?php print $row; ?>]" >
            <option value="0">--</option>
           <?php for ($i = 0; $i < $choice_count; $i++): ?>
            <option value="<?php print $i + 1; ?>"><?php print $i + 1; ?></option>
           <?php endfor; ?>
          </select>
        </td>
      </tr>
    <?php $row++; ?>
  <?php endforeach; ?>
  </tbody>
</table>