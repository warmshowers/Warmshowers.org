<?php
/**
  * Produces the markup for the map section in wsmap.
  */
?>

<a id="collapse_map" href="#"><?php print t('Collapse Map'); ?></a>
<noscript>
      <?php print t("This page isn't going to be much use to you until you turn on javascript in your browser. We apologize."); ?>
</noscript>

<div id='mapframe' style='width:100%;position:relative;clear:both;'>
    <div id='mapholder'>
        <div id='wsmap_map'></div>
    </div>
</div>
<div id="wsmap-bottom"><span id="wsmap-load-status"></span><span class="wsmap-credits"><?php print t('Geolocation courtesy of Google and Geonames.org'); ?></span></div>
