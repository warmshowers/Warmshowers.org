// $Id: advpoll-form.js,v 1.1.2.9 2007/12/16 14:59:28 chriskennedy Exp $

if (!Drupal.advpoll) {
  Drupal.advpoll = {};
}

// Update maxchoices, called when adding and removing choices
Drupal.advpoll.maxChoices = function(numChoices) {
  var selected = $("#edit-settings-max-choices").val();
  var label = $("#edit-settings-max-choices").prev();
  // Hard-code the HTML (not clone) as .html() doesn't work for select fields in IE and Opera.
  var newMaxChoices = '<select id="edit-settings-max-choices" class="form-select" name="settings[max_choices]">';
  // Build the options
  for (var i = 0; i <= numChoices; i++) {
    var name = (i ? i : Drupal.settings.advPoll.noLimit);
    newMaxChoices += '<option ';
    // Set the option user had selected
    if (i == selected) {
      newMaxChoices += 'selected="selected" ';
    }
    newMaxChoices += 'value="'+ i +'">'+ name +'</option>';
  }
  newMaxChoices += '</select>';
  // Remove old maxchoices
  $("#edit-settings-max-choices").remove();
  
  label.after(newMaxChoices);
};

// Click event for Remove link, called on pageload and when Add choice is clicked
Drupal.advpoll.removeChoiceClick = function() {
  $("a.remove-choice").unbind().click(function() {
    var nextRemoveLink = $(this).parent().next().find(".remove-choice");
    // Set focus at next or previous Remove link.
    if (nextRemoveLink.html()) {
      nextRemoveLink[0].focus();
    }
    else {
      $(this).parent().prev().find(".remove-choice")[0].focus();
    }
    // Remove choice
    $(this).parent().remove();
    var i = 1;
    $("input.choices").prev().each(function() {
      // Give each label its correct number
      $(this).html($(this).html().replace(/\d+(?=<)/g, i++));
    });
    
    Drupal.advpoll.maxChoices(i - 1);
  });
};

// Show/hide "display write-ins" option when user checks unchecks the write-ins
// box.
Drupal.advpoll.updateWriteins = function() {
  if ($("input.settings-writeins").attr("checked")) {
    $(".edit-settings-show-writeins").show();
    $("#edit-settings-show-writeins").removeAttr("disabled");
  }
  else {
    $(".edit-settings-show-writeins").hide();
    $("#edit-settings-show-writeins").attr("disabled", "disabled");
  }
};

Drupal.advpoll.nodeFormAutoAttach = function() {
  // This code is used on the node edit page and the content-type settings page.

  // Add behavior when write-in box is (un)checked.
  Drupal.advpoll.updateWriteins();
  $("input.settings-writeins").click(Drupal.advpoll.updateWriteins);

  if ($("div.poll-form").length == 0) {
    // We're just on the settings page.
    return;
  }

  // Hide "need more choices" checkbox
  $("#more-choices").hide();
  
  // Insert Remove links
  $('<a class="remove-choice">'+ Drupal.settings.advPoll.remove +'</a>').insertAfter("input.choices");
  Drupal.advpoll.removeChoiceClick();
  
  // "Backup" of the first choice
  var newChoice = $("input.choices:first").parent().clone();

  // Keep track of the highest choice id to use.
  var highestId = 0;
  $("input.choices").each(function() {
    var id = parseInt($(this).attr("id").match(/\d+/));
    if (id > highestId) {
      highestId = id;
    }
  });
  
  $('<a class="add-choice" href="#">'+ Drupal.settings.advPoll.addChoice +'</a>').insertAfter("#more-choices").click(function() {
    var numChoices = $("input.choices").length + 1;
    highestId++;
    // If all choices are removed, use a "backup" of the first choice, else clone the first.
    newChoice = ($("input.choices:first").parent().html() ? $("input.choices:first").parent().clone() : newChoice);
    // Replace choice numbers in label, name and id with the new choice number
    newChoice.html(newChoice.html().replace(/\d+(?=<)|\d+(?=-)|\d+(?=\])/g, highestId));
    // Replace the label to use a more accurate count of choices.
    $("label", newChoice).html($("label", newChoice).html().replace(/\d+(?=<)|\d+(?=-)|\d+(?=\])/g, numChoices));
    // Clear the value, insert and fade in.
    newChoice.find("input").val("").end().insertBefore("#more-choices").fadeIn();
    // Update hidden form values
    $("#edit-changed").val($("#edit-changed").val() + 1);
    
    Drupal.advpoll.removeChoiceClick();
    Drupal.advpoll.maxChoices(numChoices);
    
    return false;
  });
};

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {  
    Drupal.advpoll.nodeFormAutoAttach();
  });
}
