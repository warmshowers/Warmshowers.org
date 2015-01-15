/**
 * @file
 * Handle asynchronous requests to calculate taxes.
 */

var pane = '';
if ($("input[name*=delivery_]").length) {
  pane = 'delivery'
}
else if ($("input[name*=billing_]").length) {
  pane = 'billing'
}

$(document).ready(function() {
  getTax();
  $("select[name*=delivery_country], "
    + "select[name*=delivery_zone], "
    + "input[name*=delivery_city], "
    + "input[name*=delivery_postal_code], "
    + "select[name*=billing_country], "
    + "select[name*=billing_zone], "
    + "input[name*=billing_city], "
    + "input[name*=billing_postal_code]").change(getTax);
  $("input[name*=copy_address]").click(getTax);
  $('#edit-panes-payment-current-total').click(getTax);
});

/**
 * Get tax calculations for the current cart and line items.
 */
function getTax() {
  var order = serializeOrder();

  if (!!order) {
    $.ajax({
      type: "POST",
      url: Drupal.settings.ucURL.calculateTax,
      data: 'order=' + Drupal.encodeURIComponent(order),
      dataType: "json",
      success: function(taxes) {
        var key;
        var render = false;
        var i;
        var j;
        for (j in taxes) {
          if (taxes.hasOwnProperty(j)) {
            key = 'tax_' + taxes[j].id;
            // Check that this tax is a new line item, or updates its amount.
            if (li_values[key] == undefined || li_values[key] != taxes[j].amount) {
              set_line_item(key, taxes[j].name, taxes[j].amount, Drupal.settings.ucTaxWeight + taxes[j].weight / 10, taxes[j].summed, false);

              // Set flag to render all line items at once.
              render = true;
            }
          }
        }
        var found;
        // Search the existing tax line items and match them to a returned tax.
        for (key in li_titles) {
          // The tax id is the second part of the line item id if the first
          // part is "tax".
          keytype = key.substring(0, key.indexOf('_'));
          if (keytype == 'tax') {
            found = false;
            li_id = key.substring(key.indexOf('_') + 1);
            for (j in taxes) {
              if (taxes[j].id == li_id) {
                found = true;
                break;
              }
            }
            // No tax was matched this time, so remove the line item.
            if (!found) {
              delete li_titles[key];
              delete li_values[key];
              delete li_weight[key];
              delete li_summed[key];
              // Even if no taxes were added earlier, the display must be
              // updated.
              render = true;
            }
          }
        }
        if (render) {
          render_line_items();
        }
      }
    });
  }
}
