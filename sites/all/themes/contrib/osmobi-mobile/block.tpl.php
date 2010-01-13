<?php
// $Id: block.tpl.php,v 1.2 2010/01/13 15:09:54 tdrycker Exp $
  if (!empty ($block->content)) : ?>
      <!-- Region: osmobi-region-<?php print $block->module; ?> -->
      <div class="osmobi-region osmobi-region-<?php print $block->module; ?>" id="osmobi_region-<?php print $block->module .'-'. $block->delta; ?>" >
<?php if ( !empty($block->subject) ) : ?>      
        <div class="osmobi-region-title"><?php print $block->subject; ?></div>
<?php endif;?>
        <div class="osmobi-region-content">
<?php print $block->content; ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-<?php print $block->module; ?> -->
<?php endif; ?>