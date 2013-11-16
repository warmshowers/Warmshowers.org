/**
 * Javascript for users to set specific location.
 *
 * @type {Boolean}
 */

Drupal.behaviors.user_location_setlocation = function () {
  var defaultZoom = 10;
  var lat = Drupal.settings.user_location.location.latitude;
  var lon = Drupal.settings.user_location.location.longitude;

  var mapOptions = {
    center:new google.maps.LatLng(lat, lon),
    zoom:defaultZoom,
    mapTypeId:google.maps.MapTypeId.TERRAIN
  };

  var map = new google.maps.Map(document.getElementById('locationset'), mapOptions);
  var marker = new google.maps.Marker({
    position:mapOptions.center,
    map:map,
  });
  google.maps.event.addListener(map, 'click', function (event) {
    $('#edit-longitude').attr('value', event.latLng.lng());
    $('#edit-latitude').attr('value', event.latLng.lat());
    marker.setPosition(event.latLng);
  });
}
