[1]HTML Mail

   Lets you theme your messages the same way you theme the rest of your
   website.

  [2]Requirements

     * [3]Autoload 6.x-2.1 (New for 6.x-2.28)
     * [4]Mail System 6.x-2.x

  [5]Installation

   The following additional modules, while not required, are highly
   recommended:
     *

        [6]Echo
                Wraps your messages in a drupal theme. Now you can "brand"
                your messages with the same logo, header, fonts, and
                styles as your website.

     *

        [7]Emogrifier
                Converts stylesheets to inline style rules, for consistent
                display on mobile devices and webmail.

     *

        [8]Mail MIME
                Provides a text/plain alternative to text/html emails, and
                automatically converts image references to inline image
                attachments.

     *

        [9]Pathologic
                Converts urls from relative to absolute, so clickable
                links in your email messages work as intended.

     *

        [10]Transliteration
                Converts non-ASCII characters to their US-ASCII
                equivalents, such as from Microsoft "smart-quotes" to
                regular quotes.

                Also available as a [11]patch.

  [12]Updating from previous versions

   The [13]6.x-2.x branch shares 94% of its code with the [14]7.x-2.x
   branch, but only 8% of its code with the [15]6.x-1.x branch.

   Let your compatibility expectations be adjusted accordingly.
     * Check the module dependencies, as they have changed. The latest
       version of [16]HTML Mail depends on:
          + [17]Autoload
          + [18]Mail System (6.x-2.2 or later)
     * Run update.php immediately after uploading new code.
     * The user-interface for adding email header and footer text has been
       removed. Headers and footers may be added by template files and/or
       by enabling the [19]Echo module.
     * Any customized filters should be carefully tested, as some of the
       template variables have changed. Full documentation is provided
       both on the module configuration page (Click on the Instructions
       link) and as comments within the htmlmail.tpl.php file itself.
     * The following options have been removed from the module settings
       page. In their place, any combination of [20]over 200 filter
       modules may be used to create an email-specific [21]text format for
       post-template filtering.
          + [22]Line break converter
          + [23]URL Filter
          + [24]Relative Path to Absolute URLs
          + [25]Emogrifier
          + [26]Token support
     * Full MIME handling, including automatic generation of a plaintext
       alternative part and conversion of image references to inline image
       attachments, is available simply by enabling the [27]Mail MIME
       module.

  [28]Configuration

   Visit the [29]Mail System settings page at admin/settings/mailsystem to
   select which parts of Drupal will use [30]HTML Mail instead of the
   [31]default [32]mail system.

   Visit the [33]HTML Mail settings page at admin/settings/htmlmail to
   select a theme and post-filter for your messages.

  [34]Theming

   The email message text goes through three transformations before
   sending:
    1. Template File
       A template file is applied to your message header, subject, and
       body text. The default template is the included htmlmail.tpl.php
       file. You may copy this file to your email theme directory
       (selected below), and use it to customize the contents and
       formatting of your messages. The comments within that file contain
       complete documentation on its usage.
    2. Theming
       You may choose a theme that will hold your templates from Step 1
       above. If the [35]Echo module is installed, this theme will also be
       used to wrap your templated text in a webpage. You use any one of
       [36]over 800 themes to style your messages, or [37]create your own
       for even more power and flexibility.
    3. Post-filtering
       You may choose a [38]text format to be used for filtering email
       messages after theming. This allows you to use any combination of
       [39]over 200 filter modules to make final changes to your message
       before sending.
       Here is a recommended configuration:
          + [40]Emogrifier Converts stylesheets to inline style rules for
            consistent display on mobile devices and webmail.
          + [41]Transliteration Converts non-ASCII text to US-ASCII
            equivalents. This helps prevent Microsoft "smart-quotes" from
            appearing as question-marks in Mozilla Thunderbird.
          + [42]Pathologic Converts relative URLS to absolute URLS so that
            clickable links in your message will work as intended.

  Troubleshooting

     * Check the [43]online documentation, especially the [44]screenshots.
     * There is a special documentation page for [45]Using HTML Mail
       together with SMTP Authentication Support.
     * [46]Simplenews users attempting advanced theming should read
       [47]this page.
     * Double-check the [48]Mail System module settings and and make sure
       you selected HTMLMailSystem for your Site-wide default mail system.
     * Try selecting the [ ] (Optional) Debug checkbox at the [49]HTML
       Mail module settings page and re-sending your message.
     * Clear your cache after changing any .tpl.php files.
     * If you use a post-filter, make sure your filter settings page looks
       like [50]this.
     * Visit the [51]issue queue for support and feature requests.

  Related Modules

   Echo
          http://drupal.org/project/echo

   Emogrifier
          http://drupal.org/project/emogrifier

   HTML Purifier
          http://drupal.org/project/htmlpurifier

   htmLawed
          http://drupal.org/project/htmlawed

   Mail MIME
          http://drupal.org/project/mailmime

   Mail System
          http://drupal.org/project/mailsystem

   Pathologic
          http://drupal.org/project/pathologic

   Transliteration
          http://drupal.org/project/transliteration

  [52]Documentation

   **[53]HTML Mail

   [54]filter.module
          [55]api.drupal.org/api/drupal/modules--filter--filter.module

   [56]Installing contributed modules
          [57]drupal.org/documentation/install/modules-themes/modules-5-6

   [58]Theming guide
          [59]drupal.org/documentation/theme

  Original Author

     * [60]Chris Herberte

  Current Maintainer

     * [61]Bob Vincent

References

   1. http://drupal.org/project/htmlmail
   2. http://www.dict.org/bin/Dict?Form=Dict2&Database=*&Query=requirement
   3. http://drupal.org/node/1135590
   4. http://drupal.org/project/mailsystem
   5. http://drupal.org/documentation/install/modules-themes/modules-5-6
   6. http://drupal.org/project/echo
   7. http://drupal.org/project/emogrifier
   8. http://drupal.org/project/mailmime
   9. http://drupal.org/project/pathologic
  10. http://drupal.org/project/filter_transliteration
  11. http://drupal.org/node/1095278#comment-4219530
  12. http://drupal.org/node/250790
  13. http://drupal.org/node/1119548
  14. http://drupal.org/node/1106064
  15. http://drupal.org/node/329828
  16. http://drupal.org/project/htmlmail
  17. http://drupal.org/project/autoload
  18. http://drupal.org/project/mailsystem
  19. http://drupal.org/project/echo
  20. http://drupal.org/project/modules/?filters=type%3Aproject_project%20tid%3A63%20hash%3A1hbejm%20-bs_project_sandbox%3A1%20bs_project_has_releases%3A1
  21. http://drupal.org/node/779050
  22. http://api.drupal.org/api/drupal/modules--filter--filter.module/function/_filter_autop/6
  23. http://api.drupal.org/api/drupal/modules--filter--filter.module/function/_filter_url/6
  24. http://drupal.org/project/rel_to_abs
  25. http://www.pelagodesign.com/sidecar/emogrifier/
  26. http://drupal.org/project/token
  27. http://drupal.org/project/mailmime
  28. http://drupal.org/files/images/htmlmail_settings_2.thumbnail.png
  29. http://drupal.org/project/mailsystem
  30. http://drupal.org/project/htmlmail
  31. http://api.drupal.org/api/drupal/modules--system--system.mail.inc/class/DefaultMailSystem/7
  32. http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail_system/7
  33. http://drupal.org/project/htmlmail
  34. http://drupal.org/documentation/theme
  35. http://drupal.org/project/echo
  36. http://drupal.org/project/themes
  37. http://drupal.org/documentation/theme
  38. http://drupal.org/node/779050
  39. http://drupal.org/project/modules/?filters=type%3Aproject_project%20tid%3A63%20hash%3A1hbejm%20-bs_project_sandbox%3A1%20bs_project_has_releases%3A1
  40. http://drupal.org/project/emogrifier
  41. http://drupal.org/project/filter_transliteration
  42. http://drupal.org/project/pathologic
  43. http://drupal.org/node/1124376
  44. http://drupal.org/node/1124934
  45. http://drupal.org/node/1200142
  46. http://drupal.org/project/simplenews
  47. http://drupal.org/node/1260178
  48. http://drupal.org/project/mailsystem
  49. http://drupal.org/project/htmlmail
  50. http://drupal.org/node/1130960
  51. http://drupal.org/project/issues/htmlmail
  52. http://drupal.org/project/documentation
  53. http://drupal.org/node/1124376
  54. http://api.drupal.org/api/drupal/modules--filter--filter.module/6
  55. http://api.drupal.org/api/drupal/modules--filter--filter.module/6
  56. http://drupal.org/documentation/install/modules-themes/modules-5-6
  57. http://drupal.org/documentation/install/modules-themes/modules-5-6
  58. http://drupal.org/documentation/theme
  59. http://drupal.org/documentation/theme
  60. http://drupal.org/user/1171
  61. http://drupal.org/user/36148
