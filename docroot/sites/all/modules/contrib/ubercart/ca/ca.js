
/**
 * @file
 *   Adds some helper JS to the conditional actions forms.
 */

/**
 * Add confirmation prompts to remove buttons.
 */
Drupal.behaviors.caRemoveConfirm = function(context) {
  $('.ca-remove-confirm:not(.caRemoveConfirm-processed)', context).addClass('caRemoveConfirm-processed').click(function() {
    return confirm(Drupal.t('Are you sure you want to remove this item?'));
  });
}

