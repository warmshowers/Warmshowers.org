/*global Drupal: true, jQuery: true */
/*jslint indent: 2 */

/*
 * Advanced Ranking Poll
 * Handles behavior of Ranking polls.
*/
(function ($) {
  'use strict';

  // storing identifiers arrays to enable more than one ranking poll to render
  // properly if more than one is displayed on the same page.
  var ids          = [],
    draggable_ids  = [],
    totals         = {},
    currentIndices = {};

  Drupal.behaviors.advpollModule = {
    attach: function (context, settings) {
      var i,
        len,
        formID,
        value,
        tableDrag;

      // instant run-off and borda drag/drop
      $('.advpoll-ranking-draggable:not(".advpoll-processed")').each(function (i) {
        var $this = $(this),
          nid = $this.attr('data-nid');

        if (nid.length > 0) {
          Drupal.advpoll.draggableSetup(nid);
          $this.addClass('advpoll-processed');
          $('td.advpoll-draggable-weight select').css('display', 'none');
        }

      });

      // instant run-off and borda normal
      $('.advpoll-ranking-table-wrapper:not(".advpoll-processed")').each(function (i) {
        var $this = $(this),
          nid     = $this.attr('data-nid');

        if (nid.length > 0) {
          Drupal.advpoll.rankingSetup(nid);
          $this.addClass('advpoll-processed');
        }

      });

    }

  };

  // namespace
  Drupal.advpoll = Drupal.advpoll || {};

  /**
   * Get rid of irritating tabledrag messages
   */
  Drupal.theme.tableDragChangedWarning = function () {
    return [];
  };

  Drupal.theme.prototype.tableDragIndentation = function () {
    return [];
  };

  Drupal.theme.prototype.tableDragChangedMarker = function () {
    Drupal.advpoll.draggableUpdate();
    Drupal.advpoll.updateRankingTable();
    return [];
  };

  Drupal.advpoll.setup = function (value) {
    ids.push(value);
  };

  Drupal.advpoll.draggableUpdate = function () {
    var i,
      j,
      len,
      draggable_table,
      rows,
      $row;

    for (i = 0, len = draggable_ids.length; i < len; i += 1) {
      draggable_table = $('#advpoll-ranking-draggable-form-' + draggable_ids[i] + ' .advpoll-ranking-draggable');
      rows = $(draggable_table).find('tbody tr').length;

      for (j = 1; j <= rows; j += 1) {
        $row = $('#advpoll-ranking-draggable-form-' + draggable_ids[i] + ' table tbody tr:nth-child(' + j + ')');

        // update the select menu
        $row.find("select option[value='" + (j) + "']").attr('selected', 'selected');

        // remove attributes from the elements that aren't selected
        $row.find("select option[value!='" + (j) + "']").removeAttr('selected');

      }
    }
  };

  /*
   Initialization converts each labeled select item into a list item.
   Also creating add and remove links that enable the list items to be
   moved back and forth between the list and the table
 */
  Drupal.advpoll.rankingInitialize = function (value) {

    var formID, tableID;

    formID = '#advpoll-ranking-form-' + value;
    tableID = '#advpolltable-' + value;
    totals[value] = $(formID + ' ' + tableID + ' tbody tr').length;
    currentIndices[value] = 0;

    $(formID + ' ' + tableID + ' tfoot tr.submit-row td').append($('.advpoll-ranking-wrapper ' + formID + ' .form-submit'));
    $(formID + ' a.remove').css('display', 'none');
    $(formID + ' li.selectable select').css('display', 'none');

    /**
     * Replaces the wrapping element of the supplied jQuery DOM collection
     * with a wrapper of your choice.
     * 
     * NOTE: This will remove any events bound to the original parent element.
     *
     * NOTE: Moving around <li> elements in the DOM without their accompanying <ul> is
     * semantically incorrect, and may cause rendering errors. This function was made
     * to solve this problem.
     * 
     * @param  {object} $content jQuery collection
     * @param  {string} type The element markup you want to use as the replacement wrapper.
     * @return {object} newly wrapped jQuery collection.
     */
    function convertElement($content, type) {
      var $contents = $content.contents().clone(true);

      $contents = $contents.wrapAll(type).parent();
      $content.remove();

      return $contents;
    }

    // adding click events to add and remove links
    $(formID + ' .selectable').each(function (i) {
      var $this = $(this),
        $remove = $this.find('a.remove'),
        $add    = $this.find('a.add');

      $remove.bind('click', function () {
        var $removeButton = $(this);
        $removeButton.css('display', 'none');
        $removeButton.siblings('a.add').css('display', '');
        $this = convertElement($this, '<li class="selectable"></li>');
        $(formID + ' ul.selectable-list').append($this);
        currentIndices[value] -= 1;
        Drupal.advpoll.updateRankingTable();
        return false;
      });

      $add.bind('click', function () {
        if (totals[value] - currentIndices[value]) {
          var $addButton = $(this);
          $addButton.css('display', 'none');
          $addButton.siblings('a.remove').css('display', '');
          $this = convertElement($this, '<div class="selectable"></div>');
          $(formID + ' ' + tableID + ' tbody td').eq(currentIndices[value]).append($this);
          currentIndices[value] += 1;
          Drupal.advpoll.updateRankingTable();
        }
        return false;
      });

    });
  };

  /*
   * Update ranking table so that if there are items removed, items reorder
   * properly into the available rows if one is removed.
   */
  Drupal.advpoll.updateRankingTable = function () {
    var i,
      len,
      value,
      formID,
      tableID,
      votes;

    for (i = 0, len = ids.length; i < len; i += 1) {
      value   = ids[i];
      formID  = '#advpoll-ranking-form-' + value;
      tableID = '#advpolltable-' + value;
      votes   = totals[value] - currentIndices[value];

      // clear all select lists that are not currently in the table.
      $(formID + ' .selectable').each(function (j) {
        $(this).find("select option[value!='" + (j + 1) + "']").removeAttr('selected');
      });

      // cycle through items that have been added to the table
      $(tableID + ' td.advpoll-weight .selectable').each(function (j) {
        var $item = $(this);

        // make sure the value in the select list matches the index of the list item
        $item.find("select option[value='" + (j + 1) + "']").attr('selected', 'selected');
        $(tableID + ' td.advpoll-weight').each(function (k) {
          var td = $(this);
          // the indexes match, so we'll move the item to its matching td index to
          // ensure that it visually appears to be in the correct position in
          // the table
          if (k === j) {
            td.append($item);
          }
        });

      });

      if (votes < 1) {
        $(formID + ' ul.selectable-list li.selectable a.add').css('display', 'none');
      } else {
        $(formID + ' ul.selectable-list li.selectable a.add').css('display', '');
      }

      // update counter in table footer
      $(formID + ' ' + tableID + ' tfoot tr.message td').empty().append('<p>' + Drupal.t('Votes remaining: ') + ' ' + votes + '</p>');

    }
  };

  Drupal.advpoll.rankingSetup = function (value) {
    ids.push(value);
    Drupal.advpoll.rankingInitialize(value);
    Drupal.advpoll.updateRankingTable();
  };

  Drupal.advpoll.draggableSetup = function (value) {
    draggable_ids.push(value);
    Drupal.advpoll.draggableUpdate();
  };

}(jQuery));
