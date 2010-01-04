<?php
// $Id: node.tpl.php,v 1.1 2008/05/19 07:48:09 robloach Exp $

/**
 * @file node.tpl.php
 * 
 * Theme implementation to display a node.
 */

if ($teaser) {
  echo "<h2>$title</h2>
       <p>$content</p>";
}
else {
  print $content;
}