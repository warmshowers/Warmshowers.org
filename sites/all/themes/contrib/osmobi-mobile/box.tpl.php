<?php
// $Id: box.tpl.php,v 1.2 2010/01/13 15:09:54 tdrycker Exp $
  if (!empty($content)) : ?>
      <!-- Region: osmobi-region-box -->
      <div class="osmobi-region osmobi-region-box">
<?php if ( !empty($title) ) : ?>      
        <div class="osmobi-region-title"><?php print $title ?></div>
<?php endif;?>
        <div class="osmobi-region-content">
<?php print $content; ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-box -->
<?php endif; ?>