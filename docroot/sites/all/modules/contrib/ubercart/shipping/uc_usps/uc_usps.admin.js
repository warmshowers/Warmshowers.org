/**
 * @file
 * Utility functions to display settings summaries on vertical tabs.
 */

(function ($) {

Drupal.behaviors.uspsAdminFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-domestic', context).drupalSetSummary(function(context) {
      if ($('#edit-uc-usps-online-rates').is(':checked')) {
        return Drupal.t('Using "online" rates');
      }
      else {
        return Drupal.t('Using standard rates');
      }
    });

    $('fieldset#edit-uc-usps-markups', context).drupalSetSummary(function(context) {
      return Drupal.t('Rate markup') + ': '
        + $('#edit-uc-usps-rate-markup', context).val() + ' '
        + $('#edit-uc-usps-rate-markup-type', context).val() + '<br />'
        + Drupal.t('Weight markup') + ': '
        + $('#edit-uc-usps-weight-markup', context).val() + ' '
        + $('#edit-uc-usps-weight-markup-type', context).val();
    });
  }
};

})(jQuery);
