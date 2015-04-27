
(function($) {
	Drupal.behaviors.betterMessages = {
		attach: function (context) {
			var betterMessages = Drupal.settings.betterMessages; 
			var message_box = $('#better-messages-wrapper');
		
			/* jQuery UI Enhancements */
			if (betterMessages.jquery_ui != null) {
				if (betterMessages.jquery_ui.draggable == '1') { message_box.draggable(); }
			}
		
			/* Popup Message handling */
			if (!message_box.hasClass("better-messeges-processed")) {			
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
				/* Function to determine closing count */
				betterMessages.countDownClose = function(seconds) {
					if(seconds > 0) {
						seconds--;
						if (betterMessages.show_countdown == '1') {
              $('.message-timer').text(Drupal.t('Closing in !seconds seconds', {'!seconds': seconds}));
						}
			      if(seconds > 0) {
			      	betterMessages.countDown = setTimeout( function() {betterMessages.countDownClose(seconds);}, 1000 );
			      }
			      else {
							betterMessages.close();
						}
					}
				}
			
				/* Determine Popup Message position */
				message_box.css('width', betterMessages.width);
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
			
				/* Here we control closing and opeing effects and controls */
				if (betterMessages.opendelay != 0) { 
					setTimeout( function() {betterMessages.open()}, betterMessages.opendelay * 1000 );
				} else { betterMessages.open(); }
				if (betterMessages.autoclose != 0) {
					betterMessages.countDownClose(betterMessages.autoclose);
				}
				if (betterMessages.hover_autoclose == '1') {
					message_box.hover(function() {
						clearTimeout(betterMessages.countDown);
						$('.message-timer').fadeOut('slow');
						}, function() {
							/* Suggest something to do here! */
						}
					);
				}
				$('a.message-close').click(function() { betterMessages.close();	return false; });
				/* Esc key handler for closing the message. This doesn't work on Safari or Chrome
					 See the issue here: http://code.google.com/p/chromium/issues/detail?id=14635
				*/
				$(document).keypress(function(e){
					if(e.keyCode==27){  
						betterMessages.close();
						return false; 
					}
				});
			
				/* Determine Popup Message position for IE6 bug with fixed display */
				if (betterMessages.fixed == '1' && !($.browser.msie && $.browser.version == '6.0')) {
					message_box.css({"position":"fixed"});
				}
				else { /* IE6 handing */
					message_box.css({"position":"absolute"});
					$(window).scroll(function() { message_box.stop().css({top:($(window).scrollTop() + vertical) + 'px'});});
				}
			}
		}
	}
})(jQuery);
