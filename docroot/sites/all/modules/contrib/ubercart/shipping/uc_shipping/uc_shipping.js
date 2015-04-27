/**
 * @file
 * Adds autofill address functionality to shipment forms.
 */

/**
 * Autofills shipment address form from user selection.
 *
 * @param type
 *   Field prefix used to identify the address.
 * @param json_address
 *   JSON object of address data.
 */
function apply_address(type, json_address) {
  //if (json_address != "0") {
    eval("var address = " + json_address +";");
    jQuery('#edit-' + type + '-first-name').val(address.first_name);
    jQuery('#edit-' + type + '-last-name').val(address.last_name);
    jQuery('#edit-' + type + '-phone').val(address.phone);
    jQuery('#edit-' + type + '-company').val(address.company);
    jQuery('#edit-' + type + '-street1').val(address.street1);
    jQuery('#edit-' + type + '-street2').val(address.street2);
    jQuery('#edit-' + type + '-city').val(address.city);
    jQuery('#edit-' + type + '-postal-code').val(address.postal_code);

    if (jQuery('#edit-' + type + '-country').val() != address.country) {
      jQuery('#edit-' + type + '-country').val(address.country);
    }

    jQuery('#edit-' + type + '-zone').val(address.zone);
  //}
}
