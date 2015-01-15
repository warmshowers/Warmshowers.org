
/**
 * Disable and enable fields in the recurring fee feature form.
 */
Drupal.behaviors.UcRecurringFeatureFrom = function (context) {
  // Toggle the fee amount field according to "Set the recurring fee amount as
  // Selling price field state.
  $("#edit-fee-same-product").click(function() {
    if ($("#edit-fee-same-product").attr("checked")) {
      $("#edit-fee-amount").attr("disabled","disabled");
      $("#edit-fee-amount").val($("#edit-product-price").val());
    }
    else {
      $("#edit-fee-amount").removeAttr("disabled");
    };
  });

  // Toggle the fee amount field according to "Set the recurring fee amount as
  // selling price" field state.
  $("#edit-unlimited-intervals").click(function() {
    if ($("#edit-unlimited-intervals").attr("checked")) {
      $("#edit-number-intervals").attr("disabled","disabled");
      $("#edit-number-intervals").val($("#edit-number-intervals").val());
    }
    else {
      $("#edit-number-intervals").removeAttr("disabled");
    };
  });
};
