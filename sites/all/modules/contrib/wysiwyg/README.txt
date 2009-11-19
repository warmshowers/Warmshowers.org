/* $Id: README.txt,v 1.6 2008/10/28 22:46:05 sun Exp $ */

-- SUMMARY --

Wysiwyg API allows to users of your site to use WYSIWYG/rich-text, and other
client-side editors for editing contents.  This module depends on third-party
editor libraries, most often based on JavaScript.

For a full description visit the project page:
  http://drupal.org/project/wysiwyg
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/wysiwyg


-- REQUIREMENTS --

* None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* Go to Administer > Site configuration > Wysiwyg, and follow the displayed
  installation instructions to download and install one of the supported
  editors.


-- CONFIGURATION --

* Go to Administer > Site configuration > Input formats and

  - either configure the Full HTML format, assign it to trusted roles, and
    disable "HTML filter", "Line break converter", and (optionally) "URL filter".

  - or add a new input format, assign it to trusted roles, and ensure that above
    mentioned input filters are disabled.

* Setup editor profiles in Administer > Site configuration > Wysiwyg.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - http://www.unleashedmind.com

Previous maintainers:
* Nathan Haug (quicksketch) - http://quicksketch.org
* kreynen - http://drupal.org/user/48877
* Allie Micka - http://drupal.org/user/15091
* Theodore Serbinski (m3avrck) - http://drupal.org/user/12932
* Nedjo Rogers (nedjo) - http://drupal.org/user/4481
* Steve McKenzie - http://drupal.org/user/45890
* ufku - http://drupal.org/user/9910
* Matt Westgate - <drupal AT asitis DOT org> and
* Jeff Robbins - <robbins AT jjeff DOT com>
* Richard Bennett - <richard.b AT gritechnologies DOT com>


This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

