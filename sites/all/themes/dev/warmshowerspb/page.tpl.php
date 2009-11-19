<?php
// $Id: page.tpl.php 551 2009-09-24 11:50:32Z rfay $

function warmshowerspb_welcome_page_pics() {
  $limit = 10;   // Number of items to get in the query
  $numboxes = 4; // Number of boxes on the page
  $count = 0;
  $cache_timeout = 30 * 60;   // Number of seconds before we dump cache

  if ($cache = cache_get('warmshowerspb_header_pictures')) {
    $html = $cache->data;
  } else {
    $imagecache_enabled = module_exists('imagecache');
    $max_uid  = db_result(db_query("SELECT MAX(uid) FROM {users}"));
    $start_uid = rand(1000,$max_uid-100);

    $result1 = db_query_range("SELECT u.uid,w.fullname, u.picture
      FROM {users} u, {wsuser} w
      WHERE u.picture != '' and u.uid=w.uid and u.status
      and !w.isstale
      and !w.isunreachable
      and u.uid > %d
     ", $start_uid, 0, $limit);

    $html = '<ul class="mempics">';
     while ( ($pic = db_fetch_object($result1)) && $count <$numboxes) {
       if (!file_exists($pic->picture)) {
         continue;
       }
                $item = theme('image',$pic->picture, $pic->fullname, $pic->fullname);
       if (user_access('access user profiles')) {
         $item = l($item,"user/$pic->uid",array('html'=>TRUE));
       }
       $html .= "<li>$item</li>\n";
       $count++;

       if ($count >= $numboxes) {
         break;
       }
     }
     $html .= "</ul>";

     // Cache this html
     cache_set('warmshowerspb_header_pictures',$html,'cache', time() + $cache_timeout);
  }
  return $html;
}
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title ?></title>
  <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
</head>

<body>

<div class="hide"><a href="#content" title="<?php print t('Skip navigation') ?>." accesskey="2"><?php print t('Skip navigation') ?></a>.</div>

<div id="header">
<div id="container">
<div id="pictures">
        <?php print warmshowerspb_welcome_page_pics() ?>
        <div style="clear: both;"/>
        <div class='slogan'><?php print variable_get('site_mission',""); ?></div>
</div>
</div>

</div>


<table id="content" border="0" cellpadding="15" cellspacing="0" width="100%">
  <tr>
    <?php if ($left != ""): ?>
    <td id="sidebar-left">
      <?php print $left ?>
    </td>
    <?php endif; ?>

    <td valign="top">


      <div id="main">
        <?php if ($title != ""): ?>
          <?php print $breadcrumb ?>
          <h1 class="title"><?php print $title ?></h1>

          <?php if ($tabs != ""): ?>
            <div class="tabs"><?php print $tabs ?></div>
          <?php endif; ?>

        <?php endif; ?>

        <?php if ($show_messages && $messages != ""): ?>
          <?php print $messages ?>
        <?php endif; ?>

        <?php if ($help != ""): ?>
            <div id="help"><?php print $help ?></div>
        <?php endif; ?>

      <!-- start main content -->
      <?php print $content; ?>
      <?php print $feed_icons; ?>
      <!-- end main content -->

      </div><!-- main -->
    </td>
    <?php if ($right != ""): ?>
    <td id="sidebar-right">
      <?php print $right ?>
    </td>
    <?php endif; ?>
  </tr>
</table>

<table id="footer-menu" summary="Navigation elements." border="0" cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="center" valign="middle">
    <?php if (isset($primary_links)) : ?>
      <?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
    <?php endif; ?>
    <?php if (isset($secondary_links)) : ?>
      <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')) ?>
    <?php endif; ?>
    </td>
  </tr>
</table>

<?php if ($footer_message || $footer) : ?>
<div id="footer-message">
    <?php print $footer_message . $footer;?>
    <noscript><p>You have javascript disabled, so features like the map will not work</p></noscript>
</div>
<?php endif; ?>
<?php print $closure;?>
</body>
</html>
