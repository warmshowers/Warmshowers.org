/**
 * @file
 * uc_stripe.js
 *
 * Handles all interactions with Stripe on the client side for PCI-DSS compliance
 */


if (Drupal.jsEnabled) {

  //Initiate a new object we can store functions in
  Drupal.uc_stripe = new Object;
  Drupal.uc_stripe.systemClicked = false;

  Drupal.behaviors.uc_stripe = function (context) {
      var form = $('#uc-cart-checkout-form');
      var submitButton = form.find('#edit-continue');

      submitButton.click(function (e) {
        if ($('#edit-panes-payment-payment-method-credit').is(':checked')) {

          if (Drupal.uc_stripe.systemClicked == false) {
            e.preventDefault();
          }
          else {
            return true;
          }


          Stripe.createToken({
            number: $('#edit-cc-number').val(),
            cvc: $('#edit-cc-cvv').val(),
            exp_month: $('#edit-cc-exp-month').val(),
            exp_year: $('#edit-cc-exp-year').val()
          }, function (status, response) {
            if (response.error) {
              // Show the errors on the form
              $('#uc_stripe_messages').removeClass("hidden");
              $('#uc_stripe_messages').text(response.error.message);
              $('#edit-stripe-messages').val(response.error.message);

              // Turn off the throbber - we're done here
              $('.ubercart-throbber').remove();
              // Remove the bogus copy of the submit button added in uc_cart.js ucSubmitOrderThrobber
              submitButton.next().remove();
              // And show the hidden original button which has the behavior attached to it.
              submitButton.show();
            } else {
              // token contains id, last4, and card type
              var token = response.id;

              // Insert the token into the form so it gets submitted to the server
              $("#edit-stripe-token").val(token);

              // Since we're now submitting, make sure that uc_credit doesn't
              // find values it objects to; after "fixing" set the name back on the
              // form element.
              $('#edit-cc-number').css('visibility', 'hidden').val('424242424242' + response.card.last4).attr('name', 'cc_number');
              $("#edit-cc-cvv").css('visibility', 'hidden').val('999').attr('name', 'cc_cvv');

              Drupal.uc_stripe.systemClicked = true;

              // now actually submit to Drupal. The only "real" things going
              // are the token and the expiration date.
              submitButton.click();
            }
          });
        }
      });
  }

  // Remove 'name' from sensitive form elements so there's no way they can be submitted.
  function uc_stripe_clean_cc_form() {
    $('#edit-cc-number').removeAttr('name').removeAttr('disabled');
    $('#edit-cc-cvv').removeAttr('name').removeAttr('disabled');
    $('span#stripe-nojs-warning').parent().hide();

    // JS must enable the button; otherwise form might disclose cc info. It starts disabled
    $('#edit-continue').attr('disabled', false);
  }
}
