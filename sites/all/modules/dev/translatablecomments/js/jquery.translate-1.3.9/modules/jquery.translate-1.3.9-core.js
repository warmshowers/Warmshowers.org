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