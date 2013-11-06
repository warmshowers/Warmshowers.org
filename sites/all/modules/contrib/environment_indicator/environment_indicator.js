
/**
 * @file
 * Environment info JavaScript.
 *
 * @author Tom Kirkpatrick (mrfelton), www.kirkdesigns.co.uk
 */
 
(function ($) {

Drupal.environmentIndicator = Drupal.environmentIndicator || {};

/**
 * Core behavior for Environment Indicator.
 *
 * Test whether there is an environment indicator in the output and execute all
 * registered behaviors.
 */
Drupal.behaviors.environmentIndicator = function (context) {
  // Initialize settings.
  Drupal.settings.environment_indicator = $.extend({
    text: ' ',
    color: '#d00c0c',
    suppress: false,
    margin: false,
    position: 'left'
  }, Drupal.settings.environment_indicator || {});
  
  // Check whether environment indicator strip menu should be suppressed.
  if (Drupal.settings.environment_indicator.suppress) {
    return;
  };
  
  if ($('body:not(.environment-indicator-processed)', context).length) {
    Drupal.settings.environment_indicator.cssClass = 'environment-indicator-' + Drupal.settings.environment_indicator.position;
    
    // If we don't have an environment indicator, inject it into the document.
    var $environmentIndicator = $('#environment-indicator', context);
    if (!$environmentIndicator.length) {
      $('body', context).prepend('<div id="environment-indicator">' + Drupal.settings.environment_indicator.text + '</div>');
      $('body', context).addClass(Drupal.settings.environment_indicator.cssClass);
      
      // Set the colour.
      var $environmentIndicator = $('#environment-indicator', context);
      $environmentIndicator.css('background-color', Drupal.settings.environment_indicator.color);
      
      // Make the text appear vertically
      $environmentIndicator.html($environmentIndicator.text().replace(/(.)/g,"$1<br />"));
      
      // Adjust the margin.
      if (Drupal.settings.environment_indicator.margin) {
        $('body:not(.environment-indicator-adjust)', context).addClass('environment-indicator-adjust');
        
        // Adjust the background position
        //var pos = $("body").backgroundPosition();
        //$("body").css({backgroundPosition: pos[0]+30 + 'px ' + pos[1] + 'px'});
        
        // Adjust the width of the admin-menu
        if ($("#admin-menu").length) {
          $("#admin-menu").css('margin-'+Drupal.settings.environment_indicator.position, '30px');
          $("#admin-menu").css('width', $("#admin-menu").width() - 30 +'px');
          $(window).resize(Drupal.environmentIndicator.stretchAdminMenu);
        }
      }
    }
    $('body:not(.environment-indicator-processed)', context).addClass('environment-indicator-processed');
  }
};

/**
 * Stretch the admin menu.
 */
Drupal.environmentIndicator.stretchAdminMenu = function () {
  if (Drupal.settings.environment_indicator.margin) {
    $("#admin-menu").css('width', $("body").width() +'px');
  };
};

/**
 * Utility function to get the x and y attributes of background position.
 */
$.fn.backgroundPosition = function() {
  var p = $(this).css('background-position');
  if(typeof(p) === 'undefined') {
    p = $(this).css('background-position-x') + ' ' + $(this).css('background-position-y');
  }
  var posX = p.split(' ')[0].replace(/px/,'');
  var posY = p.split(' ')[1].replace(/px/,'');
  return [posX, posY];
};

})(jQuery);
