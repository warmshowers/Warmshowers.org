(function ($) {

Drupal.behaviors.initColorboxLogin = function (context) {
  if (!$.isFunction($.colorbox)) {
    return;
  }
  $("a[href*='/user/login'], a[href*='?q=user/login']", context).colorbox({
    initialWidth:200,
    initialHeight:200,
    onComplete:function () {
      $('#edit-name').focus();
    }
  }).each(function () {
      var path = this.href;
      var new_path = path.replace(/user\/login/,'user/login/colorbox')
      var addquery = (path.indexOf('?') !=-1) ? '&' : '?';

      // If no destination, add one to the current page.
      if (path.indexOf('destination') !=-1) {
        this.href = new_path;
      }
      else {
        this.href = new_path + addquery + 'destination=' + window.location.pathname.substr(1);
      }
  });
};

})(jQuery);
