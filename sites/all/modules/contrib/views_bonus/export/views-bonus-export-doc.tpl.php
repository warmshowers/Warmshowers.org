<?php
// $Id: views-bonus-export-doc.tpl.php,v 1.2 2009/04/03 04:22:22 neclimdul Exp $
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of haeaders(labels) for fields.
 * - $themed_rows: a array of rows with themed fields.
 * @ingroup views_templates
 */

?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  </head>
  <body>
<?php 
$table = theme('views_view_table', $view, $nodes, null);
$table = preg_replace('/<\/?(a|span) ?.*?>/', '', $table); // strip 'a' and 'span' tags
print $table;
?>
  </body>
</html>
