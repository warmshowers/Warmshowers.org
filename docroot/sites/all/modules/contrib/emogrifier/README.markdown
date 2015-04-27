## [Emogrifier](http://drupal.org/project/emogrifier)

Uses the
[emogrifier class library](http://www.pelagodesign.com/sidecar/emogrifier/)
as an input filter to convert stylesheet rules to inline style attributes. This
ensures proper display on email and mobile device readers that lack stylesheet
support.

### [Installation](http://drupal.org/documentation/install/modules-themes/modules-7)

1.  Ensure that the PHP [Document Object Model extension](http://php.net/dom)
    is available. Emogrifier requires the dom extension and will not work
    without it.

2.  Download, install, and enable the
    [Libraries module](http://drupal.org/project/libraries).

3.  Create a library directory for the Emogrifier library in one of the
    following locations:

    * sites/all/libraries/emogrifier *(recommended)*
    * sites/DOMAIN/libraries/emogrifier
    * profiles/PROFILE/libraries/emogrifier

    DOMAIN is your website domain name, and PROFILE is the installation
    profile you selected when installing Drupal.

4.  [Download](http://www.pelagodesign.com/sidecar/emogrifier/) the
    emogrifier.php and place it within the library directory you just created.

5.  [Install](http://drupal.org/documentation/install/modules-themes/modules-7)
    [this module](http://drupal.org/project/emogrifier) and enable it.

6.  Visit <u>admin/configure/content/formats</u> or click on

    >    Administer >> Configuration >> Content authoring >> Text formats

    to set up a new input format or add Emogrifier filtering to an existing
    format.
