
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
      }
    }
    $('body:not(.environment-indicator-processed)', context).addClass('environment-indicator-processed');
  }
};

})(jQuery);
