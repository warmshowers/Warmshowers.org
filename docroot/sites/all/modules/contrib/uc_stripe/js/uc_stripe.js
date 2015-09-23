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
      var submitButton = $('.uc-cart-checkout-form #edit-continue');

      var cc_container = $('.payment-details-credit');
      var cc_num = cc_container.find(':input[id*="edit-panes-payment-details-cc-numbe"]');
      var cc_cvv = cc_container.find(':input[id*="edit-panes-payment-details-cc-cv"]');

      // When this behavior fires, we can clean the form so it will behave properly,
      // Remove 'name' from sensitive form elements so there's no way they can be submitted.
      cc_num.removeAttr('name').removeAttr('disabled');
      cc_cvv.removeAttr('name').removeAttr('disabled');
      var cc_val_val = cc_num.val();
      if (cc_val_val && cc_val_val.indexOf('Last 4')) {
          cc_num.val('');
      }
      $('span#stripe-nojs-warning').parent().hide();

      // JS must enable the button; otherwise form might disclose cc info. It starts disabled
      submitButton.attr('disabled', false);

      submitButton.click(function (e) {
        if ($(':input[name="panes[payment][payment_method]"]').val() == 'credit') {

          if (Drupal.uc_stripe.systemClicked == false) {
            e.preventDefault();
          }
          else {
            return true;
          }


          Stripe.createToken({
            number: cc_num.val(),
            cvc: cc_cvv.val(),
            exp_month: $(':input[name="panes[payment][details][cc_exp_month]"]').val(),
            exp_year: $(':input[name="panes[payment][details][cc_exp_year]"]').val()
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
              cc_num
                .css('visibility', 'hidden')
                .val('555555555555' + response.card.last4)
                .attr('name', 'panes[payment][details][cc_number]');
              cc_cvv
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
