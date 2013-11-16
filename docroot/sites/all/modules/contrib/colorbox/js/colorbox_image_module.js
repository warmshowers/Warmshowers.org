(function ($) {

Drupal.behaviors.initColorboxImageModule = function (context) {
  var settings = Drupal.settings.colorbox;

  // Image Attach Functionality
  $('div.image-attach-body > a, ul.images a', context).filter(':not(.initColorboxImageModule-processed)').addClass('initColorboxImageModule-processed').each(function (i) {
    var $img = $('.image', this);
    if ($img.length === 0) {
      return true;
    }

    // Find derivative
    var matches = $img.attr('class').match(/image\-(\w+)/);
    if (matches === null) {
      return true;
    }
    var derivative = matches[1];

    // Create link path
    var path_replacement = settings.image_derivative == '_original' ? '' : '.' + settings.image_derivative;
    var href = $img.attr('src').replace('.' + derivative, path_replacement);

    // Modify link to image
    this.href = href;
    // Add rel tag to group
    this.rel = 'image-gallery';
    // Add image link title
    this.title = $img.attr('title');
    // Colorbox it
    $(this).addClass('colorbox');
  });
};

})(jQuery);
