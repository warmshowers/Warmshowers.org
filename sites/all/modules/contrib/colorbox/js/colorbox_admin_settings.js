(function ($) {

Drupal.behaviors.initColorboxAdminSettings = function (context) {
  $('#edit-colorbox-login', context).change(function () {
    if ($(this).attr('checked')) {
      $('div.colorbox-login-settings', context).show();
    }
    else {
      $('div.colorbox-login-settings', context).hide();
    }
  });
  $('div.colorbox-custom-settings-activate input.form-radio', context).change(function () {
    if (this.value == 1) {
      $('div.colorbox-custom-settings', context).show();
    }
    else {
      $('div.colorbox-custom-settings', context).hide();
    }
  });
  $('div.colorbox-slideshow-settings-activate input.form-radio', context).change(function () {
    if (this.value == 1) {
      $('div.colorbox-slideshow-settings', context).show();
    }
    else {
      $('div.colorbox-slideshow-settings', context).hide();
    }
  });
  $('div.colorbox-title-trim-settings-activate input.form-radio', context).change(function () {
    if (this.value == 1) {
      $('div.colorbox-title-trim-settings', context).show();
    }
    else {
      $('div.colorbox-title-trim-settings', context).hide();
    }
  });
};

})(jQuery);
