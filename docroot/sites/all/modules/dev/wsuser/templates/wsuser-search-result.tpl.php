<?php

/**
 * @file
 * Display single wsuser search result.
 *
 * This template renders a single search result and is collected into
 * search-results.tpl.php. This and the parent template are
 * dependent to one another sharing the markup for definition lists.
 *
 * Available variables:
 * - $account: The full account, pretty much
 *
 *
 * Other variables:
 * - $classes_array: Array of HTML class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $title_attributes_array: Array of HTML attributes for the title. It is
 *   flattened into a string within the variable $title_attributes.
 * - $content_attributes_array: Array of HTML attributes for the content. It is
 *   flattened into a string within the variable $content_attributes.
 *
 *
 * @see template_preprocess()
 * @see template_preprocess_ws_search_result()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<li class="<?php print $classes; ?>"<?php print $attributes; ?>
    xmlns="http://www.w3.org/1999/html">
  <?php if (isset($distance)): ?>
  <div class="wssearch-distance">
  <?php print $distance ?>
  </div>
  <?php endif ?>
  <div class="wssearch-description"><a href="<?php print $url; ?>"><?php print $fullname; ?></a>
    <?php print "($available) " . t('Location'); ?>: <?php print $location_link; ?>
  </div>
  <div class="wssearch-about">

    <?php print "$about_me"; ?>
  </div>
</li>
