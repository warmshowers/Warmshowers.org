/**
 * @file
 * uc_stripe.js
 *
 * Handles all interactions with Stripe on the client side for PCI-DSS compliance
 */
(function ($) {

  //Initiate a new object we can store functions in
  Drupal.uc_stripe = new Object;
  Drupal.uc_stripe.systemClicked = false;

  Drupal.behaviors.uc_stripe = {
    attach: function (context) {
      var submitButton = $('#uc-cart-checkout-form #edit-continue');

      // When this behavior fires, we can clean the form so it will behave properly,
      // Remove 'name' from sensitive form elements so there's no way they can be submitted.
      $('#edit-panes-payment-details-cc-number').removeAttr('name').removeAttr('disabled');
      $('#edit-panes-payment-details-cc-cvv').removeAttr('name').removeAttr('disabled');
      $('span#stripe-nojs-warning').parent().hide();

      // JS must enable the button; otherwise form might disclose cc info. It starts disabled
      $('#edit-continue').attr('disabled', false);

      submitButton.click(function (e) {
        if ($('#edit-panes-payment-payment-method-credit').is(':checked')) {

          if (Drupal.uc_stripe.systemClicked == false) {
            e.preventDefault();
          }
          else {
            return true;
          }


          Stripe.createToken({
            number: $('#edit-panes-payment-details-cc-number').val(),
            cvc: $('#edit-panes-payment-details-cc-cvv').val(),
            exp_month: $('#edit-panes-payment-details-cc-exp-month').val(),
            exp_year: $('#edit-panes-payment-details-cc-exp-year').val()
          }, function (status, response) {
            if (response.error) {

              // Show the errors on the form
              $('#uc_stripe_messages')
                .removeClass("hidden")
                .text(response.error.message);
              $('#edit-stripe-messages').val(response.error.message);

              // Turn off the throbber - we're done here
              $('.ubercart-throbber').remove();
              // Remove the bogus copy of the submit button added in uc_cart.js ucSubmitOrderThrobber
              submitButton.next().remove();
              // And show the hidden original button which has the behavior attached to it.
              submitButton.show();
              $("#edit-panes-payment-details-stripe-token").val("fail");
            } else {
              // token contains id, last4, and card type
              var token = response.id;

              // Insert the token into the form so it gets submitted to the server
              $("#edit-panes-payment-details-stripe-token").val(token);

              // Since we're now submitting, make sure that uc_credit doesn't
              // find values it objects to; after "fixing" set the name back on the
              // form element.
              $('#edit-panes-payment-details-cc-number')
                .css('visibility', 'hidden')
                .val('555555555555' + response.card.last4)
                .attr('name', 'panes[payment][details][cc_number]');
              $("#edit-panes-payment-details-cc-cvv")
                .css('visibility', 'hidden')
                .val('999')
                .attr('name', 'panes[payment][details][cc_cvv]');

              Drupal.uc_stripe.systemClicked = true;

              // now actually submit to Drupal. The only "real" things going
              // are the token and the expiration date.
              submitButton.click();
            }
          });
        }
      });
    }
  };


}(jQuery));
