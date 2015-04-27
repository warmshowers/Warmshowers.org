<?php

/**
 * @file
 * Theme functions for the uc_catalog module.
 */

/**
 * Themes the catalog block.
 *
 * @see theme_uc_catalog_item()
 * @ingroup themeable
 */
function theme_uc_catalog_block($variables) {
  $menu_tree = $variables['menu_tree'];

  $output = '<ul class="catalog menu">';

  foreach ($menu_tree->children as $branch) {
    list($inpath, $html) = _uc_catalog_navigation($branch);
    $output .= $html;
  }

  $output .= '</ul>';

  return $output;
}

/**
 * Displays a formatted link in the catalog block.
 *
 * @ingroup themeable
 */
function theme_uc_catalog_item($variables) {
  $here = $variables['here'];
  $link = $variables['link'];
  $lis = $variables['lis'];
  $expand = $variables['expand'];
  $inpath = $variables['inpath'];
  $count_children = $variables['count_children'];

  $output = '';

  if ($expand || $count_children) {
    if ($here) {
      $output = '<li class="expanded"><span class="trail">' . $link . "</span>\n";
      if (count($lis)) {
        $output .= '<ul class="menu">';
        foreach ($lis as $li) {
          $output .= $li . "\n";
        }
        $output .= "</ul>\n";
      }
      $output .= "</li>";
    }
    elseif ($expand || $inpath) {
      $output = '<li class="expanded"><span class="trail">' . $link . "</span>\n";
      if (count($lis)) {
        $output .= '<ul class="menu">';
        foreach ($lis as $li) {
          $output .= $li;
        }
        $output .= "</ul>\n";
      }
      $output .= "</li>";
    }
    else {
      $output = '<li class="collapsed">' . $link . "</li>\n";
    }
  }
  else {
    $output = '<li class="leaf">' . ($inpath ? '<span class="trail">' : '') . $link . ($inpath ? '</span>' : '') . "</li>\n";
  }

  return $output;
}
