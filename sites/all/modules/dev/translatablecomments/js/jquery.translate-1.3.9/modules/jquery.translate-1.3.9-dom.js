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