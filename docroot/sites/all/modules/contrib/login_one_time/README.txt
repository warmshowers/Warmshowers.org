
Login one time README

CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration
  * Usage
  * API Usage

INTRODUCTION
------------
Maintainer: Daniel Braksator (http://drupal.org/user/134005)

Instructions on http://drupal.org/project/login_one_time.


INSTALLATION
------------
1. Copy login_one_time folder to modules directory (usually sites/all/modules).
2. At admin/modules enable the Login one time module.
3. Enable permissions at admin/people/permissions.


CONFIGURATION
-------------
The configuration page for this module is at:
Configuration > People > Login one time (admin/config/people/login_one_time)

There is also an email template configurable at:
Configuration > People > Account settings (admin/config/people/accounts)


USAGE
-----
There are several ways to send one time login links:

- Pressing the "Send login one time link [...]" button in a user profile.

- Using the operations on the user administration page, or with the module
  'Views Bulk Operations': http://drupal.org/project/views_bulk_operations.
  Embedding such a view somewhere can allow you to direct users to the page
  where the view is embedded.  Check the View Reference project page for some
  ideas on how to embed views: http://drupal.org/project/viewreference.

- Configure the login one time block, and use it to select a user and send the
  link.  You can easily use this block in nodes as a CCK field using the Block
  Reference module: http://drupal.org/project/blockreference.

- Using PHP, print out a button somewhere (e.g. in a node template) or directly
  send the emails using available functions as described below in API USAGE.

API USAGE
---------
Here are some function definitions and their descriptions to point you in the
right direction.

login_one_time_button($username = NULL, $path = NULL, $select = FALSE)
  Get a login one time form.
  $username
    If supplied force the email to go to this user, if not supplied will
    display a select element with all active users.
  $path
    If supplied will force the emailed link to redirect to this path. If not
    supplied will use default setting, or fallback to the URL of the page this
    code is called from.  Supply empty string to prompt for selection.
  $select
    If TRUE will display a select element to choose from configured paths, the
    default choice will come from $path or be calculated the same way, or if 
    empty string supplied it will prompt for selection.
  Return value
    The HTML string of the form, for use in output.

login_one_time_send_mail($account, $path = NULL)
  Send the login one time link to a user via email.
  $account
    The loaded account object for the user to whom the email will be sent.
  $path
    If supplied will force the emailed link to redirect to this path. If not
    supplied will use default setting, or fallback to the URL of the page this
    code is called from.
  Return value
    The return value from drupal_mail_send(), if ends up being called.

login_one_time_bulk_send_mail($accounts, $path = NULL)
  Bulk send login one time links to users via email.
  $account
    An array of user IDs.
  $path
    If supplied will force the emailed link to redirect to this path. If not
    supplied will use default setting, or fallback to the URL of the page this
    code is called from.
  Return value
    Multidimensional array of return data including user IDs and responses
    from login_one_time_send_mail.

These are some hooks you can implement to modify login one time's behaviour.

hook_login_one_time_path_options_alter(&$options)
  Alter the list of path options that the module uses in various places.  Use
  this instead of a hook_form_alter approach to affect all forms and lists 
  with this data.

hook_login_one_time_user_options_alter(&$options)
  Alter the list of user options that the module uses in various places.  Use
  this instead of a hook_form_alter approach to affect all forms and lists 
  with this data.

hook_login_one_time_used($user) 
  Invoked when a user successfully uses a login_one_time link.