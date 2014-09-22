Drupal.behaviors.deviceGeolocationCheck = function (context) {
  $.ajax({
    url:  Drupal.settings.basePath + '?q=check-geolocation-attempt',
    type: 'POST',
    dataType: 'json',
    success: function(data) {
      if (data.ask_geolocate) {
        Drupal.settings.device_geolocation.ask_geolocate = true;
        Drupal.behaviors.deviceGeolocationAutoDetect(context);
      }
    }
  });
};