Provides a way to split a views table row into two.
In the table style settings, select a "Secondary row" option.

So if a table looks like this:
    Name | Description | Edit link | Delete link
changing the secondary row option for "Edit link" to "Name"
and changing the secondary row option for the "Delete link" to "Description"
will render two rows:
    Name        | Description
    MyNode      | Sample description
    node/1/edit | node/1/delete

As you can see, columns selected to render in the "secondary row" aren't
rendered in the table header.

Most code is adapted from the following Views files:
  views file                                    vsr file                               vsr function
  ----------                                    --------                               ------------
- views\plugins\views_plugin_style_table.inc    views_secondary_row_plugin_style_table.inc
- views\theme\views-view-table.tpl.php          views-secondary-row-view-table.tpl.php
- views\theme\theme.inc                         views_secondary_row.module             template_preprocess_views_secondary_row_view_table()
- views\includes\admin.inc                      views_secondary_row.module             theme_views_secondary_row_style_plugin_table()
