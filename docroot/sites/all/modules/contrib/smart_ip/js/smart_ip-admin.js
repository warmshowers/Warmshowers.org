// $Id$

(function ($) {
  Drupal.behaviors.smartIpAdmin = {
    attach: function (context, settings) {
      var $drupal_roles = $('input[name^="smart_ip_roles_to_geolocate"]');
      $('input[name="smart_ip_roles_to_geolocate[2]"]').click(function () {
        for (var i = 0; i < $drupal_roles.length; i++) {
          // Don't include anonymous and authenticated users
          if (i != 0 && i != 1) {
            var is_autheticated =  $(this).attr('checked');
            // When authenticated user role is checked, automatically check and disable other roles
            $('input[name="smart_ip_roles_to_geolocate[' + (i + 1) + ']"]').attr('checked', is_autheticated).attr('disabled', is_autheticated);
          }
        }
      });
    }
  };
})(jQuery);