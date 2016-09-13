/**
 * @file
 * Adds effects and behaviors to elements on the checkout page.
 */

Drupal.behaviors.ucCart = {
  attach: function(context, settings) {
    // Add a throbber to the submit order button on the review order form.
    jQuery('form#uc-cart-checkout-review-form input#edit-submit:not(.ucSubmitOrderThrobber-processed)', context).addClass('ucSubmitOrderThrobber-processed').click(function() {
      jQuery(this).clone().insertAfter(this).attr('disabled', true).after('<span class="ubercart-throbber">&nbsp;&nbsp;&nbsp;&nbsp;</span>').end().hide();
      jQuery('#uc-cart-checkout-review-form #edit-back').attr('disabled', true);
    });
  }
}

/**
 * Behaviors for the Next buttons.
 *
 * When a customer clicks a Next button, expand the next pane, remove the
 * button, and don't let it collapse again.
 */
function uc_cart_next_button_click(button, pane_id, current) {
  if (current !== 'false') {
    jQuery('#' + current + '-pane legend a').click();
  }
  else {
    button.disabled = true;
  }

  if (jQuery('#' + pane_id + '-pane').attr('class').indexOf('collapsed') > -1 && jQuery('#' + pane_id + '-pane').html() !== null) {
    jQuery('#' + pane_id + '-pane legend a').click();
  }

  return false;
}
