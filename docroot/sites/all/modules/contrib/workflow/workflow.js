(function ($) {

/**
 * Custom summary for the module vertical tab.
 */
Drupal.behaviors.workflow_node_formFieldsetSummaries = {
  attach: function (context) {
    // Use the fieldset id to identify the vertical tab element
    $('fieldset#edit-workflow', context).drupalSetSummary(function (context) {
      return Drupal.checkPlain($('.form-item-workflow-scheduled input:checked', context).next('label').text());
    });
  }
};

})(jQuery);
