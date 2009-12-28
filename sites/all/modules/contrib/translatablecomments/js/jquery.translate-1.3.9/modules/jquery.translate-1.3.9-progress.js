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