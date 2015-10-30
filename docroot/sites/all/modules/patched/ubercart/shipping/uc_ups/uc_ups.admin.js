/**
 * @file
 * Utility functions to display settings summaries on vertical tabs.
 */

(function ($) {

Drupal.behaviors.upsAdminFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-uc-ups-credentials', context).drupalSetSummary(function(context) {
      var server = $('#edit-uc-ups-connection-address :selected', context).text().toLowerCase();
      return Drupal.t('Using UPS @role server', { '@role': server });
    });

    $('fieldset#edit-uc-ups-markups', context).drupalSetSummary(function(context) {
      return Drupal.t('Rate markup') + ': '
        + $('#edit-uc-ups-rate-markup', context).val() + ' '
        + $('#edit-uc-ups-rate-markup-type', context).val() + '<br />'
        + Drupal.t('Weight markup') + ': '
        + $('#edit-uc-ups-weight-markup', context).val() + ' '
        + $('#edit-uc-ups-weight-markup-type', context).val();
    });

    $('fieldset#edit-uc-ups-quote-options', context).drupalSetSummary(function(context) {
      if ($('#edit-uc-ups-insurance').is(':checked')) {
        return Drupal.t('Packages are insured');
      }
      else {
        return Drupal.t('Packages are not insured');
      }
    });

  }
};

})(jQuery);
