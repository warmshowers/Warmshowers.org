<?php
// $Id$
?>
  <?php if (!empty($admin)) print $admin; ?>
  <div id="page" class="clearfix">
    <div id="site-header" class="container-<?php print $branding_wrapper_width; ?> clearfix">
      <div id="branding" class="grid-<?php print $header_logo_width; ?>">
        
  
        <?php if (isset($linked_logo_img)): ?>
          <?php print $linked_logo_img; ?>
        <?php endif; ?>
        
        <?php if (isset($linked_site_name)): ?>
          <?php if ($title): ?>
            <h2 id="site-name" class=""><?php print $linked_site_name; ?></h2>
          <?php else: ?>
            <h1 id="site-name" class=""><?php print $linked_site_name; ?></h1>
          <?php endif; ?>
        <?php endif; ?>
      </div><!-- /#branding -->

      <?php if ($main_menu_links || $secondary_menu_links): ?>
        <div id="site-menu" class="grid-<?php print $header_menu_width; ?>">
        <?php if($main_menu_links): ?>
          <div><?php print $main_menu_links; ?></div>
        <?php endif; ?>
        <?php if($secondary_menu_links): ?>
          <div><?php print $secondary_menu_links; ?></div>
        <?php endif; ?>
        </div><!-- /#site-menu -->
      <?php endif; ?>
    </div><!-- /#site-header -->

    <?php if($page['header_first'] || $page['header_last']): ?>
    <div id="header-regions" class="container-<?php print $header_wrapper_width; ?> clearfix">
      <?php if($page['header_first']): ?>
        <div id="header-first" class="<?php print $header_first_classes; ?>">
          <?php print render($page['header_first']); ?>
        </div><!-- /#header-first -->
      <?php endif; ?>
      <?php if($page['header_last']): ?>
        <div id="header-last" class="<?php print $header_last_classes; ?>">
          <?php print render($page['header_last']); ?>
        </div><!-- /#header-last -->
      <?php endif; ?>
    </div><!-- /#header-regions -->
    <?php endif; ?>
    
    
    
    
    
    
    
    
    
    
    
    <?php if($site_slogan && $is_front || $breadcrumb): ?>
    <div id="internal-nav" class="container-<?php print $internal_nav_wrapper_width; ?> clearfix">
      <div id="slogan-bcrumb" class="grid-<?php print $breadcrumb_slogan_width; ?>">
        <?php if ($site_slogan && $is_front): ?>
          <div id="slogan"><?php print $site_slogan; ?></div><!-- /#slogan -->
        <?php endif; ?>
        <?php if($breadcrumb): ?>
          <div id="bcrumb"><?php print $breadcrumb; ?></div><!-- /#bcrumb -->
        <?php endif; ?>
      </div>
    </div><!-- /#internal-nav -->
    <?php endif; ?>
    
    
    
    
    
    
    
    
    
    
    <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']): ?>
    <div id="preface-wrapper" class="container-<?php print $preface_wrapper_grids; ?> clearfix">
      <?php if($page['preface_first']): ?>
        <div id="preface-first" class="preface <?php print $preface_first_classes; ?>">
          <?php print render($page['preface_first']); ?>
        </div><!-- /#preface-first -->
      <?php endif; ?>
      <?php if($page['preface_middle']): ?>
        <div id="preface-middle" class="preface <?php print $preface_middle_classes; ?>">
          <?php print render($page['preface_middle']); ?>
        </div><!-- /#preface-middle -->
      <?php endif; ?>
      <?php if($page['preface_last']): ?>
        <div id="preface-last" class="preface <?php print $preface_last_classes; ?>">
          <?php print render($page['preface_last']); ?>
        </div><!-- /#preface-last -->
      <?php endif; ?>
    </div><!-- /#preface-wrapper -->
    <?php endif; ?>
    
    <?php if($messages): ?>
    <div class="container-<?php print $default_container_width; ?> clearfix">
      <div class="grid-<?php print $default_container_width; ?>">
        <?php print $messages; ?>
      </div>
    </div><!-- /.container-xx -->
    <?php endif; ?>
    
    <div id="main-content-container" class="container-<?php print $content_container_width; ?> clearfix">
      <div id="main-wrapper" class="column <?php print $main_content_classes; ?>">
        
        <?php if ($tabs): ?>
          <div id="content-tabs" class=""><?php print $tabs; ?></div><!-- /#content-tabs -->
        <?php endif; ?>
    
        <?php if ($title): ?>
          <h1 class="title" id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
    
        <div id="main-content" class="region clearfix">
          <?php print render($page['content']); ?>
        </div><!-- /#main-content -->
        
        
      </div><!-- /#main-wrapper -->
    
      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first" class="column sidebar region <?php print $sidebar_first_classes; ?>">
          <?php print render($page['sidebar_first']); ?>
        </div><!-- /#sidebar-first -->
      <?php endif; ?>
    
      <?php if ($page['sidebar_last']): ?>
        <div id="sidebar-last" class="column sidebar region <?php print $sidebar_last_classes; ?>">
          <?php print render($page['sidebar_last']); ?>
        </div><!-- /#sidebar-last -->
      <?php endif; ?>
    </div><!-- /#main-content-container -->
    
    <?php if($page['postscript_one'] || $page['postscript_two'] || $page['postscript_three'] || $page['postscript_four']): ?>
    <div id="postscript-wrapper" class="container-<?php print $postscript_container_width; ?> clearfix">
      <?php if($page['postscript_one']): ?>
        <div id="postscript-one" class="postscript <?php print $postscript_one_classes; ?>">
          <?php print render($page['postscript_one']); ?>
        </div><!-- /#postscript-one -->
      <?php endif; ?>
      <?php if($page['postscript_two']): ?>
        <div id="postscript-two" class="postscript <?php print $postscript_two_classes; ?>">
          <?php print render($page['postscript_two']); ?>
        </div><!-- /#postscript-two -->
      <?php endif; ?>
      <?php if($page['postscript_three']): ?>
        <div id="postscript-three" class="postscript <?php print $postscript_three_classes; ?>">
          <?php print render($page['postscript_three']); ?>
        </div><!-- /#postscript-three -->
      <?php endif; ?>
      <?php if($page['postscript_four']): ?>
        <div id="postscript-four" class="postscript <?php print $postscript_four_classes; ?>">
          <?php print render($page['postscript_four']); ?>
        </div><!-- /#postscript-four -->
      <?php endif; ?>
    </div><!-- /#postscript-wrapper -->
    <?php endif; ?>
    
    <?php if($page['footer_first'] || $page['footer_last']): ?>
    <div id="footer-wrapper" class="container-<?php print $footer_container_width; ?> clearfix">
      <?php if($page['footer_first']): ?>
        <div id="footer-first" class="<?php print $footer_first_classes; ?>">
          <?php print render($page['footer_first']); ?>
        </div><!-- /#footer-first -->
      <?php endif; ?>
      <?php if($page['footer_last']): ?>
        <div id="footer-last" class="<?php print $footer_last_classes; ?>">
          <?php print render($page['footer_last']); ?>
        </div><!-- /#footer-last -->
      <?php endif; ?>
    </div><!-- /#footer-wrapper -->
    <?php endif; ?>
  </div><!-- /#page -->