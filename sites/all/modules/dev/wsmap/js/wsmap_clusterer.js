/**
 * @file wsmap_clusterer.js
 *
 * Turns a div#wsmap_map on the page into a Google map.
 *
 * Uses the Clusterer2 library to present and group markers.
 *
 */
var om = null;
var map = null;
var hosts = [];

var mapStatus; // (object) initial Location to be used when presenting map.

var advcycl; // Overlay for advcycling stuff

var mapwidth; // Integer percent
var userInfo; // If map is to center on a user, set here.

var debug = false;
//var debug=true;
var base_path = null;
var lastTime = new Date().getTime();
var disable_zoomend = false;
var clusterer = null;
var redIcon;
var maxvisiblemarkers = 50;
var numHostsToDetail = 8;
var specificZoomSettings = {  // Handle countries that don't quite fit the calculation
  us:6, ca:5, ru:3, cn:4
};
var chunkSize = 2000;
/* Max size of a request to the db */
var mapdata_source = null;
var loggedin = false;
var cookieExpiration = 14;
/* days to save a lat/lon/country cookie */
var defaultLocation = {}; // Default lat/lon/zoom

// On IE we can't afford to have console.log() fire, as it's an error.
var DEBUG = false;
if (!DEBUG) {
  if (!window.console) window.console = {};
  var methods = ["log", "debug", "warn", "info"];
  for (var i = 0; i < methods.length; i++) {
    console[methods[i]] = function () {
    };
  }
}


$(document).ready(function () {
  if (typeof GBrowserIsCompatible != 'undefined' && GBrowserIsCompatible()) {
    $("body").attr("onunload", "GUnload();");  // Recommended by Google
    wsmap_main_load_entry();
  } else {
    alert(Drupal.t("You don't seem to be compatible with Google Maps"));
    $('#wsmap_map').val(Drupal.t("Sorry - Your browser does not seem to be compatible with Google Maps. You might want to try the free <a href='http://getfirefox.com'>Firefox</a> browser."));
    return;
  }
});


/**
 * Key entry point where all the work happens after the map api is loaded.
 */
function wsmap_main_load_entry() {

  try {
    // Grab necessary settings into globals.
    mapdata_source = Drupal.settings.wsmap.mapdata_source;
    loggedin = Drupal.settings.wsmap.loggedin;
    mapwidth = Drupal.settings.wsmap.mapwidth; // Integer percent
    base_path = Drupal.settings.wsmap.base_path;
    userInfo = Drupal.settings.wsmap.userInfo;
    chunkSize = Drupal.settings.wsmap.maxresults;

    $(window).resize(map_resize);
    map_resize();

    setMapStartPosition();

    redIcon = new GIcon();
    redIcon.image = base_path + '/clusterer/red.PNG';
    redIcon.shadow == base_path + '/clusterer/shadow.PNG';
    redIcon.iconSize = new GSize(20, 34);
    redIcon.shadowSize = new GSize(37, 34);
    redIcon.iconAnchor = new GPoint(9, 34);
    redIcon.infoWindowAnchor = new GPoint(9, 2);
    redIcon.infoShadowAnchor = new GPoint(18, 25);

    map = new GMap2($('#wsmap_map')[0]);
    map.setCenter(new GLatLng(defaultLocation.latitude, defaultLocation.longitude), defaultLocation.zoom);
    map.setMapType(G_PHYSICAL_MAP);

    om = new OverlayMessage($('#wsmap_map')[0]);

    map.addControl(new GLargeMapControl());
    map.addControl(new GOverviewMapControl());


    map.addControl(new GMapTypeControl());
    map.addControl(new GScaleControl());
    map.addMapType(G_PHYSICAL_MAP);
    clusterer = new Clusterer(map);

    clusterer.maxVisibleMarkers = maxvisiblemarkers;
    clusterer.icon.image = base_path + '/clusterer/blue_large.PNG';
    clusterer.icon.shadow = base_path + '/clusterer/shadow_large.PNG';
    maxlines = mapwidth > 400 ? 10 : 5;
    clusterer.SetMaxLinesPerInfoBox(6);

    // If we have a map-submit (go) button with some information configured,
    // Go to that location, but do not submit.
    $('#edit-map-submit').click(function (event) {
      event.preventDefault();
      var country = $('#edit-country').val();
      var city = $('#edit-city').val();
      var location = city.split('|');
      if (!city) {
        setMapLocationToCountry(country);
      }
      else {
        zoomToSpecific(location[0], location[1], location[2], 8);
      }
    });


    if (userInfo && userInfo.uid) {
      zoomToUser(userInfo.uid, userInfo.latitude, userInfo.longitude, 10);
    }


    // loadMarkers();
    GEvent.addListener(map, 'dragstart', dragstart_called);
    GEvent.addListener(map, 'dragend', dragend_called);
    GEvent.addListener(map, 'zoomend', dragend_called);

    // Handle setting of position cookie whenever map moves
    GEvent.addListener(map, "moveend", function () {
      var mapStatus = {
        latitude:map.getCenter().lat(),
        longitude:map.getCenter().lng(),
        zoom:map.getZoom()
      };

      $.cookie('mapStatus', JSON.stringify(mapStatus), {expires:cookieExpiration, path: '/'});

    });
    loadMarkers();
  }
  catch (e) {
    var msg = Props(e);
    console.log('Main loop:\n' + msg);

  }

}


function loadMarkers() {

  var bounds = map.getBounds();
  var center = map.getCenter();
  var numpoints = 0;
  i = 0;
  var sw = bounds.getSouthWest();
  var ne = bounds.getNorthEast();

  var rqurl = mapdata_source + "?minlat=" + sw.lat() + "&maxlat=" + ne.lat() +
    "&maxlon=" + ne.lng() + "&minlon=" + sw.lng() + "&centerlat=" + center.y
    + "&centerlon=" + center.x;
  rqurl += "&limitlow=" + 0 + "&maxresults=" + (0 + chunkSize);
  showDebug("Rqurl: " + rqurl);

  om.Set(Drupal.t('Loading...'));

  showDebug("Calling HttpGet");
  HttpGet(rqurl, RequestChecker);

}


/************************************************************\
 *
 \************************************************************/
function RequestChecker(request) {
  try {
    var xmlDoc = request.responseXML.documentElement;
    var hostElements = xmlDoc.getElementsByTagName('host');
    $('#nearby-hosts-content').html("<h2>" + Drupal.t("Closest Hosts") + "</h2>");
    for (var s = 0; s < hostElements.length; ++s) {
      var host = new Object();
      var lastlat;
      var lastlon;

      host.prov = hostElements[s].getAttribute('p');
      host.city = hostElements[s].getAttribute('c');
      host.lat = parseFloat(hostElements[s].getAttribute('la'));
      host.lng = parseFloat(hostElements[s].getAttribute('ln'));
      host.location = new GLatLng(host.lat, host.lng);
      host.name = hostElements[s].getAttribute('n');
      host.mail = hostElements[s].getAttribute('m');
      host.na = parseInt(hostElements[s].getAttribute('a'));
      host.d = parseFloat(hostElements[s].getAttribute('d'));
      host.country = hostElements[s].getAttribute('cnt');


      host.uid = hostElements[s].getAttribute('u');

      if (s < numHostsToDetail) {
        var text = '<div class="hostdetail" uid="' + host.uid + '" class="markerdesc" >';
        if (loggedin) {
          text += '<a href="/user/' + host.uid + '">' + host.name + '</a> ';
        }
        text += host.city + ", " + host.prov + ", " + host.country + '</div>';

        //if ($('#nearby-hosts').height() < $('#mapholder').height()) {
        $('#nearby-hosts-content').append(text);
        //}
      }

      if (hosts[host.uid] == null) {
        var icon = redIcon;
        if (host.na) {
          icon = grayIcon;
        }

        var marker = new GMarker(host.location, icon);
        host.marker = marker;
        GEvent.addListener(marker, 'click', MakeCaller(PopUp, host.uid));
        var link;
        if (loggedin) {
          link = '<a href="/user/' + host.uid + '">' + host.name + "</a>  (" + host.city + ", " + host.prov + ", " + host.country + ")";
        } else {
          link = host.city + ", " + host.prov + ", " + host.country;
        }

        clusterer.AddMarker(marker, link);
        //map.addOverlay(host.marker);
        hosts[host.uid] = host;
      }


    }
    om.Clear();
    showDebug("Completed Load and place");

    $('.hostdetail').mouseover(function () {
        var thisuid = $(this).attr('uid');
        PopUpWithoutPan($(this).attr('uid'));
      }

    );
    GEvent.trigger(map, 'loadMarkersComplete', hostElements.length);


  }

  catch (e) {
    var msg = Props(e);
    console.log('RequestChecker block exception: ' + msg);

  }


}


function makePopupHtml(host) {
  var txt;
  txt = '<table class="map-marker"><tbody>'
  if (host.na) {
    txt += '<tr><td>' + Drupal.t("Approx member location") + '</td></tr><tr><td>(' + Drupal.t('Not currently available') + ')</td></tr>';
  }
  else if (loggedin) {
    txt += '<tr><td><b><a href="/user/' + host.uid + '">' + host.name + '</a>' + '</b><br/> ' + host.city + ", " + host.prov + ", " + host.country + '</td></tr><tr><td><a href="/user/' + host.uid + '/contact" target="_blank">' + Drupal.t('Click to email') + '</a><td></tr>';
  } else {
    txt = '<tr><td>' + host.city + ", " + host.prov + ", " + host.country + '</td></tr><tr><td>' + Drupal.t('<a href="/user/login">Log in</a> or <a href="/user/register">register</a> to get more info') + '.</td></tr>';
  }
  txt += '</tbody></table>';
  return txt;
}

/************************************************************\
 *
 \************************************************************/
function PopUp(s) {
  try {
    var host = hosts[s];
    var style = "";
    var html = makePopupHtml(host);
    var templistener = GEvent.addListener(map, "moveend", function () {

      var opts = { maxWidth:220};
      host.marker.openInfoWindowHtml(html, opts);
      loadMarkers();
      GEvent.removeListener(templistener);
      disable_zoomend = false;
    });

    var loc = host.location;
    disable_zoomend = true;
    map.panTo(loc);

  }

  catch (e) {
    console.log('PopUp:\n' + Props(e));

  }

}


function PopUpWithoutPan(s) {
  var host = hosts[s];
  var style = "";
  var html = makePopupHtml(host);
  host.marker.openInfoWindowHtml(html, {maxWidth:220});
}


function dragend_called() {
  if (!disable_zoomend) {
    om.Clear();  // In the case where no more markers were drawn.
    loadMarkers();
  }

}

function dragstart_called() {
  map.closeInfoWindow();
}

function showDebug(dbgstring) {
  if (debug) {

    var curTime = new Date().getTime();
    var diff = curTime - lastTime;
    $("#wsmap_debug").append("(From showDebug):" + curTime + "(" + diff + ")" + dbgstring + "<br>\n");
    lastTime = curTime;

  }
}


function setMapLocationToCountry(countryCode) {
  getMapLocationForCountry(countryCode, zoomCallback);
}

function zoomCallback(data) {
  var res = Drupal.parseJson(data);
  var area = parseFloat(res.area) / 1000;
  var basecalc = Math.log(area) / Math.log(4);
  mapcountry = res.country_code;
  var zoom = specificZoomSettings[mapcountry];

  if (!zoom) {
    zoom = Math.round(10 - basecalc);
  }
  zoomToSpecific(res.country, res.latitude, res.longitude, zoom);

}


function getMapLocationForCountry(countryCode, func_to_call) {

  // Ajax GET request for autocompletion
  url = '/location_country_locator_service' + '/' + countryCode;
  $.get(url, "", func_to_call);

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

  var templistener = GEvent.addListener(map, "moveend", function () {
    GEvent.removeListener(templistener);

    var loadMarkersListener = GEvent.addListener(map, 'loadMarkersComplete', function (numLoaded) {
      GEvent.removeListener(loadMarkersListener);

      map.openInfoWindow(map.getCenter(), document.createTextNode(placename), {maxWidth:220});

    });
    loadMarkers();
  });

  map.panTo(new GLatLng(latitude, longitude));


}


function zoomToUser(uid, latitude, longitude, zoom) {
  map.setZoom(zoom);

  var templistener = GEvent.addListener(map, "moveend", function () {
    GEvent.removeListener(templistener);

    var loadMarkersListener = GEvent.addListener(map, "loadMarkersComplete", function (numLoaded) {
      GEvent.removeListener(loadMarkersListener);
      var host = hosts[uid];


      if (!host) {
        host = {
          marker: new GMarker(map.getCenter(), redIcon),
          location: map.getCenter(),
          na: 1
        };
        clusterer.AddMarker(host.marker, Drupal.t('Unvailable member'));
      }
      var txt = makePopupHtml(host);

      host.marker.openInfoWindowHtml(txt, { maxWidth: 220 });
    });

    loadMarkers();
  });
  map.panTo(new GLatLng(latitude, longitude));

}


function windowheight() {
  var browserHeight;
  if (typeof( window.innerHeight ) == 'number') {
    browserHeight = window.innerHeight;
  } else {
    browserHeight = document.documentElement.clientHeight;
  }
  return browserHeight;
}

/**
 * Set map start position based on cookie or member location (from Drupal)
 */
function setMapStartPosition() {

  var cookie = $.cookie('mapStatus');
  if (cookie) {
    defaultLocation = JSON.parse(cookie);
    return;
  }
  defaultLocation = Drupal.settings.wsmap.defaultLocation;
}


function map_resize() {
  bigsize = ($('#wsmap_map').width() > 500);
  if (bigsize) {
    $('#nearby-hosts').css('position', 'absolute');
    $('#mapholder').css('width', mapwidth + '%');
  } else {
    $('#nearby-hosts').css('position', 'static');
    $('#mapholder').css('width', '100%');
  }
}

function loadAdvCycling(kmzfile) {
  advcycl = new GGeoXml(kmzfile);
  map.addOverlay(advcycl);
}
function unloadAdvCycling() {
  map.removeOverlay(advcycl);
}

function toggleMap() { //expand and contract map
  if ($('#mapholder').css("width") == '100%') {  //if fully expanded
    $('#sidebar-left').css("display", "block");
    $('#expandText').html(Drupal.t('Expand Map'));
    $('#mapholder').animate({width:'' + mapwidth + '%'}, {duration:1000});
    $('#nearby-hosts').css("display", "block");
  } else {
    $('#sidebar-left').css("display", "none");
    $('#expandText').html(Drupal.t('Collapse Map'));
    $('#nearby-hosts').css("display", "none");
    $('#mapholder').animate({width:"100%"}, {duration:1000});
  }
  setTimeout("map.checkResize();loadMarkers();", 1000); //this is necessary due to animate function

}

