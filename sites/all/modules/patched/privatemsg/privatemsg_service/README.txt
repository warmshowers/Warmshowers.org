
Privatemsg Service
------------------
Integrates Privatemsg functionality with the services module.
http://drupal.org/node/433780



Requirements
------------
Drupal 6
PrivateMSG Module
Services Module (Privatemsg Services module is currently developed against Services 6.x-2.3)



Installation
------------
Enable Privatemsg Service in the "Site building -> Modules" administration screen.



Services
--------
See the Services browser (admin/build/services) for details regarding the services provided by this module. Here's a brief overview:

privatemsg.get
Returns all messages for the current user. Defaults to just loading the message previews. This is the equivalent of loading the inbox of a user

privatemsg.unreadCount
Returns the number of unread messages for a user. Defaults to the current user.

privatemsg.send
Sends a new message from the current user.

privatemsg.reply
Allows the current user to reply to a message.

privatemsg.getThread
Gets all messages in a thread.



Privatemsg Service Variables
----------------------------
The Privatemsg Service module currently uses three variables that define what user fields are included in the service reply. By default, Privatemsg often times returns the full user object. Since not all the fields are included (for example the user's email address), these fields can be configured by setting an array of fields. By default, Privatemsg Services returns the user id (uid) and the username.

privatemsg_service_participant_fields
Enhances the fields returned by the message previews in the privatemsg.get service (by default, Privatemsg only returns the uid).

privatemsg_service_thread_author_fields
Limits the fields of a thread author in the privatemsg.getThread service (by default, Privatemsg returns the full user object).

privatemsg_service_message_author_fields
Limits the fields of the message authors (for every message) in the privatemsg.getThread service (by default, Privatemsg returns the full user object for each message author).



Hooks
-----
hook_privatemsg_service_enhance_message($message)
Allows other modules to "enhance" the messages in a thread with additional data. Currently only implemented for the privatemsg.getThread service.
Each module needs to return a key and a value for every message. Here's the default hook implementation:
function hook_privatemsg_service_enhance_message($message) {
  $enhancement = array(
    'key' => 'test',
    'value' => time(),
  );
  return array($enhancement);
}



Credits
-------
Refactored by Daniel Hanold, (haagendazs) http://drupal.org/user/339733
Original code by tayzlor, http://drupal.org/user/274980
