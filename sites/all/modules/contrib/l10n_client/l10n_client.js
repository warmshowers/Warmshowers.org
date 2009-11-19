// $Id: l10n_client.js,v 1.10 2009/04/15 19:10:13 goba Exp $

// Store all l10n_client related data + methods in its own object
jQuery.extend(Drupal, {
  l10nClient: new (function() {
    // Set "selected" string to unselected, i.e. -1
    this.selected = -1;
    // Keybindings
    this.keys = {'toggle':'ctrl+shift+s', 'clear': 'esc'}; // Keybindings
    // Keybinding functions
    this.key = function(pressed) {
      switch(pressed) {
        case 'toggle':
          // Grab user-hilighted text & send it into the search filter
          userSelection = window.getSelection ? window.getSelection() : document.getSelection ? document.getSelection() : document.selection.createRange().text;
          userSelection = String(userSelection);
          if(userSelection.length > 0) {
            Drupal.l10nClient.filter(userSelection);
            Drupal.l10nClient.toggle(1);
            $('#l10n-client #edit-search').focus();      
          } else {
            if($('#l10n-client').is('.hidden')) {
              Drupal.l10nClient.toggle(1);
              if(!$.browser.safari) {
                $('#l10n-client #edit-search').focus();
              }
            } else { 
              Drupal.l10nClient.toggle(0);
            }
          }
        break;
        case 'clear':
          this.filter(false);      
        break;
      }
    }
    // Toggle the l10nclient
    this.toggle = function(state) {
      switch(state) {
        case 1:
          $('#l10n-client-string-select, #l10n-client-string-editor, #l10n-client .labels .label').show();
          $('#l10n-client').height('22em').removeClass('hidden');
          $('#l10n-client .labels .toggle').text('X');
          if(!$.browser.msie) {
            $('body').css('border-bottom', '22em solid #fff');
          }
          $.cookie('Drupal_l10n_client', '1', {expires: 7, path: '/'});
        break;
        case 0:
          $('#l10n-client-string-select, #l10n-client-string-editor, #l10n-client .labels .label').hide();
          $('#l10n-client').height('2em').addClass('hidden');
          $('#l10n-client .labels .toggle').text(Drupal.t('Translate Text'));
          if(!$.browser.msie) {
            $('body').css('border-bottom', '0px');
          }
          $.cookie('Drupal_l10n_client', '0', {expires: 7, path: '/'});
        break;        
      }
    }
    // Get a string from the DOM tree
    this.getString = function(index, type) {
      return $('#l10n-client-data div:eq('+index+') .'+type).text();
    }
    // Set a string in the DOM tree
    this.setString = function(index, data) {
      $('#l10n-client-data div:eq('+index+') .target').text(data);
    }
    // Filter the the string list by a search string
    this.filter = function(search) {
      if(search == false || search == '') {
        $('#l10n-client #search-filter-clear').focus();
        $('#l10n-client-string-select li').show();
        $('#l10n-client #edit-search').val('');
        $('#l10n-client #edit-search').focus();
      } else {
        if(search.length > 0) {
          $('#l10n-client-string-select li').hide();
          $('#l10n-client-string-select li:contains('+search+')').show();
          $('#l10n-client #edit-search').val(search);
        }
      }
    }
  })
});

// Attaches the localization editor behavior to all required fields.
Drupal.behaviors.l10nClient = function (context) {

  switch($.cookie('Drupal_l10n_client')) {
    case '1':
      Drupal.l10nClient.toggle(1);
    break;
    default:
      Drupal.l10nClient.toggle(0);
    break;
  }
  
  // If the selection changes, copy string values to the source and target fields.
  // Add class to indicate selected string in list widget.
  $('#l10n-client-string-select li').click(function() {
    $('#l10n-client-string-select li').removeClass('active');
    $(this).addClass('active');
    var index = $('#l10n-client-string-select li').index(this);

    $('#l10n-client-string-editor .source-text').text(Drupal.l10nClient.getString(index, 'source'));
    $('#l10n-client-form #edit-target').val(Drupal.l10nClient.getString(index, 'target'));

    Drupal.l10nClient.selected = index;
  });

  // When l10n_client window is clicked, toggle based on current state.
  $('#l10n-client .labels .toggle').click(function() {
    if($('#l10n-client').is('.hidden')) {
      Drupal.l10nClient.toggle(1);
    } else { 
      Drupal.l10nClient.toggle(0);
    }
  });

  // Copy source text to translation field on button click.
  $('#l10n-client-form #edit-copy').click(function() {
    $('#l10n-client-form #edit-target').val($('#l10n-client-string-editor .source-text').text());
  });

  // Clear translation field on button click.
  $('#l10n-client-form #edit-clear').click(function() {
    $('#l10n-client-form #edit-target').val('');
  });

  // Register keybindings using jQuery hotkeys
  if($.hotkeys) {
    $.hotkeys.add(Drupal.l10nClient.keys['toggle'], function(){Drupal.l10nClient.key('toggle')});
    $.hotkeys.add(Drupal.l10nClient.keys['clear'], {target:'#l10n-client #edit-search', type:'keyup'}, function(){Drupal.l10nClient.key('clear')});
  }
  
  // Custom listener for l10n_client livesearch
  $('#l10n-client #edit-search').keyup(function(key) {
    Drupal.l10nClient.filter($('#l10n-client #edit-search').val());
  });

  // Clear search
  $('#l10n-client #search-filter-clear').click(function() {
    Drupal.l10nClient.filter(false);
  });

  // Send AJAX POST data on form submit.
  $('#l10n-client-form').submit(function() {
    $.ajax({
      type: "POST",
      url: $('#l10n-client-form').attr('action'),
      // Send source and target strings.
      data: 'source=' + Drupal.encodeURIComponent($('#l10n-client-string-editor .source-text').text()) +
            '&target=' + Drupal.encodeURIComponent($('#l10n-client-form #edit-target').val()) +
            '&form_token=' + Drupal.encodeURIComponent($('#l10n-client-form #edit-l10n-client-form-form-token').val()),
      success: function (data) {
        // Store string in local js
        Drupal.l10nClient.setString(Drupal.l10nClient.selected, $('#l10n-client-form #edit-target').val());

        // Figure out the display of the new translation in the selection list.
        var newTranslationDisplay = '';
        var newTranslation = $('#l10n-client-form #edit-target').val();
        var newTranslationStripped = newTranslation.replace(/<\/?[^<>]+>/gi, '')
                                                   .replace(/&quot;/g, '"')
                                                   .replace(/&lt;/g, "<")
                                                   .replace(/&gt;/g, ">")
                                                   .replace(/&amp;/g, "&");
        if (newTranslationStripped.length == 0) {
          // Only contains HTML tags (edge case). Keep the whole string.
          // HTML tags will show up in the selector, but that is normal in this case.
          newTranslationDisplay = newTranslation;
        }
        else if (newTranslationStripped.length > 81) {
          // Long translation, strip length to display only first part.
          // We strip at 78 chars and add thre dots, if the total length is
          // above 81.
          newTranslationDisplay = newTranslationStripped.substr(0, 78) + '...';
        }
        
        // Mark string as translated.
        $('#l10n-client-string-select li').eq(Drupal.l10nClient.selected).removeClass('untranslated').removeClass('active').addClass('translated').text(newTranslationDisplay);

        // Empty input fields.
        $('#l10n-client-string-editor .source-text').html('');
        $('#l10n-client-form #edit-target').val('');

      },
      error: function (xmlhttp) {
        alert(Drupal.t('An HTTP error @status occured.', { '@status': xmlhttp.status }));
      }
    });
    return false;
  });
  
};
