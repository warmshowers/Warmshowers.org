[1]Emogrifier

   Uses the [2]emogrifier class library as an input filter to convert
   stylesheet rules to inline style attributes. This ensures proper
   display on email and mobile device readers that lack stylesheet
   support.

  [3]Installation

    1. Ensure that the PHP [4]Document Object Model extension is
       available. Emogrifier requires the dom extension and will not work
       without it.
    2. Download, install, and enable the [5]Libraries module.
    3. Create a library directory for the Emogrifier library in one of the
       following locations:
          + sites/all/libraries/emogrifier (recommended)
          + sites/DOMAIN/libraries/emogrifier
          + profiles/PROFILE/libraries/emogrifier
       DOMAIN is your website domain name, and PROFILE is the installation
       profile you selected when installing Drupal.
    4. [6]Download the emogrifier.php and place it within the library
       directory you just created.
    5. [7]Install [8]this module and enable it.
    6. Visit admin/configure/content/formats or click on

     Administer >> Configuration >> Content authoring >> Text formats
       to set up a new input format or add Emogrifier filtering to an
       existing format.

References

   1. http://drupal.org/project/emogrifier
   2. http://www.pelagodesign.com/sidecar/emogrifier/
   3. http://drupal.org/documentation/install/modules-themes/modules-7
   4. http://php.net/dom
   5. http://drupal.org/project/libraries
   6. http://www.pelagodesign.com/sidecar/emogrifier/
   7. http://drupal.org/documentation/install/modules-themes/modules-7
   8. http://drupal.org/project/emogrifier
