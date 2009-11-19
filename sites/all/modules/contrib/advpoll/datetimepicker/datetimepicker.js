// $Id: datetimepicker.js,v 1.1.2.2 2007/09/08 18:09:46 fajerstarter Exp $

/* JavaScript files used by the date and time picker. 
 * 
 * jQuery Calendar: http://marcgrabanski.com/code/jquery-calendar
 * Dimensions: http://jquery.com/plugins/project/dimensions 
 * timePicker 
 */

/* jQuery Calendar v2.7
   Written by Marc Grabanski (m@marcgrabanski.com) and enhanced by Keith Wood (kbwood@iprimus.com.au).

   Copyright (c) 2007 Marc Grabanski (http://marcgrabanski.com/code/jquery-calendar)
   Dual licensed under the GPL (http://www.gnu.org/licenses/gpl-3.0.txt) and 
   CC (http://creativecommons.org/licenses/by/3.0/) licenses. "Share or Remix it but please Attribute the authors."
   Date: 09-03-2007  */
eval(function(p,a,c,k,e,r){e=function(c){return(c<a?'':e(parseInt(c/a)))+((c=c%a)>35?String.fromCharCode(c+29):c.toString(36))};if(!''.replace(/^/,String)){while(c--)r[e(c)]=k[c]||e(c);k=[function(e){return r[e]}];e=function(){return'\\w+'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}('I 2m(){8.3t=0;8.2h=[];8.1L=O;8.1i=[];8.1t=X;8.1v=X;8.2g=[];8.2g[\'\']={2Q:\'5R\',3R:\'5p\',2u:\'&58;51\',2H:\'4D&4z;\',3d:\'4t\',2k:[\'4o\',\'4k\',\'4g\',\'4a\',\'5P\',\'5G\',\'5D\'],3O:[\'5o\',\'5l\',\'5j\',\'5d\',\'57\',\'56\',\'50\',\'4V\',\'4P\',\'4J\',\'4C\',\'4A\'],1R:\'4x/\'};8.1E={3f:\'1z\',3c:\'\',31:\'...\',30:\'\',2W:X,2U:1c,2R:X,2P:1c,2N:1c,2K:\'-10:+10\',2I:0,3Z:1c,3W:X,28:O,26:O,1J:\'5u\',3M:O,3K:O,2w:O};$.14(8.1E,8.2g[\'\']);8.S=$(\'<V 3A="3y"></V>\');$(1d.1r).1K(8.S);$(1d.1r).55(8.3F)}$.14(2m.3I,{3q:I(a){F b=8.3t++;8.2h[b]=a;N b},U:I(a){N 8.2h[a]||a},4F:I(a){$.14(8.1E,a||{})},2p:I(e){F a=G.U(8.1g);H(G.1t){4y(e.2l){1a 9:G.1q(a,\'\');11;1a 13:G.1P(a);11;1a 27:G.1q(a,a.J(\'1J\'));11;1a 33:G.T(a,-1,(e.1j?\'Y\':\'M\'));11;1a 34:G.T(a,+1,(e.1j?\'Y\':\'M\'));11;1a 35:H(e.1j)G.2y(a);11;1a 36:H(e.1j)G.2B(a);11;1a 37:H(e.1j)G.T(a,-1,\'D\');11;1a 38:H(e.1j)G.T(a,-7,\'D\');11;1a 39:H(e.1j)G.T(a,+1,\'D\');11;1a 40:H(e.1j)G.T(a,+7,\'D\');11}}19 H(e.2l==36&&e.1j){G.1A(8)}},2J:I(e){F a=G.U(8.1g);F b=45.5N(e.43==5K?e.2l:e.43);N(b<\' \'||b==a.J(\'1R\').1k(3)||(b>=\'0\'&&b<=\'9\'))},3U:I(a,b){F c=$(a);F d=b.J(\'3c\');H(d){c.3Q(\'<1F K="5v">\'+d+\'</1F>\')}F e=b.J(\'3f\');H(e==\'1z\'||e==\'2r\'){c.1z(8.1A)}H(e==\'1x\'||e==\'2r\'){F f=b.J(\'31\');F g=b.J(\'30\');F h=b.J(\'2W\');F i=$(h?\'<20 K="1s" 2v="\'+g+\'" 3E="\'+f+\'" 3C="\'+f+\'"/>\':\'<1x 2a="1x" K="1s">\'+(g!=\'\'?\'<20 2v="\'+g+\'" 3E="\'+f+\'" 3C="\'+f+\'"/>\':f)+\'</1x>\');c.5i(\'<1F K="5h"></1F>\').3Q(i);i.5f(8.1A)}c.3z(8.2p).5c(8.2J);c[0].1g=b.R},3w:I(a,b){$(a).1K(b.S);a.1g=b.R;F c=P W();b.17=c.16();b.Q=c.1b();b.L=c.15();G.T(b)},53:I(a,b,c,d){F e=8.3r;H(!e){e=8.3r=P 1H({},X);8.1m=$(\'<1I 2a="4U" 4S="1" 3N="2C: 2q; 25: -3X;"/>\');8.1m.3z(8.2p);$(\'1r\').1K(8.1m);8.1m[0].1g=e.R}$.14(e.1l,c||{});8.1m.2G(a);H(2F.41){1W=2F.4B;1V=2F.41}19 H(1d.1U&&1d.1U.2o){1W=1d.1U.3m;1V=1d.1U.2o}19 H(1d.1r){1W=1d.1r.3m;1V=1d.1r.2o}8.1h=d||[(1W/2)-3l,(1V/2)-3l];8.1m.18(\'2n\',8.1h[0]+\'1S\').18(\'25\',8.1h[1]+\'1S\');e.1l.2w=b;8.1v=1c;8.S.3k(\'3j\');8.1A(8.1m[0]);H($.1C){$.1C(8.S)}},4w:I(c){c=(c.3i?c:$(c));c.1D(I(){8.1Q=X;$(\'../1x.1s\',8).1D(I(){8.1Q=X});$(\'../20.1s\',8).18({3h:\'1.0\',3g:\'\'});F b=8;G.1i=$.3e(G.1i,I(a){N(a==b?O:a)})})},4v:I(c){c=(c.3i?c:$(c));c.1D(I(){8.1Q=1c;$(\'../1x.1s\',8).1D(I(){8.1Q=1c});$(\'../20.1s\',8).18({3h:\'0.5\',3g:\'4u\'});F b=8;G.1i=$.3e(G.1i,I(a){N(a==b?O:a)});G.1i[G.1i.1B]=8})},4s:I(a,b){F c=8.U(a.1g);H(c){$.14(c.1l,b||{});8.1w(c)}},4r:I(a,b){F c=8.U(a.1g);H(c){c.3b(b)}},4q:I(a){F b=8.U(a.1g);N(b?b.3a():O)},1A:I(a){F b=(a.1O&&a.1O.2j()==\'1I\'?a:8);H(b.1O.2j()!=\'1I\'){b=$(\'1I\',b.4p)[0]}H(G.2i==b){N}1f(F i=0;i<G.1i.1B;i++){H(G.1i[i]==b){N}}F c=G.U(b.1g);G.1q(c,\'\');G.2i=b;c.2Z(b);H(G.1v){b.24=\'\'}H(!G.1h){G.1h=G.2Y(b);G.1h[1]+=b.3H}c.S.18(\'2C\',(G.1v&&$.1C?\'4n\':\'2q\')).18(\'2n\',G.1h[0]+\'1S\').18(\'25\',G.1h[1]+\'1S\');G.1h=O;F d=c.J(\'3K\');$.14(c.1l,(d?d(b):{}));G.2V(c)},2V:I(a){F b=8.U(a);G.1w(b);H(!b.1G){F c=b.J(\'1J\');b.S.4m(c,I(){G.1t=1c;G.2f(b)});H(c==\'\'){G.1t=1c;G.2f(b)}H(b.Z[0].2a!=\'2e\'){b.Z[0].1z()}8.1L=b}},1w:I(a){a.S.4l().1K(a.2T());H(a.Z&&a.Z!=\'2e\'){a.Z[0].1z()}},2f:I(a){H($.2d.2D){$(\'#2S\').18({4j:a.S[0].4i+4,4h:a.S[0].3H+4})}},1q:I(a,b){F c=8.U(a);H(G.1t){b=(b!=O?b:c.J(\'1J\'));c.S.4f(b,I(){G.2E(c)});H(b==\'\'){G.2E(c)}G.1t=X;G.2i=O;c.1l.2O=O;H(G.1v){G.1m.18(\'2C\',\'2q\').18(\'2n\',\'4e\').18(\'25\',\'-3X\');H($.1C){$.4d();$(\'1r\').1K(8.S)}}G.1v=X}G.1L=O},2E:I(a){a.S.2M(\'3j\');$(\'.2L\',a.S).4b()},3F:I(a){H(!G.1L){N}F b=$(a.49);H((b.48("#3y").1B==0)&&(b.47(\'K\')!=\'1s\')&&G.1t&&!(G.1v&&$.1C)){G.1q(G.1L,\'\')}},T:I(a,b,c){F d=8.U(a);d.T(b,c);8.1w(d)},2B:I(a){F b=P W();F c=8.U(a);c.17=b.16();c.Q=b.1b();c.L=b.15();8.T(c)},2b:I(a,b,c){F d=8.U(a);d.1M=X;d[c==\'M\'?\'Q\':\'L\']=b.46[b.4c].24-0;8.T(d)},2c:I(a){F b=8.U(a);H(b.Z&&b.1M&&!$.2d.2D){b.Z[0].1z()}b.1M=!b.1M},44:I(b,a){F c=8.U(b);F d=c.J(\'2k\');F e=a.5M.5L;1f(F i=0;i<7;i++){H(d[i]==e){c.1l.2I=i;11}}8.1w(c)},42:I(a,b){F c=8.U(a);c.17=$("a",b).5J();8.1P(a)},2y:I(a){8.1P(a,\'\')},1P:I(a,b){F c=8.U(a);b=(b!=O?b:c.3Y());H(c.Z){c.Z.2G(b)}F d=c.J(\'2w\');H(d){d(b)}19{c.Z.5I(\'5H\')}H(c.1G){8.1w(c)}19{8.1q(c,c.J(\'1J\'))}},5F:I(a){F b=a.3V();N[(b>0&&b<6),\'\']},2Y:I(a){H(a.2a==\'2e\'){a=a.5E}F b=1N=0;H(a.3T){b=a.3S;1N=a.2X;5B(a=a.3T){F c=b;b+=a.3S;H(b<0){b=c}1N+=a.2X}}N[b,1N]}});I 1H(a,b){8.R=G.3q(8);8.17=0;8.Q=0;8.L=0;8.Z=O;8.1G=b;8.S=(!b?G.S:$(\'<V 3A="5A\'+8.R+\'" K="5z"></V>\'));H(b){F c=P W();8.1n=c.16();8.1o=c.1b();8.1p=c.15()}8.1l=$.14({},a||{})}$.14(1H.3I,{J:I(a){N(8.1l[a]!=O?8.1l[a]:G.1E[a])},2Z:I(a){8.Z=$(a);F b=8.J(\'1R\');F c=8.Z.2G().3P(b.1k(3));H(c.1B==3){8.1n=1u(c[b.2A(\'D\')],10);8.1o=1u(c[b.2A(\'M\')],10)-1;8.1p=1u(c[b.2A(\'Y\')],10)}19{F d=P W();8.1n=d.16();8.1o=d.1b();8.1p=d.15()}8.17=8.1n;8.Q=8.1o;8.L=8.1p;8.T()},3b:I(a){8.17=8.1n=a.16();8.Q=8.1o=a.1b();8.L=8.1p=a.15();8.T()},3a:I(){N P W(8.1p,8.1o,8.1n)},2T:I(){F a=P W();a=P W(a.15(),a.1b(),a.16());F b=\'<V K="5t">\'+\'<a K="5s" 1e="G.2y(\'+8.R+\');">\'+8.J(\'2Q\')+\'</a>\'+\'<a K="5r" 1e="G.1q(\'+8.R+\');">\'+8.J(\'3R\')+\'</a></V>\';F c=8.J(\'2O\');F d=8.J(\'2U\');F e=8.J(\'2R\');F f=(c?\'<V K="2L">\'+c+\'</V>\':\'\')+(d&&!8.1G?b:\'\')+\'<V K="5q">\'+(8.2t(-1)?\'<a K="3L" \'+\'1e="G.T(\'+8.R+\', -1, \\\'M\\\');">\'+8.J(\'2u\')+\'</a>\':(e?\'\':\'<1T K="3L">\'+8.J(\'2u\')+\'</1T>\'))+(8.2x(a)?\'<a K="5n" \'+\'1e="G.2B(\'+8.R+\');">\'+8.J(\'3d\')+\'</a>\':\'\')+(8.2t(+1)?\'<a K="3J" \'+\'1e="G.T(\'+8.R+\', +1, \\\'M\\\');">\'+8.J(\'2H\')+\'</a>\':(e?\'\':\'<1T K="3J">\'+8.J(\'2H\')+\'</1T>\'))+\'</V><V K="5m">\';F g=8.J(\'28\');F h=8.J(\'26\');F i=8.J(\'3O\');H(!8.J(\'2P\')){f+=i[8.Q]+\'&3G;\'}19{F j=(g&&g.15()==8.L);F k=(h&&h.15()==8.L);f+=\'<1X K="5k" \'+\'3D="G.2b(\'+8.R+\', 8, \\\'M\\\');" \'+\'1e="G.2c(\'+8.R+\');">\';1f(F l=0;l<12;l++){H((!j||l>=g.1b())&&(!k||l<=h.1b())){f+=\'<1Y 24="\'+l+\'"\'+(l==8.Q?\' 1Z="1Z"\':\'\')+\'>\'+i[l]+\'</1Y>\'}}f+=\'</1X>\'}H(!8.J(\'2N\')){f+=8.L}19{F m=8.J(\'2K\').3P(\':\');F n=0;F o=0;H(m.1B!=2){n=8.L-10;o=8.L+10}19 H(m[0].1k(0)==\'+\'||m[0].1k(0)==\'-\'){n=8.L+1u(m[0],10);o=8.L+1u(m[1],10)}19{n=1u(m[0],10);o=1u(m[1],10)}n=(g?21.5g(n,g.15()):n);o=(h?21.3B(o,h.15()):o);f+=\'<1X K="5e" 3D="G.2b(\'+8.R+\', 8, \\\'Y\\\');" \'+\'1e="G.2c(\'+8.R+\');">\';1f(;n<=o;n++){f+=\'<1Y 24="\'+n+\'"\'+(n==8.L?\' 1Z="1Z"\':\'\')+\'>\'+n+\'</1Y>\'}f+=\'</1X>\'}f+=\'</V><3p K="3s" 5b="0" 5a="0"><3x>\'+\'<23 K="59">\';F p=8.J(\'2I\');F q=8.J(\'3Z\');F r=8.J(\'2k\');1f(F s=0;s<7;s++){f+=\'<22>\'+(!q?\'\':\'<a 1e="G.44(\'+8.R+\', 8);">\')+r[(s+p)%7]+(q?\'</a>\':\'\')+\'</22>\'}f+=\'</23></3x><3v>\';F t=8.2s(8.L,8.Q);8.17=21.3B(8.17,t);F u=(8.3u(8.L,8.Q)-p+7)%7;F v=P W(8.1p,8.1o,8.1n);F w=P W(8.L,8.Q,8.17);F x=P W(8.L,8.Q,1-u);F y=21.54((u+t)/7);F z=8.J(\'3M\');F A=8.J(\'3W\');1f(F B=0;B<y;B++){f+=\'<23 K="52">\';1f(F s=0;s<7;s++){F C=(z?z(x):[1c,\'\']);F D=(x.1b()!=8.Q);F E=D||!C[0]||(g&&x<g)||(h&&x>h);f+=\'<22 K="4Z\'+((s+p+6)%7>=5?\' 4Y\':\'\')+(D?\' 4X\':\'\')+(x.1y()==w.1y()?\' 2z\':\'\')+(E?\' 4W\':\'\')+(!D||A?\' \'+C[1]:\'\')+(x.1y()==v.1y()?\' 4T\':(x.1y()==a.1y()?\' 5w\':\'\'))+\'"\'+(E?\'\':\' 5x="$(8).3k(\\\'2z\\\');"\'+\' 5y="$(8).2M(\\\'2z\\\');"\'+\' 1e="G.42(\'+8.R+\', 8);"\')+\'>\'+(D?(A?x.16():\'&3G;\'):(E?x.16():\'<a>\'+x.16()+\'</a>\'))+\'</22>\';x.3o(x.16()+1)}f+=\'</23>\'}f+=\'</3v></3p>\'+(!d&&!8.1G?b:\'\')+\'<V 3N="4R: 2r;"></V>\'+(!$.2d.2D?\'\':\'<!--[H 4Q 5C 6.5]><3n 2v="4O:X;" K="2S"></3n><![4N]-->\');N f},T:I(a,b){F c=P W(8.L+(b==\'Y\'?a:0),8.Q+(b==\'M\'?a:0),8.17+(b==\'D\'?a:0));F d=8.J(\'28\');F e=8.J(\'26\');c=(d&&c<d?d:c);c=(e&&c>e?e:c);8.17=c.16();8.Q=c.1b();8.L=c.15()},2s:I(a,b){N 32-P W(a,b,32).16()},3u:I(a,b){N P W(a,b,1).3V()},2t:I(a){F b=P W(8.L,8.Q+a,1);H(a<0){b.3o(8.2s(b.15(),b.1b()))}N 8.2x(b)},2x:I(a){F b=8.J(\'28\');F c=8.J(\'26\');N((!b||a>=b)&&(!c||a<=c))},3Y:I(){F a=8.1n=8.17;F b=8.1o=8.Q;F c=8.1p=8.L;b++;F d=8.J(\'1R\');F e=\'\';1f(F i=0;i<3;i++){e+=d.1k(3)+(d.1k(i)==\'D\'?(a<10?\'0\':\'\')+a:(d.1k(i)==\'M\'?(b<10?\'0\':\'\')+b:(d.1k(i)==\'Y\'?c:\'?\')))}N e.4M(d.1k(3)?1:0)}});$.4L.3s=I(f){N 8.1D(I(){F a=O;1f(29 4K G.1E){F b=8.4I(\'4H:\'+29);H(b){a=a||{};4G{a[29]=5O(b)}4E(5Q){a[29]=b}}}F c=8.1O.2j();H(c==\'1I\'){F d=(a?$.14($.14({},f||{}),a||{}):f);F e=(e&&!a?e:P 1H(d,X));G.3U(8,e)}19 H(c==\'V\'||c==\'1F\'){F d=$.14($.14({},f||{}),a||{});F e=P 1H(d,1c);G.3w(8,e)}})};$(1d).5S(I(){G=P 2m()});',62,365,'||||||||this|||||||||||||||||||||||||||||||||var|popUpCal|if|function|_get|class|_selectedYear||return|null|new|_selectedMonth|_id|_calendarDiv|_adjustDate|_getInst|div|Date|false||_input||break|||extend|getFullYear|getDate|_selectedDay|css|else|case|getMonth|true|document|onclick|for|_calId|_pos|_disabledInputs|ctrlKey|charAt|_settings|_dialogInput|_currentDay|_currentMonth|_currentYear|hideCalendar|body|calendar_trigger|_popUpShowing|parseInt|_inDialog|_updateCalendar|button|getTime|focus|showFor|length|blockUI|each|_defaults|span|_inline|PopUpCalInstance|input|speed|append|_curInst|_selectingMonthYear|curtop|nodeName|_selectDate|disabled|dateFormat|px|label|documentElement|windowHeight|windowWidth|select|option|selected|img|Math|td|tr|value|top|maxDate||minDate|attrName|type|_selectMonthYear|_clickMonthYear|browser|hidden|_afterShow|regional|_inst|_lastInput|toLowerCase|dayNames|keyCode|PopUpCal|left|clientHeight|_doKeyDown|absolute|both|_getDaysInMonth|_canAdjustMonth|prevText|src|onSelect|_isInRange|_clearDate|calendar_daysCellOver|indexOf|_gotoToday|position|msie|_tidyDialog|self|val|nextText|firstDay|_doKeyPress|yearRange|calendar_prompt|removeClass|changeYear|prompt|changeMonth|clearText|hideIfNoPrevNext|calendar_cover|_generateCalendar|closeAtTop|_showCalendar|buttonImageOnly|offsetTop|_findPos|_setDateFromField|buttonImage|buttonText|||||||||_getDate|_setDate|appendText|currentText|map|autoPopUp|cursor|opacity|jquery|calendar_dialog|addClass|100|clientWidth|iframe|setDate|table|_register|_dialogInst|calendar|_nextId|_getFirstDayOfMonth|tbody|_inlineCalendar|thead|calendar_div|keydown|id|min|title|onchange|alt|_checkExternalClick|nbsp|offsetHeight|prototype|calendar_next|fieldSettings|calendar_prev|customDate|style|monthNames|split|after|closeText|offsetLeft|offsetParent|_connectCalendar|getDay|showOtherMonths|100px|_formatDate|changeFirstDay||innerHeight|_selectDay|charCode|_changeFirstDay|String|options|attr|parents|target|We|remove|selectedIndex|unblockUI|0px|hide|Tu|height|offsetWidth|width|Mo|empty|show|static|Su|parentNode|getDateFor|setDateFor|reconfigureFor|Today|default|disableFor|enableFor|DMY|switch|gt|December|innerWidth|November|Next|catch|setDefaults|try|cal|getAttribute|October|in|fn|substring|endif|javascript|September|lte|clear|size|calendar_currentDay|text|August|calendar_unselectable|calendar_otherMonth|calendar_weekEndCell|calendar_daysCell|July|Prev|calendar_daysRow|dialogCalendar|ceil|mousedown|June|May|lt|calendar_titleRow|cellspacing|cellpadding|keypress|April|calendar_newYear|click|max|calendar_wrap|wrap|March|calendar_newMonth|February|calendar_header|calendar_current|January|Close|calendar_links|calendar_close|calendar_clear|calendar_control|medium|calendar_append|calendar_today|onmouseover|onmouseout|calendar_inline|calendar_div_|while|IE|Sa|nextSibling|noWeekends|Fr|change|trigger|html|undefined|nodeValue|firstChild|fromCharCode|eval|Th|err|Clear|ready'.split('|'),0,{}));


/* Copyright (c) 2007 Paul Bakaus (paul.bakaus@googlemail.com) and Brandon Aaron (brandon.aaron@gmail.com || http://brandonaaron.net)
 * Dual licensed under the MIT (http://www.opensource.org/licenses/mit-license.php)
 * and GPL (http://www.opensource.org/licenses/gpl-license.php) licenses.
 *
 * $LastChangedDate: 2007-08-17 13:14:11 -0500 (Fri, 17 Aug 2007) $
 * $Rev: 2759 $
 *
 * Version: 1.1.2
 *
 * Requires: jQuery 1.1.3+
 */
eval(function(p,a,c,k,e,r){e=function(c){return(c<a?'':e(parseInt(c/a)))+((c=c%a)>35?String.fromCharCode(c+29):c.toString(36))};if(!''.replace(/^/,String)){while(c--)r[e(c)]=k[c]||e(c);k=[function(e){return r[e]}];e=function(){return'\\w+'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}('(9($){l e=$.1q.C,r=$.1q.r;$.1q.M({C:9(){3(!1[0])f();3(1[0]==p)3($.7.O||($.7.E&&U($.7.13)>11))6 n.19-(($(5).C()>n.19)?i():0);k 3($.7.E)6 n.19;k 6 $.I&&5.P.1E||5.o.1E;3(1[0]==5)6 1C.1y(($.I&&5.P.1w||5.o.1w),5.o.1u);6 e.1T(1,1P)},r:9(){3(!1[0])f();3(1[0]==p)3($.7.O||($.7.E&&U($.7.13)>11))6 n.1b-(($(5).r()>n.1b)?i():0);k 3($.7.E)6 n.1b;k 6 $.I&&5.P.1N||5.o.1N;3(1[0]==5)3($.7.1M){l a=n.1p;n.1a(27,n.1o);l b=n.1p;n.1a(a,n.1o);6 5.o.1c+b}k 6 1C.1y((($.I&&!$.7.E)&&5.P.1L||5.o.1L),5.o.1c);6 r.1T(1,1P)},19:9(){3(!1[0])f();6 1[0]==p||1[0]==5?1.C():1.14(\':N\')?1[0].1u-h(1,\'q\')-h(1,\'1I\'):1.C()+h(1,\'1h\')+h(1,\'1H\')},1b:9(){3(!1[0])f();6 1[0]==p||1[0]==5?1.r():1.14(\':N\')?1[0].1c-h(1,\'s\')-h(1,\'1F\'):1.r()+h(1,\'1v\')+h(1,\'1D\')},21:9(a){3(!1[0])f();a=$.M({A:w},a||{});6 1[0]==p||1[0]==5?1.C():1.14(\':N\')?1[0].1u+(a.A?(h(1,\'L\')+h(1,\'1x\')):0):1.C()+h(1,\'q\')+h(1,\'1I\')+h(1,\'1h\')+h(1,\'1H\')+(a.A?(h(1,\'L\')+h(1,\'1x\')):0)},1Y:9(a){3(!1[0])f();a=$.M({A:w},a||{});6 1[0]==p||1[0]==5?1.r():1.14(\':N\')?1[0].1c+(a.A?(h(1,\'K\')+h(1,\'1U\')):0):1.r()+h(1,\'s\')+h(1,\'1F\')+h(1,\'1v\')+h(1,\'1D\')+(a.A?(h(1,\'K\')+h(1,\'1U\')):0)},m:9(a){3(!1[0])f();3(a!=1S)6 1.1Q(9(){3(1==p||1==5)p.1a(a,$(p).u());k 1.m=a});3(1[0]==p||1[0]==5)6 n.1p||$.I&&5.P.m||5.o.m;6 1[0].m},u:9(a){3(!1[0])f();3(a!=1S)6 1.1Q(9(){3(1==p||1==5)p.1a($(p).m(),a);k 1.u=a});3(1[0]==p||1[0]==5)6 n.1o||$.I&&5.P.u||5.o.u;6 1[0].u},12:9(a){6 1.1O({A:w,J:w,v:1.z()},a)},1O:9(b,c){3(!1[0])f();l x=0,y=0,H=0,G=0,8=1[0],4=1[0],T,10,Z=$.D(8,\'12\'),F=$.7.1M,S=$.7.26,18=$.7.O,1n=$.7.E,R=$.7.E&&U($.7.13)>11,1m=w,1l=w,b=$.M({A:Q,15:w,1k:w,J:Q,1K:w,v:5.o},b||{});3(b.1K)6 1.1J(b,c);3(b.v.1j)b.v=b.v[0];3(8.B==\'Y\'){x=8.V;y=8.X;3(F){x+=h(8,\'K\')+(h(8,\'s\')*2);y+=h(8,\'L\')+(h(8,\'q\')*2)}k 3(18){x+=h(8,\'K\');y+=h(8,\'L\')}k 3((S&&1g.I)){x+=h(8,\'s\');y+=h(8,\'q\')}k 3(R){x+=h(8,\'K\')+h(8,\'s\');y+=h(8,\'L\')+h(8,\'q\')}}k{17{10=$.D(4,\'12\');x+=4.V;y+=4.X;3((F&&!4.B.1G(/^t[d|h]$/i))||S||R){x+=h(4,\'s\');y+=h(4,\'q\');3(F&&10==\'1i\')1m=Q;3(S&&10==\'25\')1l=Q}T=4.z||5.o;3(b.J||F){17{3(b.J){H+=4.m;G+=4.u}3(18&&($.D(4,\'24\')||\'\').1G(/23-22|20/)){H=H-((4.m==4.V)?4.m:0);G=G-((4.u==4.X)?4.u:0)}3(F&&4!=8&&$.D(4,\'1e\')!=\'N\'){x+=h(4,\'s\');y+=h(4,\'q\')}4=4.1B}W(4!=T)}4=T;3(4==b.v&&!(4.B==\'Y\'||4.B==\'1d\')){3(F&&4!=8&&$.D(4,\'1e\')!=\'N\'){x+=h(4,\'s\');y+=h(4,\'q\')}3(((1n&&!R)||18)&&10!=\'1r\'){x-=h(T,\'s\');y-=h(T,\'q\')}1A}3(4.B==\'Y\'||4.B==\'1d\'){3(((1n&&!R)||(S&&$.I))&&Z!=\'1i\'&&Z!=\'1z\'){x+=h(4,\'K\');y+=h(4,\'L\')}3(R||(F&&!1m&&Z!=\'1z\')||(S&&Z==\'1r\'&&!1l)){x+=h(4,\'s\');y+=h(4,\'q\')}1A}}W(4)}l a=j(8,b,x,y,H,G);3(c){$.M(c,a);6 1}k{6 a}},1J:9(b,c){3(!1[0])f();l x=0,y=0,H=0,G=0,4=1[0],z,b=$.M({A:Q,15:w,1k:w,J:Q,v:5.o},b||{});3(b.v.1j)b.v=b.v[0];17{x+=4.V;y+=4.X;z=4.z||5.o;3(b.J){17{H+=4.m;G+=4.u;4=4.1B}W(4!=z)}4=z}W(4&&4.B!=\'Y\'&&4.B!=\'1d\'&&4!=b.v);l a=j(1[0],b,x,y,H,G);3(c){$.M(c,a);6 1}k{6 a}},z:9(){3(!1[0])f();l a=1[0].z;W(a&&(a.B!=\'Y\'&&$.D(a,\'12\')==\'1r\'))a=a.z;6 $(a)}});l f=9(){1Z"1X: 1g 1W 14 1V";};l h=9(a,b){6 U($.D(a.1j?a[0]:a,b))||0};l j=9(a,b,x,y,d,c){3(!b.A){x-=h(a,\'K\');y-=h(a,\'L\')}3(b.15&&(($.7.E&&U($.7.13)<11)||$.7.O)){x+=h(a,\'s\');y+=h(a,\'q\')}k 3(!b.15&&!(($.7.E&&U($.7.13)<11)||$.7.O)){x-=h(a,\'s\');y-=h(a,\'q\')}3(b.1k){x+=h(a,\'1v\');y+=h(a,\'1h\')}3(b.J&&(!$.7.O||a.V!=a.m&&a.X!=a.m)){d-=a.m;c-=a.u}6 b.J?{1f:y-c,1t:x-d,u:c,m:d}:{1f:y,1t:x}};l g=0;l i=9(){3(!g){l a=$(\'<1s>\').D({r:16,C:16,1e:\'2d\',12:\'1i\',1f:-1R,1t:-1R}).2c(\'o\');g=16-a.2b(\'<1s>\').2a(\'1s\').D({r:\'16%\',C:29}).r();a.28()}6 g}})(1g);',62,138,'|this||if|parent|document|return|browser|elem|function|||||||||||else|var|scrollLeft|self|body|window|borderTopWidth|width|borderLeftWidth||scrollTop|relativeTo|false|||offsetParent|margin|tagName|height|css|safari|mo|st|sl|boxModel|scroll|marginLeft|marginTop|extend|visible|opera|documentElement|true|sf3|ie|op|parseInt|offsetLeft|while|offsetTop|BODY|elemPos|parPos|520|position|version|is|border|100|do|oa|innerHeight|scrollTo|innerWidth|offsetWidth|HTML|overflow|top|jQuery|paddingTop|absolute|jquery|padding|relparent|absparent|sf|pageYOffset|pageXOffset|fn|static|div|left|offsetHeight|paddingLeft|scrollHeight|marginBottom|max|fixed|break|parentNode|Math|paddingRight|clientHeight|borderRightWidth|match|paddingBottom|borderBottomWidth|offsetLite|lite|scrollWidth|mozilla|clientWidth|offset|arguments|each|1000|undefined|apply|marginRight|empty|collection|Dimensions|outerWidth|throw|inline|outerHeight|row|table|display|relative|msie|99999999|remove|200|find|append|appendTo|auto'.split('|'),0,{}));
/*
 * Copyright (c) 2007 Sam Collett (http://www.texotela.co.uk)
 * Dual licensed under the MIT (http://www.opensource.org/licenses/mit-license.php)
 * and GPL (http://www.opensource.org/licenses/gpl-license.php) licenses.
 * 
 * Extended by Anders Fajerson (http://perifer.se). 
 */
(function($){
  $.fn.timePicker = function(options, clickCallback, focusCallback) {
    var settings = {step:30, startTime:"00:00", endTime:"23:30"};
    $.extend(settings, options);
    
    this.each(function() {
      var elm = this, times = [], tpActive = false;
      var time = $.timePicker._timeStringToDate(settings.startTime);
      var endTime = $.timePicker._timeStringToDate(settings.endTime);
      
      $(elm).attr('autocomplete', 'OFF'); // Disable browser autocomplete
      while(time <= endTime) {
        times[times.length] = $.timePicker._formatTime(time);
        time = new Date(time.setMinutes(time.getMinutes() + settings.step));
      }
      var $tpDiv = $('<div class="time-picker"></div>');
      var $tpList = $('<ul></ul>');
      for(var i = 0; i < times.length; i++) {
        $tpList.append("<li>" + times[i] + "</li>");
      }
      $tpDiv.append($tpList);
      
      $("li", $tpList).hover(function() {
        $(this).siblings().removeClass("selected").end().addClass("selected");
      },function() {
        $(this).removeClass("selected");
      }).mousedown(function() {
         tpActive = true;
      }).click(function() {
        // Update input field
        elm.value = $(this).text();
        // Keep focus for all but IE (which doesn't like it)
        if (!$.browser.msie)
          elm.focus();
        // Remove picker
        $tpDiv.remove();
        tpActive = false;
        // Execute calback function if it's defined
        if (typeof clickCallback == 'function') {
          clickCallback(elm.value);
        }
      });
      
      $(this).focus(function() {
        // Store element offset using dimension plugin
        var elmOffset = $(elm).offset();
        // Remove other time pickers, only needed when close on blur fails.
        $("div.time-picker").remove();
        // Insert and position picker
        $tpDiv.appendTo('body').unbind().css({'top':elmOffset.top, 'left':elmOffset.left})
        .mouseover(function() { // Have to use mouseover instead of mousedown because of Opera
          tpActive = true;
        }).mouseout(function() {
          tpActive = false;
        }); 
        $("li", $tpDiv).removeClass("selected");
        if (this.value) { // This is needed as contains() returns all time lists if input is empty
          var time = $.timePicker._timeStringToDate(this.value);
          var minutes = $.timePicker._timeStringToDate(settings.startTime).getMinutes();
          // Try to find a time in the list that matches the entered time.
          // Todo: this fails with some steps that are not evenly divided to 60 (7,8,9 etc)
          time = new Date(time.setMinutes(Math.round(time.getMinutes() / settings.step) * settings.step + minutes));
          var matchedTime = $("li", $tpDiv).contains($.timePicker._formatTime(time));
          if (matchedTime.length) {
            matchedTime.addClass("selected");
            // Scroll to matched time using dimension plugin
            $tpDiv.scrollTop(matchedTime[0].offsetTop);
          }
        }
        // Execute calback function if it's defined
        if (typeof focusCallback == 'function') {
          focusCallback(elm.value);
        }
      })
      // Remove timepicker on blur
      .blur(function() {
        if (!tpActive && $tpDiv[0].parentNode) { // Don't remove when timePicker is clicked or when already removed
          $tpDiv.remove();
        }
      });
    
    });
    return this;
  };
  
  // Helper functions
  $.timePicker = {
    _formatTime: function(input) {
      if(input && input.constructor == Date) {
        return input.toUTCString().match(/\d{2}:\d{2}/);
      }
      throw "Not a valid date.";
    },
    _timeStringToDate: function(input) {
      var error;
      if(typeof input != "string") {
        error = "A string must be supplied.";
      }
      else if(input.match(/^\d{2}:\d{2}$/)) {
        var s = input.split(":");
        var hours = parseFloat(s[0]);
        var minutes = parseFloat(s[1]);
        minutes += hours * 60;
        var output = new Date();
        output.setTime(minutes * 60 * 1000);
        return output;
      }
      else {
        error = "Not a valid time string - should be in 24 hour format, i.e. 15:00.";
      }
      if(error) {
        throw error;
      }
    }
  }
  
})(jQuery);
