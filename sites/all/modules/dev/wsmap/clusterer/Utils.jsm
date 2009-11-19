function CreateXMLHttpRequest()
{var r;try
{r=new XMLHttpRequest();}
catch(e1)
{try
{r=new ActiveXObject('Microsoft.XMLHTTP');}
catch(e2)
{try
{r=new ActiveXObject('Msxml2.XMLHTTP');}
catch(e2)
{r=null;}}}
return r;}
function HttpGet(url,okCallback,failCallback)
{if(failCallback==null)
failCallback=DefaultFailCallback;var request=CreateXMLHttpRequest();request.open('GET',url,true);request.setRequestHeader('Referer',location.href);request.onreadystatechange=MakeCaller(HttpGetRequestChecker,request,okCallback,failCallback);request.send(null);}
function HttpGetRequestChecker(request,okCallback,failCallback)
{if(request.readyState==4)
if(request.status==200)
{if(okCallback!=null)
okCallback(request);}
else
{if(failCallback!=null)
failCallback(request);}}
function DefaultFailCallback(request)
{if(request==null)
alert('XMLHttpRequest create failed!');else
alert('XML fetch failed! ('+request.status+' '+request.statusText+')');}
function HttpGetProxy(url,okCallback,failCallback)
{if(url.substring(0,7)=='http://')
{var host=url.substring(7);var slash=host.indexOf('/');if(slash!=-1)
host=host.substring(0,slash);if(host!=window.location.host)
url='/resources/proxy.cgi?url='+encodeURIComponent(url);}
HttpGet(url,okCallback,failCallback);}
function GetXmlText(element)
{var value='';var child=element.firstChild;while(child!=null)
{if(value!='')
value+=' ';value+=child.nodeValue;child=child.nextSibling;}
return value;}
function GetXmlValue(elements)
{var values='';for(var i=0;i<elements.length;++i)
{if(elements[i]!=null&&elements[i].firstChild!=null)
{if(values!='')
values+=' ';values+=elements[i].firstChild.nodeValue;}}
return values;}
function FindChildNamed(element,name)
{var child=element.firstChild;while(child!=null)
{if(child.nodeName==name)
return child;child=child.nextSibling;}
return null;}
function FindDeepChildNamed(element,name)
{if(element.nodeName==name)
return element;var child=element.firstChild;while(child!=null)
{var d=FindDeepChildNamed(child,name);if(d!=null)
return d;child=child.nextSibling;}
return null;}
function CountNodes(element)
{var count=1;var child=element.firstChild;while(child!=null)
{count+=CountNodes(child);child=child.nextSibling;}
return count;}
var endOfTime='Tue, 19-Jan-2038 03:14:07 GMT';var beginningOfTime='Thu, 01-Jan-1970 00:00:00 GMT';function SaveCookie(cookieName,cookieValue)
{document.cookie=cookieName+'='+encodeURIComponent(cookieValue)+'; expires='+endOfTime;}
function ClearCookie(cookieName)
{document.cookie=cookieName+'=; expires='+beginningOfTime;}
function GetCookie(cookieName)
{if(document.cookie.length>0)
{var cookieNameEq=cookieName+'=';var cookies=document.cookie.split(';');for(var i=0;i<cookies.length;++i)
{while(cookies[i].charAt(0)==' ')
cookies[i]=cookies[i].substr(1);if(cookies[i].indexOf(cookieNameEq)==0)
return decodeURIComponent(cookies[i].substr(cookieNameEq.length));}}
return null;}
function EntityToIso8859(inStr)
{var outStr='';for(var i=0;i<inStr.length;++i)
{var c=inStr.charAt(i);if(c!='&')
outStr+=c;else
{var semi=inStr.indexOf(';',i);if(semi==-1)
outStr+=c;else
{var entity=inStr.substring(i+1,semi);if(entity=='iexcl')outStr+='\xa1';else if(entity=='copy')outStr+='\xa9';else if(entity=='laquo')outStr+='\xab';else if(entity=='reg')outStr+='\xae';else if(entity=='deg')outStr+='\xb0';else if(entity=='raquo')outStr+='\xbb';else if(entity=='iquest')outStr+='\xbf';else if(entity=='Agrave')outStr+='\xc0';else if(entity=='Aacute')outStr+='\xc1';else if(entity=='Acirc')outStr+='\xc2';else if(entity=='Atilde')outStr+='\xc3';else if(entity=='Auml')outStr+='\xc4';else if(entity=='Aring')outStr+='\xc5';else if(entity=='AElig')outStr+='\xc6';else if(entity=='Ccedil')outStr+='\xc7';else if(entity=='Egrave')outStr+='\xc8';else if(entity=='Eacute')outStr+='\xc9';else if(entity=='Ecirc')outStr+='\xca';else if(entity=='Euml')outStr+='\xcb';else if(entity=='Igrave')outStr+='\xcc';else if(entity=='Iacute')outStr+='\xcd';else if(entity=='Icirc')outStr+='\xce';else if(entity=='Iuml')outStr+='\xcf';else if(entity=='Ntilde')outStr+='\xd1';else if(entity=='Ograve')outStr+='\xd2';else if(entity=='Oacute')outStr+='\xd3';else if(entity=='Ocirc')outStr+='\xd4';else if(entity=='Otilde')outStr+='\xd5';else if(entity=='Ouml')outStr+='\xd6';else if(entity=='Oslash')outStr+='\xd8';else if(entity=='Ugrave')outStr+='\xd9';else if(entity=='Uacute')outStr+='\xda';else if(entity=='Ucirc')outStr+='\xdb';else if(entity=='Uuml')outStr+='\xdc';else if(entity=='Yacute')outStr+='\xdd';else if(entity=='szlig')outStr+='\xdf';else if(entity=='agrave')outStr+='\xe0';else if(entity=='aacute')outStr+='\xe1';else if(entity=='acirc')outStr+='\xe2';else if(entity=='atilde')outStr+='\xe3';else if(entity=='auml')outStr+='\xe4';else if(entity=='aring')outStr+='\xe5';else if(entity=='aelig')outStr+='\xe6';else if(entity=='ccedil')outStr+='\xe7';else if(entity=='egrave')outStr+='\xe8';else if(entity=='eacute')outStr+='\xe9';else if(entity=='ecirc')outStr+='\xea';else if(entity=='euml')outStr+='\xeb';else if(entity=='igrave')outStr+='\xec';else if(entity=='iacute')outStr+='\xed';else if(entity=='icirc')outStr+='\xee';else if(entity=='iuml')outStr+='\xef';else if(entity=='ntilde')outStr+='\xf1';else if(entity=='ograve')outStr+='\xf2';else if(entity=='oacute')outStr+='\xf3';else if(entity=='ocirc')outStr+='\xf4';else if(entity=='otilde')outStr+='\xf5';else if(entity=='ouml')outStr+='\xf6';else if(entity=='oslash')outStr+='\xf8';else if(entity=='ugrave')outStr+='\xf9';else if(entity=='uacute')outStr+='\xfa';else if(entity=='ucirc')outStr+='\xfb';else if(entity=='uuml')outStr+='\xfc';else if(entity=='yacute')outStr+='\xfd';else if(entity=='yuml')outStr+='\xff';else if(entity=='nbsp')outStr+=' ';else if(entity=='lt')outStr+='<';else if(entity=='gt')outStr+='>';else if(entity=='amp')outStr+='&';else outStr+='&'+entity+';';i+=entity.length+1;}}}
return outStr;}
function DeEntityize(inStr)
{var outStr='';for(var i=0;i<inStr.length;++i)
{var c=inStr.charAt(i);if(c!='&')
outStr+=c;else
{var semi=inStr.indexOf(';',i);if(semi!=-1)
i=semi;}}
return outStr;}
function DeElementize(inStr)
{var outStr='';for(var i=0;i<inStr.length;++i)
{var c=inStr.charAt(i);if(c!='<')
outStr+=c;else
{var gt=inStr.indexOf('>',i);if(gt!=-1)
i=gt;}}
return outStr;}
function DeHtmlize(str)
{return DeEntityize(EntityToIso8859(DeElementize(str)));}
function MakeCaller(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
{return function(){func(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);};}
function GetParameters()
{var query_string=location.search.substring(1,location.search.length);var namevals=query_string.split('&');var params=[];for(var i=0;i<namevals.length;++i)
{var nameval=namevals[i].split('=');if(nameval.length==2)
params[nameval[0]]=decodeURIComponent(nameval[1]);}
return params;}
function Substitute(str,from,to)
{var fromLen=from.length;var newStr='';while(str.length>0)
{if(str.substr(0,fromLen)==from)
{newStr+=to;str=str.substr(fromLen);}
else
{newStr+=str.charAt(0);str=str.substr(1);}}
return newStr;}
function Props(o)
{var s='';for(p in o)
{if(s.length!=0)
s+='\n';s+=p+': '+o[p];}
return s;}
function GetBrowserWidth()
{var width=null;try
{width=innerWidth;}
catch(e1)
{try
{width=document.documentElement.offsetWidth;}
catch(e2)
{try
{width=document.documentElement.clientWidth;}
catch(e2)
{try
{width=document.body.clientWidth;}
catch(e2)
{}}}}
return width;}
function GetBrowserHeight()
{var height=null;try
{height=innerHeight;}
catch(e1)
{try
{height=document.documentElement.offsetHeight;}
catch(e2)
{try
{height=document.documentElement.clientHeight;}
catch(e2)
{try
{height=document.body.clientHeight;}
catch(e2)
{}}}}
return height;}
function AppendElement(parent,elementType,properties)
{var element=document.createElement(elementType);for(property in properties)
if(property=='style')
for(nestedProperty in properties[property])
element[property][nestedProperty]=properties[property][nestedProperty];else
element[property]=properties[property];parent.appendChild(element);return element;}
function instanceOf(object,clas)
{while(object!=null)
{if(object==clas.prototype)
return true;object=object.__proto__;}
return false;}
