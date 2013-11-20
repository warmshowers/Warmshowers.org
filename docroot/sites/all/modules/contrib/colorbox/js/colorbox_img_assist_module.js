(function ($) {

Drupal.behaviors.initColorboxImgAssistModule = function (context) {
  var settings = Drupal.settings.colorbox;

  // Handle "image-img_assist_custom" images.
  $('span.inline > a', context).filter(':not(.initColorboxImgAssistModule-processed)').addClass('initColorboxImgAssistModule-processed').each(function (i) {
    var $img = $('.image', this);
    if ($img.length === 0) {
      return true;
    }

    // Find derivative
    var matches = $img.attr('class').match(/image\-(img_assist_custom\-\w+)/);
    if (matches === null) {
      return true;
    }
    var derivative = matches[1];

    // Create link path
    var path_replacement = settings.img_assist_derivative == '_original' ? '' : '.' + settings.img_assist_derivative;
    var href = $img.attr('src').replace('.' + derivative, path_replacement);

    // Modify link to image
    this.href = href;
    // Add rel tag to group
    this.rel = 'img_assist-gallery';
    // Add image link title
    this.title = $img.attr('title');
    // Colorbox it
    $(this).addClass('colorbox');
  });
};

})(jQuery);
