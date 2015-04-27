/**
 * @file
 * Small enhancement for configuring the options of the Image Resize Filter.
 */

/**
 * Show the link class option if the "Link to the original" option is checked.
 */
jQuery(document).ready(function() {
  jQuery('.image-resize-filter-link-options input.form-checkbox').change(function() {
    if (this.checked) {
      jQuery('span.image-resize-filter-rel').css('display', 'inline');
      jQuery('span.image-resize-filter-class').css('display', 'inline');
    }
    else {
      jQuery('span.image-resize-filter-rel').css('display', 'none');
      jQuery('span.image-resize-filter-class').css('display', 'none');
    }
  });

  if (jQuery('.image-resize-filter-link-options input').is('[checked]') == false) {
    jQuery('span.image-resize-filter-rel').css('display', 'none');
    jQuery('span.image-resize-filter-class').css('display', 'none');
  }
});
