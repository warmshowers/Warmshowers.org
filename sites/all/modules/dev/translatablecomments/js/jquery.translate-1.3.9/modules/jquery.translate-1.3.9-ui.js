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