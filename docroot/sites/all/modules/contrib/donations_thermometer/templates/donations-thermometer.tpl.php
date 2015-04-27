  <div class="gauge-<?php print $size ?>">
    <div class="current-value" id="campaign-progress-current" style="height: <?php print $percent; ?>%;">
      <p><?php print $percent; ?>% </p>
    </div>
  </div>
  <p>
    <span class="donations_thermometer-label"><?php print t('Current Amount:'); ?></span>
    <span class="donations_thermometer-amount"><?php print $currency; ?><?php print $amount; ?></span>
  </p>
  <p>
    <span class="donations_thermometer-label"><?php print t('Target:'); ?></span>
    <span class="donations_thermometer-amount"><?php print $currency; ?><?php print $target; ?></span>
  </p>
