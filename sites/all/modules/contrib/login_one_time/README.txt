CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration
  * Usage


INTRODUCTION
------------
Maintainer: Daniel Braksator (http://drupal.org/user/134005)

Instructions on http://drupal.org/project/login_one_time.


INSTALLATION
------------
1. Copy login_one_time folder to modules directory.
2. At admin/build/modules enable the Login One Time module.
3. Enable permissions at admin/user/permissions.
4. profit


CONFIGURATION
-------------
You can configure the one-time login email template on the user settings page.


USAGE
-----
To put a one time button somewhere for user object $account use this php:

print login_one_time_button($account);

This will create a button, that when pressed sends an email to the email address of that account giving them a one-time login link.

If you would like them to start on a particular page, you can add an extra parameter $path like so:

print login_one_time_button($account, $path);

If you would like to skip the button and just call a function that send the email straight away:

login_one_time_send_mail($account, $path);