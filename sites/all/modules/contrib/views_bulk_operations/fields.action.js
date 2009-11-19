// $Id: fields.action.js,v 1.1.2.2 2009/03/11 01:10:30 kratib Exp $
(function ($) {
// START jQuery

Drupal.fieldsAction = Drupal.fieldsAction || {};

Drupal.fieldsAction.updateToggler = function(toggler) {
  var parent = $(toggler).parents('tr')[0];
  if ($(toggler).is(':checked')) {
    $('.fields-action-togglable :input', parent).removeAttr('disabled');
  }
  else {
    $('.fields-action-togglable :input', parent).attr('disabled', true);
  }
}

$(document).ready(function() {
  $('.fields-action-toggler').click(function() {
    Drupal.fieldsAction.updateToggler(this);
  });

  $('th.select-all').click(function() {
    $('.fields-action-toggler').each(function() {
      Drupal.fieldsAction.updateToggler(this);
    });
  });
  
  // Disable all by default.
  $('.fields-action-togglable :input').attr('disabled', true);
});

// END jQuery
})(jQuery);

