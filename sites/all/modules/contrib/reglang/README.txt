// $Id: README.txt,v 1.1.2.1 2009/02/20 10:15:55 karst Exp $
*************************************************
           RegLang - Registration Language
*************************************************
Decide if a new registered user should have a language set.
Author: Karsten Frohwein
This is the Drupal 6 version.

What it does
--------------------
This module let you decide if a new user will have none, the sites default
language or the current language set.

In Drupal 6 a new user that did register through the sites process would have
no language set. The signup emails wouldn't have a language set so we can't
translate them and the user would have to set his language on the first signup
as the site would use the default language which might not be the language of
the user.

To adress this we can use hook_user and set the language
on the "insert" operation.

Still the user system emails will be set to what you entered into the user
settings form. But you can use this module to translate them properly:
http://drupal.org/project/mail_edit

Any suggestions, comments and bugs are very welcome. Please use the issue cue at drupal.org.

Thanks for your interest
Karsten Frohwein http://www.comm-press.de