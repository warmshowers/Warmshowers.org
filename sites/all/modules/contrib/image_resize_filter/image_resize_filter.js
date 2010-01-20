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
      jQuery('span.image-resize-filter-rel').show();
      jQuery('span.image-resize-filter-class').show();
    }
    else {
      jQuery('span.image-resize-filter-rel').hide();
      jQuery('span.image-resize-filter-class').hide();
    }
  });

  if (jQuery('.image-resize-filter-link-options input').is('[checked]') == false) {
    jQuery('span.image-resize-filter-rel').hide();
    jQuery('span.image-resize-filter-class').hide();
  }
})
