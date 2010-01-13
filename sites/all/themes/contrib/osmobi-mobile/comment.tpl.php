<?php 
// $Id: comment.tpl.php,v 1.2 2010/01/13 15:09:54 tdrycker Exp $
?>
<!-- Region: osmobi-region-comment -->
<div class="osmobi-region osmobi-region-comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
  <div class="osmobi-region-image"><?php print $picture ?></div>
  <div class="osmobi-region-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></div>
<?php if ($submitted): ?>
  <div class="osmobi-region-submitted"><?php print $submitted; ?></div>
<?php endif; ?>
<?php if ($comment->new) : ?>
  <div class="osmobi-region-new"><?php print drupal_ucfirst($new) ?></div>
<?php endif; ?>
  <div class="osmobi-region-content">
    <?php print $content ?>
  </div>
<?php if ($signature): ?>
  <div class="osmobi-region-signature">
    <?php print $signature ?>
  </div>
<?php endif; ?>
<?php if ($links): ?>
  <div class="osmobi-region-content links">
    <?php print $links; ?>
  </div>
<?php endif; ?>
</div>
<!-- End Region: osmobi-region-comment -->