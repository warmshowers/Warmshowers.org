<?php
// $Id: page.tpl.php,v 1.2 2010/01/13 15:09:54 tdrycker Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <?php print $scripts; ?>
  </head>
  <body id="<?php print $body_id; ?>" class="osmobi-drupal <?php print $body_classes; ?>">
    <div class="osmobi-info">
      <!-- Here we can add info to pass to the osmobi transcoding engine -->
      <div id="osmobi-mobile-version">1.6.2</div>
    </div>
    <div id="osmobi-container">
      <!-- Region: osmobi-region-header -->
      <div id="osmobi-header" class="osmobi-region osmobi-region-header">
        <a  id="osmobi-frontpage-link" href="<?php echo $frontpage ?>">
          <div id="osmobi-title"><?php echo $title ?></div>
          <img id="osmobi-logo" src="" alt="<?php echo $title ?>" ></img>
          <div id="osmobi-description"><?php echo $description ?></div>
        </a>
      </div>
<?php if (isset($breadcrumb) && !empty($breadcrumb)): ?>
      <div id="osmobi-breadcrumb" class="osmobi-region osmobi-region-breadcrumb">
        <?php print $breadcrumb; ?>
      </div>
<?php endif; ?>
<?php if ($show_messages && isset($messages) && !empty($messages) ): ?>
      <!-- Region: osmobi-region-message -->
      <div id="osmobi-message" class="osmobi-region osmobi-region-message">
        <div class="osmobi-region-title">
          Messages
        </div>
        <div class="osmobi-region-content">
          <?php print $messages; ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-message -->
<?php endif; ?>
<?php if (isset($search_box) && !empty($search_box)): ?>
      <!-- Region: osmobi-region-search -->
      <div class="osmobi-region osmobi-region-search">
        <div class="osmobi-region-content">
          <?php print $search_box ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-search -->
<?php endif; ?>
<?php if (isset($primary_links) && !empty($primary_links)): ?>
      <!-- Region: osmobi-region-mainmenu -->
      <div id="osmobi-primary-links" class="osmobi-region osmobi-region-mainmenu">
        <div class="osmobi-region-title">
          Main Menu
        </div>
        <div class="osmobi-region-content">
          <?php print theme('links', $primary_links, array('class' => 'menu')) ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-mainmenu -->
<?php endif; ?>
<?php if (isset($secondary_links) && !empty($secondary_links)) : ?>
      <!-- Region: osmobi-region-mainmenu -->
      <div id="osmobi-secondary-links" class="osmobi-region osmobi-region-mainmenu">
        <div class="osmobi-region-title">
          Sub Menu
        </div>
        <div class="osmobi-region-content">
          <?php print theme('links', $secondary_links, array('class' => 'menu')) ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-mainmenu -->
<?php endif; ?>

<?php if (isset($tabs) && !empty($tabs)) : ?>
      <!-- Region: osmobi-region-mainmenu -->
      <div id="osmobi-main-tabs" class="osmobi-region osmobi-region-mainmenu">
        <div class="osmobi-region-title">
          Main Tabs
        </div>
        <div class="osmobi-region-content">
          <ul class="menu"><?php print $tabs ?></ul>
        </div>
      </div> 
      <!-- End Region: osmobi-region-mainmenu -->
<?php endif; ?>

<?php if (isset($tabs2) && !empty($tabs2)) : ?>
      <!-- Region: osmobi-region-mainmenu -->
      <div id="osmobi-secondary-tabs" class="osmobi-region osmobi-region-mainmenu">
        <div class="osmobi-region-title">
          Sub Tabs
        </div>
        <div class="osmobi-region-content">
          <ul class="menu"><?php print $tabs2 ?></ul>
        </div>
      </div> 
      <!-- End Region: osmobi-region-mainmenu -->
<?php endif; ?>
<?php 
if ($left):
  print $left;
endif;
if ($def):
  print $def;
endif;
?>
      <!-- Region: osmobi-region-center -->
      <div id="osmobi-region-center" class="osmobi-region osmobi-region-center">
        <div class="osmobi-region-content">
          <?php print $content; ?>
        </div>
      </div>
      <!-- End Region: osmobi-region-center -->
<?php if ($right): ?>
      <?php print $right ?>
<?php endif; ?>
<?php if ($footer): ?>
      <?php print $footer ?>
<?php endif; ?>
    </div> <!-- /container -->
  </div>
  <?php print $closure ?>
  </body>
</html>
