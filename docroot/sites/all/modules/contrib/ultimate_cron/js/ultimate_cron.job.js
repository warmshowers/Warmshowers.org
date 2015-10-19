
(function ($) {

Drupal.behaviors.ultimateCronJobFieldsetSummaries = {
  attach: function (context) {
    $('#edit-settings-scheduler', context).drupalSetSummary(function (context) {
      return $('#edit-settings-scheduler-name', context).find(":selected").text();
    });
    $('#edit-settings-launcher', context).drupalSetSummary(function (context) {
      return $('#edit-settings-launcher-name', context).find(":selected").text();
    });
    $('#edit-settings-logger', context).drupalSetSummary(function (context) {
      return $('#edit-settings-logger-name', context).find(":selected").text();
    });
  }
};

})(jQuery);
