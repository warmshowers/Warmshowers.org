/*! jQuery Translate plugin and related components */

/*! 
 * jQuery Translate plugin 
 * 
 * Version: 1.3.9
 * 
 * http://code.google.com/p/jquery-translate/
 * 
 * Copyright (c) 2009 Balazs Endresz (balazs.endresz@gmail.com)
 * Dual licensed under the MIT (http://www.opensource.org/licenses/mit-license.php)
 * and GPL (http://www.opensource.org/licenses/gpl-license.php) licenses.
 * 
 * This plugin uses the 'Google AJAX Language API' (http://code.google.com/apis/ajaxlanguage/)
 * You can read the terms of use at http://code.google.com/apis/ajaxlanguage/terms.html
 * 
 */
;(function($){

var undefined, GL, GLL, isReady = false, loading = false, readyList = [];

function loaded(){
	GL = T.GL = google.language;
	GLL = GL.Languages;
	isReady = true;
	var fn;
	while(fn = readyList.shift()) fn();
}

function $fn(){}

function T(){
	//copy over static methods during each instantiation
	//for backward compatibility and access inside callback functions
	this.extend($.translate);
	delete this.defaults;
	delete this.fn;
}

T.prototype={
	version: "1.3.9",
	
	translateInit: function(t, o){ 
		var that = this;
		this.options = o;
		o.from = this.toLanguageCode(o.from) || "";
		o.to = this.toLanguageCode(o.to) || "";
		
		if(o.fromOriginal && o.nodes[0]){
			o.nodes.each(function(i){
				var data = $.translate.getData(this, o.from, o);
				if( !data ) return false;
				t[i] = data;
			});
		}
		
		if(typeof t === "string"){
			if(!o.comments)
				t = this.stripComments(t);
			this.rawSource = '<div>' + t + '</div>';
			this.isString = true;
		}else{
			if(!o.comments)
				t = $.map(t, function(e){ return $.translate.stripComments(e); });
			this.rawSource = '<div>' + t.join('</div><div>') + '</div>';
			this.isString = false;
		}
		
		this.from = o.from;
		this.to = o.to;
		this.source = t;
		this.elements = o.nodes;
		this.rawTranslation = '';
		this.translation = [];
		this.startPos = 0;
		this.i = 0;
		this.stopped = false;
		
		o.start.call(this, o.nodes[0] ? o.nodes : t , o.from, o.to, o);
		
		if(o.timeout>0){
			this.timeout = setTimeout(function(){
				o.onTimeout.call(that, o.nodes[0] ? o.nodes : t, o.from, o.to, o);
			}, o.timeout);
		}
		
		(o.toggle && o.nodes[0]) ? this._toggle() : this.translate();
		return this;
	},
	
	translate: function(){
		if(this.stopped)
			return;
		var that = this, o = this.options;

		this.rawSourceSub = this.truncate( this.rawSource.substr(this.startPos), 1750);
		this.startPos += this.rawSourceSub.length;
		
		//---------handle each callbacks as transl arrived----------
		var i = this.rawTranslation.length, lastpos;

		while( (lastpos = this.rawTranslation.lastIndexOf("</div>", i)) > -1){

			i = lastpos - 1;
		
			var subst = this.rawTranslation.substr(0, i + 1),			
				divst = subst.match(/<div[> ]/gi),
				divcl = subst.match(/<\/div>/gi);

			divst = divst ? divst.length : 0;
			divcl = divcl ? divcl.length : 0;
	
			if(divst != divcl + 1) continue; //if there are some unclosed divs

			var divscompl = $( this.rawTranslation.substr(0, i + 7) ), 
				divlen = divscompl.length, 
				l = this.i;
			
			if(l == divlen) break; //if no new elements have been completely translated

			divscompl.slice(l, divlen).each(function(j, e){ (function(){
				if(this.stopped)
					return false;
				var tr = $(e).html().replace(/^\s/,""), i = l + j, src = this.source,
					from = this.from.length < 2 && this.detectedSourceLanguage || this.from;
				this.translation[i] = tr;//create an array for complete callback

				if(!o.nodes[0]){//called from function
					if(this.isString)
						this.translation = tr;
					else
						src = this.source[i];
					o.each.call(this, i, tr, src, from, this.to, o);
				}else{//called from method
					this.each(i, this.elements[i], tr, this.source[i], from, this.to, o);
					o.each.call(this, i, this.elements[i], tr, this.source[i], from, this.to, o);
				}
				this.i++;
			}).call(that); });
			
			break;
		}
	
		//---------translate one part of text-----------
		if(this.rawSourceSub.length > 0){
			GL.translate(this.rawSourceSub, this.from, this.to, function(result){ (function(){
				if(result.error)
					return o.error.call(this, result.error, this.rawSourceSub, this.from, this.to, o);
		
				this.rawTranslation += result.translation || this.rawSourceSub;
				this.detectedSourceLanguage = result.detectedSourceLanguage;
					
				this.translate();
			}).call(that); });
			
			if(!o.nodes[0])
				return;
		}else{
	
		//------------translation complete------------
			
			if(!this.rawTranslation)
				return;
	
			var from = this.from.length < 2 && this.detectedSourceLanguage || this.from;
			
			if(this.timeout)
				clearTimeout(this.timeout);
	
			if(!o.nodes[0]){//called from function
				o.complete.call(this, this.translation, this.source, from, this.to, o);
			}else
				o.complete.call(this, this.elements.end(), this.elements, this.translation, this.source, from, this.to, o);
		}
	},
	
	stop: function(){
		if(this.stopped)
			return this;
		this.stopped = true;
		this.options.error.call(this, {message:"stopped"});
		return this;
	}

};



$.translate = function(t, a, b, c){
	if(t == undefined)
		return new T();
	if( $.isFunction(t) )
		return $.translate.ready(t, a);
	var that = new T();
	return $.translate.ready( function(){ return that.translateInit( t, $.translate._getOpt(a, b, c) );  }, false, that );
};

$.translate.fn = $.translate.prototype = T.prototype;

$.translate.fn.extend = $.translate.extend = $.extend;



$.translate.extend({
	stripComments: function(t){ return t.replace(/<![ \r\n\t]*(--([^\-]|[\r\n]|-[^\-])*--[ \r\n\t]*)>/g, ''); },
	
	truncate: function(text, limit){
		var i, m1, m2, m3, m4, t, encoded = encodeURIComponent( text );
		
		for(i = 0; i < 10; i++){
			try {
				t = decodeURIComponent( encoded.substr(0, limit - i) );
			} catch(e){ continue; }
			if(t) break;
		}
		
		return ( !( m1 = /<(?![^<]*>)/.exec(t) ) ) ? (  //if no broken tag present
			( !( m2 = />\s*$/.exec(t) ) ) ? (  //if doesn't end with '>'
				( m3 = /[\.\?\!;:](?![^\.\?\!;:]*[\.\?\!;:])/.exec(t) ) ? (  //if broken sentence present
					( m4 = />(?![^>]*<)/.exec(t) ) ? ( 
						m3.index > m4.index ? t.substring(0, m3.index+1) : t.substring(0, m4.index+1)
					) : t.substring(0, m3.index+1) ) : t ) : t ) : t.substring(0, m1.index);
	},

	getLanguages: function(a, b){
		if(a == undefined || (b == undefined && !a))
			return GLL;
		
		var nowObj = {}, filter = b, languages = GLL;
			
		if(b)
			languages = $.translate.getLanguages(a);
		else if (typeof a === "object")
			filter = a;
		
		if(filter)
        	for(var i = 0, length = filter.length, lc, l; i < length; i++){
				lc = $.translate.toLanguageCode(filter[i]);
				for (l in languages)
					if( lc === languages[l] )
						nowObj[l] = languages[l];
			}
		else
			for (var l in GLL)
				if(GL.isTranslatable(GLL[l]))
					nowObj[l] = GLL[l];
		
		return nowObj;
	},
	
	toLanguage: function(a, format){
		for(var l in GLL)
			if( a === l || a === GLL[l] || a.toUpperCase() === l || a.toLowerCase() === GLL[l].toLowerCase() )
				return format === "lowercase" ? l.toLowerCase() : format === "capitalize" ? 
					l.charAt(0).toUpperCase() + l.substr(1).toLowerCase() : l;
	},
	
	toLanguageCode: function(a){
		return GLL.a || GLL[ $.translate.toLanguage(a) ];
	},
		
	same: function(a, b){
		return a === b || $.translate.toLanguageCode(a) === $.translate.toLanguageCode(b);
	},
		
	isTranslatable: function(l){
		return GL.isTranslatable( $.translate.toLanguageCode(l) );
	},
	
	getBranding: function(a, b, c){ return $( GL.getBranding(a, b, c) ); },
	
	load: function(key, api, version){
		loading = true;
		function load(){ google.load(api || "language", version || "1", {"callback" : loaded}); }
		
		(typeof google !== "undefined" && google.load) ? load() :
			$.getScript("http://www.google.com/jsapi?" + (key ? "key=" + key : ""), load);
		return $.translate;
	},
	
	ready: function(fn, preventAutoload, that){
		isReady ? fn() : readyList.push(fn);
		if(!loading && !preventAutoload)
			$.translate.load();
		return that || $.translate;
	},
	
	_getOpt: function (a, b, c, method){
		var from, to, o = {};
		if(typeof a === "object"){o = a;}else{
			if(!b && !c) to = a;
			if(!c && b){ if(typeof b === "object"){ to = a; o = b; }else{ from = a; to = b; } }
			if(a != undefined && b && c){ from = a; to = b; o = c; }
			o.from = from || o.from || '';
			o.to = to || o.to || '';
		}
		if(o.fromOriginal) o.toggle = true;
		if(o.toggle) o.data = true;
		if(o.async === true) o.async = 2;

		return $.extend({}, $.translate._defaults, (method ? $.fn.translate.defaults : $.translate.defaults), o);
	},

	_defaults: {
		comments: false,
		start: $fn,
		error: $fn,
		each: $fn,
		complete: $fn,
		onTimeout: $fn,
		timeout: 0,
		from: '',
		to: '',
		nodes: [], //internal
		walk: true,
		returnAll: false,
		replace: true,
		rebind: true,
		data: true,
		setLangAttr: false,
		subject: true,
		not: '',
		altAndVal:true,
		async: false,
		toggle: false,
		fromOriginal: false
	}

});

$.translate.defaults = $.extend({}, $.translate._defaults);

})(jQuery);

/*! 
 * DOM extension for the jQuery Translate plugin 
 * Version: 1.3.9
 * http://code.google.com/p/jquery-translate/
 */
;(function($){

var isInput = {text:true, button:true, submit:true};

function toggleDir(e, dir){
	var align = e.css("text-align");
	e.css("direction", dir);
	if(align === "right") e.css("text-align", "left");
	if(align === "left") e.css("text-align", "right");
}

function getType(el, o){
	var nodeName = el.nodeName.toUpperCase(),
		type = nodeName === 'INPUT' && $.attr(el, 'type').toLowerCase();
	return typeof o.subject === "string" ? o.subject :
		o.altAndVal && (nodeName === 'IMG' || type === "image" )  ? "alt" :
		o.altAndVal && isInput[ type ] ? "value" :
		nodeName === "TEXTAREA" ? "value" : "html";
}


$.translate.fn._toggle = function(){
	var that = this, o = this.options, el = o.nodes, to = o.to, stop = false;
	
	el.each(function(i){
		that.i = i;
		var e = $(this), tr = that.getData(this, to, o);
		
		if(!tr) return !(stop = true);
		
		that.translation.push(tr);
		that.setLangAttr(e, to, o);
		that.replace(e, tr, o);

		o.each.call(that, i, that.elements[i], tr, that.source[i], that.from, to, o);
		//'from' will be undefined if it wasn't set
	});
	
	!stop ? o.complete.call(this, el.end(), el, that.translation, this.source, this.from, this.to, o) : this.translate();
}



// now these are internal methods but might be exposed sometime after some refactoring:

$.translate.extend({
	isRtl: {"ar":true, "he":true, "iw":true, "fa":true, "ur":true},
	
	each: function(i, el, t, s, from, to, o){
		var e = $(el);
		$.translate.setData(el, t, s, from, to, o);
		$.translate.replace(e, t, o);
		$.translate.setLangAttr(e, to, o);
	},
	
	getData: function(el, lang, o){
		var data = $.data(el, "translation");
		return data && data[lang] && data[lang][ getType(el, o) ];
	},
	
	setData: function(el, t, s, from, to, o){
		if(!o.data) return;
		
		var type = getType(el, o),
			data = $.data(el, "translation");
		
		data = data || $.data(el, "translation", {});
		(data[from] = data[from] || {})[type] = s;
		(data[to] = data[to] || {})[type] = t;
		
		//deprecated:
		$.data(el, "translation." + from + "." + type, s);
		$.data(el, "translation." + to   + "." + type, t);
	},
	
	
	replace: function(e, t, o){
		if(!o.replace) return;
		
		if(typeof o.subject === "string")
			return e.attr(o.subject, t);

		var el = e[0], 
			nodeName = el.nodeName.toUpperCase(),
			type = nodeName === 'INPUT' && $.attr(el, 'type').toLowerCase(),
			isRtl = $.translate.isRtl,
			lang = $.data(el, "lang");
		
		if( lang === o.to )
			return;
		
		if( isRtl[ o.to ] !== isRtl[ lang || o.from ] ){
			if( isRtl[ o.to ] )
				toggleDir(e, "rtl");
			else if( e.css("direction") === "rtl" )
				toggleDir(e, "ltr");
		}
				
		if( o.altAndVal && (nodeName === 'IMG' || type === "image" ) )
			e.attr("alt", t);
		else if( nodeName === "TEXTAREA" || o.altAndVal && isInput[ type ] )
			e.val(t);
		else{
			if(o.rebind){
				var origContents = e.find("*").not("script"), newElem = $("<div/>").html(t);
				$.translate.copyEvents( origContents, newElem.find("*") );
				e.html( newElem.contents() );
			}else
				e.html(t);
		}
		
		//used for determining if the text-align property should be changed,
		//it's much faster than setting the "lang" attribute, see bug #13
		$.data(el, "lang", o.to);
	},
	
	setLangAttr: function(e, to, o){	
		if(o.setLangAttr)
			e.attr(o.setLangAttr === true ? "lang" : o.setLangAttr, to);
	},

	copyEvents: function(from, to){
		to.each(function(i){
			var from_i = from[i];
			if( !this || !from_i ) //in some rare cases the translated html structure can be slightly different
				return false;
			if( ({SCRIPT:1, NOSCRIPT:1, STYLE:1, OBJECT:1, IFRAME:1})[ from_i.nodeName.toUpperCase() ])
				return true;
			var events = $.data(from_i, "events");
			if(!events)
				return true;
			for(var type in events)
				for(var handler in events[type])
					$.event.add(this, type, events[type][handler], events[type][handler].data);
		});
	}
	
});


$.fn.translate = function(a, b, c){
	var o = $.translate._getOpt(a, b, c, true),
		ncto = $.extend( {}, $.translate._defaults, $.fn.translate.defaults, o,
			{ complete:function(e,t){ o.nodes = e; $.translate(t, o); }, each: function(){} } );

	if(this.nodesContainingText)
		return this.nodesContainingText(ncto);
	
	//fallback if nodesContainingText method is not present:
	o.nodes = this;
	$.translate($.map(this, function(e){ return $(e).html() || $(e).val(); }), o);
	return this;
};

$.fn.translate.defaults = $.extend({}, $.translate._defaults);

})(jQuery);

/*! 
 * Simple user interface extension for the jQuery Translate plugin 
 * Version: 1.3.9
 * http://code.google.com/p/jquery-translate/
 */
;(function($){
$.translate.ui = $.translate.fn.ui = function(a, b, c){
	var str='', cs='', cl='';
	if(c){ cs='<'+c+'>'; cl='</'+c+'>'; }
	$.each( $.translate.getLanguages(true), function(l, lc){
		str+=('<'+b+'>'+cs+l.charAt(0)+l.substring(1).toLowerCase()+cl+'</'+b+'>');
	});
	return $('<'+a+' class="jq-translate-ui">'+str+'</'+a+'>');
}
})(jQuery);

/*! 
 * Progress indicator extension for the jQuery Translate plugin 
 * Version: 1.3.9
 * http://code.google.com/p/jquery-translate/
 */

;(function($){
$.translate.fn.progress = function(selector, options){
	if(!this.i) this.pr = 0;
	this.pr += this.source[this.i].length;
	var progress = 100 * this.pr / ( this.rawSource.length - ( 11 * (this.i + 1) ) );

	if(selector){
		var e = $(selector);
		if( !this.i && !e.hasClass("ui-progressbar") )
			e.progressbar(options)
		e.progressbar( "option", "value", progress );
		//e.progressbar( "progress", progress );
	}
	
	return progress;
}
})(jQuery);

/*! 
 * jQuery nodesContainingText plugin 
 * 
 * Version: 1.1.0
 * 
 * http://code.google.com/p/jquery-translate/
 * 
 * Copyright (c) 2009 Balazs Endresz (balazs.endresz@gmail.com)
 * Dual licensed under the MIT (http://www.opensource.org/licenses/mit-license.php) 
 * and GPL (http://www.opensource.org/licenses/gpl-license.php) licenses.
 * 
 */
 
;(function($){

function Nct(){}

Nct.prototype={
	init: function(jq, o){
		this.textArray = [];
		this.elements = [];
		this.options = o;
		this.jquery = jq;
		this.n = -1;
		if(o.async === true) o.async = 2;
		
		if(o.not){
			jq = jq.not(o.not);
			jq = jq.add( jq.find("*").not(o.not) ).not( $(o.not).find("*") );
		}else{
			jq = jq.add( jq.find("*") );
		}
		this.jq = jq;
		this.jql = this.jq.length;
		return this.process();

	},

	process: function(){
		this.n++;
		var that = this, o = this.options, text = "", hasTextNode = false,
			hasChildNode = false, el = this.jq[this.n], e, c, ret;
		
		if(this.n==this.jql){
			ret = this.jquery.pushStack(this.elements, "nodesContainingText");
			o.complete.call(ret, ret, this.textArray);
			
			if(o.returnAll === false && o.walk === false)
				return this.jquery;
			return ret;
		}
		
		if(!el)
			return this.process();
		e=$(el);

		var nodeName = el.nodeName.toUpperCase(),
			type = nodeName === "INPUT" && $.attr(el, "type").toLowerCase();
		
		if( ({SCRIPT:1, NOSCRIPT:1, STYLE:1, OBJECT:1, IFRAME:1})[ nodeName ] )
			return this.process();
		
		if(typeof o.subject === "string"){
			text=e.attr(o.subject);
		}else{	
			if(o.altAndVal && (nodeName === "IMG" || type === "image" ) )
				text = e.attr("alt");
			else if( o.altAndVal && ({text:1, button:1, submit:1})[ type ] )
				text = e.val();
			else if(nodeName === "TEXTAREA")
				text = e.val();
			else{
				//check childNodes:
				c=el.firstChild;
				if(o.walk !== true)
					hasChildNode = true;
				else{
					while(c){
						if(c.nodeType == 1){
							hasChildNode = true;
							break;
						}
						c=c.nextSibling;
					}
				}

				if(!hasChildNode)
					text = e.text();
				else{//check textNodes:
					if(o.walk !== true)
						hasTextNode = true;
					
					c=el.firstChild;
					while(c){
						if(c.nodeType==3 && c.nodeValue.match(/\S/) !== null){//textnodes with text
							if(c.nodeValue.match(/<![ \r\n\t]*(--([^\-]|[\r\n]|-[^\-])*--[ \r\n\t]*)>/) !== null){
								if(c.nodeValue.match(/(\S+(?=.*<))|(>(?=.*\S+))/) !== null){
									hasTextNode = true;
									break;
								}
							}else{
								hasTextNode = true;
								break;
							}
						}
						c = c.nextSibling;
					}

					if(hasTextNode){//remove child nodes from jq
						//remove scripts:
						text = e.html().replace(/<script[^>]*>([\s\S]*?)<\/script>/gi, "");
						this.jq = this.jq.not( e.find("*") );
					}
				}
			}
		}

		if(!text)
			return this.process();
		this.elements.push(el);
		if(o.comments === false)
			text = this.stripComments(text);
		this.textArray.push(text);

		o.each.call(el, this.elements.length - 1, el, text);
		
		if(o.async){
			setTimeout(function(){that.process();}, o.async);
			return this.jquery;
		}else
			return this.process();
		
	},

	stripComments: function(t){ return t.replace(/<![ \r\n\t]*(--([^\-]|[\r\n]|-[^\-])*--[ \r\n\t]*)>/g, ""); }

}

$.fn.nodesContainingText = function(o){
	o = $.extend({}, defaults, $.fn.nodesContainingText.defaults, o);
	return new Nct().init(this, o);
}

var defaults = {
	not: "",
	async: false,
	each: function(){},
	complete: function(){},
	comments: false,
	returnAll: true,
	walk: true,
	altAndVal: false,
	subject: true
}
	
$.fn.nodesContainingText.defaults = defaults;

})(jQuery);