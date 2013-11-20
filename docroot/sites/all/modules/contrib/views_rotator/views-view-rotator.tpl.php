<?php
// $Id: views-view-rotator.tpl.php,v 1.4 2008/09/23 02:28:12 mfer Exp $
/**
 * @file
 * Displays the first row and hides the others to be displayed by the js rotator.
 *
 * @ingroup views_templates
 */
?>
<div class="item-list views-rotator <?php print $views_rotator_id ?> clear-block">
  <span><a id ="<?php print $views_rotator_id ?>-views-rotator-prev"></a></span>
  <span><a id ="<?php print $views_rotator_id ?>-views-rotator-next"></a></span>
  <div id="<?php print $views_rotator_id ?>">
    <?php foreach ($rows as $row): ?>
      <div class="views-rotator-item"<?php if (theme('views_rotator_display_item', $views_rotator_id)) print ' style="display: none;"' ?>><?php print $row ?></div>
    <?php endforeach; ?>
  </div>
</div>