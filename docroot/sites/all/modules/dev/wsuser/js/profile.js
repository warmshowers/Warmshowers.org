/**
 * Profile presentation JS
 *
 * @param context
 */

Drupal.behaviors.profileBehaviors = function(context) {

    // Show hide "Become available" date based on whether
    // currently unavailable is checked or not

    var checkbox = $('#edit-notcurrentlyavailable');
    var checkboxWrapper = $('#edit-notcurrentlyavailable-wrapper');
    var dateWrapper = $('#edit-becomeavailable-wrapper');

    // Set initial conditions for checkbox wrapper and date field visibility.
    if (checkbox.is(':checked')) {
        checkboxWrapper.parent().addClass('highlight-notcurrentlyavailable-wrapper');
    }
    else {
        dateWrapper.hide();
    }

    checkbox.click(function () {
        dateWrapper.toggle(this.checked);
        if (this.checked) {
          checkboxWrapper.parent().addClass('highlight-notcurrentlyavailable-wrapper');
        }
        else {
          checkboxWrapper.parent().removeClass('highlight-notcurrentlyavailable-wrapper');
        }
    });
};
