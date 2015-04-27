(function ($) {
  Drupal.behaviors.deviceGeolocationCheck = {
    attach: function (context, settings) {
      var uri = location.pathname.substring(1, location.pathname.length);
      $.ajax({
        url:  settings.basePath + '?q=check-geolocation-attempt&uri=' + uri,
        type: 'POST',
        dataType: 'json',
        success: function(data) {
          if (data.ask_geolocate) {
            settings.device_geolocation.ask_geolocate = true;
            Drupal.behaviors.deviceGeolocationAutoDetect.attach(context, settings);
          }
        }
      });
    }
  };  
})(jQuery);