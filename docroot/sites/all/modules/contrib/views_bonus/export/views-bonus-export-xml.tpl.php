<?php
// $Id: views-bonus-export-xml.tpl.php,v 1.2 2009/06/24 17:27:53 neclimdul Exp $
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of headers(labels) for fields.
 * - $themed_rows: a array of rows with themed fields.
 * @ingroup views_templates
 */

// Short tags act bad below in the html so we print it here.
print '<?xml version="1.0" encoding="UTF-8" ?>';
?>
<xml>
<?php foreach ($themed_rows as $count => $row): ?>
  <node>
<?php foreach ($row as $field => $content):
    $label = $header[$field] ? $header[$field] : $field;
?>
    <<?php print $label; ?>><?php print $content; ?></<?php print $label; ?>>
<?php endforeach; ?>
  </node>
<?php endforeach; ?>
</xml>
