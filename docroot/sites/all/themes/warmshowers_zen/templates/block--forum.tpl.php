<?php
/**
 * @file
 * New block template for upper right forums block as part of 2012 redesign
 */
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div class="block-wrapper clearfix">
    <div id="forum-title-wrapper">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h2 class="title"><?php print $title; ?></h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print l("Create a Post",'node/add/forum',array("attributes"=>array("id"=>"create-a-post-link"))); ?>
    </div>
    <div class="content">
      <?php print $content; ?>
    </div>
    <?php print $edit_links; ?>
  </div><!-- /.block-wrapper -->
</div><!-- /.block -->
