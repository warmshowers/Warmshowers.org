Drupal.behaviors.langDropdownAdmin = function(context) {
  var wrapper = $('#lang-dropdown-js-widget-settings-wrapper');
	($('input#edit-js-widget:checked').length > 0) ? wrapper.show() : wrapper.css('display', 'none');
	$('input#edit-js-widget').click(function() {
		($(this).is(':checked')) ? wrapper.slideDown() : wrapper.slideUp();
	});
};