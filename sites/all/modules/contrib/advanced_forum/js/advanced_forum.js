(function ($) {
  Drupal.advanced_forum = Drupal.advanced_forum || {};

  Drupal.behaviors.advanced_forum = function(context) {
    // Retrieve collapsed status from stored cookie.
    // cookie format is: "/page1=1,2,3/page2=1,4,5/page3=5,6,1"
    var cookieString = 'Drupal.advanced_forum.collapsed=';
    if (document.cookie.length > 0) {
      offset = document.cookie.indexOf(cookieString);
      if (offset != -1) {
        offset += cookieString.length;
        var end = document.cookie.indexOf(';', offset);
        if (end == -1) {
          end = document.cookie.length;
        }
        var cookie = unescape(document.cookie.substring(offset, end));
      }
    }
    var pages = cookie ? cookie.split('/') : new Array();

    // Create associative array where key=page path and value=comma-separated list of collapsed forum ids
    Drupal.advanced_forum.collapsed_page = new Array();
    if (pages) {
      for (x in pages) {
        tmp = pages[x].split('=');
        Drupal.advanced_forum.collapsed_page[tmp[0]] = tmp[1].split(',');
      }
    }

    // Get data for current page
    Drupal.advanced_forum.collapsed_current = Drupal.advanced_forum.collapsed_page[encodeURIComponent(window.location.pathname)];
    if (!Drupal.advanced_forum.collapsed_current) Drupal.advanced_forum.collapsed_current = new Array();
    var handleSpan = $('span.advanced-forum-toggle', context);

    // Set initial collapsed state
    handleSpan.not('.advanced-forum-collapsible-processed').addClass('advanced-forum-collapsible-processed').each(Drupal.advanced_forum.init);

    // Set click handler
    handleSpan.addClass('clickable').click(function() {

      // Get forum id
      var id = $(this).attr('id').split('-')[2];

      if ($(this).hasClass('advanced-forum-collapsed')) {
        Drupal.advanced_forum.expand(id);
        Drupal.advanced_forum.collapsed_current.splice($.inArray(id, Drupal.advanced_forum.collapsed_current),1);
      }
      else {
        Drupal.advanced_forum.collapse(id);
        Drupal.advanced_forum.collapsed_current.push(id);
      }

      // Store updated status
      Drupal.advanced_forum.collapsed_page[encodeURIComponent(window.location.pathname)] = Drupal.advanced_forum.collapsed_current;

      // Build cookie string
      cookie = '';
      for (x in Drupal.advanced_forum.collapsed_page) {
        cookie += '/' + x + '=' + Drupal.advanced_forum.collapsed_page[x];
      }
      // Save new cookie
      var exp_date = new Date(2020, 0, 1);
      document.cookie = cookieString + encodeURIComponent(cookie.substr(1)) + '; expires=' + exp_date.toUTCString() + '; path=/';
    });
  };

  /**
   * Initialize and set collapsible status
   */
  Drupal.advanced_forum.init = function() {
    // get forum id
    var id = $(this).attr('id').split('-')[2];

    // Check if item is collapsed
    if ($.inArray(id, Drupal.advanced_forum.collapsed_current) > -1) {
      $(this).addClass('advanced-forum-collapsed');
      $('#forum-table-' + id).hide();
    } else {
      $(this).removeClass('advanced-forum-collapsed');
      $('#forum-table-' + id).show();
    }
  };

  /**
   * Collapse forum.
   */
  Drupal.advanced_forum.collapse = function(id) {
    $('#forum-collapsible-' + id).addClass('advanced-forum-collapsed');
    switch (Drupal.settings.advanced_forum.effect) {
      case 'slide':
        $('#forum-table-' + id).slideUp();
        break;
      case 'fade':
        $('#forum-table-' + id).fadeOut(150);
        break;
      case 'toggle':
        $('#forum-table-' + id).hide();
        break;
    }
  };

  /**
   * Expand forum.
   */
  Drupal.advanced_forum.expand = function(id) {
    $('#forum-collapsible-' + id).removeClass('advanced-forum-collapsed');
    switch (Drupal.settings.advanced_forum.effect) {
      case 'slide':
        $('#forum-table-' + id).slideDown();
        break;
      case 'fade':
        $('#forum-table-' + id).fadeIn(150);
        break;
      case 'toggle':
        $('#forum-table-' + id).show();
        break;
    }
  };

})(jQuery);
