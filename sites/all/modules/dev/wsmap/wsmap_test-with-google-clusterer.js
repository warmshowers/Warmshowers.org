$Id: wsmap_test-with-google-clusterer.js 505 2009-05-24 18:55:09Z rfay $

var mapDiv=document.getElementById('wsmap_map');
//var panelDiv=document.getElementById('panel');
var om=null;
var map=null;
var hosts=[];
var redIcon=new GIcon(G_DEFAULT_ICON);
var grayIcon=new GIcon(G_DEFAULT_ICON);
var startlat = 46.8;
var startlon = -71;
var startZoom =  6;
var debug=false;
//var debug=true;
var	base_path = null;
var lastTime = new Date().getTime();
var disable_zoomend = false;
//var clusterer=null;
var maxvisiblemarkers=50;
var numHostsToDetail=15;
var markermgr;
var specificZoomSettings  = {  // Handle countries that don't quite fit the calculation
us: 5, ca: 4, ru:4
};



$(document).ready( function() {
	if (GBrowserIsCompatible()) {
		$("body").attr( "onunload", "GUnload();" );  // Recommended by Google
		wsmap_main_load_entry();
	}
} );
	

//else {
//	mapDiv.innerHTML='Sorry, your browser is not compatible with Google Maps.';
//
//}



var chunkSize = 5000; /* Max size of a request to the db */


var mapdata_source=null;
var loggedin = false;



/************************************************************\
*
\************************************************************/
function wsmap_main_load_entry()
{
	$('#edit-city')[0].select();
	
	
	mapDiv=document.getElementById('wsmap_map');

	mapdata_source = document.getElementById('mapdata_source').innerHTML;
	loggedin = parseInt(document.getElementById('loggedin').innerHTML);


	base_path = document.getElementById('base_path').innerHTML;
	
	redIcon.image=base_path + '/clusterer/red.PNG';
	redIcon.shadow==base_path + '/clusterer/shadow.PNG';
	redIcon.iconSize=new GSize(20,34);
	redIcon.shadowSize=new GSize(37,34);
	redIcon.iconAnchor=new GPoint(9,34);
	redIcon.infoWindowAnchor=new GPoint(9,2);
	redIcon.infoShadowAnchor=new GPoint(18,25);

	map=new GMap2(mapDiv);
	//var mgrOptions = { borderPadding: 50, maxZoom: 15, trackMarkers: true };

	
	om=new OverlayMessage(mapDiv);
	map.addControl(new GLargeMapControl());

	map.addControl(new GMapTypeControl());
	map.addControl(new GScaleControl());

	//SavePositionZoomTypeCookieOnChanges(map);

	//map.setMapType(G_NORMAL_MAP);
	loadmain()


}

function loadmain() {

	// TODO: Use cookie from drupal!
	//var mapPosCookie = getMapPosCookie();
//	if (mapPosCookie != null) {
//		startlat = parseFloat(mapPosCookie[0]);
//		startlon=parseFloat(mapPosCookie[1]);
//		startZoom = parseInt(mapPosCookie[2]);
//	} else 
	if (document.getElementById('ipl_latitude')) { // No cookie: Use IP location
		startlat = parseFloat(document.getElementById('ipl_latitude').innerHTML);
		startlon = parseFloat(document.getElementById('ipl_longitude').innerHTML);
	} // Otherwise just use the default

	map.setCenter(new GLatLng(startlat,startlon), startZoom);
	
	markermgr = new MarkerManager(map);

	
	replaceAutocompleteHooks();

	if ($('#showuser').length) {
		var user =  $('#showuser');
		$('#edit-country').attr('value',user.attr('country'));
		editCountryReset();
		zoomToUser(user.attr('uid'),user.attr('latitude'), user.attr('longitude'),7);
	} 
	else {

		editCountryOnchange();   // Pretend we edited country, to update
	}
	

	// loadMarkers();
	GEvent.addListener(map,'dragend',dragend_called);
	GEvent.addListener(map,'zoomend', dragend_called);


}


function loadMarkers() {

	var bounds = map.getBounds();
	var center = map.getCenter();
	var numpoints = 0;
	i=0;
	var sw = bounds.getSouthWest();
	var ne = bounds.getNorthEast();

	var rqurl = mapdata_source + "?minlat="+ sw.lat() +"&maxlat="+ ne.lat() +
	"&maxlon=" + ne.lng() + "&minlon="+ sw.lng() + "&centerlat=" + center.y 
	+ "&centerlon=" + center.x;
	rqurl += "&limitlow=" + 0 + "&maxresults=" + (0+chunkSize);
	showDebug("Rqurl: " + rqurl);

	om.Set('Loading...');

	showDebug("Calling HttpGet");
	HttpGet(rqurl,RequestChecker);
	
}


/************************************************************\
*
\************************************************************/
function RequestChecker(request)
{
	try
	{
		var xmlDoc=request.responseXML.documentElement;
		var hostElements=xmlDoc.getElementsByTagName('host');
		$('#markerlist').empty();
		$('#nearby-hosts').html("<div><h2>Hosts Closest to Map Center</h2></div>");
		
		var markers = new Array();

		for(var s=0; s<hostElements.length; ++s)
		{
			var host=new Object();
			var lastlat;
			var lastlon;
			
			host.prov=hostElements[s].getAttribute('p');
			host.city=hostElements[s].getAttribute('c');
			host.lat=parseFloat(hostElements[s].getAttribute('la'));
			host.lng=parseFloat(hostElements[s].getAttribute('ln'));
			host.location=new GLatLng(host.lat,host.lng);
			host.name=hostElements[s].getAttribute('n');
			host.mail=hostElements[s].getAttribute('m');
			host.na=parseInt(hostElements[s].getAttribute('a'));
			host.d=parseFloat(hostElements[s].getAttribute('d'));

				
			host.uid=hostElements[s].getAttribute('u');
			
			if (s < numHostsToDetail) {
				var text = 	'<div class="hostdetail" uid="' + host.uid + '" class="markerdesc" >';
				if (loggedin) { text+= '<a href="/user/' + host.uid + '">'	+ host.name + '</a> ' ; }
				text += host.city + ", " + host.prov + '</div>';
					
				$('#markerlist').append(text);
			}

			if (hosts[host.uid] == null) {
				var icon = redIcon;
				if ( host.na ) { icon = grayIcon; }

				var marker=new GMarker(host.location, icon);
				host.marker=marker;
				GEvent.addListener(marker,'click',MakeCaller(PopUp,host.uid));
//				var link;
//				if (loggedin) {
//					link = '<a href="/user/' + host.uid + '">' + host.name + "</a>  (" + host.city + ", " + host.prov + ")";
//				} else {
//					link =  host.city + ", " + host.prov ;
//				}

				//clusterer.AddMarker(marker,link );
				//map.addOverlay(host.marker);
				hosts[host.uid] = host;
				markers.push(marker);
			}
			

		}
		om.Clear();
		showDebug("Completed Load and place");
		
		$('.hostdetail').mouseover( function() {
			var thisuid=$(this).attr('uid');
			PopUpWithoutPan($(this).attr('uid'));
		}
		
		);
		GEvent.trigger(map, 'loadMarkersComplete', hostElements.length);
		markermgr.addMarkers(markers,3);
		markermgr.refresh();


	}

	catch(e)
	{
		var msg = Props(e);
		console.log('RequestChecker:\n'+msg);

	}
	


}


function adddiv(host) {
//				host.prov=hostElements[s].getAttribute('p');
//				host.city=hostElements[s].getAttribute('c');
//				host.lat=parseFloat(hostElements[s].getAttribute('la'));
//				host.lng=parseFloat(hostElements[s].getAttribute('ln'));
//				host.location=new GLatLng(host.lat,host.lng);
//				host.name=hostElements[s].getAttribute('n');
//				host.mail=hostElements[s].getAttribute('m');
//				host.na=parseInt(hostElements[s].getAttribute('a'));
//				host.d=parseFloat(hostElements[s].getAttribute('d'));
	
	var txt = host.d + " miles:" + host.city + "," + host.prov + "<br/>" +
		host.name + " email" + host.e + "<br/>";
	$('#nearby-hosts').append(txt)	;
}

function makePopupHtml(host) {
	var txt;
	var style="";
		if (loggedin) {
			txt='<table style="' + style + '" ><tbody><tr><td><b><a href="/user/'+host.uid+'">'+host.name+'</a>' +  '</b><br/> '+host.city + ", " + host.prov+'</td></tr><tr><td>Email:' +host.mail+'</td></tr><tr></tr></tbody></table>';
		} else {
			txt='<table style="' + style + '" ><tbody><tr><td>'+host.city + ", " + host.prov+'</td></tr><tr><td><a href="/user/login">Log in</a> or <a href="/user/register">register</a> to get more info.</td></tr><tr></tr></tbody></table>';
		}
	return txt;
	
	
}

/************************************************************\
*
\************************************************************/
function PopUp(s)
{
	try
	{
		var host=hosts[s];
		var style = "";
		var html = makePopupHtml(host);
		var templistener =	GEvent.addListener( map, "moveend", function() {
			host.marker.openInfoWindowHtml(html);
			loadMarkers();
			GEvent.removeListener(templistener);
			disable_zoomend = false;
		} );

		var loc = host.location;
		disable_zoomend=true;
		map.panTo(loc);

	}

	catch(e)
	{
		//GLog.write('PopUp:\n'+Props(e));

	}

}


function PopUpWithoutPan(s)
{
	var host=hosts[s];
	var style = "";
	var html = makePopupHtml(host);
	host.marker.openInfoWindowHtml(html);
}

function setCookie(cookieName, cookieVal) {
	var later = new Date().getTime() + 86400000; // 24*60*60*1000; // One day of ms
	var exp = new Date();
	exp.setTime(later)
	document.cookie=cookieName + "=" + cookieVal + "; expires=" + exp.toGMTString();
}

function getMapPosCookie() {
	var values = new Array(3);
	var mapPosCookie = getCookieData("mappos2");
	var stringIndex = 0;
	var slashLoc;
	for (i=0; i<3; i++) {
		slashLoc = mapPosCookie.indexOf("/",stringIndex);
		if (slashLoc == -1) {
			return null;
		}
		values[i] = mapPosCookie.substring(stringIndex,slashLoc);
		stringIndex = slashLoc+1;

	}
	return values;

}
function getCookieData(labelName)  {
	var labelLen = labelName.length;
	var cookieData = document.cookie;
	var cLen = cookieData.length;
	var i=0;
	var cEnd;
	while (i<cLen){
		var j=i+labelLen;
		if (cookieData.substring(i,j) == labelName) {
			cEnd = cookieData.indexOf(";", j);
			if (cEnd == -1) {
				cEnd = cookieData.length;
			}
			return unescape(cookieData.substring(j+1,cEnd));
		}
		i++;
	}
	return "";
}

function dragend_called() {
	if (!disable_zoomend) {
		om.Clear();  // In the case where no more markers were drawn.
		loadMarkers();
	}

}

function showDebug(dbgstring) {
	if (debug) {

		var curTime = new Date().getTime();
		var diff = curTime - lastTime;
		$("#wsmap_debug").append("(From showDebug):" + curTime + "(" + diff + ")" + dbgstring + "<br>\n");
		lastTime=curTime;
		
	}
}


function setMapLocationToCountry(countryCode) {
	    // Ajax GET request for autocompletion
	url = '/location_country_locator_service' + '/' + countryCode;
    $.ajax({
      type: "GET",
      url: url,
      success: function (data) {
        // Parse back result
        var res = Drupal.parseJson(data);
        var area = parseFloat(res.area)/1000;
        var basecalc = Math.log(area)/Math.log(4);
        var zoom = specificZoomSettings[countryCode];
        
        if (!zoom) { 
        	zoom= Math.round(10-basecalc);
        }
        
        
		     
      zoomToSpecific(res.country,res.latitude,res.longitude,zoom);

        },
      
      error: function (xmlhttp) {
        alert('HTTP error in setMapLocationToCountry '+ xmlhttp.status +' occurred at url: '+ url);
      }
    }, 2);

}

function editCountryReset() {
	
	replaceAutocompleteHooks(this);

	// HACK ALERT: Drupal 5.1 http://groups.drupal.org/node/3471.
	// Unless you do this Drupal.autocompleteAutoAttach keeps adding more GETs
	var auto=$('#edit-city-autocomplete');
	var country=$('#edit-country').attr('value');
	auto.attr('value','/location_autocomplete/' + country );
	with (document.getElementById('edit-city')) {
		for (i in events.keyup) delete events.keyup[i];
		for (i in events.blur) delete events.blur[i];
		for (i in events.keydown) delete events.keydown[i];
	}
	Drupal.autocompleteAutoAttach();
	$('#edit-city').attr('value',"");
}


function editCountryOnchange() {
	
	editCountryReset(); 
	var country=$('#edit-country').attr('value');

	if (country != 'xx' && country != '') {
		setMapLocationToCountry(country);
	}

 
	
}



function zoomToSpecific(placename, latitude, longitude, zoom) {
	map.setZoom(zoom);

	var templistener =	GEvent.addListener( map, "moveend", function() {
		map.openInfoWindow(
		map.getCenter(),
		document.createTextNode(placename)
		);
		GEvent.removeListener(templistener);
		disable_zoomend = false;
		loadMarkers();
	});

	disable_zoomend = true;
	map.panTo(new GLatLng(latitude, longitude));


}


function zoomToUser(uid, latitude, longitude, zoom) {
	map.setZoom(zoom);
	
	var templistener =	GEvent.addListener( map, "moveend", function() {
		GEvent.removeListener(templistener);
	
		//clusterer.SetMaxVisibleMarkers(5000); // Turn off clusterer for now
	
		//disable_zoomend = false;
		var loadMarkersListener = GEvent.addListener(map,"loadMarkersComplete", function(numLoaded) {
			GEvent.removeListener(loadMarkersListener);
			var host = hosts[uid];
			
			if (!host) { 
				map.openInfoWindowHtml(
					map.getCenter(),
					"User's Location<br/>(Not currently available)");
				return;
			}

			txt = makePopupHtml(host);
			var tlistener = GEvent.addListener(map,"moveend", function() {
				GEvent.removeListener(tlistener);
				//clusterer.SetMaxVisibleMarkers(maxvisiblemarkers);
			}
			);
			if (numLoaded > 50) {  // Too much clutter; zoom in.
				map.setZoom( map.getZoom() +2 );
			}
			host.marker.openInfoWindowHtml(txt);
			//clusterer.SetMaxVisibleMarkers(maxvisiblemarkers);
		} );
			
		loadMarkers();
	});
	map.panTo(new GLatLng(latitude,longitude));


	//disable_zoomend = true;


}

function replaceAutocompleteHooks() {
		Drupal.jsAC.prototype.hidePopup = function (keycode) {
		var loc=null;
		// Select item if the right key or mousebutton was pressed
		if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
			this.input.value = this.selected.autocompleteValue;  // Use the data value to fill
			// Do the map call with autocompleteValue
			loc = this.selected.autocompleteData.split('|');
	       	
 		}
		// Hide popup
		var popup = this.popup;
		if (popup) {
			this.popup = null;
			$(popup).fadeOut('fast', function() { $(popup).remove(); });
		}
		this.selected = false;
		var zoom = 6; // Arbitrary experiment
		if (loc) {   // If we had a selected value above
			zoomToSpecific(this.input.value,loc[0],loc[1], zoom);
			$('#edit-city')[0].select();  // So they can enter the next thing
		}

	};
	
	/**
	* Fills the suggestion popup with any matches received
	*/
	Drupal.jsAC.prototype.found = function (matches) {

		// If no value in the textfield, do not show the popup.
		if (!this.input.value.length) {
			return false;
		}

		// Prepare matches
		var ul = document.createElement('ul');
		var ac = this;
		for (key in matches) {
			var li = document.createElement('li');
			$(li)
			.html('<div>'+ key +'</div>')  // rfay: Use key instead of value
			.mousedown(function () { ac.select(this); })
			.mouseover(function () { ac.highlight(this); })
			.mouseout(function () { ac.unhighlight(this); });
			li.autocompleteValue = key;
			li.autocompleteData = matches[key];
			$(ul).append(li);
		}
		// Show popup with matches, if any
		if (this.popup) {
			if (ul.childNodes.length > 0) {
				$(this.popup).empty().append(ul).show();
			}
			else {
				$(this.popup).css({visibility: 'hidden'});
				this.hidePopup();
			}
		}
	}

//	Drupal.autocompleteSubmit = function () {
//		console.log('In Hijacked autocompleteSubmit');
//		var result= $('#autocomplete').each(function () {
//			this.owner.hidePopup();
//		}).size() == 0;
//		console.log('Hijacked func now returning');
//		console.log('value set to' + $('#edit-city').attr('value'));
//		return result;
//
//	}

}
