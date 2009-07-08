<?php
// $Id$
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <?php if (!empty($admin)) print $admin; ?>
  <div id="page" class="container-16 clear-block">
  
    <div id="site-header" class="grid-16 clear-block">
      
      <div id="branding" class="grid-4 alpha">
      <?php if ($linked_logo_img): ?>
        <?php print $linked_logo_img; ?>
      <?php endif; ?>
      <?php if ($linked_site_name): ?>
        <h1 id="site-name" class=""><?php print $linked_site_name; ?></h1>
      <?php endif; ?>
      </div><!-- /#branding -->
    
      
      
    <?php if ($main_menu_links || $secondary_menu_links): ?>
      <div id="site-menu" class="grid-12 omega">
        <?php print $main_menu_links; ?>
        <?php print $secondary_menu_links; ?>
      </div>
    <?php endif; ?>
    
    <?php print $header_first; ?>
    <?php print $header_last; ?>
    </div><!-- /#site-header -->

    <div id="internal-nav" class="grid-16">
      <div id="slogan-bcrumb" class="grid-10 alpha">
        <?php if ($site_slogan && $is_front): ?>
          <div id="slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
        <?php if($breadcrumb): ?>
          <div id="bcrumb"><?php print $breadcrumb; ?></div>
        <?php endif; ?>
      </div>
      <?php if ($search_box): ?>
      <div id="search-box" class="grid-6 omega"><?php print $search_box; ?></div>
    <?php endif; ?>
      
    </div>

    <div id="preface-wrapper" class="grid-16">
      <?php print $preface_first; ?>
      <?php print $preface_middle; ?>
      <?php print $preface_last; ?>
    </div><!-- /preface-wrapper -->
    
    <div class="grid-16"><?php print $help; ?><?php print $messages; ?></div>
    <div>
    <div id="main-wrapper" class="column <?php print $main_content_classes; ?>">
      

	    <?php print $mission; ?>

      
      <div id="content-top">
        <?php print $content_top; ?>
      </div>
      
      <?php if ($tabs): ?>
        <div id="content-tabs" class=""><?php print $tabs; ?></div>
      <?php endif; ?>
      
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>

      <div id="main-content" class="region clear-block">
        <?php print $content; ?>
      </div>
      
      <div id="content-bottom">
        <?php print $content_bottom; ?>
      </div>
    </div>
    </div>
  <?php if ($sidebar_first): ?>
    <div id="sidebar-first" class="column sidebar region <?php print $sidebar_first_classes; ?>">
      <?php print $sidebar_first; ?>
    </div>
  <?php endif; ?>

  <?php if ($sidebar_last): ?>
    <div id="sidebar-last" class="column sidebar region <?php print $sidebar_last_classes; ?>">
      <?php print $sidebar_last; ?>
    </div>
  <?php endif; ?>


  <div id="postscript-wrapper" class="grid-16">
      <?php print $postscript_one; ?>
      <?php print $postscript_two; ?>
      <?php print $postscript_three; ?>
      <?php print $postscript_four; ?>
  </div><!-- /postscript-wrapper -->
  
  
  <div id="footer-first" class="grid-16 clear-block">
    <?php print $footer_first; ?>
  </div>
  <div id="footer-last" class="grid-16 clear-block">
    <?php print $footer_last; ?>
    <?php if ($footer_message): ?>
      <div id="footer-message">
        <?php print $footer_message; ?>
      </div>
    <?php endif; ?>
  </div>


  </div>
  <?php print $closure; ?>
</body>
</html>
