<?php

/**
 * @file
 * Display Suite - WS default stacked layout.
 */
?>

<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <?php if (!empty($header)): ?>
    <<?php print $header_wrapper; ?> class="ds-left<?php print $header_classes; ?>">
      <?php print $header; ?>
    </<?php print $header_wrapper; ?>>
  <?php endif; ?>

  <?php if (!empty($main)): ?>
    <<?php print $main_wrapper; ?> class="ds-right<?php print $main_classes; ?>">
      <?php print $main; ?>
    </<?php print $main_wrapper; ?>>
  <?php endif; ?>

  <?php if (!empty($footer)): ?>
    <<?php print $footer_wrapper; ?> class="ds-left<?php print $footer_classes; ?>">
      <?php print $footer; ?>
    </<?php print $footer_wrapper; ?>>
  <?php endif; ?>

  <?php if (!empty($comments)): ?>
    <<?php print $comments_wrapper; ?> class="ds-left<?php print $comments_classes; ?>">
      <?php print $comments; ?>
    </<?php print $comments_wrapper; ?>>
  <?php endif; ?>


</<?php print $layout_wrapper ?>>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>