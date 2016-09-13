(function ($) {
  Drupal.behaviors.deviceGeolocationAutoDetect = {
    attach: function (context, settings) {
      var geolocation_source = 1; // Default it to Maxmind
      if (!settings.device_geolocation.ask_geolocate) {
        // Don't ask user for geolocation. Duration of frequency checking is set.
        return;
      }
      settings.device_geolocation.ask_geolocate = false;
      if (isset(settings.device_geolocation.longitude)) {
        longitude = !isNaN(settings.device_geolocation.longitude) ? settings.device_geolocation.longitude : (!isNaN(settings.device_geolocation.longitude[0]) ? settings.device_geolocation.longitude[0] : null);
      }
      else {
        longitude = null;
      }
      if (isset(settings.device_geolocation.latitude)) {
        latitude = !isNaN(settings.device_geolocation.latitude) ? settings.device_geolocation.latitude : (!isNaN(settings.device_geolocation.latitude[0]) ? settings.device_geolocation.latitude[0] : null);
      }
      else {
        latitude = null;
      }
      // Try W3C Geolocation (Preferred) to detect user's location
      if (navigator.geolocation && !settings.device_geolocation.debug_mode) {
        navigator.geolocation.getCurrentPosition(function(position) {
          geolocation_source = 2; // W3C
          geocoder_send_address(position.coords.latitude, position.coords.longitude);
        }, function() {
          // Smart IP (Maxmind) fallback
          geocoder_send_address(latitude, longitude);
        });
      }
      // Smart IP (Maxmind) fallback or using debug mode coordinates
      else {
        geocoder_send_address(latitude, longitude);
      }
      /**
       * Possible array items:
       * -street_number;
       * -postal_code;
       * -route;
       * -neighborhood;
       * -locality;
       * -sublocality;
       * -establishment;
       * -administrative_area_level_N;
       * -country;
       */
      function geocoder_send_address(latitude, longitude) {
        if (latitude != null && longitude != null && !isNaN(latitude) && !isNaN(longitude)) {
          var geocoder = new google.maps.Geocoder();
          var latlng   = new google.maps.LatLng(latitude, longitude);
          var address  = new Object;
          geocoder.geocode({'latLng': latlng}, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
              if (results[0]) {
                for (var i = 0; i < results[0].address_components.length; ++i) {
                  var long_name  = results[0].address_components[i].long_name || '';
                  var short_name = results[0].address_components[i].short_name || '';
                  var type = results[0].address_components[i].types[0];
                  if (long_name != null) {
                    // Manipulate the result to our liking
                    switch(type) {
                      case 'country':
                        address['country'] = long_name;
                        if (short_name != null) {
                          address['country_code'] = short_name;
                        }
                        break;
                      default:
                        address[type] = long_name;
                    }
                  }
                }
                address['source']    = geolocation_source;
                address['latitude']  = latitude;
                address['longitude'] = longitude;
                $.ajax({
                  url:  Drupal.settings.basePath + '?q=geolocate-user',
                  type: 'POST',
                  dataType: 'json',
                  data: address
                });
              }
            }
            else {
              $.ajax({
                url:  Drupal.settings.basePath + '?q=geolocate-user',
                type: 'POST',
                dataType: 'json',
                data: ({
                  latitude:  latitude,
                  longitude: longitude
                })
              });
              if (window.console) {
                console.log('Geocoder failed due to: ' + status);
              }
            }
          });
        }
      }
    }
  };  
})(jQuery);

function isset() {  
  var a = arguments
  var l = a.length, i = 0;
  
  if (l === 0) {
    throw new Error('Empty'); 
  }
  while (i !== l) {
    if (typeof(a[i]) == 'undefined' || a[i] === null) { 
        return false; 
    }
    else { 
      i++; 
    }
  }
  return true;
}