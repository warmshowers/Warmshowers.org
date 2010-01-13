<?php
// $Id: template.php,v 1.2 2010/01/13 15:09:54 tdrycker Exp $

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<span class="breadcrumb">'. implode(' › ', $breadcrumb) .'</span>';
  }
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function phptemplate_preprocess_page(&$vars) {
  $vars['tabs2'] = menu_secondary_local_tasks();
  $site_fields = array();
  if ($vars['site_name']) {
    $site_fields[] = check_plain($vars['site_name']);
  }
  if ($$vars['site_slogan']) {
    $site_fields[] = check_plain($vars['site_slogan']);
  }
  $vars['title'] = implode(' ', $site_fields);;
  $vars['description'] = $vars['mission'];
  $vars['frontpage'] = check_url($vars['frontpage']);
  
  $body_classes = array();
  $body_classes[] = $vars['is_front'] ? 'front' : 'not-front';
  $body_classes[] = $vars['logged_in'] ? 'logged-in' : 'not-logged-in';
  $body_classes[] = preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-'. form_clean_id(drupal_strtolower(arg(0))));
  // If on an individual node page, add the node type.
  if (isset($vars['node']) && $vars['node']->type) {
    $vars['body_id'] = 'node-type-'. form_clean_id($vars['node']->type); 
  }
  if ($vars['is_front']) {
    $vars['body_id'] = 'frontpage';
  }
  // Implode with spaces.
  $vars['body_classes'] = implode(' ', $body_classes);
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs. Overridden to split the secondary tasks.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  return menu_primary_local_tasks();
}

function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

function phptemplate_node_submitted($node) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}
