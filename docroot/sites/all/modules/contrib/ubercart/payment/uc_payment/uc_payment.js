
/**
 * Calculate the number of bytes of a Unicode string.
 *
 * Gratefully stolen from http://dt.in.th/2008-09-16.string-length-in-bytes.html.
 * Javascript String.length returns the number of characters, but PHP strlen()
 * returns the number of bytes. When building serialize()d strings in JS,
 * use this function to get the correct string length.
 */
String.prototype.bytes = function() {
  // Drupal.encodeURIComponent() gets around some weirdness in
  // encodeURIComponent(), but encodes some characters twice. The first
  // replace takes care of those while the second lets String.length count
  // the multi-byte characters.
  return Drupal.encodeURIComponent(this).replace(/%252[36F]/g, 'x').replace(/%../g, 'x').length;
};

// Arrays for order total preview data.
var li_titles = {};
var li_values = {};
var li_weight = {};
var li_summed = {};

// Timestamps for last time line items or payment details were updated.
var line_update = 0;
var payment_update = 0;

var do_payment_details = true;

if (Drupal.jsEnabled) {
  jQuery.extend(Drupal.settings, {
    ucShowProgressBar: false,
    ucDefaultPayment: '',
    ucOrderInitiate: false
  });

  $(document).ready(
    function() {

      // attach a progressbar if requested
      if (Drupal.settings.ucShowProgressBar) {
        show_progressBar('#line-items-div');
      }

      // initialize payment details
      if (Drupal.settings.ucDefaultPayment != '') {
        init_payment_details(Drupal.settings.ucDefaultPayment);
      }

      // disable the submission buttons and get payment details
      if (Drupal.settings.ucOrderInitiate) {
        add_order_save_hold();
        get_payment_details(Drupal.settings.ucURL.adminOrders + $('#edit-order-id').val() + '/payment_details/' + $('#edit-payment-method').val());
      }
    }
  )
}

function show_progressBar(id) {
  var progress = new Drupal.progressBar('paymentProgress');
  progress.setProgress(-1, '');
  $(id).empty().append(progress.element);
}

function serializeOrder() {
  var products = $("[name=cart_contents]").val();
  if (!products) {
    return false;
  }

  var uid = $("input[name*=uid]").val() || 0;
  var p_email = $("input[name*=primary_email]").val() || '';
  var s_f_name = $("input[name*=delivery_first_name]").val() || '';
  var s_l_name = $("input[name*=delivery_last_name]").val() || '';
  var s_street1 = $("input[name*=delivery_street1]").val() || '';
  var s_street2 = $("input[name*=delivery_street2]").val() || '';
  var s_city = $("input[name*=delivery_city]").val() || '';
  var s_zone = $("select[name*=delivery_zone]").val() || '0';
  var s_code = $("input[name*=delivery_postal_code]").val() || '';
  var s_country = $("select[name*=delivery_country]").val() || '0';

  var b_f_name = $("input[name*=billing_first_name]").val() || '';
  var b_l_name = $("input[name*=billing_last_name]").val() || '';
  var b_street1 = $("input[name*=billing_street1]").val() || '';
  var b_street2 = $("input[name*=billing_street2]").val() || '';
  var b_city = $("input[name*=billing_city]").val() || '';
  var b_zone = $("select[name*=billing_zone]").val() || '0';
  var b_code = $("input[name*=billing_postal_code]").val() || '';
  var b_country = $("select[name*=billing_country]").val() || '0';

  var line_item = '';
  var key;
  var type;
  var i = 0;
  for (key in li_titles) {
    temp = key.split('_', 2);
    if (temp[1] != undefined && temp[1].match(/^\d+$/)) {
      type = temp[0];
    }
    else {
      type = key;
    }
    line_item = line_item + 'i:' + i + ';a:5:{s:5:"title";s:' + li_titles[key].bytes() + ':"' + li_titles[key] + '";s:4:"type";s:'+ type.bytes() + ':"'+ type + '";s:6:"amount";d:' + li_values[key] + ';s:6:"weight";d:' + li_weight[key] + ';s:6:"summed";i:' + li_summed[key] + ';}';
    i++;
  }
  line_item = 's:10:"line_items";a:' + i + ':{' + line_item + '}';

  var order_size = 21;

  var shipping = '';
  var shipping_option = $('input:radio[name=quote-option]:checked').val() || $('input:[name=quote-option]').val();
  if (shipping_option) {
    shipping_option = /(.*)---.*$/.exec(shipping_option)[1];
    shipping = 's:5:"quote";a:1:{s:6:"method";s:' + shipping_option.bytes() + ':"' + shipping_option + '";}';
    order_size++;
  }

  var order = 'O:8:"stdClass":' + order_size + ':{s:8:"products";' + products
    + 's:8:"order_id";i:0;'
    + 's:3:"uid";i:' + uid + ';'
    + 's:13:"primary_email";s:' + p_email.bytes() + ':"' + p_email
    + '";s:19:"delivery_first_name";s:' + s_f_name.bytes() + ':"' + s_f_name
    + '";s:18:"delivery_last_name";s:' + s_l_name.bytes() + ':"' + s_l_name
    + '";s:16:"delivery_street1";s:' + s_street1.bytes() + ':"' + s_street1
    + '";s:16:"delivery_street2";s:' + s_street2.bytes() + ':"' + s_street2
    + '";s:13:"delivery_city";s:' + s_city.bytes() + ':"' + s_city
    + '";s:13:"delivery_zone";i:' + s_zone
    + ';s:20:"delivery_postal_code";s:' + s_code.bytes() +':"' + s_code
    + '";s:16:"delivery_country";i:' + s_country + ';'
    + 's:18:"billing_first_name";s:' + b_f_name.bytes() + ':"' + b_f_name
    + '";s:17:"billing_last_name";s:' + b_l_name.bytes() + ':"' + b_l_name
    + '";s:15:"billing_street1";s:' + b_street1.bytes() + ':"' + b_street1
    + '";s:15:"billing_street2";s:' + b_street2.bytes() + ':"' + b_street2
    + '";s:12:"billing_city";s:' + b_city.bytes() + ':"' + b_city
    + '";s:12:"billing_zone";i:' + b_zone
    + ';s:19:"billing_postal_code";s:' + b_code.bytes() +':"' + b_code
    + '";s:15:"billing_country";i:' + b_country + ';'
    + shipping + line_item + '}';

  return order;
}

/**
 * Sets a line item in the order total preview.
 */
function set_line_item(key, title, value, weight, summed, render) {
  var do_update = false;

  if (summed === undefined) {
    summed = 1;
  }
  // Check to see if we're actually changing anything and need to update.
  if (window.li_values[key] === undefined) {
    do_update = true;
  }
  else {
    if (li_titles[key] != title || li_values[key] != value || li_weight[key] != weight || li_summed[key] != summed) {
      do_update = true;
    }
  }

  if (do_update) {
    // Set the values passed in, overriding previous values for that key.
    if (key != "") {
      li_titles[key] = title;
      li_values[key] = value;
      li_weight[key] = weight;
      li_summed[key] = summed;
    }
    if (render == null || render) {
      render_line_items();
    }
  }
}

function render_line_items() {
  // Set the timestamp for this update.
  var this_update = new Date();

  // Set the global timestamp for the update.
  line_update = this_update.getTime();

  // Put all the existing line item data into a single array.
  var cur_total = 0;
  $.each(li_titles,
    function(a, b) {
      // Tally up the current order total for storage in a hidden item.
      if (li_titles[a] != '' && li_summed[a] == 1) {
        cur_total += li_values[a];
      }
    }
  );
  $('#edit-panes-payment-current-total').val(cur_total).click();

  $('#order-total-throbber').addClass('ubercart-throbber').html('&nbsp;&nbsp;&nbsp;&nbsp;');

  // Post the line item data to a URL and get it back formatted for display.
  $.post(Drupal.settings.ucURL.checkoutLineItems, {order: serializeOrder()},
    function(contents) {
      // Only display the changes if this was the last requested update.
      if (this_update.getTime() == line_update) {
        $('#line-items-div').empty().append(contents);
      }
    }
  );
}

function remove_line_item(key) {
  delete li_titles[key];
  delete li_values[key];
  delete li_weight[key];
  delete li_summed[key];
  render_line_items();
}

/**
 * Doesn't refresh the payment details if they've already been loaded.
 */
function init_payment_details(payment_method) {
  if (payment_update == 0) {
    get_payment_details(Drupal.settings.ucURL.checkoutPaymentDetails + payment_method);
  }
}

/**
 * Display the payment details when a payment method radio button is clicked.
 */
function get_payment_details(path) {
  var progress = new Drupal.progressBar('paymentProgress');
  progress.setProgress(-1, '');
  $('#payment_details').empty().append(progress.element).removeClass('display-none');

  // Get the timestamp for the current update.
  var this_update = new Date();

  // Set the global timestamp for the update.
  payment_update = this_update.getTime();

  var data;
  if ($('#edit-payment-details-data').length) {
    data = { 'payment-details-data' : $('#edit-payment-details-data').val() };
  }
  else {
    data = { 'payment-details-data' : '' };
  }
  // Make the post to get the details for the chosen payment method.
  $.post(path, data,
    function(details) {
      if (this_update.getTime() == payment_update) {
        // If the response was empty, throw up the default message.
        if (details == '') {
          $('#payment_details').empty().html(Drupal.settings.defPaymentMsg);
        }
        // Otherwise display the returned details.
        else {
          $('#payment_details').empty().append(details);
        }
      }

      // If on the order edit screen, clear out the order save hold.
      if (window.remove_order_save_hold) {
        remove_order_save_hold();
      }
    }
  );
}

/**
 * Toggle the payment fields on and off on the receive check form.
 */
function receive_check_toggle(checked) {
  if (!checked) {
    $('#edit-amount').removeAttr('disabled').val('');
    $('#edit-comment').removeAttr('disabled').val('');
  }
  else {
    $('#edit-amount').attr('disabled', 'true').val('-');
    $('#edit-comment').attr('disabled', 'true').val('-');
  }
}

