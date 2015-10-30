/**
 * @file
 * Handles asynchronous requests for order editing forms.
 */

var customer_select = '';

/**
 * Adds double click behavior to the order and customer admin tables.
 */
Drupal.behaviors.ucOrderClick = {
  attach: function(context, settings) {
    jQuery('.view-uc-orders tbody tr, .view-uc-customers tbody tr', context).dblclick(
      function() {
        window.location = jQuery(this).find('.views-field-order-id a').attr('href');
      }
    );
  }
}

/**
 * Adds the submit behavior to the order form
 */
Drupal.behaviors.ucOrderSubmit = {
  attach: function(context, settings) {
    jQuery('#uc-order-edit-form:not(.ucOrderSubmit-processed)', context).addClass('ucOrderSubmit-processed').submit(
      function() {
        jQuery('#products-selector').empty().removeClass();
        jQuery('#delivery_address_select').empty().removeClass();
        jQuery('#billing_address_select').empty().removeClass();
        jQuery('#customer-select').empty().removeClass();
      }
    );
  }
}

/**
 * Copies the shipping data on the order edit screen to the corresponding
 * billing fields if they exist.
 */
function uc_order_copy_shipping_to_billing() {
  if (jQuery('#edit-delivery-zone').html() != jQuery('#edit-billing-zone').html()) {
    jQuery('#edit-billing-zone').empty().append(jQuery('#edit-delivery-zone').children().clone());
  }

  jQuery('#uc-order-edit-form input, select, textarea').each(
    function() {
      if (this.id.substring(0, 13) == 'edit-delivery') {
        jQuery('#edit-billing' + this.id.substring(13)).val(jQuery(this).val());
      }
    }
  );
}

/**
 * Copies the billing data on the order edit screen to the corresponding
 * shipping fields if they exist.
 */
function uc_order_copy_billing_to_shipping() {
  if (jQuery('#edit-billing-zone').html() != jQuery('#edit-delivery-zone').html()) {
    jQuery('#edit-delivery-zone').empty().append(jQuery('#edit-billing-zone').children().clone());
  }

  jQuery('#uc-order-edit-form input, select, textarea').each(
    function() {
      if (this.id.substring(0, 12) == 'edit-billing') {
        jQuery('#edit-delivery' + this.id.substring(12)).val(jQuery(this).val());
      }
    }
  );
}

/**
 * Loads the address book div on the order edit screen.
 */
function load_address_select(uid, div, address_type) {
  var options = {
    'uid'  : uid,
    'type' : address_type,
    'func' : "apply_address('" + address_type + "', this.value);"
  };

  jQuery.post(Drupal.settings.ucURL.adminOrders + 'address_book', options,
    function (contents) {
      jQuery(div).empty().addClass('address-select-box').append(contents);
    }
  );
}

/**
 * Applys the selected address to the appropriate fields in the order edit form.
 */
function apply_address(type, address_str) {
  eval('var address = ' + address_str + ';');
  jQuery('#edit-' + type + '-first-name').val(address['first_name']);
  jQuery('#edit-' + type + '-last-name').val(address['last_name']);
  jQuery('#edit-' + type + '-phone').val(address['phone']);
  jQuery('#edit-' + type + '-company').val(address['company']);
  jQuery('#edit-' + type + '-street1').val(address['street1']);
  jQuery('#edit-' + type + '-street2').val(address['street2']);
  jQuery('#edit-' + type + '-city').val(address['city']);
  jQuery('#edit-' + type + '-postal-code').val(address['postal_code']);

  if (jQuery('#edit-' + type + '-country').val() != address['country']) {
    jQuery('#edit-' + type + '-country').val(address['country']);
  }

  jQuery('#edit-' + type + '-zone').val(address['zone']);
}

/**
 * Closes the address book div.
 */
function close_address_select(div) {
  jQuery(div).empty().removeClass('address-select-box');
  return false;
}

/**
 * Loads the customer select div on the order edit screen.
 */
function load_customer_search() {
  if (customer_select == 'search' && jQuery('#customer-select #edit-back').val() == null) {
    return close_customer_select();
  }

  jQuery.post(Drupal.settings.ucURL.adminOrders + 'customer', {},
    function (contents) {
      jQuery('#customer-select').empty().addClass('customer-select-box').append(contents);
      jQuery('#customer-select #edit-first-name').val(jQuery('#edit-billing-first-name').val());
      jQuery('#customer-select #edit-last-name').val(jQuery('#edit-billing-last-name').val());
      customer_select = 'search';
    }
  );

  return false;
}

/**
 * Displays the results of the customer search.
 */
function load_customer_search_results() {
  jQuery.post(Drupal.settings.ucURL.adminOrders + 'customer/search',
    {
      first_name: jQuery('#customer-select #edit-first-name').val(),
      last_name: jQuery('#customer-select #edit-last-name').val(),
      email: jQuery('#customer-select #edit-email').val()
    },
    function (contents) {
      jQuery('#customer-select').empty().append(contents);
    }
  );
  return false;
}

/**
 * Sets customer values from search selection.
 */
function select_customer_search() {
  var data = jQuery('#edit-cust-select').val();
  var i = data.indexOf(':');
  return select_existing_customer(data.substr(0, i), data.substr(i + 1));
}

/**
 * Displays the new customer form.
 */
function load_new_customer_form() {
  if (customer_select == 'new') {
    return close_customer_select();
  }

  jQuery.post(Drupal.settings.ucURL.adminOrders + 'customer/new', {},
    function (contents) {
      jQuery('#customer-select').empty().addClass('customer-select-box').append(contents);
      customer_select = 'new';
    }
  );
  return false;
}

/**
 * Validates the customer's email address.
 */
function check_new_customer_address() {
  var options = {
    'email' : jQuery('#customer-select #edit-email').val(),
    'sendmail' : jQuery('#customer-select #edit-sendmail').attr('checked')
  };
  jQuery.post(Drupal.settings.ucURL.adminOrders + 'customer/new/check', options,
    function (contents) {
      jQuery('#customer-select').empty().append(contents);
    }
  );
  return false;
}

/**
 * Loads existing customer as new order's customer.
 */
function select_existing_customer(uid, email) {
  jQuery('input[name=uid], #edit-uid-text').val(uid);
  jQuery('input[name=primary_email], #edit-primary-email-text').val(email);
  try {
    jQuery('#edit-submit-changes').click();
  }
  catch (err) {
  }
  return close_customer_select();
}

/**
 * Hides the customer selection form.
 */
function close_customer_select() {
  jQuery('#customer-select').empty().removeClass('customer-select-box');
  customer_select = '';
  return false;
}
