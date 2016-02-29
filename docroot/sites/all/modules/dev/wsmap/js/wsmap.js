/**
 * @file wsmap.js
 *
 * Turns a div#wsmap_map on the page into a Google map.
 * Google Javascript Maps API v3
 *
 * Used the basic techniques as in various blogs.
 * The MarkerClusterer is used for clustering:
 * http://google-maps-utility-library-v3.googlecode.com/svn/trunk/markerclusterer/
 */

(function ($) {
  Drupal.behaviors.wsmap = {

// Basic pseudoglobal variables
    attach: function (context, settings) {
      var infoWindow = new google.maps.InfoWindow();
      var markers = {};
      var markerPositions = [];
      var markerImages = {};
      var markerCluster;
      var defaultLocation;

      var adventure_cycling_overlay;
      var mapwidth; // Integer percent
      var userInfo; // If map is to center on a user, set here.
      var map;
      var base_path; // Base path for icons, etc.

      var marker_base_opacity;
      var marker_combined_opacity;
      var marker_refresh_required = false;

// This is used to determine a one-off zoom setting for large countries.
// Most countries work with the area calculation done in the code.
      var specificZoomSettings = {  // Handle countries that don't quite fit the calculation
        us: 4, ca: 5, ru: 3, cn: 4
      };

      $(document).ready(function () {
        wsmap_initialize();
      });


      function wsmap_initialize() {

        // Grab necessary settings into globals.
        mapdata_source = Drupal.settings.wsmap.mapdata_source;
        loggedin = Drupal.settings.wsmap.loggedin;
        mapwidth = Drupal.settings.wsmap.mapwidth; // Integer percent
        base_path = Drupal.settings.wsmap.base_path;
        userInfo = Drupal.settings.wsmap.userInfo;
        chunkSize = Drupal.settings.wsmap.maxresults;
        defaultLocation = Drupal.settings.wsmap.defaultLocation;
        marker_base_opacity = Drupal.settings.wsmap.marker_base_opacity;
        marker_combined_opacity = Drupal.settings.wsmap.marker_combined_opacity;


        // If we have a map-submit (go) button with some information configured,
        // Go to that location, but do not submit.
        $('#edit-map-submit').click(function (event) {
          event.preventDefault();
          var country = $('#edit-country').val();
          var city = $('[name=city]').val();
          var matches = city.match(/(lat|lng)+:(([\"'])(?:\\\1|.)*?\1|\S)+/g);
          var location = [];
          for (var i = 0; i < matches.length; i++) {
            var tmp = matches[i];
            tmp = tmp.split(':');
            location[tmp[0]] = tmp[1];
          }
          if (!city) {
            setMapLocationToCountry(country);
          }
          else {
            zoomToSpecific(city, location.lat, location.lng, 8);
          }
        });


        // If we're centering on a particular user, change the defaultLocation
        // to be that user and zoom of 10.
        if (userInfo && userInfo.uid) {
          defaultLocation = {
            latitude: userInfo.latitude,
            longitude: userInfo.longitude,
            zoom: 10
          };
        }

        var mapOptions = {
          center: new google.maps.LatLng(defaultLocation.latitude, defaultLocation.longitude),
          zoom: defaultLocation.zoom,
          mapTypeId: google.maps.MapTypeId.TERRAIN,
          scaleControl: true,
          overviewMapControl: true
        };

        map = new google.maps.Map(document.getElementById("wsmap_map"), mapOptions);

        markerCluster = new MarkerClusterer(map, [],
          {
            maxZoom: Drupal.settings.wsmap.clusterer.maxZoom,
            gridSize: Drupal.settings.wsmap.clusterer.gridSize
          });
        var bikeLayer = new google.maps.BicyclingLayer();
        bikeLayer.setMap(map);

        google.maps.event.addListener(map, 'idle', function () {
          mapBounds = map.getBounds();
          var ne = mapBounds.getNorthEast();
          var sw = mapBounds.getSouthWest();
          var center = map.getCenter();

          $('#wsmap-load-status').html(Drupal.t('Loading...'));
          // Note that the actual limit here is set by the Drupal variable.

          $.ajax({
            url: '/services/rest/hosts/by_location',
            type: 'post',
            beforeSend: function (xhrObj) {
              xhrObj.setRequestHeader("X-CSRF-Token", Drupal.settings.wsmap.csrf_token);
            },
            data: {
              minlat: sw.lat(),
              maxlat: ne.lat(),
              minlon: sw.lng(),
              maxlon: ne.lng(),
              centerlat: center.lat(),
              centerlon: center.lng(),
              limit: 2000
            },
            dataType: 'json',
            success: function (json) {
              addMarkersToMap(map, json);

              if (userInfo) {
                // If we are centering on a host and the host is already on the map,
                // just do its normal infoWindow.
                marker = markers[userInfo.uid] || null;
                userInfo = null; // We only want to use this one time.
                if (marker) {
                  infoWindow.setContent(Drupal.theme('wsmap_infoWindow', marker));
                  infoWindow.open(map, marker);
                }
                // If we have a user location as the center, put up an infoWindow.
                else {
                  content = Drupal.t('Member is not currently available.') + '<br/>' + Drupal.t('Approximate location shown');
                  infoWindow.setContent(content);
                  infoWindow.setPosition(mapOptions.center);
                  infoWindow.open(map);
                }
              }
            }
          });
        });
        addMapBehaviors();
      }

      function addMarkersToMap(map, parsed) {

        for (var i = 0; i < parsed.accounts.length; i++) {
          var host = parsed.accounts[i];
          if (markers[host.uid] && marker_refresh_required) {
            markers[host.uid].setOpacity(marker_base_opacity);
            continue;
          }
          if (markers[host.uid]) {
            continue;
          }

          var position = new google.maps.LatLng(host.latitude, host.longitude);
          var marker = new google.maps.Marker({
            position: position,
            map: map,
            title: host.fullname + "\n" + host.city + ', ' + host.province,
            host: host,
            hostcount: 1,
            zIndex: 1,
            opacity: marker_base_opacity
          });

          if (!markerPositions[host.position]) {
            markerPositions[host.position] = [host.uid];
          }
          // Handle the case where this host is in the exact same location as the
          // a previous host.
          else {
            // This one will hide under the first one (diff color, etc.)
            marker.setZIndex(0);
            marker.setOpacity(marker_combined_opacity);
            markerPositions[host.position].push(host.uid);
            markerCount = markerPositions[host.position].length;
            if (!markerImages[markerCount]) {
              markerImages[markerCount] = new google.maps.MarkerImage(base_path + '/markerIcons/largeTDRedIcons/marker' + markerCount + '.png');
            }
            markers[markerPositions[host.position][0]].setIcon(markerImages[markerCount]);
            markers[markerPositions[host.position][0]].setZIndex(1);
            markers[markerPositions[host.position][0]].hostcount++;
          }

          google.maps.event.addListener(marker, 'click', function () {
            infoWindow.setContent(Drupal.theme('wsmap_infoWindow', this));
            infoWindow.open(map, this);
          });
          markers[host.uid] = marker;
          markerCluster.addMarker(marker);
        }

        var status = Drupal.t('All %total hosts in this map area are shown.', {'%total': parsed.status.totalresults});
        if (i < parsed.status.totalresults) {
          status = Drupal.t('Only %done of %total hosts in this map area were loaded.', {
              '%done': i,
              '%total': parsed.status.totalresults
            }) + '<br/>' + Drupal.t('Zoom in or move the map to see more detail.');
        }
        $('#wsmap-load-status').html(status);

        // If we were doing an opacity refresh, reset the variable.
        marker_refresh_required = false;
      }

      function setMapLocationToCountry(countryCode) {
        var url = 'location_country_locator_service' + '/' + countryCode;
        $.getJSON(url)
          .done(function (res) {
            var area = parseFloat(res.area) / 1000;
            var basecalc = Math.log(area) / Math.log(4);
            var mapCountry = res.country_code;
            var zoom = specificZoomSettings[mapCountry];

            if (!zoom) {
              zoom = Math.round(10 - basecalc);
            }
            zoomToSpecific(res.country, res.latitude, res.longitude, zoom);
          })
      }


      /**
       * Zoom to named place and put a marker there
       * @param placename
       * @param latitude
       * @param longitude
       * @param zoom
       * @return
       */
      function zoomToSpecific(placename, latitude, longitude, zoom) {
        map.setZoom(zoom);
        map.panTo(new google.maps.LatLng(latitude, longitude));
        infoWindow.setContent(placename);
        infoWindow.setPosition(map.getCenter());
        infoWindow.open(map);
      }


      /**
       * Load the adventure cycling overlay
       *
       * @param kmzfile
       */
      function loadAdvCycling(kmzfile) {
        if (!adventure_cycling_overlay) {
          adventure_cycling_overlay = new google.maps.KmlLayer(kmzfile, {preserveViewport: true});
        }
        adventure_cycling_overlay.setMap(map);
      }

      function unloadAdvCycling() {
        if (adventure_cycling_overlay) {
          adventure_cycling_overlay.setMap(null);
        }
      }

      /**
       * Theme function to theme the infoWindow for a single host.
       * @param host
       * @return {String}
       */
      Drupal.theme.prototype.wsmap_infoWindow = function (marker) {
        var html = '<div class="wsmap-infowindow">';
        var default_picture = Drupal.settings.wsmap.user_picture_default;
        position = marker.host.position;
        hostcount = marker.hostcount;
        if (hostcount > 1) {
          html += '<span class="wsmap-numhosts">' + Drupal.t('%numhosts hosts at this location', {'%numhosts': hostcount}) + '</span>';
        }
        for (var i = 0; i < hostcount; i++) {
          var host = markers[markerPositions[position][i]].host;
          if (!host.profile_image_map_infoWindow) {
            host.profile_image_map_infoWindow = default_picture;
          }
          html += '<div class="wsmap-infowindow-host">';
          html += '<div class="wsmap-infowindow-picture"><img src="' + host.profile_image_map_infoWindow + '"></div>';
          html += '<div class="wsmap-infowindow-hostinfo">';
          var cboxlink = "/user/" + host.uid;
          var link = cboxlink;
          var colorbox = '$.colorbox({href: \'' + cboxlink + '\', iframe: true, width: \'90%\', height: \'90%\' });'

          html += '<a target="_blank" href="' + link + '">' + host.fullname + '</a><br/>';

          if (host.street) {
            html += host.street + '<br/>';
          }
          html += host.city + ', ' + host.province + ' ' + host.postal_code + ' ' + host.country;
          html += '</div>'; // end wsmap-infowindow-hostinfo
          html += '</div>'; // end wsmap-infowindow-host
        }
        html += '</div>';  // End wsmap-infowindow
        return html;
      }

      /**
       * Initialize map behaviors for the dashboard page
       */
      function addMapBehaviors() {
        var originalMapHeight = $('#wsmap_map').height();
        var sidebar = $('aside.first');
        var highlighted = $('.region-highlighted');
        var map = $('#main');
        $("#expand_map").click(function () {
          highlighted.hide(1000);
          sidebar.hide(1000);
          $('#tab-area').hide();

          var height = map.height();
          // If the window can handle, let's expand the height too
          if ($(window).height() - 150 > height) {
            height = $(window).height() - 150;
          }

          map.css({
            float: 'none',
            width: '100%',
            marginLeft: '0px',
            height: height + 'px'
          }).animate(1000, function () {
            $("#collapse_map").show().css("display", "block");
          });


          return false;
        });

        $("#collapse_map").click(function () {
          //$("body.with-highlight #navigation .section").hide(1000);
          sidebar.show(1000);
          highlighted.show(1000);
          map.css({
            float: 'left',
            width: '75%',
            marginLeft: '25%',
            height: originalMapHeight + 'px'
          }).animate(1000);
          $('#collapse_map').hide();

        });

        return false;
      }


// Toogle checkbox for showing/hiding Adventure Cycling KML
      $('#adv_cyc_checkbox').click(function () {
        if ($(this).is(':checked')) {
          loadAdvCycling(Drupal.settings.wsmap.advCycKML)
        } else {
          unloadAdvCycling();
        }
      });

// Toggle opacity of markers
      $('#hide_markers_checkbox').click(function () {
        if ($(this).is(':checked')) {
          marker_base_opacity = Drupal.settings.wsmap.marker_base_opacity * Drupal.settings.wsmap.marker_dimming_factor;
          marker_combined_opacity = Drupal.settings.wsmap.marker_combined_opacity * Drupal.settings.wsmap.marker_dimming_factor;
          ;
        } else {
          marker_base_opacity = Drupal.settings.wsmap.marker_base_opacity;
          marker_combined_opacity = Drupal.settings.wsmap.marker_combined_opacity;
        }
        // Force redraw
        marker_refresh_required = true;
        google.maps.event.trigger(map, 'idle');
      });
    }
  }

})
(jQuery)
