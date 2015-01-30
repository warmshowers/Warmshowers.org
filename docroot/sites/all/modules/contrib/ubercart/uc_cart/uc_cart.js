
/**
 * @file
 * Adds effects and behaviors to elements on the checkout page.
 */

var copy_box_checked = false;

/**
 * Scan the DOM and displays the cancel and continue buttons.
 */
Drupal.behaviors.ucShowOnLoad = function(context) {
  $('.show-onload:not(.ucShowOnLoad-processed)', context).addClass('ucShowOnLoad-processed').show();
}

/**
 * Add a throbber to the submit order button on the review order form.
 */
Drupal.behaviors.ucSubmitOrderThrobber = function(context) {
  $('form#uc-cart-checkout-review-form input#edit-submit:not(.ucSubmitOrderThrobber-processed)', context).addClass('ucSubmitOrderThrobber-processed').click(function() {
    $(this).clone().insertAfter(this).attr('disabled', true).after('<span class="ubercart-throbber">&nbsp;&nbsp;&nbsp;&nbsp;</span>');
    $(this).hide();
    $('#uc-cart-checkout-review-form #edit-back').attr('disabled', true);
  });
}

/**
 * Behavior for hte Next buttons.
 *
 * When a customer clicks a Next button, expand the next pane, remove the
 * button, and don't let it collapse again.
 */
function uc_cart_next_button_click(button, pane_id, current) {
  if (current !== 'false') {
    $('#' + current + '-pane legend a').click();
  }
  else {
    button.disabled = true;
  }

  if ($('#' + pane_id + '-pane').attr('class').indexOf('collapsed') > -1 && $('#' + pane_id + '-pane').html() !== null) {
    $('#' + pane_id + '-pane legend a').click();
  }

  return false;
}

/**
 * Behavior for the copy address checkbox.
 *
 * Copy the delivery information to the payment information on the checkout
 * screen if corresponding fields exist.
 */
function uc_cart_copy_address(checked, source, target) {
  if (!checked) {
    $('#' + target + '-pane div.address-pane-table').slideDown();
    copy_box_checked = false;

    // Unblock refreshing of the zone data when the country changes
    $('#edit-panes-' + target + '-' + target + '-country').data('block_zone_refresh', false);

    return false;
  }

  if (target == 'billing') {
    var x = 28;
  }
  else {
    var x = 26;
  }

  // Hide the target information fields.
  $('#' + target + '-pane div.address-pane-table').slideUp();
  copy_box_checked = true;

  // Copy over the zone options manually.
  if ($('#edit-panes-' + target + '-' + target + '-zone').html() != $('#edit-panes-' + source + '-' + source + '-zone').html()) {
    $('#edit-panes-' + target + '-' + target + '-zone').empty().append($('#edit-panes-' + source + '-' + source + '-zone').children().clone());
    $('#edit-panes-' + target + '-' + target + '-zone').attr('disabled', $('#edit-panes-' + source + '-' + source + '-zone').attr('disabled'));
  }

  // Copy over the information and set it to update if delivery info changes.
  $('#' + source + '-pane input, select, textarea').each(
    function() {
      if (this.id.substring(0, x) == 'edit-panes-' + source + '-' + source) {
        $('#edit-panes-' + target + '-' + target + this.id.substring(x)).val($(this).val());
        if (target == 'billing') {
          $(this).change(function () { update_billing_field(this); });
        }
        else {
          $(this).change(function () { update_delivery_field(this); });
        }
      }
    }
  );

  // Block refreshing of the zone data when the country is copied
  $('#edit-panes-' + target + '-' + target + '-country').data('block_zone_refresh', true);

  return false;
}

/**
 * Copy a value from the delivery address to the billing address.
 */
function update_billing_field(field) {
  if (copy_box_checked) {
    if (field.id.substring(29) == 'zone') {
      $('#edit-panes-billing-billing-zone').empty().append($('#edit-panes-delivery-delivery-zone').children().clone());
      $('#edit-panes-billing-billing-zone').attr('disabled', $('#edit-panes-delivery-delivery-zone').attr('disabled'));
    }

    $('#edit-panes-billing-billing' + field.id.substring(28)).val($(field).val()).change();
  }
}

/**
 * Copy a value from the billing address to the delivery address.
 */
function update_delivery_field(field) {
  if (copy_box_checked) {
    if (field.id.substring(27) == 'zone') {
      $('#edit-panes-delivery-delivery-zone').empty().append($('#edit-panes-billing-billing-zone').children().clone());
      $('#edit-panes-delivery-delivery-zone').attr('disabled', $('#edit-panes-billing-billing-zone').attr('disabled'));
    }

    $('#edit-panes-delivery-delivery' + field.id.substring(26)).val($(field).val()).change();
  }
}

/**
 * Apply the selected address to the appropriate fields in the cart form.
 */
function apply_address(type, address_str) {
  if (address_str == '0') {
    return;
  }

  eval('var address = ' + address_str + ';');
  var temp = type + '-' + type;

  $('#edit-panes-' + temp + '-first-name').val(address.first_name).trigger('change');
  $('#edit-panes-' + temp + '-last-name').val(address.last_name).trigger('change');
  $('#edit-panes-' + temp + '-phone').val(address.phone).trigger('change');
  $('#edit-panes-' + temp + '-company').val(address.company).trigger('change');
  $('#edit-panes-' + temp + '-street1').val(address.street1).trigger('change');
  $('#edit-panes-' + temp + '-street2').val(address.street2).trigger('change');
  $('#edit-panes-' + temp + '-city').val(address.city).trigger('change');
  $('#edit-panes-' + temp + '-postal-code').val(address.postal_code).trigger('change');

  if ($('#edit-panes-' + temp + '-country').val() != address.country) {
    $('#edit-panes-' + temp + '-country').val(address.country).data('block_zone_refresh', true).trigger('change').data('block_zone_refresh', false);
    try {
      uc_update_zone_select('edit-panes-' + temp + '-country', address.zone);
    }
    catch (err) { }
  }

  $('#edit-panes-' + temp + '-zone').val(address.zone).trigger('change');
}
