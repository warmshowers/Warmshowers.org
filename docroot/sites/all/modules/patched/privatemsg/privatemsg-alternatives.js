
(function ($) {
  Drupal.behaviors.PrivatemsgAlternatives = {
    attach: function (context) {
      // Replace span with a link.
      $("span.privatemsg-recipient-alternative").each(function() {
        $(this).after(
          // Replace the span with a link, add href and class.
          $('<a>')
            .attr({'href' : '#'})
            .addClass('privatemsg-recipient-alternative')
            .text($(this).text())
            // Add a on click function.
            .click(function () {
              // Replace the value of the recipient field with the
              // previous content but replace the unclear recipient
              // with the one that user clicked on.
              $('#edit-recipient')
                .val(
                  $('#edit-recipient')
                    .val()
                    .replace(
                      // Get the original recipient string for this suggestion.
                      Drupal.settings.privatemsg_duplicates[$(this).text()],
                      $(this).text()
                ))

              // Add a new status message.
              $(this).closest('div.messages')
                .after('<div class="messages status"><h2 class="element-invisible">' + Drupal.t('Status message') + '</h2>' + Drupal.t('The recipient field has been updated. You may now send your message.') + '</div>');

              // Hide the error message. Hide the parent of the span, this is
              // either div if there is only a single message or the li.
              $(this).closest('span.privatemsg-alternative-description').parent().hide();
            }));
        // Remove the span.
        $(this).remove();
      })
    }
  }

})(jQuery);