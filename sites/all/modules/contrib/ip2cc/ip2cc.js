if (Drupal.jsEnabled) {
  $(document).ready(function() {
    // Remove focus from the "Update" button
    $('#ip2cc-update-form .form-submit').eq(1).focus();

    // When "Update" button is pushed, make an AJAX call to initiate the
    // database update without leaving this page.  Throw up a progress
    // marker because it takes a long time.
    $('#ip2cc-update-form #edit-submit').click(function(){
      var databaseUpdated = function(data) {
        var result = Drupal.parseJson(data);
        $('#dbthrobber').removeClass('working');
        if (result['count']) {
          $('#dbthrobber').html(result['message'] + '  ' + result['count']).addClass('completed');
        } else {
          $('#dbthrobber').html(result['message']).addClass('message');
        }
      }
      $('#dbthrobber').removeClass('message completed').addClass('working').html('Working...');
      $.get(Drupal.settings.basePath + 'admin/settings/ip2cc/update/ajax', null, databaseUpdated);
      return false;
    });
  });
}
