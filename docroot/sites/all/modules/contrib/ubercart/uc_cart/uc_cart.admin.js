/**
 * @file
 * Utility functions to display settings summaries on vertical tabs.
 */

(function ($) {

Drupal.behaviors.ucCartAdminFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-lifetime', context).drupalSetSummary(function(context) {
      return Drupal.t('Anonymous users') + ': '
        + $('#edit-uc-cart-anon-duration', context).val() + ' '
        + $('#edit-uc-cart-anon-unit', context).val() + '<br />'
        + Drupal.t('Authenticated users') + ': '
        + $('#edit-uc-cart-auth-duration', context).val() + ' '
        + $('#edit-uc-cart-auth-unit', context).val();
    });

    $('fieldset#edit-checkout', context).drupalSetSummary(function(context) {
      if ($('#edit-uc-checkout-enabled').is(':checked')) {
        return Drupal.t('Checkout is enabled.');
      }
      else {
        return Drupal.t('Checkout is disabled.');
      }
    });
    $('fieldset#edit-anonymous', context).drupalSetSummary(function(context) {
      if ($('#edit-uc-checkout-anonymous').is(':checked')) {
        return Drupal.t('Anonymous checkout is enabled.');
      }
      else {
        return Drupal.t('Anonymous checkout is disabled.');
      }
    });
  }
};

})(jQuery);
