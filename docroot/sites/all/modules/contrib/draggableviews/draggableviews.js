// Check js availability.
if (Drupal.jsEnabled) {
  // Start at onload-event.
  $(document).ready(draggableviews_load);
}

// Load editfield-plugin.
function draggableviews_load(){
  $("table.views-table").each( function(i) {
    var table_id = $(this).attr('id');

    if (!Drupal.settings.draggableviews[table_id]) return;

    $(this).find("tr.draggable").each( function(i) {
      var nid = $(this).find('td > .hidden_nid').attr('value');
      // append icon only if we find at least one child
      if ($("#" + table_id + " tr:has(td > ." + Drupal.settings.draggableviews[table_id].parent + "[value=" + nid + "])").size() > 0) {
        $(this).find('td:first').each( function(i) {
          $(this).append('<div class="draggableviews-expand" href="#"></div>').children('.draggableviews-expand').bind('click', function(){draggableviews_collapse(nid, table_id);});
        });
      }

      // Apply collapsed/expanded state.
      if (Drupal.settings.draggableviews[table_id]) {
        if (Drupal.settings.draggableviews[table_id].states) {
          if (Drupal.settings.draggableviews[table_id].states[nid]) {
            // When list should be collapsed..
            if (Drupal.settings.draggableviews[table_id].states[nid] == 1) {
              // ..collapse list.
              draggableviews_collapse(nid, table_id);

              // ..and set hidden field.
              draggableviews_set_state_field(nid, table_id, true);
            }
          }
        }
      }
    });

    // collapse all by default if set
    if( Drupal.settings.draggableviews.expand_default && Drupal.settings.draggableviews[table_id].expand_default == 1 ) {
      draggableviews_collapse_all(table_id);
    }
  });
}

// Expand recursively.
function draggableviews_expand(parent_id, table_id, force){
  if (force || draggableviews_get_state_field(parent_id, table_id)) {
    // show elements
    draggableviews_show(parent_id, table_id);

    // swap link to collapse link
    $("#" + table_id + " tr:has(td .hidden_nid[value="+parent_id+"])")
    .find('.draggableviews-collapse').each( function (i){
      $(this).unbind('click');
      $(this).attr('class', 'draggableviews-expand');
      $(this).bind('click', function(){ draggableviews_collapse(parent_id, table_id); });
      // set state as value of a hidden field
      draggableviews_set_state_field(parent_id, table_id, false);
    });
  }
}

// show recursively
function draggableviews_show(parent_id, table_id) {
  $("table[id='" + table_id + "'] tr:has(td ." + Drupal.settings.draggableviews[table_id].parent + "[value="+parent_id+"])").each( function (i) {
    $(this).show();
    var nid = $(this).find('td .hidden_nid').attr('value');
    if (nid) {
      draggableviews_expand(nid, table_id, false);
    }
  });
}

function draggableviews_collapse(parent_id, table_id) {
  // hide elements
  draggableviews_hide(parent_id, table_id);

  // swap link to expand link
  $("#" + table_id + " tr:has(td .hidden_nid[value=" + parent_id + "])")
  .find('.draggableviews-expand').each( function (i){
    $(this).unbind('click');
    $(this).attr('class', 'draggableviews-collapse');
    $(this).bind('click', function(){ draggableviews_expand(parent_id, table_id, true); });

    // set state as value of a hidden field
    draggableviews_set_state_field(parent_id, table_id, true);
  });
}

// hide recursively
function draggableviews_hide(parent_id, table_id) {
  $("#" + table_id + " tr:has(td ." + Drupal.settings.draggableviews[table_id].parent + "[value=" + parent_id+"])").each( function (i) {
    $(this).hide();
    var nid = $(this).find('td .hidden_nid').attr('value');
    if (nid) {
      draggableviews_hide(nid, table_id, false);
    }
  });
}

function draggableviews_collapse_all(table_id) {
  // hide elements
  $("#" + table_id + " tr:has(td ." + Drupal.settings.draggableviews[table_id].parent + "[value<>0])").each( function (i) {
    $(this).hide();
  });
  
  // swap links to expand links
  $("#" + table_id + " tr:has(td .hidden_nid)").each( function (i){
    var parent_id = $(this).find('td .hidden_nid').attr('value');
    
    $(this).find('.draggableviews-expand').each( function (i){
      // set new action and class
      $(this).unbind('click');
      $(this).attr('class', 'draggableviews-collapse');
      $(this).bind('click', function() { draggableviews_expand(parent_id, table_id); });
      
      // set collapsed/expanded state
      draggableviews_set_state_field(parent_id, table_id, true);
    });
  });
}

// save state of expanded/collapsed fields in a hidden field
function draggableviews_set_state_field(parent_id, table_id, state) {
  //build field name
  var field_name = 'draggableviews_collapsed_' + parent_id;
  
  $("table[id='" + table_id + "'] input[name='hidden_nid'][value='" + parent_id + "']")
  .parent().each( function(i) {
    var replaced = false;

    // "check" if field already exists (..by just selecting it)
    $(this).find('input[name="' + field_name + '"]').each( function(i) {
      $(this).attr('value', state ? '1' : '0');
      replaced = true;
    });
    // if no field existed yet -> create it
    if (replaced == false) {
      $(this).append('<input type="hidden" name="' + field_name + '" value="' + (state ? '1' : '0') + '" />');
    }
  });
}

// Get state of expanded/collapsed field.
function draggableviews_get_state_field(parent_id, table_id) {
  //build field name
  var field_name = 'draggableviews_collapsed_' + parent_id;

  var value = $('table[id="' + table_id + '"] input[name="' + field_name + '"]').attr('value');

  if (value == 1) return false;

  return true;
}