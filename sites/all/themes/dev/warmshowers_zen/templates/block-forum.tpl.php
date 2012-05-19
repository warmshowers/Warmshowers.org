<?php
 // New block template for upper right forums block as part of 2012 redesign
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div class="block-wrapper clearfix">
    <div id="forum-title-wrapper">
      <?php if ($title): ?>
        <h2 class="title"><?php print $title; ?></h2>
      <?php endif; ?>
      <div id="create-a-post-link"><?php print l("Create a Post",'node/add/forum'); ?></div>
    </div>
    <br />
    <div class="content">
      <?php print $content; ?>
    </div>
    <?php print $edit_links; ?>
  </div><!-- /.block-wrapper -->
</div><!-- /.block -->
