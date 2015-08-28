Persistent Login module for Drupal
==================================

The Persistent Login module provides the familiar "Remember Me" option in the
user login form.


## Description

The module's settings allow the administrator to:

- Control how long user logins are remembered.
- Control how many different persistent logins are remembered per user.
- Control which pages a remembered user can or cannot access without explicitly
  logging in with a username and password (e.g. you cannot edit your account or
  change your password with just a persistent login).

Each user's 'My account' view tab gives them option of explicitly clearing all
of his/her remembered logins.

Persistent Login is independent of the PHP session settings and is more secure
(and user-friendly) than simply setting a long PHP session lifetime. For a
detailed discussion of the design and security of Persistent Login, see
[Improved Persistent Login Cookie Best Practice](http://www.jaspan.com/improved_persistent_login_cookie_best_practice).


## Installation

1. Add Persistent Login files to your modules directory, and enable the module.

2. For maximum security, edit your settings.php file so PHP session cookies have
   a lifetime of the browser session:

   `ini_set('session.cookie_lifetime', 0);`

3. Visit admin >> settings >> persistent_login to set how long persistent
   sessions should last and which pages users cannot access without a
   password-based login.

4. If using a reverse-proxy cache (e.g. Varnish), the configuration must be
   updated to not respond from the cache for requests that send a persistent
   login cookie.
