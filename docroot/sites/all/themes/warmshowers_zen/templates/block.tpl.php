<?php
/**
 * @file
 * Returns the HTML for a block.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728246
 */
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div class="block-wrapper clearfix">
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h2 class="title"><?php print $title; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <div class="content">
      <?php print $content; ?>
    </div>

    <?php print $edit_links; ?>
  </div><!-- /.block-wrapper -->
</div><!-- /.block -->
