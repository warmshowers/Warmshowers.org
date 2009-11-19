// $Id: better_messages.js,v 1.1.2.8 2009/10/07 12:22:21 doublethink Exp $

if (Drupal.jsEnabled) {	
	Drupal.behaviors.betterMessages = function (context) {
		var betterMessages = Drupal.settings.betterMessages; var message_box = $('#better-messages-wrapper');
		if (!message_box.hasClass("better-messeges-processed")) {
			message_box.css('width', betterMessages.width);
			betterMessages.fixed == '1' && !($.browser.msie && $.browser.version == '6.0') ?
				message_box.css({"position":"fixed"}) : message_box.css({"position":"absolute"});
			/* Functions to determine the popin/popout animation */
			betterMessages.open = function() {
				switch (betterMessages.popin.effect) {
					case 'fadeIn': message_box.fadeIn(betterMessages.popin.duration);
						break;
					case 'slideDown': message_box.slideDown(betterMessages.popin.duration);
						break;
					default: message_box.fadeIn(betterMessages.popin.duration);
						break;
				}
			}
			betterMessages.close = function() {
				switch (betterMessages.popout.effect) {
					case 'fadeOut':	message_box.fadeOut(betterMessages.popout.duration);
						break;
					case 'slideUp':	message_box.slideUp(betterMessages.popout.duration);
						break;
					default: message_box.fadeOut(betterMessages.popout.duration);
						break;
				}
				message_box.addClass("better-messeges-processed");
			}
			var vertical = betterMessages.vertical;	var horizontal = betterMessages.horizontal;
			switch (betterMessages.position) {
				case 'center':
					vertical = ( $(window).height() - message_box.height() ) / 2;
					horizontal = ( $(window).width() - message_box.width() ) / 2;
					message_box.css({"top":vertical + 'px', "left":horizontal + 'px'});
					break;
				case 'tl':
					message_box.css({"top":vertical + 'px', "left":horizontal + 'px'});
					break;
				case 'tr':
					message_box.css({"top":vertical + 'px', "right":horizontal + 'px'});
					break;
				case 'bl':
					message_box.css({"bottom":vertical + 'px', "left":horizontal + 'px'});
					break;
				case 'br':
					message_box.css({"bottom":vertical + 'px', "right":horizontal + 'px'});
					break;
			}
			/* Here we set the time to popin, or popout */
			if (betterMessages.opendelay != 0) 
				setTimeout( function() {betterMessages.open()}, betterMessages.opendelay * 1000 );
			else betterMessages.open();
			if (betterMessages.autoclose != 0)
				setTimeout( function() {betterMessages.close()}, betterMessages.autoclose * 1000 );
			$('a.message-close').click(function() {
				betterMessages.close();
				return false;
			});
			/* Esc key handler for closing the message. This doesn't work on Safari or Chrome
			   See the issue here: http://code.google.com/p/chromium/issues/detail?id=14635
			*/
			$(document).keypress(function(e){
				if(e.keyCode==27){  
					betterMessages.close();
					return false; 
				}
			});
		}
	}
}
