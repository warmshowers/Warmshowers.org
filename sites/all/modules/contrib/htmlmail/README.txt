/* $Id: README.txt,v 1.4.2.3 2009/07/15 05:34:11 chrisherberte Exp $ */

HTML Mail
---------

HTML Mail empowers Drupal with the ability to send emails in HTML, providing
formatting and semantic markup capabilities in e-mail that are not available
with plain text.

This module is very simple in operation. It changes headers in all outgoing
e-mail modifying e-mail sent from Drupal to be HTML with the option of header,
footer and CSS inclusion.

For a full description of the module, visit the project page:
  http://drupal.org/project/htmlmail

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/htmlmail

  
Installation
------------

Install as usual, see http://drupal.org/node/70151 for further information.


Customisation
-------------
E-mails can be themed by copying htmlmail.tpl.php to you active theme's 
directory and editing the contents.

Important
---------

Remember that many email clients will not be happy with certain code, your 
CSS may conflict with a web-mail providers CSS and HTML in email may expose 
security hazards. Beyond this, if your still really, really must have HTML in 
your email, you may find this module useful.


Maintainers
-----------

Chris Herberte - http://drupal.org/user/1171
