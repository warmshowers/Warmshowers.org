
/**
 * @file
 * Handles asynchronous requests for order editing forms.
 */

var customer_select = '';
var add_product_browser = '';
var order_save_holds = 0;

/**
 * Adds the double click behavior to the order table at admin/store/orders.
 */
Drupal.behaviors.ucOrderClick = function(context) {
  $('.uc-orders-table tr.odd, .uc-orders-table tr.even:not(.ucOrderClick-processed)', context).addClass('ucOrderClick-processed').each(
    function() {
      $(this).dblclick(
        function() {
          var url = Drupal.settings.ucURL.adminOrders + this.id.substring(6);
          window.location = url;
        }
      );
    }
  );
}

/**
 * Adds the submit behavior to the order form.
 */
Drupal.behaviors.ucOrderSubmit = function(context) {
  $('#uc-order-edit-form:not(.ucOrderSubmit-processed)', context).addClass('ucOrderSubmit-processed').submit(
    function() {
      $('#products-selector').empty().removeClass();
      $('#delivery_address_select').empty().removeClass();
      $('#billing_address_select').empty().removeClass();
      $('#customer-select').empty().removeClass();
    }
  );
}

$(document).ready(
  function() {
    if (order_save_holds == 0) {
      release_held_buttons();
    }

    $('.uc-orders-table tr.odd, .uc-orders-table tr.even').each(
      function() {
        $(this).dblclick(
          function() {
            var url = Drupal.settings.ucURL.adminOrders + this.id.substring(6);
            window.location = url;
          }
        );
      }
    );

    $('#uc-order-edit-form').submit(
      function() {
        $('#products-selector').empty().removeClass();
        $('#delivery_address_select').empty().removeClass();
        $('#billing_address_select').empty().removeClass();
        $('#customer-select').empty().removeClass();
      }
    );
  }
);

/**
 * Copys the shipping data on the order edit screen to the corresponding billing
 * fields if they exist.
 */
function uc_order_copy_shipping_to_billing() {
  if ($('#edit-delivery-zone').html() != $('#edit-billing-zone').html()) {
    $('#edit-billing-zone').empty().append($('#edit-delivery-zone').children().clone());
  }

  $('#uc-order-edit-form input, select, textarea').each(
    function() {
      if (this.id.substring(0, 13) == 'edit-delivery') {
        $('#edit-billing' + this.id.substring(13)).val($(this).val());
      }
    }
  );
}

/**
 * Copys the billing data on the order edit screen to the corresponding shipping
 * fields if they exist.
 */
function uc_order_copy_billing_to_shipping() {
  if ($('#edit-billing-zone').html() != $('#edit-delivery-zone').html()) {
    $('#edit-delivery-zone').empty().append($('#edit-billing-zone').children().clone());
  }

  $('#uc-order-edit-form input, select, textarea').each(
    function() {
      if (this.id.substring(0, 12) == 'edit-billing') {
        $('#edit-delivery' + this.id.substring(12)).val($(this).val());
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

  $.post(Drupal.settings.ucURL.adminOrders + 'address_book', options,
         function (contents) {
           $(div).empty().addClass('address-select-box').append(contents);
         }
  );
}

/**
 * Applys the selected address to the appropriate fields in the order edit form.
 */
function apply_address(type, address_str) {
  eval('var address = ' + address_str + ';');
  $('#edit-' + type + '-first-name').val(address['first_name']);
  $('#edit-' + type + '-last-name').val(address['last_name']);
  $('#edit-' + type + '-phone').val(address['phone']);
  $('#edit-' + type + '-company').val(address['company']);
  $('#edit-' + type + '-street1').val(address['street1']);
  $('#edit-' + type + '-street2').val(address['street2']);
  $('#edit-' + type + '-city').val(address['city']);
  $('#edit-' + type + '-postal-code').val(address['postal_code']);

  if ($('#edit-' + type + '-country').val() != address['country']) {
    $('#edit-' + type + '-country').val(address['country']);
    try {
      uc_update_zone_select('edit-' + type + '-country', address['zone']);
    }
    catch (err) {}
  }

  $('#edit-' + type + '-zone').val(address['zone']);
}

/**
 * Closes the address book div.
 */
function close_address_select(div) {
  $(div).empty().removeClass('address-select-box');
  return false;
}

/**
 * Loads the customer select div on the order edit screen.
 */
function load_customer_search() {
  if (customer_select == 'search' && $('#customer-select #edit-back').val() == null) {
    return close_customer_select();
  }

  $.post(Drupal.settings.ucURL.adminOrders + 'customer', {},
         function (contents) {
           $('#customer-select').empty().addClass('customer-select-box').append(contents);
           $('#customer-select #edit-first-name').val($('#edit-billing-first-name').val());
           $('#customer-select #edit-last-name').val($('#edit-billing-last-name').val());
           customer_select = 'search';
         }
  );

  return false;
}

/**
 * Displays the results of the customer search.
 */
function load_customer_search_results() {
  $.post(Drupal.settings.ucURL.adminOrders + 'customer/search',
    {
      first_name: $('#customer-select #edit-first-name').val(),
      last_name: $('#customer-select #edit-last-name').val(),
      email: $('#customer-select #edit-email').val(),
      username: $('#customer-select #edit-username').val()
    },
    function (contents) {
      $('#customer-select').empty().append(contents);
    }
  );
  return false;
}

/**
 * Sets customer values from search selection.
 */
function select_customer_search() {
  var data = $('#edit-cust-select').val();
  $('#edit-uid').val(data.substr(0, data.indexOf(':')));
  $('#edit-uid-text').val(data.substr(0, data.indexOf(':')));
  $('#edit-primary-email').val(data.substr(data.indexOf(':') + 1));
  $('#edit-primary-email-text').val(data.substr(data.indexOf(':') + 1));
  try {
    $('#edit-submit-changes').get(0).click();
  }
  catch (err) {
  }
  return close_customer_select();
}

/**
 * Displays the new customer form.
 */
function load_new_customer_form() {
  if (customer_select == 'new') {
    return close_customer_select();
  }

  $.post(Drupal.settings.ucURL.adminOrders + 'customer/new', {},
         function (contents) {
           $('#customer-select').empty().addClass('customer-select-box').append(contents);
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
    'email' : $('#customer-select #edit-email').val(),
    'sendmail' : $('#customer-select #edit-sendmail').attr('checked')
  };
  $.post(Drupal.settings.ucURL.adminOrders + 'customer/new/check', options,
         function (contents) {
           $('#customer-select').empty().append(contents);
         }
  );
  return false;
}

/**
 * Loads existing customer as new order's customer.
 */
function select_existing_customer(uid, email) {
  $('#edit-uid').val(uid);
  $('#edit-uid-text').val(uid);
  $('#edit-primary-email').val(email);
  $('#edit-primary-email-text').val(email);
  try {
    $('#edit-submit-changes').click();
  }
  catch (err) {
  }
  return close_customer_select();
}

/**
 * Hides the customer selection form.
 */
function close_customer_select() {
  $('#customer-select').empty().removeClass('customer-select-box');
  customer_select = '';
  return false;
}

/**
 * Loads the products div on the order edit screen.
 */
function uc_order_load_product_edit_div(order_id) {
  $(document).ready(
    function() {
      add_order_save_hold();

      show_product_throbber();

      $.post(Drupal.settings.ucURL.adminOrders + order_id + '/products',
             { action: 'view' },
             function(contents) {
               if (contents != '') {
                 $('#products-container').empty().append(contents);
               }
               remove_order_save_hold();
               hide_product_throbber();
             });
    }
  );
}

/**
 * Loads the product selection form.
 */
function load_product_select(order_id, search) {
  if (search == true) {
    options = {'search' : $('#edit-product-search').val()};
  }
  else {
    options = { };
  }

  show_product_throbber();

  $.post(Drupal.settings.ucURL.adminOrders + order_id + '/product_select', options,
         function (contents) {
           $('#products-selector').empty().addClass('product-select-box2').append(contents);
           hide_product_throbber();
         }
  );

  return false;
}

/**
 * Deprecated?
 */
function select_product() {
  add_product_form();
  return false;
}

/**
 * Hides product selection form.
 */
function close_product_select() {
  $('#products-selector').empty().removeClass('product-select-box2');
  return false;
}

/**
 * Loads the quantity and other extra product fields.
 */
function add_product_form() {
  add_product_browser = $('#products-selector').html();

  show_product_throbber();

  if (parseInt($('#edit-unid').val()) > 0) {
    $.post(Drupal.settings.ucURL.adminOrders + $('#edit-order-id').val() + '/add_product/' + $('#edit-unid').val(), { },
           function(contents) {
             $('#products-selector').empty().append(contents);
             hide_product_throbber();
           }
    );
  }
}

/**
 * Adds the selected product to the order.
 */
function add_product_to_order(order_id, node_id) {
  var post_vars = fetch_product_data();
  post_vars['action'] = 'add';
  post_vars['nid'] = node_id;
  post_vars['qty'] = $('#edit-add-qty').val();

  $('#uc-order-add-product-form :input').not(':radio:not(:checked), :checkbox:not(:checked)').each(
    function() {
      post_vars[$(this).attr('name')] = $(this).val();
    }
  );

  show_product_throbber();

  $.post(Drupal.settings.ucURL.adminOrders + order_id + '/products', post_vars,
         function(contents) {
           if (contents != '') {
             $('#products-container').empty().append(contents);
           }
           hide_product_throbber();
         }
  );

  $('#add-product-button').click();

  return false;
}

/**
 * Gathers all of the products' data fields into one array.
 */
function fetch_product_data() {
  var pdata = { };

  $('#products-container :input').each(function() {
    if (this.type == 'checkbox') {
      if (this.checked) {
        pdata[$(this).attr('name')] = $(this).val();
      }
    }
    else {
      pdata[$(this).attr('name')] = $(this).val();
    }
  });

  return pdata;
}

/**
 * Button to create a new row of empty data fields.
 */
function add_blank_line_button(order_id) {
  var post_vars = fetch_product_data();
  post_vars['action'] = 'add_blank';

  show_product_throbber();

  $.post(Drupal.settings.ucURL.adminOrders + order_id + '/products',
         post_vars,
         function(contents) {
           if (contents != '') {
             $('#products-container').empty().append(contents);
           }
           hide_product_throbber();
         }
  );
}

/**
 * Button to remove product from the order.
 */
function remove_product_button(message, opid) {
  if (confirm(message)) {
    var post_vars = fetch_product_data();
    post_vars['action'] = 'remove';
    post_vars['opid'] = opid;

    show_product_throbber();

    $.post(Drupal.settings.ucURL.adminOrders + $('#edit-order-id').val() + '/products',
           post_vars,
           function(contents) {
             if (contents != '') {
               $('#products-container').empty().append(contents);
             }
             hide_product_throbber();
           }
    );
  }
}

/**
 * Prevents mistakes by confirming deletion.
 */
function confirm_line_item_delete(message, img_id) {
  if (confirm(message)) {
    var li_id = img_id.substring(3);
    $('#edit-li-delete-id').val(li_id);
    $('#uc-order-edit-form #edit-submit-changes').get(0).click();
  }
}

/**
 * Disables order submit button while parts of the page are still loading.
 */
function add_order_save_hold() {
  order_save_holds++;
  $('#uc-order-edit-form input.save-button').attr('disabled', 'disabled');
}

/**
 * Removes a hold and enable the save buttons when all holds are gone!
 */
function remove_order_save_hold() {
  order_save_holds--;

  if (order_save_holds == 0) {
    release_held_buttons();
  }
}

/**
 * Removes the disable attribute on any input item with the save-button class.
 */
function release_held_buttons() {
  $('#uc-order-edit-form input.save-button').removeAttr('disabled');
}

/**
 * User feedback that something is happening.
 */
function show_product_throbber() {
  $('#product-div-throbber').attr('style', 'background-image: url(' + Drupal.settings.basePath + 'misc/throbber.gif); background-repeat: no-repeat; background-position: 100% -20px;').html('&nbsp;&nbsp;&nbsp;&nbsp;');
}

/**
 * Done loading forms.
 */
function hide_product_throbber() {
  $('#product-div-throbber').removeAttr('style').empty();
}
