;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Persistent Login module for Drupal 6
;;
;; Current Mantainer: markus_petrux (http://drupal.org/user/39593)
;; Original Author  : bjaspan (http://drupal.org/user/46413)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS OF THIS FILE
=====================
* OVERVIEW
* INSTALLATION
* UPGRADING
* DESCRIPTION


OVERVIEW
========

The Persistent Login module provides the familiar "Remember Me" option
in the user login form.


INSTALLATION
============

1. Install and activate Persistent Login like every other Drupal module.

2. For maximum security, edit your settings.php file so PHP session
   cookies have a lifetime of the browser session:

   ini_set('session.cookie_lifetime', 0);

3. Visit admin >> settings >> persistent_login to set how long
   persistent sessions should last and which pages users cannot
   access without a password-based login.


UPGRADING
=========

Because Persistent Login interacts with the user login process, some
caution is required when upgrading it to a new version.

1. Log in as Administrator.

2. Visit administer >> settings and open the Site Maintenance box.
   Select "Off-line" and press Save configuration.  This is so users
   do not receive any error messages before the upgrade is complete.

3. Install the new Persistent Login module files.

4. Visit http://yoursite/update.php to update the Persistent Login
   database schema if necessary (you should do this every time you
   upgrade any module).

5. Return to administer >> settings >> Site Maintenance and put your
   site back online.

NOTE: If update.php shows a version update for Persistent Login, all
currently remembered login sessions for all users may be lost.
Everyone will have to log in again with their username and password.


DESCRIPTION
===========

The Persistent Login module provides the familiar "Remember Me" option in
the user login form.

The module's settings allow the administrator to:

- Control how long user logins are remembered.

- Control how many different persistent logins are remembered per user.

- Control which pages a remembered user can or cannot access without
  explicitly logging in with a username and password (e.g. you cannot
  edit your account or change your password with just a persistent
  login).

Each user's 'My account' view tab gives them option of explicitly
clearing all of his/her remembered logins.

Persistent Login is independent of the PHP session settings and is
more secure (and user-friendly) than simply setting a long PHP session
lifetime. For a detailed discussion of the design and security of
Persistent Login, see "Improved Persistent Login Cookie Best Practice"
<http://www.jaspan.com/improved_persistent_login_cookie_best_practice>.
