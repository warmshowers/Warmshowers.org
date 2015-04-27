(function ($) {

/**
 * Update the summary for nodeblock's vertical tab.
 */
Drupal.behaviors.nodeblock_fieldsetSummary = {
  attach: function (context) {
    // Use the fieldset id attribute to identify the vertical tab element.
    $('fieldset#edit-nodeblock-settings', context).drupalSetSummary(function (context) {
      return $('input[name=nodeblock]:checked + label').text();
    });
  }
};
})(jQuery);
