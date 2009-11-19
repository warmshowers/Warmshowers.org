== Description ==

This module is a little helper to maintain an administrator role which has full permissions. By default, Drupal only has one super users, this changes that. Note the update.php will still only work for the "real" admin though.

== Installation & Configuration ==

   1. Enable the module
   2. Create your "Administrator" Role if it doesn't exist already
   3. Go to Admin -> User -> Admin Role (http://example.com/admin/user/adminrole)
   4. Select your role

== Usage ==

Now this role has all permissions. When you add a new module, that role will get all those permissions too.
