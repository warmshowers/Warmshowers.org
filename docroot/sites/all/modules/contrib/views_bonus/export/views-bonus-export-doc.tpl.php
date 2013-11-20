<?php
// $Id: views-bonus-export-doc.tpl.php,v 1.3 2009/07/06 15:29:31 neclimdul Exp $
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
$table = theme('table', $header, $themed_rows);
$table = preg_replace('/<\/?(a|span) ?.*?>/', '', $table); // strip 'a' and 'span' tags
print $table;
?>
  </body>
</html>
