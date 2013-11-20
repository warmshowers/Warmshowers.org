(function ($) {

Drupal.behaviors.initColorboxDefaultStyle = function (context) {
  $(document).bind('cbox_complete', function () {
    // Only run if there is a title.
    if ($('#cboxTitle:empty', context).length == false) {
      setTimeout(function () { $('#cboxTitle', context).slideUp() }, 1500);
      $('#cboxLoadedContent img', context).bind('mouseover', function () {
        $('#cboxTitle', context).slideDown();
      });
      $('#cboxOverlay', context).bind('mouseover', function () {
        $('#cboxTitle', context).slideUp();
      });
    }
    else {
      $('#cboxTitle', context).hide();
    }
  });
};

})(jQuery);
