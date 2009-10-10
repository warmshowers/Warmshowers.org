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
  <div id="page" class="clear-block">
  
    <div id="site-header" class="container-<?php print $branding_wrapper_width; ?> clear-block">
      
      <div id="branding" class="grid-<?php print $header_logo_width; ?>">
      <?php if ($linked_logo_img): ?>
        <?php print $linked_logo_img; ?>
      <?php endif; ?>
      <?php if ($linked_site_name): ?>
        <h1 id="site-name" class=""><?php print $linked_site_name; ?></h1>
      <?php endif; ?>
      </div><!-- /#branding -->
    
      
      
    <?php if ($main_menu_links || $secondary_menu_links): ?>
      <div id="site-menu" class="grid-<?php print $header_menu_width; ?>">
        <div><?php print $main_menu_links; ?></div>
        <div><?php print $secondary_menu_links; ?></div>
      </div>
    <?php endif; ?>
    </div><!-- /#site-header -->
    
    <div id="header-regions" class="container-<?php print $header_wrapper_width; ?> clear-block">
	    <div id="header-first" class="<?php print $header_first_classes; ?>">
	      <?php print $header_first; ?>
	    </div>
	    
	    <div id="header-first" class="<?php print $header_last_classes; ?>">
	      <?php print $header_last; ?>
	    </div>
    </div>
    
    
    
    
    
    
    
    
    <div id="internal-nav" class="container-<?php print $internal_nav_wrapper_width; ?> clear-block">
      <div id="slogan-bcrumb" class="grid-<?php print $breadcrumb_slogan_width; ?>">
        <?php if ($site_slogan && $is_front): ?>
          <div id="slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
        <?php if($breadcrumb): ?>
          <div id="bcrumb"><?php print $breadcrumb; ?></div>
        <?php endif; ?>
      </div>
      <?php if ($search_box): ?>
      <div id="search-box" class="grid-<?php print $search_width; ?>"><?php print $search_box; ?></div>
    <?php endif; ?>
      
    </div>

    <div id="preface-wrapper" class="container-<?php print $preface_wrapper_grids; ?>">
      <div id="preface-first" class="preface <?php print $preface_first_classes; ?>">
        <?php print $preface_first; ?>
      </div>
      <div id="preface-middle" class="preface <?php print $preface_middle_classes; ?>">
        <?php print $preface_middle; ?>
      </div>
      <div id="preface-last" class="preface <?php print $preface_last_classes; ?>">
        <?php print $preface_last; ?>
      </div>
    </div><!-- /preface-wrapper -->
    
    <div class="container-<?php print $default_container_width; ?> clear-block">
      <div class="grid-<?php print $default_container_width; ?>">
        <?php print $help; ?><?php print $messages; ?>
      </div>
    </div>
    
    <div id="main-content-container" class="container-<?php print $content_container_width; ?> clear-block">
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
  </div>

  <div id="postscript-wrapper" class="container-<?php print $postscript_container_width; ?> clear-block">
    <div id="postscript-one" class="postscript <?php print $postscript_one_classes; ?>">
      <?php print $postscript_one; ?>
    </div>
    <div id="postscript-two" class="postscript <?php print $postscript_two_classes; ?>">
      <?php print $postscript_two; ?>
    </div>
    <div id="postscript-three" class="postscript <?php print $postscript_three_classes; ?>">
      <?php print $postscript_three; ?>
    </div>
    <div id="postscript-four" class="postscript <?php print $postscript_four_classes; ?>">
      <?php print $postscript_four; ?>
    </div>
  </div><!-- /postscript-wrapper -->
  
  
  
  <div id="footer-wrapper" class="container-<?php print $footer_container_width; ?>">
	  <div id="footer-first" class="grid-<?php print $footer_first_classes; ?>">
	    <?php print $footer_first; ?>
	  </div>
	  <div id="footer-last" class="grid-<?php print $footer_last_classes; ?>">
	    <?php print $footer_last; ?>
	    <?php if ($footer_message): ?>
	      <div id="footer-message">
	        <?php print $footer_message; ?>
	      </div>
	    <?php endif; ?>
	  </div>
  </div>

  </div>
  <?php print $closure; ?>
</body>
</html>
