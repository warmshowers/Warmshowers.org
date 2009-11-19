$Id: README.txt 505 2009-05-24 18:55:09Z rfay $

Content Profile Module
------------------------
by Wolfgang Ziegler, nuppla@zites.net

With this module you can build user profiles with drupal's content types.


Installation 
------------
 * Copy the module's directory to your modules directory and activate the module.
 
 Usage:
--------
 * There will be a new content type "profile". Customize its settings at
   admin/content/types.
 * At the bottom of each content type edit form, there is a checkbox, which allows
   you to mark a content type as profile.
 * When you edit a profile content type there will be a further tab "Content profile",
   which provides content profile specific settings.


Content profiles per role:
--------------------------
You may, but you need not, mark multiple content types as profile. By customizing 
the permissions of a content type, this allows you to create different profiles for
different roles.


Hints:
------

 * When using content profiles the "title" field is sometimes annoying. You can rename
   it at the content types settings or hide it in the form and auto generate a title by
   using the auto nodetitle module http://drupal.org/project/auto_nodetitle.
   
 * If you want to link to a content profile of a user, you can always link to the path
   "user/UID/profile/TYPE" where UID is the users id and TYPE the machine readable content
   type name, an example path would be "user/1/profile/profile".
   This path is working regardless the user has already profile content created or not.

 * If you want to theme your content profile, you can do it like with any other content.
   Read http://drupal.org/node/266817.
   
 * If you want a content profile to be private while your site content should be available
   to the public, you need a module that allows configuring more fine grained access control
   permissions, e.g. the module Content Access (http://drupal.org/project/content_access)
   allows you to that.
   
 * There is also rules integration which is useful for customizing the behaviour of the
   module. See below for more.



Theming: Easily use profile information in your templates! 
-----------------------------------------------------------
Content Profile adds a new variable $content_profile to most templates related to users.
So this variable allows easy access to the data contained in the users' profiles.
Furthermore it does its job fast by lazy-loading and caching the needed content profile
nodes.

The $content_profile variable is available in the page, node, comment, user_name,
user_profile, user_signature, search_result and some other templates. 

$content_profile lets you access all variables of a profile, which are you used to
have in a common node template. See http://drupal.org/node/11816.

So in any of these templates you may use the $content_profile like this:

<?php
 // Just output the title of the content profile of type 'profile'
 // If there is no such profile, it will output nothing.
 echo $content_profile->get_variable('profile', 'title');

 // Get all variables of the content profile of type 'profile'
 $variables = $content_profile->get_variables('profile');
 
 // Print out a list of all available variables
 // If the user has no profile created yet, $variables will be FALSE.
 print_r($variables);

 if ($variables) {
   // Print the title and the content.
   echo $variables['title'];
   echo $variables['content'];
 }
 else {
   // No profile created yet.
 }
 
 // $content_profile also allows you to easily display the usual content profile's view
 // supporting the same parameters as node_view().
 echo $content_profile->get_view('profile');

?>

 Check the source of content_profile.theme_vars.inc to see what methods $content_profile
 supports else.


Adding $content_profile to further templates
--------------------------------------------

If you miss $content_profile in some templates containing user information (id), just
fill a issue in content profile's queue so we can add it to the module.
Furthermore you may let content_profile its variable to your custom templates by specifying
the setting 'content_profile_extra_templates' in your site's settings.php.

E.g. you may add:
  $conf['content_profile_extra_templates'] = array('my_template');

Where 'my_template' has to be the key of your template's entry in the theme_registry (hook_theme()).



Rules integration
------------------

There is some integration to the rules module (http://drupal.org/project/rules), which offers
a condition to check whether a user has already created a profile of a certain type. Then it
offers an action for loading the content profile of a user, which makes it available to token
replacements as well as to all other existing rules actions which deal with content.

So this integration allows one to build some profile related rules with the rules module. As
example the module ships with one deactivated default rule:

  "Redirect to profile creation page, if users have no profile."
  
If you activate it at the rules "Triggered rules" page, it's going to be evaluated when a user
logs in. Of course you can also alter the default rule and customize it so that it fits your needs,
e.g. you could remove the redirect action so that only a message is displayed.
