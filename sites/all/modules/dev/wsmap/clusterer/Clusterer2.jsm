
Clusterer=function(map)
{this.map=map;this.markers=[];this.clusters=[];this.timeout=null;this.currentZoomLevel=map.getZoom();this.maxVisibleMarkers=Clusterer.defaultMaxVisibleMarkers;this.gridSize=Clusterer.defaultGridSize;this.minMarkersPerCluster=Clusterer.defaultMinMarkersPerCluster;this.maxLinesPerInfoBox=Clusterer.defaultMaxLinesPerInfoBox;this.icon=Clusterer.defaultIcon;GEvent.addListener(map,'zoomend',Clusterer.MakeCaller(Clusterer.Display,this));GEvent.addListener(map,'moveend',Clusterer.MakeCaller(Clusterer.Display,this));GEvent.addListener(map,'infowindowclose',Clusterer.MakeCaller(Clusterer.PopDown,this));};Clusterer.defaultMaxVisibleMarkers=150;Clusterer.defaultGridSize=5;Clusterer.defaultMinMarkersPerCluster=5;Clusterer.defaultMaxLinesPerInfoBox=10;Clusterer.defaultIcon=new GIcon();Clusterer.defaultIcon.image='http://www.acme.com/resources/images/markers/blue_large.PNG';Clusterer.defaultIcon.shadow='http://www.acme.com/resources/images/markers/shadow_large.PNG';Clusterer.defaultIcon.iconSize=new GSize(30,51);Clusterer.defaultIcon.shadowSize=new GSize(56,51);Clusterer.defaultIcon.iconAnchor=new GPoint(13,34);Clusterer.defaultIcon.infoWindowAnchor=new GPoint(13,3);Clusterer.defaultIcon.infoShadowAnchor=new GPoint(27,37);Clusterer.prototype.SetIcon=function(icon)
{this.icon=icon;};Clusterer.prototype.SetMaxVisibleMarkers=function(n)
{this.maxVisibleMarkers=n;};Clusterer.prototype.SetMinMarkersPerCluster=function(n)
{this.minMarkersPerCluster=n;};Clusterer.prototype.SetMaxLinesPerInfoBox=function(n)
{this.maxLinesPerInfoBox=n;};Clusterer.prototype.AddMarker=function(marker,title)
{if(marker.setMap!=null)
marker.setMap(this.map);marker.title=title;marker.onMap=false;this.markers.push(marker);this.DisplayLater();};Clusterer.prototype.RemoveMarker=function(marker)
{for(var i=0;i<this.markers.length;++i)
if(this.markers[i]==marker)
{if(marker.onMap)
this.map.removeOverlay(marker);for(var j=0;j<this.clusters.length;++j)
{var cluster=this.clusters[j];if(cluster!=null)
{for(var k=0;k<cluster.markers.length;++k)
if(cluster.markers[k]==marker)
{cluster.markers[k]=null;--cluster.markerCount;break;}
if(cluster.markerCount==0)
{this.ClearCluster(cluster);this.clusters[j]=null;}
else if(cluster==this.poppedUpCluster)
Clusterer.RePop(this);}}
this.markers[i]=null;break;}
this.DisplayLater();};Clusterer.prototype.DisplayLater=function()
{if(this.timeout!=null)
clearTimeout(this.timeout);this.timeout=setTimeout(Clusterer.MakeCaller(Clusterer.Display,this),50);};Clusterer.Display=function(clusterer)
{var i,j,marker,cluster;clearTimeout(clusterer.timeout);var newZoomLevel=clusterer.map.getZoom();if(newZoomLevel!=clusterer.currentZoomLevel)
{for(i=0;i<clusterer.clusters.length;++i)
if(clusterer.clusters[i]!=null)
{clusterer.ClearCluster(clusterer.clusters[i]);clusterer.clusters[i]=null;}
clusterer.clusters.length=0;clusterer.currentZoomLevel=newZoomLevel;}
var bounds=clusterer.map.getBounds();var sw=bounds.getSouthWest();var ne=bounds.getNorthEast();var dx=ne.lng()-sw.lng();var dy=ne.lat()-sw.lat();if(dx<300&&dy<150)
{dx*=0.10;dy*=0.10;bounds=new GLatLngBounds(new GLatLng(sw.lat()-dy,sw.lng()-dx),new GLatLng(ne.lat()+dy,ne.lng()+dx));}
var visibleMarkers=[];var nonvisibleMarkers=[];for(i=0;i<clusterer.markers.length;++i)
{marker=clusterer.markers[i];if(marker!=null)
if(bounds.contains(marker.getPoint()))
visibleMarkers.push(marker);else
nonvisibleMarkers.push(marker);}
for(i=0;i<nonvisibleMarkers.length;++i)
{marker=nonvisibleMarkers[i];if(marker.onMap)
{clusterer.map.removeOverlay(marker);marker.onMap=false;}}
for(i=0;i<clusterer.clusters.length;++i)
{cluster=clusterer.clusters[i];if(cluster!=null&&!bounds.contains(cluster.marker.getPoint())&&cluster.onMap)
{clusterer.map.removeOverlay(cluster.marker);cluster.onMap=false;}}
if(visibleMarkers.length>clusterer.maxVisibleMarkers)
{var latRange=bounds.getNorthEast().lat()-bounds.getSouthWest().lat();var latInc=latRange/clusterer.gridSize;var lngInc=latInc/Math.cos((bounds.getNorthEast().lat()+bounds.getSouthWest().lat())/2.0*Math.PI/180.0);for(var lat=bounds.getSouthWest().lat();lat<=bounds.getNorthEast().lat();lat+=latInc)
for(var lng=bounds.getSouthWest().lng();lng<=bounds.getNorthEast().lng();lng+=lngInc)
{cluster=new Object();cluster.clusterer=clusterer;cluster.bounds=new GLatLngBounds(new GLatLng(lat,lng),new GLatLng(lat+latInc,lng+lngInc));cluster.markers=[];cluster.markerCount=0;cluster.onMap=false;cluster.marker=null;clusterer.clusters.push(cluster);}
for(i=0;i<visibleMarkers.length;++i)
{marker=visibleMarkers[i];if(marker!=null&&!marker.inCluster)
{for(j=0;j<clusterer.clusters.length;++j)
{cluster=clusterer.clusters[j];if(cluster!=null&&cluster.bounds.contains(marker.getPoint()))
{cluster.markers.push(marker);++cluster.markerCount;marker.inCluster=true;}}}}
for(i=0;i<clusterer.clusters.length;++i)
if(clusterer.clusters[i]!=null&&clusterer.clusters[i].markerCount<clusterer.minMarkersPerCluster)
{clusterer.ClearCluster(clusterer.clusters[i]);clusterer.clusters[i]=null;}
for(i=clusterer.clusters.length-1;i>=0;--i)
if(clusterer.clusters[i]!=null)
break;else
--clusterer.clusters.length;for(i=0;i<clusterer.clusters.length;++i)
{cluster=clusterer.clusters[i];if(cluster!=null)
{for(j=0;j<cluster.markers.length;++j)
{marker=cluster.markers[j];if(marker!=null&&marker.onMap)
{clusterer.map.removeOverlay(marker);marker.onMap=false;}}}}
for(i=0;i<clusterer.clusters.length;++i)
{cluster=clusterer.clusters[i];if(cluster!=null&&cluster.marker==null)
{var xTotal=0.0,yTotal=0.0;for(j=0;j<cluster.markers.length;++j)
{marker=cluster.markers[j];if(marker!=null)
{xTotal+=(+marker.getPoint().lng());yTotal+=(+marker.getPoint().lat());}}
var location=new GLatLng(yTotal/cluster.markerCount,xTotal/cluster.markerCount);marker=new GMarker(location,{icon:clusterer.icon});cluster.marker=marker;GEvent.addListener(marker,'click',Clusterer.MakeCaller(Clusterer.PopUp,cluster));}}}
for(i=0;i<visibleMarkers.length;++i)
{marker=visibleMarkers[i];if(marker!=null&&!marker.onMap&&!marker.inCluster)
{clusterer.map.addOverlay(marker);if(marker.addedToMap!=null)
marker.addedToMap();marker.onMap=true;}}
for(i=0;i<clusterer.clusters.length;++i)
{cluster=clusterer.clusters[i];if(cluster!=null&&!cluster.onMap&&bounds.contains(cluster.marker.getPoint()))
{clusterer.map.addOverlay(cluster.marker);cluster.onMap=true;}}
Clusterer.RePop(clusterer);};Clusterer.PopUp=function(cluster)
{var clusterer=cluster.clusterer;var html='<table width="300">';var n=0;for(var i=0;i<cluster.markers.length;++i)
{var marker=cluster.markers[i];if(marker!=null)
{++n;html+='<tr><td>';if(marker.getIcon().smallImage!=null)
html+='<img src="'+marker.getIcon().smallImage+'">';else
html+='<img src="'+marker.getIcon().image+'" width="'+(marker.getIcon().iconSize.width/2)+'" height="'+(marker.getIcon().iconSize.height/2)+'">';html+='</td><td>'+marker.title+'</td></tr>';if(n==clusterer.maxLinesPerInfoBox-1&&cluster.markerCount>clusterer.maxLinesPerInfoBox)
{html+='<tr><td colspan="2">...and '+(cluster.markerCount-n)+' more</td></tr>';break;}}}
html+='</table>';clusterer.map.closeInfoWindow();cluster.marker.openInfoWindowHtml(html);clusterer.poppedUpCluster=cluster;};Clusterer.RePop=function(clusterer)
{if(clusterer.poppedUpCluster!=null)
Clusterer.PopUp(clusterer.poppedUpCluster);};Clusterer.PopDown=function(clusterer)
{clusterer.poppedUpCluster=null;};Clusterer.prototype.ClearCluster=function(cluster)
{var i,marker;for(i=0;i<cluster.markers.length;++i)
if(cluster.markers[i]!=null)
{cluster.markers[i].inCluster=false;cluster.markers[i]=null;}
cluster.markers.length=0;cluster.markerCount=0;if(cluster==this.poppedUpCluster)
this.map.closeInfoWindow();if(cluster.onMap)
{this.map.removeOverlay(cluster.marker);cluster.onMap=false;}};Clusterer.MakeCaller=function(func,arg)
{return function(){func(arg);};};GMarker.prototype.setMap=function(map)
{this.map=map;};GMarker.prototype.addedToMap=function()
{this.map=null;};GMarker.prototype.origOpenInfoWindow=GMarker.prototype.openInfoWindow;GMarker.prototype.openInfoWindow=function(node,opts)
{if(this.map!=null)
return this.map.openInfoWindow(this.getPoint(),node,opts);else
return this.origOpenInfoWindow(node,opts);};GMarker.prototype.origOpenInfoWindowHtml=GMarker.prototype.openInfoWindowHtml;GMarker.prototype.openInfoWindowHtml=function(html,opts)
{if(this.map!=null)
return this.map.openInfoWindowHtml(this.getPoint(),html,opts);else
return this.origOpenInfoWindowHtml(html,opts);};GMarker.prototype.origOpenInfoWindowTabs=GMarker.prototype.openInfoWindowTabs;GMarker.prototype.openInfoWindowTabs=function(tabNodes,opts)
{if(this.map!=null)
return this.map.openInfoWindowTabs(this.getPoint(),tabNodes,opts);else
return this.origOpenInfoWindowTabs(tabNodes,opts);};GMarker.prototype.origOpenInfoWindowTabsHtml=GMarker.prototype.openInfoWindowTabsHtml;GMarker.prototype.openInfoWindowTabsHtml=function(tabHtmls,opts)
{if(this.map!=null)
return this.map.openInfoWindowTabsHtml(this.getPoint(),tabHtmls,opts);else
return this.origOpenInfoWindowTabsHtml(tabHtmls,opts);};GMarker.prototype.origShowMapBlowup=GMarker.prototype.showMapBlowup;GMarker.prototype.showMapBlowup=function(opts)
{if(this.map!=null)
return this.map.showMapBlowup(this.getPoint(),opts);else
return this.origShowMapBlowup(opts);};