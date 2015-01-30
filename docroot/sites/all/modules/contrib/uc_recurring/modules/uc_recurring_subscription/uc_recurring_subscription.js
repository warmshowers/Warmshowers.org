/**
 * Disable and enable fields in the recurring subscription form.
 */
Drupal.behaviors.UcRecurringSubscriptionForm = function (context) {
  $(".unlimited-checkbox").change(function() {
    var id = $(this).attr('id');
    text_id = id.replace('unlimited', 'number-intervals');
    if ($("#"+id).attr("checked")) {
      $("#"+text_id).attr("disabled","disabled");
      $("#"+text_id).val('');
    }
    else {
      $("#"+text_id).removeAttr("disabled");
    }
  });
};

