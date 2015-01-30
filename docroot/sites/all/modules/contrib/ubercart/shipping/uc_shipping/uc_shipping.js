
/**
 * @file
 * Add autofill address functionality to shipment forms.
 */

/**
 * Autofill shipment address form from user selection.
 *
 * @param type
 *   Field prefix used to identify the address.
 * @param json_address
 *   JSON object of address data.
 */
function apply_address(type, json_address) {
  //if (json_address != "0") {
    eval("var address = " + json_address +";");
    $('#edit-' + type + '-first-name').val(address.first_name);
    $('#edit-' + type + '-last-name').val(address.last_name);
    $('#edit-' + type + '-phone').val(address.phone);
    $('#edit-' + type + '-company').val(address.company);
    $('#edit-' + type + '-street1').val(address.street1);
    $('#edit-' + type + '-street2').val(address.street2);
    $('#edit-' + type + '-city').val(address.city);
    $('#edit-' + type + '-postal-code').val(address.postal_code);

    if ($('#edit-' + type + '-country').val() != address.country) {
      $('#edit-' + type + '-country').val(address.country);
      try {
        uc_update_zone_select('edit-' + type + '-country', address.zone);
      }
      catch (err) {}
    }

    $('#edit-' + type + '-zone').val(address.zone);
  //}
}
