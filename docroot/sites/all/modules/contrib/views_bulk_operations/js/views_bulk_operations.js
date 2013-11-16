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

  // Adjust selection and update server.
  var updateSelection = function(selectall, selection) {
    selection = selection || {};
    selection.selectall = Number(selectall);

    // Adjust form value.
    $('input#edit-objects-selectall', $form).val(Number(selectall > 0));

    // Adjust UI.
    $('.views-field-select-all input:radio#' + (selectall > 0 ? 'select-all-pages' : 'select-this-page'), $form).attr('checked', 'checked');
    $('.views-field-select-all span.select', $form)[$('th.select-all input:checkbox', $table).is(':checked') ? 'show' : 'hide']();
    
    // Update selection on server.
    if (Drupal.settings.vbo[form_id].options.preserve_selection) {
      $.post(
        Drupal.settings.vbo[form_id].ajax_select, 
        { 
          view_name: Drupal.settings.vbo[form_id].view_name, 
          view_id: Drupal.settings.vbo[form_id].view_id, 
          selection: JSON.stringify(selection)
        },
        function(data) {
          var count = data.selectall ? Drupal.settings.vbo[form_id].total_rows - data.unselected : data.selected;
          $('.views-field-select-all span.count', $form).text(count);
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
      $('.views-field-select-all span.count', $form).text(count);
    }
  }

  // Handle select-all checkbox.
  $('th.select-all', $table).click(function() {
    var selection = {};
    var checked = $('input:checkbox', this).attr('checked');
    $('input:checkbox.select', $form).each(function() {
      selection[this.value] = checked;
    });
    setTimeout(function() {
      updateSelection(false, selection);
    }, 1);
  });

  // Handle select-all-pages button.
  $('.views-field-select-all span.select input:radio', $form).click(function() {
    updateSelection($(this).val());
  });

  // Handle clear-selection button.
  $('.views-field-select-all input#clear-selection', $form).click(function() {
    $('th.select-all input:checkbox', $table).attr('checked', false);
    $('input:checkbox.select', $form).attr('checked', false).each(function() {
      $(this).parents('tr:first').removeClass('selected');
    });
    updateSelection(-1); // reset selection
  });

  // Save the operation value.
  $('#views-bulk-operations-dropdown select', $form).change(function() {
    if (Drupal.settings.vbo[form_id].options.preserve_selection) {
      $.post(
        Drupal.settings.vbo[form_id].ajax_select, 
        {
          view_name: Drupal.settings.vbo[form_id].view_name, 
          view_id: Drupal.settings.vbo[form_id].view_id, 
          selection: JSON.stringify({'operation': this.options[this.selectedIndex].value})
        }
      );
    }
  });

  // Save the selected items.
  var $checkboxes = $('input:checkbox.select', $form).click(function() {
    $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
    var selection = {};
    selection[this.value] = this.checked;
    setTimeout(function() { // setTimeout is used to ensure that whatever events are queued to be executed will get executed before this code.
      updateSelection($('input#edit-objects-selectall', $form).val(), selection);
    }, 1);
  }).each(function() {
    $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
  });

  // Set up the ability to click anywhere on the row to select it.
  $('tr.rowclick', $form).click(function(event) {
    if (event.target.nodeName.toLowerCase() != 'input' && event.target.nodeName.toLowerCase() != 'a') {
      $('input:checkbox.select', this).each(function() {
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

  // Set up UI based on initial values.
  setTimeout(function() { // setTimeout is used to ensure that whatever events are queued to be executed will get executed before this code.
    if ($checkboxes.length == $checkboxes.filter(':checked').length) {
      $('th.select-all input:checkbox', $table).attr('checked', true);
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
