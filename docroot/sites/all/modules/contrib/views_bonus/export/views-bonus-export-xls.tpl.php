<?php
// $Id: views-bonus-export-xls.tpl.php,v 1.1 2010/01/07 22:56:47 neclimdul Exp $
/**
 * @file
 * Template to display a view as an excel XLS file.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of headers(labels) for fields.
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
