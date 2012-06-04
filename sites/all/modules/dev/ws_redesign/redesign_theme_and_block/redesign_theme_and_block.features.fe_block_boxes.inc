<?php

/**
 * Implementation of hook_default_fe_block_boxes().
 */
function redesign_theme_and_block_default_fe_block_boxes() {
  $export = array();

  $fe_block_boxes = new stdClass;
  $fe_block_boxes->info = 'Anon Left Content';
  $fe_block_boxes->format = '1';
  $fe_block_boxes->machine_name = 'anon_left_content';
  $fe_block_boxes->body = 'Enter one page per line as Drupal paths. The \'*\' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. <front> is the front page. If the PHP-mode is chosen, enter PHP code between <?php ?>. Note that executing incorrect PHP-code can break your Drupal site.

Enter one page per line as Drupal paths. The \'*\' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. <front> is the front page. If the PHP-mode is chosen, enter PHP code between <?php ?>. Note that executing incorrect PHP-code can break your Drupal site.
';

  $export['anon_left_content'] = $fe_block_boxes;

  $fe_block_boxes = new stdClass;
  $fe_block_boxes->info = 'Social Links';
  $fe_block_boxes->format = '3';
  $fe_block_boxes->machine_name = 'social_links';
  $fe_block_boxes->body = '<ul class="social">   <li><a href="https://www.facebook.com/groups/135049549858210/" class="social_fb"><span class="social_top">Follow Warm Showers on</span><span class="social_network">Facebook</span></a></li>   <li><a href="https://twitter.com/#!/warmshowers" class="social_tw"><span class="social_top">Follow Warm Showers on</span><span class="social_network">Twitter</span></a></li>   <li><a href="http://www.warmshowers.org/rssfeed" class="social_rs"><span class="social_top">Keep up to date with</span><span class="social_network">RSS</span></a></li>  </ul>';

  $export['social_links'] = $fe_block_boxes;

  return $export;
}
