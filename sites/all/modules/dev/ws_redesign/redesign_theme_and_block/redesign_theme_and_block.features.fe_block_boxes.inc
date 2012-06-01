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

  return $export;
}
