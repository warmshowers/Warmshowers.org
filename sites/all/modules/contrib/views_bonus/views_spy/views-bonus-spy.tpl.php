<?php
// $Id: views-bonus-spy.tpl.php,v 1.1 2009/02/16 15:20:38 manuelgarcia Exp $
/**
 * @file
 * Displays the items of the spy.
 *
 * @ingroup views_templates
 *
 *  Note that the spy depends on:
 *  1. <div id="<?php print $views_spy_id ?>"> is used for selecting to add the spy effect.
 *  2. <?php print $row ?> to be wrapped by an element
 *  3. The parent element of <div id="<?php print $views_spy_id ?>"> is used for pausing the spy on hover
 *
 *  The current div wraping each row gets two css classes, which should be enough for most cases:
 *     "views-spy-item"
 *      and a unique per row class like item-0
 */
?>
<div class="item-list views-spy <?php print $views_spy_id ?>">
  <div id="<?php print $views_spy_id ?>">
    <?php foreach ($rows as $id => $row): ?>
      <div class="views-spy-item item-<?php print $id ?>">
        <?php print $row ?>
      </div>
    <?php endforeach; ?>
  </div>
</div>
