/* 
 * Advanced Ranking Poll
 * Handles behavior of Ranking polls.
 */

(function ($) {
    
  var currentIndex = 0;
  var totalItems = 0;
    
  Drupal.behaviors.advpollModule = {
    attach: function (context, settings) {
      if (Drupal.settings){
        // get unique node identifier - supports multiple polls on a single page.
        
        var formID = '#advpoll-ranking-form-'+$('.advpoll-identity').attr('nid');
        // only rebuild draggable form if it doesn't yet exist'
        if ($(formID+' ul.selectable-list').length < 1) {
          currentIndex = 0;
          $('.advpoll-ranking-wrapper '+formID+' #advpolltable').css('display', 'block');
          $('.advpoll-ranking-wrapper '+formID+' select').css('display', 'none');
          $('.advpoll-ranking-wrapper '+formID+' input.form-text').css('visibility', 'hidden');
          $('.advpoll-ranking-wrapper '+formID+' label').append('<a href="" class="vote add">'+Drupal.t('Add')+' ></a>');
          $('.advpoll-ranking-wrapper '+formID+' .form-type-select').wrap('<li class="selectable" />');
          $('.advpoll-ranking-wrapper '+formID+' .form-type-textfield').wrap('<li class="selectable" />');
          $('.advpoll-ranking-wrapper '+formID+' li').wrapAll('<ul class="selectable-list" />');
          $('.advpoll-ranking-wrapper '+formID+' #advpolltable').append('<tfoot><tr class="submit-row"><td></td></tr><tr class="message"><td></td></tr></tfoot>');
          $('.advpoll-ranking-wrapper '+formID+' #advpolltable tfoot tr.submit-row td').append($('.advpoll-ranking-wrapper '+formID+' .form-submit'));
          $('.advpoll-ranking-wrapper '+formID+' li a').each(function(index){
            $(this).data('index', index);
          });                              
                
          // how many possible choices can user make?
          totalItems = $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody tr').length;
                
          // these calls are checking to see if form has been returned with selected
          // values - need to place them back into the list in the right order if so.
          $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li').each(function(index) {
            if($(this).find('select').length) {
              var select = $(this).find('select');
              var selected = $(".advpoll-ranking-wrapper "+formID+" #"+select.attr('id')+" option[selected = selected]");
                        
              if (select && selected.length) {
                if (selected.val() > 0) {
                  var element = $(selected).parents('li');
                  $(element).find('a.vote').removeClass('add').addClass('remove').html('(x)');
                  $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody td').eq(selected.val()-1).append(element);
                  $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody tr').eq(selected.val()-1).css('visibility', 'visible');
                  currentIndex++;
                }
              } 
            } else {
                            
              var input = $(".advpoll-ranking-wrapper "+formID+" input[name='write_in_weight']").attr('value');
              if (input > 0) {
                var element = $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li .form-item-write-in');
                element = element.parents('li');
                $(element).find('a.vote').removeClass('add').addClass('remove').html('(x)');
                $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody td').eq(input-1).append(element);
                $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody tr').eq(input-1).css('visibility', 'visible');
                currentIndex++;
                                
              }
            }                    
          });
                                
          Drupal.advpollUpdateEvents();

        }      

        if (Drupal.tableDrag) {
          tableDrag = Drupal.tableDrag.advpolltable;
              

          // Add a handler for when a row is swapped.
          tableDrag.row.prototype.onSwap = function (swappedRow) {

          };

          // Add a handler so when a row is dropped, update fields dropped into new regions.
          tableDrag.onDrop = function () {                    
            Drupal.advpollUpdateSelect();
            return true;
          };

        }
            
      } 
    }
        
  };
    
  // called when an item is added or removed from the list or upon initialization
  Drupal.advpollUpdateEvents =  function () {
    var formID = '#advpoll-ranking-form-'+$('.advpoll-identity').attr('nid');
    Drupal.advpollRemoveEvents();
    $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li a.add').bind('click', function(){
      var element = $(this).parents('li');
      if (currentIndex < totalItems) {
        $(this).removeClass('add').addClass('remove').html('(x)');
      }
      $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody td').eq(currentIndex).append(element);
      $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody tr').eq(currentIndex).css('visibility', 'visible');
      currentIndex++;
      Drupal.advpollUpdateEvents();
      return false;
    });


    $('.advpoll-ranking-wrapper '+formID+' td a.remove').bind('click', function(){
      var element = $(this).parents('li');
      var select = element.find('select');
      $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list').append(element);
      $(this).removeClass('remove').addClass('add').html(Drupal.t('Add >'));
            
      $(".advpoll-ranking-wrapper "+formID+" #"+select.attr('id')+" option[value='0']").attr('selected', 'selected');
      currentIndex--;
      // items are removed so reweight them in the list.
      Drupal.advpollReorderChoices();
      Drupal.advpollUpdateEvents();
      return false;
    });
    Drupal.advpollUpdateCount();

  }

  // called when items are dragged in selected list.
  Drupal.advpollReorderChoices = function() {
    var formID = '#advpoll-ranking-form-'+$('.advpoll-identity').attr('nid');
    var choices = [];
        
    $('.advpoll-ranking-wrapper '+formID+' tr li').each(function() {
      choices.push($(this));
    });
       
    for (var i = 0; i < choices.length; i++) {
      $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody td').eq(i).append(choices[i]);
    }
  }
    
  // Called to ensure that we never bind the click events multiple times.
  Drupal.advpollRemoveEvents =  function () {
    var formID = '#advpoll-ranking-form-'+$('.advpoll-identity').attr('nid');
    $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li a.add').unbind('click');
    $('.advpoll-ranking-wrapper '+formID+' td a.remove').unbind('click');
    Drupal.advpollUpdateSelect();
  }
   
  // Update markup and field values when items are rearranged.
  Drupal.advpollUpdateSelect = function() {
    var formID = '#advpoll-ranking-form-'+$('.advpoll-identity').attr('nid');
    $('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody tr').each(function(index) {
      if ($(this).find('select').length) {
        $(this).css('visibility', 'visible');

        var select = $(this).find('select');
        $(".advpoll-ranking-wrapper "+formID+" #"+select.attr('id')+" option[value='"+(index + 1)+"']").attr('selected', 'selected');
                
      } else if ($(this).find('input').length) {
        $(this).css('visibility', 'visible');
        $(this).find('input').css({
          visibility: 'visible', 
          position: 'relative'
        });
        $(".advpoll-ranking-wrapper "+formID+" input[name='write_in_weight']").attr('value', (index+1));
      } else {
        $(this).css('visibility', 'hidden');
      }
    });
        
    if ($('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li input').length) {
      $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li input').css({
        visibility: 'hidden', 
        position: 'absolute'
      });
      $(".advpoll-ranking-wrapper "+formID+" input[name='write_in_weight']").attr('value', 0);
    }
        
    // if the user has selected the maximum number of votes, hide the add buttons
    if ($('.advpoll-ranking-wrapper '+formID+' #advpolltable tbody li').length >= totalItems) {
      $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li a').css('visibility', 'hidden');
    } else {
      $('.advpoll-ranking-wrapper '+formID+' ul.selectable-list li a').css('visibility', 'visible');            
    }
  }
   
  Drupal.advpollUpdateCount = function () {
    var formID = '#advpoll-ranking-form-'+$('.advpoll-identity').attr('nid');
    var votes = totalItems - currentIndex;
    $('.advpoll-ranking-wrapper '+formID+' #advpolltable tfoot tr.message td').empty().append('<p>'+Drupal.t('Votes remaining:')+' '+votes+'</p>');   
  }
   
  /**
   * Get rid of irritating tabledrag messages
   */
  Drupal.theme.tableDragChangedWarning = function () {
    return [];
  }
  
  Drupal.theme.prototype.tableDragIndentation = function () {
    return [];
  };

  Drupal.theme.prototype.tableDragChangedMarker = function () {
    return [];
  };   
    
})(jQuery);
