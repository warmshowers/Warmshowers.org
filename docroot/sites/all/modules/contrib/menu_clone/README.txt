
README file for the Menu Clone Drupal module.


Description
===========

This module allows you to clone entire menus, much like Node Clone
(http://drupal.org/project/node_clone) does with nodes.

The module will add a tab 'Clone Menu' to a menu overview page, allowing for a
good integration in sync the Drupal logic. You will also be able to customise
the menu before the cloning process begins.


Caution
=======

Issues with cloning the default navigation menu have been reported, especially
when deleting these clones. During testing the root cause was found and has been
addressed with this release so it is still possible to clone system default
menus. No side effects of deleting these clones have been discovered during
testing however if you still experience problems with this, please create an
issue on the project page.


Recommended modules
===================

- Internationalisation (http://drupal.org/project/i18n) and its Menu translation
  submodule.
  In multilingual setups, cloning menus can be especially interesting. When i18n
  and i18nmenu are enabled, additional language options will be available.


Installation
============

1. Extract the 'menu_clone' module directory, including all its
   subdirectories, into your Drupal modules directory.

2. Go to the Modules page, and enable the module.

3. Go to the People > Permissions page, and grant 'access
   menu clone' permissions to the desired roles.


Usage
=====
Next to the basic menu properties such as Menu name, Title and Description, the
clone form allows you to customise the menu itself before cloning it.

The optional Internationalization (i18n) module together with the i18n submodule
Menu translation will activate the Change language option. This select box
allows you to change the language of all menu items during the cloning process.

In Edit menu you can customise the menu tree before cloning it. Next to the
Enabled and Expanded checkboxes, the menu tree can be rearranged as well. To
rearrange menu items, grab a drag-and-drop handle under the Menu item column and
drag the items (or group of items) to a new location in the list. (Grab a handle
by clicking and holding the mouse while hovering over a handle icon.
Everything under the Ignore row will not be copied over to the new menu. It does
not matter on what depth the Ignore row resides, everything below it will simply
be ignored.


Support
=======

For support requests, bug reports, and feature requests, please us the issue cue
of Menu Clone on http://drupal.org/project/issues/menu_clone.


Credits
=======

- Project initiated by Garrett Albright (http://drupal.org/user/191212,
http://raygunrobot.com).

- All developers of the Drupal core Menu module. Some code was taken from there.

- Sponsored in part by Nascom (http://www.nascom.be)

