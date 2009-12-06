// $Id: memcache.js,v 1.1.2.1 2007/07/31 15:54:11 robertDouglass Exp $

// Global Killswitch
if (Drupal.jsEnabled) {
$(document).ready(function() {
    $("body").append($("#memcache-devel"));
  });
}
