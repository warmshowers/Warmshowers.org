
$(document).ready(function() {

  // Attach onclick event to document only and catch clicks on all elements.
  $(document.body).click(function(event) {
    // Catch only the first parent link of a clicked element.
    $(event.target).parents("a:first,area:first").andSelf().filter("a,area").each(function() {

      var ga = Drupal.settings.googleanalytics;
      // Expression to check for absolute internal links.
      var isInternal = new RegExp("^(https?):\/\/" + window.location.host, "i");
      // Expression to check for special links like gotwo.module /go/* links.
      var isInternalSpecial = new RegExp("(\/go\/.*)$", "i");
      // Expression to check for download links.
      var isDownload = new RegExp("\\.(" + ga.trackDownloadExtensions + ")$", "i");

      // Is the clicked URL internal?
      if (isInternal.test(this.href)) {
        // Is download tracking activated and the file extension configured for download tracking?
        if (ga.trackDownload && isDownload.test(this.href)) {
          // Download link clicked.
          var extension = isDownload.exec(this.href);
          _gaq.push(["_trackEvent", "Downloads", extension[1].toUpperCase(), this.href.replace(isInternal, '')]);
        }
        else if (isInternalSpecial.test(this.href)) {
          // Keep the internal URL for Google Analytics website overlay intact.
          _gaq.push(["_trackPageview", this.href.replace(isInternal, '')]);
        }
      }
      else {
        if (ga.trackMailto && $(this).is("a[href^=mailto:],area[href^=mailto:]")) {
          // Mailto link clicked.
          _gaq.push(["_trackEvent", "Mails", "Click", this.href.substring(7)]);
        }
        else if (ga.trackOutgoing && this.href) {
          if (ga.trackOutboundAsPageview) {
            // Track all external links as page views after URL cleanup.
            // Currently required, if click should be tracked as goal.
            _gaq.push(["_trackPageview", '/outbound/' + this.href.replace(/^(https?|ftp|news|nntp|telnet|irc|ssh|sftp|webcal):\/\//i, '').split('/').join('--')]);
          }
          else {
            // External link clicked.
            _gaq.push(["_trackEvent", "Outbound links", "Click", this.href]);
          }
        }
      }
    });
  });
});
