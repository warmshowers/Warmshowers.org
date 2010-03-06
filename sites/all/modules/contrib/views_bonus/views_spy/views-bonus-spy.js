// $Id: views-bonus-spy.js,v 1.2 2009/07/16 15:45:05 neclimdul Exp $

// Plugin based on http://jqueryfordesigners.com/simple-jquery-spy-effect/
(function ($) {
  $.fn.simpleSpy = function (limit, interval, fadespeed, slidespeed, autopause) {
    return this.each(function () {
      var $list = $(this),
          items = [], // uninitialised
          currentItem = limit,
          total = 0, // initialise later on
          height = $list.children(':first').height(),
          interval = (interval + fadespeed + slidespeed);

      // capture the cache
      i = 0;
      $list.children().each(function () {
        // dont ask me why, but if we dont set items like this -push($(this).html())-, it will act weird if list.children().length is less than twice the limit.
        items.push('<div class="views-spy-item item-' + i +'">' + $(this).html() + '</div>'); 
        i++;
      });
      total = items.length;

      $list.wrap('<div class="spy-wrapper" />').parent().css({ height : height * limit });
      // remove items as specified in limit
      $list.children().filter(':gt(' + (limit - 1) + ')').remove();

      // Spy effect
      var running = true;
      function spy() {
        if (running) {
          // insert a new item with opacity and height of zero
          var $insert = $(items[currentItem]).css({
              height : 0,
              opacity : 0,
              display : 'none'
          }).prependTo($list);
                      
          // fade the LAST item out
          $list.children().filter(':last').animate({ opacity : 0}, fadespeed, function () {
              // increase the height of the NEW first item
              $insert.animate({ height : height }, slidespeed).animate({ opacity : 1 }, fadespeed);
              // remove the last one
              $(this).remove();
            });
          
          currentItem++;
          if (currentItem >= total) {
            currentItem = 0;
          }
          
        }
        setTimeout(spy, interval);
      }
      setTimeout(spy, interval);
      
      if (autopause) {
        /*
         * Stop cycling on mouse over the whole thing
         */
        $(this).parent().parent().hover(function () { running = false; }, function () { running = true; });
      }
    });
  };
   
})(jQuery);

Drupal.behaviors.views_spy = function(context) {
  $.each(Drupal.settings.views_spy, function(id) {
    /*
     * Our view settings
     */
    var interval = this.interval;  // How fast we'll be switching through the elements
    var limit = this.limit;  // How many items to show at a time
    var fadespeed = this.fadespeed; // How fast the opacity animation will be
    var slidespeed = this.slidespeed; // How fast the sliding animation will be
    var autopause = this.autopause; // pause on mouse over
    
    $('#' + id).simpleSpy(limit, interval, fadespeed, slidespeed, autopause);
  });
};
