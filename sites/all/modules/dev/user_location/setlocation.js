// $Id: setlocation.js 505 2009-05-24 18:55:09Z rfay $ 

var debug = false;
var lastTime = new Date().getTime();

$(document).ready( function() {
	if (!document.getElementById('locationset') 
	  || typeof GBrowserIsCompatible == 'undefined' || !GBrowserIsCompatible()) {
	   return;
	}  
  $(window).unload( function () { GUnload(); } );

  showSetLocationMap();
	
} );


function showDebug(dbgstring) {
	if (debug) {
		var curTime = new Date().getTime();
		var diff = curTime - lastTime;
		document.getElementById("locationmap_debug").innerHTML += curTime + "(" + diff + ")" + dbgstring + "<br>\n";
		lastTime=curTime;
	}
}

function showSetLocationMap(lat, lon) {
	if (debug && document.getElementById("locationmap_debug")) {
		document.getElementById("locationmap_debug").style.display = "block";
	}

	if (document.getElementById("locationset")) {
		var defaultZoom=10;

		lon = document.getElementById("location_lon").innerHTML;
		lat = document.getElementById("location_lat").innerHTML;

		showDebug("Got coords: lat" + lat  + "," + lon);


		map = new GMap2(document.getElementById("locationset"));
    map.addMapType(G_PHYSICAL_MAP);

		var mypoint = new GLatLng(lat, lon);
		map.setCenter(mypoint,defaultZoom);
    map.setMapType(G_PHYSICAL_MAP);
		var mymarker = new GMarker(mypoint);


		GEvent.addListener(map, 'click', function(overlay, point) {
			map.removeOverlay(mymarker);
			if (point) {
				mypoint = point;
				mymarker = new GMarker(mypoint);
				map.addOverlay(mymarker);
				$('#edit-longitude').attr('value',point.x);
				$("#edit-latitude").attr('value',point.y);
				map.panTo(mypoint);
			}
		});


		map.addControl(new GLargeMapControl());
		map.addControl(new GMapTypeControl());
    map.addControl(new GOverviewMapControl());

		map.addOverlay(mymarker);
		
	}
}
