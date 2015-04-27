// $Id$
(function ($) {
  Drupal.behaviors.external = {
    attach: function (context, settings) {
      // Open external links in new tabs.
      $("a[href^=http\\:\\/\\/]:not(.external-processed)", context).each(function() {
        if(this.href.toLowerCase().indexOf(location.hostname) == -1 || this.href.toLowerCase().indexOf(location.hostname) > 13) {

          $(this).click(externalNewWindow);
        }
      }).addClass('external-processed');
      // If the setting is enabled, open PDFs in new tabs.
      if (Drupal.settings.externalpdf) {
        $("a[href*=\\.pdf]:not(.external-processed)", context).each(function() {
            $(this).click(externalNewWindow);
        }).addClass('external-processed');
      };
      // Open any links with class="newtab" in new tabs
      $("a.newtab:not(.external-processed)", context).click(externalNewWindow).addClass('external-processed');

      // Utility function that does the work.
      function externalNewWindow() {
        window.open(this.href);
        return false;
      }
    }
  };
}(jQuery));

