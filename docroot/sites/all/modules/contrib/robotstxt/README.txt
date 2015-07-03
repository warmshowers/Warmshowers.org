
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Frequently Asked Questions (FAQ)
 * Known Issues
 * How Can You Contribute?


INTRODUCTION
------------

Maintainer: hass <http://drupal.org/user/85918>
Project Page: http://drupal.org/project/robotstxt

Use this module when you are running multiple Drupal sites from a single code
base (multisite) and you need a different robots.txt file for each one. This
module generates the robots.txt file dynamically and gives you the chance to
edit it, on a per-site basis.

For developers, you can automatically add paths to the robots.txt file by
implementing hook_robotstxt(). See robotstxt.api.php for more documentation.


INSTALLATION
------------

See http://drupal.org/getting-started/install-contrib for instructions on
how to install or update Drupal modules.

Once you have the RobotsTxt modules installed, make sure to delete or rename
the robots.txt file in the root of your Drupal installation. Otherwise, the
module cannot intercept requests for the /robots.txt path.


FREQUENTLY ASKED QUESTIONS
--------------------------

Q: Can this module work if I have clean URLs disabled?
A: Yes it can! In the .htaccess file of your Drupal's root directory, add the
   following two lines to the mod_rewrite section, immediately after the line
   that says "RewriteEngine on":

   # Add redirection for the robots.txt path for use with the RobotsTxt module.
   RewriteRule ^(robots.txt)$ index.php?q=$1

Q: Does this module work together with Drupal Core "Fast 404 pages" feature?
A: Yes, but you need to add robots.txt to the 'exclude_paths' of your settings.php.
   
   Drupal (default):
   $conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';

   Drupal with RobotsTxt module:
   $conf['404_fast_paths_exclude'] = '/\/(?:styles)\/|(?:robots.txt)/';

Q: How can I install the module with custom default robots.txt?
A: The module allows adding a default.robots.txt to the defaults folder.

   1. Remove the robots.txt from site root.
   2. Save your custom robots.txt to "/sites/default/default.robots.txt"
   3. Run the module installation.


KNOWN ISSUES
------------

There are no known issues at this time.

To report new bug reports, feature requests, and support requests, visit
http://drupal.org/project/issues/robotstxt.


HOW CAN YOU CONTRIBUTE?
---------------------

- Report any bugs, feature requests, etc. in the issue tracker.
  http://drupal.org/project/issues/robotstxt

