<?php
/**
  * Produces the markup for the map section in wsmap.
  */
?>

<a id="collapse_map" class="wsmap-collapse" href="#"><?php print t('Collapse Map'); ?></a>
<noscript>
      <?php print t("This page isn't going to be much use to you until you turn on javascript in your browser. We apologize."); ?>
</noscript>

<div id='mapframe' class="wsmap-frame">
    <div id='mapholder' class="wsmap-wrapper">
        <div id='wsmap_map' class="wsmap"></div>
    </div>
</div>
<div id="wsmap-bottom" class="wsmap-bottom"><span id="wsmap-load-status" class="wsmap-load-status"></span><span class="wsmap-credits"><?php print t('Geolocation courtesy of Google and Geonames.org'); ?></span></div>
