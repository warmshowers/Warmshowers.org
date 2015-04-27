README.txt
____________________


DESCRIPTION
____________________

The Private Message module is designed to be a flexible and powerful system for sending and receiving messages. This includes user-to-user messages, user-to-role messages, messages from the site administrator, and much more. If you want some or all users on your site to have their own "mailbox"--and other users with the proper permissions to be able to message them through this mailbox--then this is the module for you.

One of the strengths of Private Message is that it has a broad feature set and a modular architecture. The core Private Message module includes features such as threaded conversations (making it easier to keep track of messages and replies), search capability, new message alerts (via Drupal messages and blocks), and message tokens (similar to a mail merge).


SUB-MODULES
____________________

In addition to the core Private Message module, you are able to enable the following sub-modules based on the specific features that you need:

* Block User Messages (pm_block_user): Allows users to block other users from sending them messages. It also allows site administrators to block users with certain roles from messaging users with other roles.

* Privatemsg filter (privatemsg_filter): This sub-module allows you to tag messages and filter them according to these tags. Additionally, the module can define "Inbox," "Sent Messages," and "All Messages" tabs to help users organize their messages.

* Privatemsg Email Notification (pm_email_notify): This sub-module can notify users of new private messages via e-mail. Users may enable or disable this feature from the user edit page.

* Privatemsg Roles (privatemsg_roles): Allows a user to send messages to all members of a role. It also allows the site administrator to control whether recipients can see that the message was sent to others in the role.

* Privatemsg Rules (privatemsg_rules): For those using the Rules module, this sub-module defines an "A message is sent" event. A variety of actions are available such as 'Send a message' and 'Reply to a message.' 

PERMISSIONS
____________________

The core Private Message module and its sub-modules define a variety of permissions. These include permissions for reading messages, writing messages, deleting messages, and much more. All of these permissions can be found at admin/people/permissions.


INSTALLATION
____________________

To install this module, do the following:

1. Extract the tar ball that you downloaded from Drupal.org.

2. Upload the privatemsg directory and all its contents to your modules directory.

3. Visit admin/modules and enable the "Private messages" module and any of its sub-modules.  All of these modules can be found within the "Mail" fieldset.


CONFIGURATION
____________________

To configure this module do the following:

1. Go to People -> Permissions (admin/people/permissions) and find the relevant module permissions underneath the "Private messages" section. If you are not logged in as user #1, you must give at least one role (probably the administrator role) the 'Administer privatemsg' permission to configure this module. 

2.  On this same Permissions page, give at least one role the 'Read private messages' permission and the 'Write new private messages' permission.  This will allow members of that role to read and write private messages.

3. Go to Configuration -> Private messages (admin/config/messaging/privatemsg) and configure the module settings per your requirements. If you have various sub-modules enabled, their settings pages may appear as tabs on this page. 

4. Login as a user with the role we specified in Step #2. Visit /messages to see the user's mailbox. Visit /messages/new to write a new message.


RECOMMENDED MODULES
____________________

* Views (http://drupal.org/project/views):  Enabling this module allows you to include (in a user view) a link to send a private message to each user.  This link is available as a Views field.

* Rules (http://drupal.org/project/rules):  Enabling this module gives you the ability to take various actions when a message is sent.  Additionally, messages can be sent when other Drupal events occur.

* Token (http://drupal.org/project/token):  Enabling this module will provide a "token tree" that shows available tokens for those with the 'Use tokens in private messages' permission.  Clicking on a token will add it to the private message text.


API
____________________

This module has a robust and powerful API. Complete documentation for each version of this API can be found at:  http://blog.worldempire.ch/api


DEMO SITE
____________________

This module is actively developed.  New features and fixes are tested at the following demo site: http://demo.worldempire.ch.  Feedback is welcome.


FAQ
____________________

Q:  I renamed a menu item from "Messages" to "Private Messages".  Previously, it would tell the user how many messages are unread, but now it doesn't do this anymore.  How do I re-enable this functionality?

A:  Reset the menu item. Instead of changing the name there, use the String Overrides module (http://drupal.org/project/string_override) to change the name.  Alternately, you can employ hook_menu_alter() and use a different title callback (in which you can use whatever strings you want).

Q:  How can I send messages from the site administrator to each user?

A: The easiest way to do this is to set up a special user account for this purpose.  Give this user account an official sounding user name like "Site Administrator."  Then give a role (that this user belongs to) the 'Write private message to roles' permission (admin/people/permissions).  Finally, you can configure site-wide blocking rules (admin/config/messaging/privatemsg/block) to prevent users from blocking this user or sending messages to this user (if you don't want to allow replies to site administrator messages).
