// $Id: uc_free_order.js,v 1.1.4.4 2009/10/29 21:01:26 rszrama Exp $

var free_order_initialized = false;
var using_free_order = false;

// Adds a click function to the total so we can check its updated values.
$(document).ready(
  function() {
    $('#edit-panes-payment-current-total').click(function() { free_order_check_total(this.value); });
  }
);

/**
 * Checks the current total and updates the available/selected payment methods
 * accordingly.
 */
function free_order_check_total(total) {
  total = parseFloat(total);

  // Disable the free order option and select the first available method.
  if (total >= .005 && (using_free_order != false || free_order_initialized == false)) {
    // Show the other payment method radios.
    $("#payment-pane .form-radios input:radio").removeAttr('disabled').parent().show(0);

    // Hide the free order radio.
    $("input:radio[value=free_order]").attr('disabled', 'disabled').parent().hide(0);

    // Find the first available payment method.
    var uc_free_order_next_method = $(':radio[name="panes[payment][payment_method]"]:enabled:first').val();

    // Select the first payment method.
    $("input:radio[value=" + uc_free_order_next_method + "]").attr('checked', 'checked');

    // Refresh the payment details section.
    get_payment_details(Drupal.settings.ucURL.checkoutPaymentDetails + uc_free_order_next_method);

    using_free_order = false;
  }
  else if (total < .005 && using_free_order != true) {
    // Hide the fallback payment method radio.
    $("#payment-pane .form-radios input:radio").attr('disabled', 'disabled').parent().hide(0);

    // Show and select the free order payment method.
    $("input:radio[value=free_order]").removeAttr('disabled').attr('checked', 'checked').parent().show(0);

    // Refresh the payment details section.
    get_payment_details(Drupal.settings.ucURL.checkoutPaymentDetails + 'free_order');

    using_free_order = true;
  }

  free_order_initialized = true;
}

