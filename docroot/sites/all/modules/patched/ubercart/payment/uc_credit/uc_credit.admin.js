/**
 * @file
 * Utility functions to display settings summaries on vertical tabs.
 */

(function ($) {

Drupal.behaviors.creditAdminFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-cc-security', context).drupalSetSummary(function(context) {
      return Drupal.t('Encryption key path') + ': '
        + $('#edit-uc-credit-encryption-path', context).val();
    });

  }
};

})(jQuery);
