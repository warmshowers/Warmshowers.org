<?php
// $Id: node.tpl.php,v 1.2 2010/01/13 15:09:54 tdrycker Exp $
?>
<!-- Region: osmobi-region-node -->
<div class="osmobi-region osmobi-region-node<?php if ($sticky) { print ' sticky'; } ?>" id="osmobi-region-node-<?php print $node->nid; ?>">
  <div class="osmobi-region-image"><?php print $picture ?></div>
  <div class="osmobi-region-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></div>
<?php if ($submitted): ?>
  <div class="osmobi-region-submitted"><?php print $submitted; ?></div>
<?php endif; ?>
  <div class="osmobi-region-content">
    <?php print $content ?>
  </div>
<?php if ($taxonomy): ?>
  <div class="osmobi-region-content terms">
    <?php print $terms ?>
  </div>
<?php endif;?>
<?php if ($links): ?>
  <div class="osmobi-region-content links">
    <?php print $links; ?>
  </div>
<?php endif; ?>
</div>
<!-- End Region: osmobi-region-node -->