[1]Mail MIME

   Provides a class for creating MIME messages.
     *

        NOTE: This module does not send mail.
                If you came here looking for a mail-sending module, try
                [2]HTML Mail (which can use this module for MIME-handling)
                or [3]Mime Mail (which comes with its own MIME-handling
                library).

   [4]Mail Mime extends certain [5]PEAR mail-handling classes to provide a
   drupal-friendly library for creating and parsing MIME messages. Neither
   [6]Mail Mime nor the original [7]PEAR classes send mail nor do anything
   useful on their own. They are intended as code libraries to be used by
   other programs that send or receive mail.

   [8]Mail Mime started out as part of [9]HTML Mail. I separated into its
   own module for two reasons:
    1. The separation helped clarify in my own mind which parts of the
       mail-sending process require MIME and which do not. The resulting
       code is simpler and easier to maintain.
    2. It is possible that another mail-sending or mail-reading module may
       find the [10]Mail Mime library useful.

  Requirements

   The following files, available from [11]PEAR, must be installed and
   available somewhere on the [12]include_path.
     * Mail/mime.php
     * Mail/mimeDecode.php
     * Mail/mimePart.php
     * PEAR.php
     * PEAR5.php
     * One way to satisfy the requirements is to run the following commands
       from a Unix root shell prompt:
       pear install -a Mail_Mime
       pear install Mail_mimeDecode
       The -a parameter ensures that dependencies, including
       Mail/mimePart.php, are also installed.
     * Another way is to install and enable the [13]Include module before
       enabling the [14]Mail MIME module.

  [15]Installation

    1. Download and install [16]as usual.
    2. When enabled, [17]Mail Mime will
          + Auto-detect whether the required files are present.
          + If any files are missing, and the [18]Include module is
            available, [19]Mail Mime will use [20]Include to auto-download
            and enable the missing files.
          + If all of the above fails, [21]Mail Mime will disable itself
            and print a warning to both the screen and the error log.

References

   1. http://drupal.org/project/mailmime
   2. http://drupal.org/project/htmlmail
   3. http://drupal.org/project/mimemail
   4. http://drupal.org/project/mailmime
   5. http://pear.php.net/
   6. http://drupal.org/project/mailmime
   7. http://pear.php.net/
   8. http://drupal.org/project/mailmime
   9. http://drupal.org/project/htmlmai
  10. http://drupal.org/project/mailmime
  11. http://pear.php.net/
  12. http://php.net/manual/ini.core.php#ini.include-path
  13. http://drupal.org/project/include
  14. http://drupal.org/project/mailmime
  15. http://drupal.org/documentation/install/modules-themes/modules-5-6
  16. http://drupal.org/documentation/install/modules-themes/modules-5-6
  17. http://drupal.org/project/mailmime
  18. http://drupal.org/project/include
  19. http://drupal.org/project/mailmime
  20. http://drupal.org/project/include
  21. http://drupal.org/project/mailmime
