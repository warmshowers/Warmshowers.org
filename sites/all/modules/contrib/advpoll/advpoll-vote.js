// $Id: advpoll-vote.js,v 1.1.2.11.2.9 2010/06/25 20:12:41 mirodietiker Exp $

/**
 * Submit advpoll forms with ajax
 */
Drupal.behaviors.attachVoteAjax = function(context) {
  $("form.advpoll-vote", context).each(function() {
    var thisForm = this;
    var options = {
      dataType: "json",
      success: function(data) {
        // Remove previous messages
        $("div.messages").remove(); 
        
        // Insert response
        if (data.errors) {
          $(data.errors).insertBefore(thisForm).fadeIn();
        }
        else {
          $(thisForm).hide();
          $(data.statusMsgs).insertBefore(thisForm).fadeIn();
          $(data.response).insertBefore(thisForm);
        }

        // Re-enable the Vote button, in case there was an error message.
        $(".form-submit", thisForm).removeAttr("disabled");

      },
      beforeSubmit: function() {
        // Disable the Vote button.
        $(".form-submit", thisForm).attr("disabled", "disabled");
      }
    };
    // Tell the server we are passing the form values with ajax and attach the function
    $("input.ajax", thisForm).val(true);
    $(this).ajaxForm(options);
  });
};

Drupal.behaviors.handleWriteins = function(context) {
  $("form.advpoll-vote:not(.handleWriteins-processed)", context).addClass("handleWriteins-processed").each(function() {
    var poll = this;
    if ($(".writein-choice", poll).length == 0) {
      // No write-ins in this poll.
      return;
    }
    // Toggle display of the write-in text box for radios/checkboxes.
    $(".vote-choices input[type=radio], .vote-choices input[type=checkbox]", poll).click(function() {
      var isLast = $(this).val() == $(".vote-choices input[type=radio]:last, .vote-choices input[type=checkbox]:last", poll).val();
      var type = $(this).attr("type"); 
      // The logic here is tricky but intentional.
      if (isLast || type == "radio") {
        var showChoice = isLast && (type == "radio" || $(this).attr("checked"));
        $(".writein-choice", poll).css("display", showChoice ? "inline" : "none");
        if (showChoice) {
          $(".writein-choice", poll)[0].focus();
        }
        else {
          $(".writein-choice", poll).val("");
        }
      }
    });

    // Toggle display of the write-in text box for select boxes.
    // Fire on change() rather than click(), for Safari compatibility.
    $(".vote-choices select:last", poll).change(function() {
      var showChoice = $(this).val() > 0;
      var alreadyVisible = $(".writein-choice", poll).css("display") == "inline";
      $(".writein-choice", poll).css("display", showChoice ? "inline" : "none");
      if (!showChoice) {
        $(".writein-choice", poll).val("");
      }
      else if (!alreadyVisible) {
        $(".writein-choice", poll)[0].focus();
      }
    });
  });
};

Drupal.behaviors.rankingDragAndDrop = function(context) {
  $('form.advpoll-vote.drag-and-drop:not(.advpoll-drag-and-drop-processed)', context).addClass('advpoll-drag-and-drop-processed').each(function() {
    var mainForm = $(this);
    var formId = mainForm.attr('id');
    var $existingChoicesTable = $('.advpoll-existing-choices-table', mainForm);
    var stringRemoveFromVote = Drupal.t('(x)');
    var stringRemoveFromVoteURL = document.location.href + '/' + Drupal.t('remove-from-vote');
    var stringAddToVote = Drupal.t('add &gt;');
    var stringAddToVoteURL = document.location.href + '/' + Drupal.t('add-to-vote');
    var voteButton = $(".form-submit", mainForm);
    var newVoteButton = voteButton.clone().attr("disabled", "true").css("margin-left", "10px").css("margin-top", "4px").addClass("vote-button");
    // Remove the old vote button
    voteButton.remove();
    $('.advpoll-drag-box', mainForm).append(newVoteButton);
   // Copy write-in text field and specifically add to end of list.
    $(this).parent().parent().siblings().filter(".writein-choice").clone().insertAfter($("select.advpoll-writeins", mainForm));
    // Hide the selects.
    $("select.form-select", mainForm).css('display', 'none');//remove();
    $("select.form-select", mainForm).addClass('advpoll-choice-order');
    var maxChoices = parseInt(mainForm.find('input[name=max_choices]').val());

    // Loop through the choices and perform initial setup.
    $('.vote-choices div.form-item', $(this))
      .wrapAll('<ul class="advpoll-pending-choices-list"></ul>')
      .wrap('<li class="advpoll-choice-container"></li>').removeClass("form-item").addClass("advpoll-pending-choice").each(function() {
        // Each form element, outside of which is an <li></li>.
        $(this).parent().attr("id", "choice-" + parseInt($(this).attr("id").replace(/[^0-9]/g, "")));
        $(this).append('<a href="' + stringAddToVoteURL + '" class="advpoll-add-to-vote advpoll-choice-action-link">' + stringAddToVote + '</a>');

        function handleChoiceClick(event) {
          // Stop browser from forwarding to "#" in href.
          event.preventDefault();
          // Element that issued click.
          var $element = $(event.target);
          // Handle a pending choice or an existing choice.
          var isPending = $element.parent('.advpoll-pending-choice').size() > 0;
          var choiceType = isPending ? 'pending' : 'existing';
          var choiceNotType = isPending ? 'existing' : 'pending';
          var $choice = $element.parent('.advpoll-'+ choiceType +'-choice');
          var $pendingChoicesList = $('.advpoll-pending-choices-list', mainForm);
          var $newChoice = $choice.clone(true)
            // Migrate choice outer div.
            .removeClass('advpoll-'+ choiceType + '-choice').addClass('advpoll-'+ choiceNotType + '-choice')
            // Migrate action link.
            .find("a.advpoll-choice-action-link").html(isPending ? stringRemoveFromVote : stringAddToVote).attr('href', isPending ? stringRemoveFromVoteURL : stringAddToVoteURL).removeClass(isPending ? 'advpoll-add-to-vote' : 'advpoll-remove-from-vote').addClass(isPending ? 'advpoll-remove-from-vote' : 'advpoll-add-to-vote').end();
          // Unchained to avoid undefined parent JS error - unsure why needed.
          var $newRow = isPending ? $('<tr class="draggable advpoll-choice-container"><td class="advpoll-receive-choice"></td></tr>') : $('<li class="advpoll-receive-choice advpoll-choice-container"></li>');
          // TODO: unify these two cases into a single command.
          if (isPending) {
            $newRow.find('.advpoll-receive-choice').append($newChoice);
            $existingChoicesTable.append($newRow)
          }
          else {
            $newRow.append($newChoice);
            $pendingChoicesList.append($newRow);
            // Reset weight.
            $newRow.find('select.advpoll-choice-order').val(0);
          }
          $choice.parents('.advpoll-choice-container').remove();

        var currentOrder = 0;
        $("tr.advpoll-choice-container select.advpoll-choice-order", $existingChoicesTable).each(function() {
          if ($(this).val() != currentOrder + 1) {
            $(this).val(currentOrder + 1);
          }
          currentOrder++;
        });
        var currentChoices = currentOrder;
        // Hide drag-icon if there's only one choice in the current vote.
        if (currentChoices <= 1) {
          // TODO: take another stab at implementing this.
        }
        else {
          // Hack to have tabledrag.js parse the new table rows.
          $existingChoicesTable.removeClass('dragtable-processed');
          Drupal.attachBehaviors($existingChoicesTable);
        }
        $(".vote-status", mainForm).show().html(Drupal.t("Choices remaining: %choices", {"%choices" : maxChoices - currentChoices}));
        if (currentChoices > maxChoices) {
          // Don't allow more votes if we have hit the limit.
          $(".vote-status", mainForm).addClass("error");
          newVoteButton.attr("disabled", "true");
        }
        else {
          newVoteButton.attr("disabled", "");
          $(".vote-status", mainForm).removeClass("error");
        }

        // If we went from 0 choices to 1, enable the vote button.
        if (currentChoices == 1) {
          newVoteButton.attr("disabled", "");
        }
        else if (currentChoices == 0) {
          // Back at 0, so the user can't cast a vote.
          newVoteButton.attr("disabled", "true");
        }

        // Re-apply table-dragging to access for updated table.
        $('.tabledrag-handle', $existingChoicesTable).remove();
        $existingChoicesTable.removeClass('tabledrag-processed');
        Drupal.attachBehaviors($existingChoicesTable);
      }
      // Allow clicks to trigger adding the choice to the vote.
      $("label", this).click(handleChoiceClick);
      $("a.advpoll-choice-action-link", this).click(handleChoiceClick);

    });

    // Show the write-in box if it exists.
    if ($('.writein-choice input', mainForm).size() > 0) {
      var newInput = $(".writein-choice input", mainForm).clone().css("display", "inline");
      $(".writein-choice", mainForm).remove();
      $("li:last .advpoll-pending-choice", mainForm).append("<br />").append(newInput);
    }

    newVoteButton.click(function() {
      // Re-do tabledrag ordering so poll choice selects start at 1... lame.
      var currentOrder = 1;
      $("tr.advpoll-choice-container select.advpoll-choice-order", mainForm).each(function() {
        $(this).val(currentOrder);
        currentOrder++;
      }); 
    });
  });
};

