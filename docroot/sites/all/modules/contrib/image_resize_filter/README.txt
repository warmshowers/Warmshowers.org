// $Id: README.txt,v 1.2 2009/05/13 22:48:29 quicksketch Exp $

This filter makes it easy to resize images, especially when combined with a
WYSIWYG editor such as tinyMCE or FCKeditor. Users never have to worry about
scaling image sizes again, just insert an image and set it's height and width
properties in HTML and the image is resized on output.

Author: Nathan Haug (quicksketch)

This module Built By Robots: http://www.lullabot.com.

Dependencies
------------
 * Drupal 5 or 6

Install
-------
1) Place the entire image_resize_filter directory in sites/all/modules. Then
   enable the module in Drupal.

2) Visit the Adminsiter->Site configuration->Input formats
   (admin/settings/filters). Click "configure" next to the input format you want
   to enable the image resize filter on.

3) Check the box for "Image resize filter" under the list of filters and save
   the configuration.

4) IMPORTANT: Click the "Rearrange" tab to check the order of the filters.

   If using the Image Resize Filter on the "Filtered HTML" input format, you
   MUST ensure A) that the <img> tag is in the list of allowed tags and B) The
   "Image resize filter" is run BEFORE the "HTML filter".

   If using the Image Resize Filter with BBCode or some other non-HTML filter,
   the "Image resize filter" must be run AFTER the BBCode filter.

5) Optional. Click "configure" next to the input format the image resize filter
   has been enabled on, then click the "Configure" tab so set additional
   configuration for the the image resize filter.

Support
-------
If you experience a problem with Image Resize Filter, file a request or issue in
the Image Resize Filter queue at
http://drupal.org/project/issues/image_resize_filter.
DO NOT POST IN THE FORUMS. Posting in the issue queues is a direct line of
communication with the module authors.
