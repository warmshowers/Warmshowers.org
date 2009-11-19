
var LANG_UNKNOWN=0;var LANG_ENGLISH=1;var LANG_FRENCH=2;var currentLanguage=LANG_UNKNOWN;var _mInstructions;function SetLanguage(language)
{if(language!=currentLanguage)
{switch(language)
{case LANG_ENGLISH:_mInstructions='Drag the map with your mouse, or double-click to center.';_mSiteName='Google Maps';_mDataCopy='Map data &#169;2005 ';_mZenrinCopy='Map &#169;2005 ';_mNormalMap='Map';_mNormalMapShort='Map';_mHybridMap='Hybrid';_mHybridMapShort='Hyb';_mKeyholeMap='Satellite';_mKeyholeMapShort='Sat';_mNew='New!';_mTerms='Terms of Use';_mKeyholeCopy='Imagery &#169;2005 ';_mDecimalPoint='.';_mThousandsSeparator=',';_mZoomIn='Zoom In';_mZoomOut='Zoom Out';_mZoomSet='Click to set zoom level';_mZoomDrag='Drag to zoom';_mPanWest='Go left';_mPanEast='Go right';_mPanNorth='Go up';_mPanSouth='Go down';_mLastResult='Return to the last result';_mScale='Scale at the center of the map';break;case LANG_FRENCH:_mInstructions='Faites glisser la carte avec la souris ou double-cliquez sur un point pour la recentrer.';_mSiteName='Cartes Google';_mDataCopy='Donn&eacute;es cartographiques &#169;2005 ';_mZenrinCopy='Carte &#169;2005 ';_mNormalMap='Carte';_mNormalMapShort='Car';_mHybridMap='Mixte';_mHybridMapShort='Mix';_mKeyholeMap='Satellite';_mKeyholeMapShort='Sat';_mNew='Nouvelle!';_mTerms='Limites d\'utilisation';_mKeyholeCopy='Images &#169;2005 ';_mDecimalPoint=',';_mThousandsSeparator='.';_mZoomIn='Zoom avant';_mZoomOut='Zoom arri&egrave;re';_mZoomSet='Cliquez pour d&eacute;finir le facteur de zoom';_mZoomDrag='Faites glisser le curseur pour zoomer';_mPanWest='D&eacute;placer vers la gauche';_mPanEast='D&eacute;placer vers la droite';_mPanNorth='D&eacute;placer vers le haut';_mPanSouth='D&eacute;placer vers le bas';_mLastResult='Revenir au r&eacute;sultat pr&eacute;c&eacute;dent';_mScale='&Eacute;chelle au centre de la carte';break;}
_mZoomIn=EntityToIso8859(_mZoomIn);_mZoomOut=EntityToIso8859(_mZoomOut);_mZoomSet=EntityToIso8859(_mZoomSet);_mZoomDrag=EntityToIso8859(_mZoomDrag);_mPanWest=EntityToIso8859(_mPanWest);_mPanEast=EntityToIso8859(_mPanEast);_mPanNorth=EntityToIso8859(_mPanNorth);_mPanSouth=EntityToIso8859(_mPanSouth);_mLastResult=EntityToIso8859(_mLastResult);_mScale=EntityToIso8859(_mScale);currentLanguage=language;}}
var pztCookieName='positionZoomType2';var oldPztCookieName='positionZoomType';function SavePositionZoomTypeCookie(map)
{var mapCenter=map.getCenter();var mapZoom=map.getZoom();var mapTypeLetter=MapTypeToLetter(map.getCurrentMapType());var cookieValue=mapCenter.lat().toFixed(5)+','+mapCenter.lng().toFixed(5)+','+mapZoom+','+mapTypeLetter;SaveCookie(pztCookieName,cookieValue);ClearCookie(oldPztCookieName);}
function GetPositionZoomTypeCookie(map)
{var cookieValue=GetCookie(pztCookieName);if(cookieValue==null)
return false;var vals=cookieValue.split(',');if(vals.length!=4)
return false;var mapY=parseFloat(vals[0]);var mapX=parseFloat(vals[1]);var mapZoomStr=vals[2];var mapTypeLetter=vals[3];var mapZoom=parseInt(mapZoomStr);map.setCenter(new GLatLng(mapY,mapX),mapZoom);map.setMapType(LetterToMapType(mapTypeLetter));return true;}
function SavePositionZoomTypeCookieOnChanges(map)
{var caller=MakeCaller(SavePositionZoomTypeCookie,map);GEvent.addListener(map,'move',MakeCaller(SpztcChecker,caller));GEvent.addListener(map,'zoomend',caller);GEvent.addListener(map,'maptypechanged ',caller);}
var spztcMoveEndTimer=null;var spztcMoveEndCheckMsecs=700;var spztcMoveCount,spztcPrevMoveCount;function SpztcChecker(caller)
{if(spztcMoveEndTimer==null)
{spztcMoveEndTimer=setTimeout(MakeCaller(SpztcTimeChecker,caller),spztcMoveEndCheckMsecs);spztcPrevMoveCount=spztcMoveCount=0;}
++spztcMoveCount;}
function SpztcTimeChecker(caller)
{if(spztcMoveCount==spztcPrevMoveCount)
{spztcMoveEndTimer=null;caller();}
else
{spztcMoveEndTimer=setTimeout(MakeCaller(SpztcTimeChecker,caller),spztcMoveEndCheckMsecs);spztcPrevMoveCount=spztcMoveCount;}}
function MapTypeToLetter(mapType)
{switch(mapType)
{case G_NORMAL_MAP:return'M';case G_SATELLITE_MAP:return'S';case G_HYBRID_MAP:return'H';case WMS_TOPO_MAP:return'T';case WMS_DOQ_MAP:return'O';case WMS_NEXRAD_MAP:return'N';default:return'-';}}
function LetterToMapType(letter)
{switch(letter)
{case'M':return G_NORMAL_MAP;case'S':return G_SATELLITE_MAP;case'H':return G_HYBRID_MAP;case'T':return WMS_TOPO_MAP;case'O':return WMS_DOQ_MAP;case'N':return WMS_NEXRAD_MAP;default:return G_NORMAL_MAP;}}
var degreesPerRadian=180.0/Math.PI;var radiansPerDegree=Math.PI/180.0;function Bearing(from,to)
{var lat1=from.lat()*radiansPerDegree;var lon1=from.lng()*radiansPerDegree;var lat2=to.lat()*radiansPerDegree;var lon2=to.lng()*radiansPerDegree;var angle=-Math.atan2(Math.sin(lon1-lon2)*Math.cos(lat2),Math.cos(lat1)*Math.sin(lat2)-Math.sin(lat1)*Math.cos(lat2)*Math.cos(lon1-lon2));if(angle<0.0)
angle+=Math.PI*2.0;angle=angle*degreesPerRadian;return angle;}
function BadBearing(from,to)
{var a=from.lat();var b=to.lat();var l=to.lng()-from.lng();var episilon=0.0000000001;if(Math.abs(l)<=episilon)
if(a>b)
return 180.0;else
return 0.0;else if(Math.abs(Math.abs(l)-180.0)<=episilon)
if(a>=0.0&&b>=0.0)
return 0.0;else if(a<0.0&&b<0.0)
return 180.0;else if(a>=0.0)
if(a>-b)
return 0.0;else
return 180.0;else
if(a>-b)
return 180.0;else
return 0.0;a*=radiansPerDegree;b*=radiansPerDegree;l*=radiansPerDegree;var d=Math.acos(Math.sin(a)*Math.sin(b)+Math.cos(a)*Math.cos(b)*Math.cos(l));var angle=Math.acos((Math.sin(b)-Math.sin(a)*Math.cos(d))/(Math.cos(a)*Math.sin(d)));angle=angle*degreesPerRadian;if(Math.sin(l)<0)
angle=360.0-angle;return angle;}
function Direction(bearing)
{if(bearing>=348.75||bearing<11.25)
return"N";if(bearing>=11.25&&bearing<33.75)
return"NxNE";if(bearing>=33.75&&bearing<56.25)
return"NE";if(bearing>=56.25&&bearing<78.75)
return"ExNE";if(bearing>=78.75&&bearing<101.25)
return"E";if(bearing>=101.25&&bearing<123.75)
return"ExSE";if(bearing>=123.75&&bearing<146.25)
return"SE";if(bearing>=146.25&&bearing<168.75)
return"SxSE";if(bearing>=168.75&&bearing<191.25)
return"S";if(bearing>=191.25&&bearing<213.75)
return"SxSW";if(bearing>=213.75&&bearing<236.25)
return"SW";if(bearing>=236.25&&bearing<258.75)
return"WxSW";if(bearing>=258.75&&bearing<281.25)
return"W";if(bearing>=281.25&&bearing<303.75)
return"WxNW";if(bearing>=303.75&&bearing<326.25)
return"NW";if(bearing>=326.25&&bearing<348.75)
return"NxNW";return"???"}
function Direction8(bearing)
{if(bearing>=337.5||bearing<22.5)
return"N";if(bearing>=22.5&&bearing<67.5)
return"NE";if(bearing>=67.5&&bearing<112.5)
return"E";if(bearing>=112.5&&bearing<157.5)
return"SE";if(bearing>=157.5&&bearing<202.5)
return"S";if(bearing>=202.5&&bearing<247.5)
return"SW";if(bearing>=247.5&&bearing<292.5)
return"W";if(bearing>=292.5&&bearing<337.5)
return"NW";return"???"}
var clickZoomMap=null;var clickZoomListener;var clickZoomClicked;var clickZoomDoubleClicked;function ClickZoom(map)
{if(map==clickZoomMap)
return;if(clickZoomMap!=null)
ClickZoomOff();clickZoomMap=map;clickZoomListener=GEvent.addListener(clickZoomMap,'click',ClickZoomClickHandler);clickZoomClicked=false;clickZoomDoubleClicked=false;}
function ClickZoomOff()
{if(clickZoomMap!=null)
{GEvent.removeListener(clickZoomListener);clickZoomListener=null;clickZoomMap=null;}}
function ClickZoomClickHandler(overlay,point)
{if(overlay==null&&point!=null)
{if(clickZoomClicked)
clickZoomDoubleClicked=true;else
{clickZoomClicked=true;clickZoomDoubleClicked=false;setTimeout(MakeCaller(ClickZoomLaterHandler,point),250);}}}
function ClickZoomLaterHandler(point)
{if(!clickZoomDoubleClicked)
clickZoomMap.setCenter(point,clickZoomMap.getZoom()+1);clickZoomClicked=false;}
var mouseWheelZoomMap=null;function MouseWheelZoom(map)
{if(map==mouseWheelZoomMap)
return;if(mouseWheelZoomMap!=null)
MouseWheelZoomOff();mouseWheelZoomMap=map;var container=mouseWheelZoomMap.getContainer();if(container.addEventListener)
container.addEventListener('DOMMouseScroll',MouseWheelZoomHandler,false);else
container.onmousewheel=window.onmousewheel=document.onmousewheel=MouseWheelZoomHandler;}
function MouseWheelZoomOff()
{if(mouseWheelZoomMap!=null)
{var container=mouseWheelZoomMap.getContainer();if(container.removeEventListener)
container.removeEventListener('DOMMouseScroll',MouseWheelZoomHandler,false);else
container.onmousewheel=window.onmousewheel=document.onmousewheel=null;mouseWheelZoomMap=null;}}
function MouseWheelZoomHandler(e)
{if(e==null)
{if(event!=null)
e=event;else if(window.event!=null)
e=window.event;}
if(e!=null)
{var data=0;if(e.wheelData!=null)
{data=e.wheelData
if(window.opera)
data=-data;}
else if(e.detail!=null)
data=-e.detail;if(data>0)
mouseWheelZoomMap.setZoom(mouseWheelZoomMap.getZoom()+1);else if(data<0)
mouseWheelZoomMap.setZoom(mouseWheelZoomMap.getZoom()-1);}}
function GetPointFromIP()
{var request=CreateXMLHttpRequest();if(request==null)
return null;request.open('GET','/resources/hostip_proxy.cgi',false);request.send(null);if(request.readyState!=4)
return null;if(request.status!=200)
return null;if(request.responseXML==null)
return null;if(request.responseXML.documentElement==null)
return null;var coordElement=FindDeepChildNamed(request.responseXML.documentElement,'gml:coordinates');if(coordElement==null)
return null;var coordText=GetXmlText(coordElement);var coords=coordText.split(',');if(coords.length!=2)
return null;var lng=parseFloat(coords[0]);var lat=parseFloat(coords[1]);return new GLatLng(lat,lng);}
function ZoomToMarkers(map,markers)
{if(markers.length==0)
return;var minLng=9999.0,maxLng=-9999.0,minLat=9999.0,maxLat=-9999.0;for(var i in markers)
{if(markers[i].getPoint().lng()<minLng)
minLng=markers[i].getPoint().lng();if(markers[i].getPoint().lng()>maxLng)
maxLng=markers[i].getPoint().lng();if(markers[i].getPoint().lat()<minLat)
minLat=markers[i].getPoint().lat();if(markers[i].getPoint().lat()>maxLat)
maxLat=markers[i].getPoint().lat();}
var center=new GLatLng((minLat+maxLat)/2.0,(minLng+maxLng)/2.0);map.setCenter(center);var bounds=new GLatLngBounds(new GLatLng(minLat,minLng),new GLatLng(maxLat,maxLng));map.setZoom(map.getBoundsZoomLevel(bounds));}
var crosshairsSize=19;GMap2.prototype.addCrosshairs=function()
{var container=this.getContainer();if(this.crosshairs)
this.removeCrosshairs();var crosshairs=document.createElement("img");crosshairs.src='http://www.acme.com/resources/images/crosshairs.gif';crosshairs.style.width=crosshairsSize+'px';crosshairs.style.height=crosshairsSize+'px';crosshairs.style.border='0';crosshairs.style.position='relative';crosshairs.style.top=((container.clientHeight-crosshairsSize)/2)+'px';crosshairs.style.left=((container.clientWidth-crosshairsSize)/2)+'px';crosshairs.style.zIndex='500';container.appendChild(crosshairs);this.crosshairs=crosshairs;return crosshairs;};GMap2.prototype.removeCrosshairs=function()
{if(this.crosshairs)
{this.getContainer().removeChild(this.crosshairs);this.crosshairs=null;}};