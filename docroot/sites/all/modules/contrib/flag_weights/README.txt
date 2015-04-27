
This is a very simple module that adds a 'weight' field to the tables already
used by the excellent Flag module.

  http://drupal.org/handbook/modules/flag
  
This weight can then be used to provide sorting of flagged items. This module
also provides a Views field which can be used to sort flagged items. One
interesting use of this ability is to integrate with Draggable Views to create a
user-sortable list of flagged items (however this currently requires extending
that module to support non-CCK sort fields, and a custom save method).

  http://drupal.org/project/draggableviews

Recommended Modules
-------------------
- Views

Installation
------------
1) Copy the flag_weights directory to the modules folder in your installation.

2) Enable the module using Administer -> Modules (/admin/build/modules)

Support
-------
If you experience a problem with flag or have a problem, file a
request or issue on the flag_weights queue at
http://drupal.org/project/issues/flag_weights. DO NOT POST IN THE FORUMS.
Posting in the issue queues is a direct line of communication with the module
authors.

No guarantee is provided with this software, no matter how critical your
information, module authors are not responsible for damage caused by this
software or obligated in any way to correct problems you may experience.

Licensed under the GPL 2.0.
http://www.gnu.org/licenses/gpl-2.0.txt

