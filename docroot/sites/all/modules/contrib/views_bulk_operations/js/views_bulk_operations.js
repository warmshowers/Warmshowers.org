(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};

Drupal.behaviors.vbo = function(context) {
  // Force Firefox to reload the page if Back is hit.
  // https://developer.mozilla.org/en/Using_Firefox_1.5_caching
  window.onunload = function(){}

  // Prepare VBO forms for processing.
  $('form.views-bulk-operations-form', context)
    .not('.views-bulk-operations-form-step-2, .views-bulk-operations-form-step-3')
    .each(Drupal.vbo.prepareAction)
    .each(Drupal.vbo.prepareSelectors);
}

Drupal.vbo.prepareSelectors = function() {
  var $form = $(this);
  var form_id = $form.attr('id');
  var $table = $('table.views-table', $form);
  var queue = new Array();
  var queueProcess = false;
  var strings = { 'selectAll': Drupal.t('Select all items in this table'), 'selectNone': Drupal.t('Deselect all items in this table') }; 
  var lastChecked, rowShiftKey;

  // Do not add a "Select all" checkbox if there are no rows with checkboxes in the table.
  if ($('td input:checkbox', $table).size() == 0) {
    return;
  }

  var updateSelectAll = function(state) {
    $('th.vbo-select-all input:checkbox', $table).each(function() {
      $(this).attr('title', state ? strings.selectNone : strings.selectAll);
      this.checked = state;
    });
  };

  // Adjust selection and update server.
  var updateSelection = function(selectall, selection, recursive) {
    selection = selection || {};
    selection.selectall = Number(selectall);
    recursive = recursive || false;

    // Adjust form value.
    $('input#edit-objects-selectall', $form).val(Number(selectall > 0));

    // Adjust UI.
    $('.views-field-select-all input:radio#' + (selectall > 0 ? 'select-all-pages' : 'select-this-page'), $form).attr('checked', 'checked');
    $('.views-field-select-all span.select', $form)[$('th.vbo-select-all input:checkbox', $table).is(':checked') ? 'show' : 'hide']();
    
    // Update selection on server.
    if (Drupal.settings.vbo[form_id].options.preserve_selection) {
      if (queueProcess) {
        // Already processing a request: just add to queue for now.
        queue.push({'selectall': selectall, 'selection': selection});
        return;
      }
      queueProcess = true;

      // Disable the submit button(s).
      if (!recursive) {
        $('#views-bulk-operations-select input:submit', $form).attr('disabled', 'disabled').filter(':last').after('<span class="views-throbbing">&nbsp</span>');
      }

      $.post(
        Drupal.settings.vbo[form_id].ajax_select, 
        { 
          view_name: Drupal.settings.vbo[form_id].view_name, 
          view_id: Drupal.settings.vbo[form_id].view_id, 
          selection: JSON.stringify(selection)
        },
        function(data) {
          var count = data.selectall ? Drupal.settings.vbo[form_id].total_rows - data.unselected : data.selected;
          $('.views-field-select-all span.selected', $form).text(count);

          queueProcess = false;
          if (queue.length > 0) {
            // Resume queue if it's not empty.
            var elm = queue.shift();
            updateSelection(elm.selectall, elm.selection, true);
          }
          else {
            // Enable the submit button(s).
            $('#views-bulk-operations-select input:submit', $form).removeAttr('disabled');
            $('#views-bulk-operations-select span.views-throbbing', $form).remove();
          }
        },
        'json'
      );
    }
    else {
      // Adjust item count for local page.
      var count;
      switch (Number(selectall)) {
        case -1:
          count = 0;
          break;
        case 0:
          count = $checkboxes.filter(':checked').length;
          break;
        case 1:
          count = Drupal.settings.vbo[form_id].total_rows - $checkboxes.filter(':not(:checked)').length;
          break;
        default:
          console.log('[vbo] Unknown value ' + selectall + ' when refreshing item count.');
          break;
      }
      $('.views-field-select-all span.selected', $form).text(count);
    }
  }

  // Handle select-all checkbox.
  $('th.vbo-select-all', $table).prepend($('<input type="checkbox" class="form-checkbox" />').attr('title', strings.selectAll)).click(function(e) {
    if ($(e.target).is('input:checkbox')) {
      var selection = {};
      // Loop through all checkboxes and set their state to the select all checkbox' state.
      $checkboxes.each(function() {
        this.checked = e.target.checked;
        // Either add or remove the selected class based on the state of the check all checkbox.
        $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
        selection[this.value] = this.checked;
      });
      // Update the title and the state of the check all box.
      updateSelectAll(e.target.checked);
      setTimeout(function() {
        updateSelection(false, selection);
      }, 1);
    }
  });

  // Handle select-all-pages button.
  $('.views-field-select-all span.select input:radio', $form).click(function() {
    updateSelection($(this).val());
  });

  // Handle clear-selection button.
  $('.views-field-select-all input#clear-selection', $form).click(function() {
    updateSelectAll(false);
    $checkboxes.attr('checked', false).each(function() {
      $(this).parents('tr:first').removeClass('selected');
    });
    updateSelection(-1); // reset selection
  });

  // Save the operation value.
  $('#views-bulk-operations-dropdown select', $form).change(function() {
    var selection = {}
    selection['operation'] = this.options[this.selectedIndex].value;
    updateSelection($('input#edit-objects-selectall', $form).val(), selection);
  });

  // Save the selected items.
  var $checkboxes = $('input:checkbox.select', $form).click(function(e) {
    // Either add or remove the selected class based on the state of the check all checkbox.
    $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');

    // If this is a shift click, we need to highlight everything in the range.
    // Also make sure that we are actually checking checkboxes over a range and
    // that a checkbox has been checked or unchecked before.
    if ((e.shiftKey || (typeof(e.shiftKey) == 'undefined' && rowShiftKey)) && lastChecked && lastChecked != e.target) {
      // We use the checkbox's parent TR to do our range searching.
      tableSelectRange($(e.target).parents('tr')[0], $(lastChecked).parents('tr')[0], e.target.checked);
    }

    // If all checkboxes are checked, make sure the select-all one is checked too, otherwise keep unchecked.
    updateSelectAll(($checkboxes.length == $checkboxes.filter(':checked').length));

    // Keep track of the last checked checkbox.
    lastChecked = e.target;

    var selection = {};
    selection[this.value] = this.checked;
    setTimeout(function() { // setTimeout is used to ensure that whatever events are queued to be executed will get executed before this code.
      updateSelection($('input#edit-objects-selectall', $form).val(), selection);
    }, 1);
  }).each(function() {
    $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
  });

  // Set up the ability to click anywhere on the row to select it.
  $('tr.rowclick', $form).click(function(e) {
    if (e.target.nodeName.toLowerCase() != 'input' && e.target.nodeName.toLowerCase() != 'a') {
      rowShiftKey = e.shiftKey;
      $('input:checkbox.select', this).each(function () {
        var checked = this.checked;
        // trigger() toggles the checkmark *after* the event is set,
        // whereas manually clicking the checkbox toggles it *beforehand*.
        // that's why we manually set the checkmark first, then trigger the
        // event (so that listeners get notified), then re-set the checkmark
        // which the trigger will have toggled. yuck!
        this.checked = !checked;
        $(this).trigger('click');
        this.checked = !checked;
      });
    }
  });

  var tableSelectRange = function(from, to, state) {
    // We determine the looping mode based on the the order of from and to.
    var mode = from.rowIndex > to.rowIndex ? 'previousSibling' : 'nextSibling';

    // Traverse through the sibling nodes.
    var selection = {};
    for (var i = from[mode]; i; i = i[mode]) {
      // Make sure that we're only dealing with elements.
      if (i.nodeType != 1) {
        continue;
      }

      // Either add or remove the selected class based on the state of the target checkbox.
      $(i)[ state ? 'addClass' : 'removeClass' ]('selected');
      $('input:checkbox', i).each(function() {
        this.checked = state;
        selection[this.value] = this.checked;
      });

      if (to.nodeType) {
        // If we are at the end of the range, stop.
        if (i == to) {
          break;
        }
      }
      // A faster alternative to doing $(i).filter(to).length.
      else if (jQuery.filter(to, [i]).r.length) {
        break;
      }
    }

    setTimeout(function() { // setTimeout is used to ensure that whatever events are queued to be executed will get executed before this code.
      updateSelection($('input#edit-objects-selectall', $form).val(), selection);
    }, 1);
  };


  // Set up UI based on initial values.
  setTimeout(function() { // setTimeout is used to ensure that whatever events are queued to be executed will get executed before this code.
    if ($checkboxes.length == $checkboxes.filter(':checked').length) {
      updateSelectAll(true);  
      $('.views-field-select-all span.select', $form).show();
    }
  }, 1);
}

Drupal.vbo.prepareAction = function() {
  // Skip if no view is Ajax-enabled.
  if (typeof(Drupal.settings.views) == "undefined" || typeof(Drupal.settings.views.ajaxViews) == "undefined") return;

  var $form = $(this);
  $.each(Drupal.settings.views.ajaxViews, function(i, view) {
    if (view.view_name == Drupal.settings.vbo[$form.attr('id')].view_name) {
      var action = $form.attr('action');
      var params = {};
      var query = action.replace(/.*?\?/, '').split('&');
      var cleanUrl = true, replaceAction = false;
      $.each(query, function(i, str) {
        var element = str.split('=');
        if (element[0] == 'view_path') {
          action = Drupal.settings.vbo[$form.attr('id')].view_path;
          replaceAction = true;
        }
        else if (element[0] == 'q') {
          cleanUrl = false;
        }
        else if (typeof(view[element[0]]) == 'undefined' && typeof(element[1]) != 'undefined') {
          params[element[0]] = element[1];
        }
      });
      if (replaceAction) {
        params = $.param(params);
        if (cleanUrl) {
          // Do nothing
        }
        else {
          params = 'q=' + action + (params.length > 0 ? '&' + params : '');
          action = Drupal.settings.basePath;
        }
        $form.attr('action', action + (params.length > 0 ? '?' + params : ''));
      }
    }
  });
}

Drupal.vbo.ajaxViewResponse = function(target, response) {
  $.each(Drupal.settings.vbo, function(form_dom_id, settings) {
    if (settings.form_id == response.vbo.form_id) {
      Drupal.settings.vbo[form_dom_id].view_id = response.vbo.view_id;
    }
  });
}

// END jQuery
})(jQuery);

