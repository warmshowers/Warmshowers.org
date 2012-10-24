Overview:
--------
The inactive_user module provides Drupal administrators with an automatic
way to manage inactive user accounts.  This module has two goals:  to help
keep users coming back to your site by reminding them when they've been away
for a configurable period of time, and to cleanup unused accounts.

One or more of the following actions can be automatically taken for users that
have exceeded configurable periods of inactivity:
  - send an email to the user
  - send an email to the site administrator
  - block the account (a warning can first be issued, and notification can
     be sent to the user and/or site administrator when the action occurs)
  - delete the account (a warning can first be issued, and notification can
     be sent to the user and/or site administrator when the action occurs)
  - optionally prevent deletion of user that have created site content

All events triggered by this module are logged via the watchdog.


Installation and configuration:
------------------------------
Enable the module at Administer->Site Building->Modules.
Database tables will be automatically created.
Grant the 'change inactive user settings' permission to the appropriate roles.
Configure as desired at Administer->User configuration->Inactive users.


Requires:
--------
 - Drupal 6.x
 - Working crontab


Credits:
-------
 - Written by Jeremy Andrews <jeremy@kerneltrap.org>
 - Converted to 4.7 by Dries Knapen <drieske@hotmail.com>
 - Converted to 5.0 by Adam Cowell <adam.cowell@gmail.com> and Larry Garfield <larry@garfieldtech.com>
 - Converted to 6.x by Tim Lievens and David Norman <http://deekayen.net>
 - Currently maintained by Larry Garfield <larry@garfieldtech.com> and David Norman <http://deekayen.net>
