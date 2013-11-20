// $Id: external.js,v 1.8 2010/03/22 19:12:55 mcrittenden Exp $
Drupal.behaviors.external = function(context) {
  // Open external links in new tabs.
  $("a[href^=http://]", context).each(function() {
    if(this.href.toLowerCase().indexOf(location.hostname) == -1) {
      $(this).click(externalNewWindow);
    }
  });
  // If the setting is enabled, open PDFs in new tabs.
  if (Drupal.settings.externalpdf) {
    $("a[href*=.pdf]", context).each(function() {
        $(this).click(externalNewWindow);
    });
  };
  // Open any links with class="newtab" in new tabs
  $("a.newtab", context).click(externalNewWindow);

  // Utility function that does the work.
  function externalNewWindow() {
    window.open(this.href);
    return false;
  }
}

