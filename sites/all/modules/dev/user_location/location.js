// $Id: location.js 505 2009-05-24 18:55:09Z rfay $

var debug = false;
var lastTime = new Date().getTime();
var locmap;


$(document).ready( function() {
  if (!document.getElementById('locationmap') 
	  || typeof GBrowserIsCompatible == 'undefined' || !GBrowserIsCompatible()) {
	   return;
	}  
  $(window).unload( function () { GUnload(); } );

  showSingleLocationMap();
	
} );


function showDebug(dbgstring) {
	if (debug) {
		var curTime = new Date().getTime();
		var diff = curTime - lastTime;
		document.getElementById("locationmap_debug").innerHTML += curTime + "(" + diff + ")" + dbgstring + "<br>\n";
		lastTime=curTime;
	}
}


function showSingleLocationMap(lat, lon) {
	if (debug && document.getElementById("locationmap_debug")) {
		document.getElementById("locationmap_debug").style.display = "block";
	}

	if (document.getElementById("locationmap")) {
		var defaultZoom=7;

		lon = document.getElementById("location_lon").innerHTML;
		lat = document.getElementById("location_lat").innerHTML;

		showDebug("Got coords: lat" + lat  + "," + lon);


		locmap = new GMap2(document.getElementById("locationmap"));
    locmap.addMapType(G_PHYSICAL_MAP);

		var mypoint = new GLatLng(lat,lon);
		locmap.setCenter(mypoint,defaultZoom);
    locmap.setMapType(G_PHYSICAL_MAP);
		var mymarker = new GMarker(mypoint);
		locmap.addOverlay(mymarker);

		GEvent.addListener(mymarker, "click", function() {

//			locmap.setCenter(mypoint);
			mymarker.openInfoWindowHtml(document.getElementById("infowindowhtml").innerHTML);
		});


		locmap.addControl(new GLargeMapControl());
		locmap.addControl(new GMapTypeControl());
    locmap.addControl(new GOverviewMapControl());
	}
}
