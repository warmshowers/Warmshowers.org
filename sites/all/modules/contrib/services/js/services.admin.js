// $Id: resource.js,v 1.2.4.6 2009/12/14 23:29:36 boombatower Exp $
// Core: Id: resource.js,v 1.11 2009/04/27 20:19:37 webchick Exp
//(function ($) {

/**
 * Add the cool table collapsing on the testing overview page.
 */
//Drupal.behaviors.resourceMenuCollapse = {
//  attach: function (context, settings) {
Drupal.behaviors.resourceMenuCollapse = function() {
    var timeout = null;
    // Adds expand-collapse functionality.
    $('div.resource-image:not(.resource-image-processed)').addClass('resource-image-processed').each(function () {
//      direction = settings.resource[$(this).attr('id')].imageDirection;
//      $(this).html(settings.resource.images[direction]);
      direction = Drupal.settings.resource[$(this).attr('id')].imageDirection;
      $(this).html(Drupal.settings.resource.images[direction]);
    });

    // Adds group toggling functionality to arrow images.
    $('div.resource-image:not(.resource-click-processed)').addClass('resource-click-processed').click(function () {
//      var trs = $(this).parents('tbody').children('.' + settings.resource[this.id].testClass);
//      var direction = settings.resource[this.id].imageDirection;
      var trs = $(this).parents('tbody').children('.' + Drupal.settings.resource[this.id].methodClass);
      var direction = Drupal.settings.resource[this.id].imageDirection;
      var row = direction ? trs.size() - 1 : 0;

      // If clicked in the middle of expanding a group, stop so we can switch directions.
      if (timeout) {
        clearTimeout(timeout);
      }

      // Function to toggle an individual row according to the current direction.
      // We set a timeout of 20 ms until the next row will be shown/hidden to
      // create a sliding effect.
      function rowToggle() {
        if (direction) {
          if (row >= 0) {
            $(trs[row]).hide();
            row--;
            timeout = setTimeout(rowToggle, 20);
          }
        }
        else {
          if (row < trs.size()) {
            $(trs[row]).removeClass('js-hide').show();
            row++;
            timeout = setTimeout(rowToggle, 20);
          }
        }
      }

      // Kick-off the toggling upon a new click.
      rowToggle();

      // Toggle the arrow image next to the test group title.
//      $(this).html(settings.resource.images[(direction ? 0 : 1)]);
//      settings.resource[this.id].imageDirection = !direction;
      $(this).html(Drupal.settings.resource.images[(direction ? 0 : 1)]);
      Drupal.settings.resource[this.id].imageDirection = !direction;

    });
//  }
};

/**
 * Select/deselect all the inner checkboxes when the outer checkboxes are
 * selected/deselected.
 */
//Drupal.behaviors.resourceSelectAll = {
//  attach: function (context, settings) {
Drupal.behaviors.resourceSelectAll = function() {
    $('td.resource-select-all').each(function () {
      var methodCheckboxes = Drupal.settings.resource['resource-method-group-' + $(this).attr('id')].methodNames;
      var groupCheckbox = $('<input type="checkbox" class="form-checkbox" id="' + $(this).attr('id') + '-select-all" />');

      // Each time a single-method checkbox is checked or unchecked, make sure
      // that the associated group checkbox gets the right state too.
      var updateGroupCheckbox = function () {
        var checkedTests = 0;
        for (var i = 0; i < methodCheckboxes.length; i++) {
          $('#' + methodCheckboxes[i]).each(function () {
            if (($(this).attr('checked'))) {
              checkedTests++;
            }
          });
        }
        $(groupCheckbox).attr('checked', (checkedTests == methodCheckboxes.length));
      };
      $('th.select-all input').change(function(){
        updateGroupCheckbox();
      });
      // Have the single-method checkboxes follow the group checkbox.
      groupCheckbox.change(function () {
        var checked = !!($(this).attr('checked'));
        for (var i = 0; i < methodCheckboxes.length; i++) {
          $('#' + methodCheckboxes[i]).attr('checked', checked);
        }
      });

      // Have the group checkbox follow the single-method checkboxes.
      for (var i = 0; i < methodCheckboxes.length; i++) {
        $('#' + methodCheckboxes[i]).change(function () {
          updateGroupCheckbox();
        });
      }

      // Initialize status for the group checkbox correctly.
      updateGroupCheckbox();
      $(this).append(groupCheckbox);
    });
//  }
};

//})(jQuery);
