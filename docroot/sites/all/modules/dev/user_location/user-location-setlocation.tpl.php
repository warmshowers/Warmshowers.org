<?php
/**
 * Template for the set-home-location function.
 */
?>

<div class="mapholder">
    <p> <?php print t('Here is a map showing where the website currently thinks you live.
        Please move the map and zoom in to the exact correct location, and
        click in that location to select the latitude and longitude of the
        point you double-click on. Alternately, you can fill in the exact location
        (in decimal form) on the form below. Then click "submit" to update your coordinates.'); ?>
    </p>
    <div id="locationset" style="width: 100%; height: 480px; border: 1px solid black;">
        <noscript>
        <b> <?php print t('Sorry, you need a more recent browser (IE6+, Firefox, etc.) and javascript must be turned on. JavaScript must be enabled in order for you to see or use the maps on Warmshowers.org.</b>
        However, it seems JavaScript is either disabled or not supported by your browser.
        To view the map, enable JavaScript by changing your browser options, and then
        try again.  Alternately, you might want to try the excellent free <a href="http://getfirefox.com">Firefox browser</a>.<br/><br/>
        You <i>can</i> enter your latitude and longitude below in decimal format.'); ?>
        </noscript>
    </div>

    <div><?php print t('Geocoding accuracy = '). $account->source; ?>
      (<?php print wsuser_source_to_description($account->source); ?>)
    </div>
    <?php print t('Need a converter degrees/minutes/seconds to decimal? !link',
      array('!link' => l(t('Try this'), 'https://www.fcc.gov/encyclopedia/degrees-minutes-seconds-tofrom-decimal-degrees'))); ?>
  </div>

