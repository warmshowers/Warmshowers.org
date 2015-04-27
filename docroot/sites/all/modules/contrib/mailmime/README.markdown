## [Mail MIME](http://drupal.org/project/mailmime)

Provides a class for creating MIME messages.

*   *NOTE: This module does not send mail.*

    :   If you came here looking for a mail-sending module, try
        [HTML Mail](http://drupal.org/project/htmlmail) (which can use this
        module for MIME-handling) or
        [Mime Mail](http://drupal.org/project/mimemail) (which comes with its
        own MIME-handling library).

[Mail Mime](http://drupal.org/project/mailmime) extends certain
[PEAR](http://pear.php.net/) mail-handling classes to provide a
drupal-friendly library for creating and parsing MIME messages.  Neither
[Mail Mime](http://drupal.org/project/mailmime) nor the original
[PEAR](http://pear.php.net/) classes send mail nor do anything
useful on their own.  They are intended as code libraries to be used by *other*
programs that send or receive mail.

[Mail Mime](http://drupal.org/project/mailmime) started out as part of
[HTML Mail](http://drupal.org/project/htmlmai).  I separated into its own
module for two reasons:

1.  The separation helped clarify in my own mind which parts of the mail-sending
    process require MIME and which do not.  The resulting code is simpler and
    easier to maintain.

2.  It is possible that another mail-sending or mail-reading module may find the
    [Mail Mime](http://drupal.org/project/mailmime) library useful.

### <a id="requirements">Requirements</a>

The following files, available from [PEAR](http://pear.php.net/), must be
installed and available somewhere on the
[`include_path`](http://php.net/manual/ini.core.php#ini.include-path).

*    `Mail/mime.php`
*    `Mail/mimeDecode.php`
*    `Mail/mimePart.php`
*    `PEAR.php`
*    `PEAR5.php`

1.   One way to satisfy the requirements is to run the following
     commands from a Unix root shell prompt:

     `pear install -a Mail_Mime`
     `pear install Mail_mimeDecode`

     The `-a` parameter ensures that dependencies, including
     `Mail/mimePart.php`, are also installed.

2.   Another way is to install and enable the
     [Include](http://drupal.org/project/include) module before enabling the
     [Mail MIME](http://drupal.org/project/mailmime) module.

### [Installation](http://drupal.org/documentation/install/modules-themes/modules-5-6)

1.  Download and install
    [as usual](http://drupal.org/documentation/install/modules-themes/modules-5-6).

2.  When enabled, [Mail Mime](http://drupal.org/project/mailmime) will

    *   Auto-detect whether the required files are present.

    *   If any files are missing, and the
        [Include](http://drupal.org/project/include) module is available,
        [Mail Mime](http://drupal.org/project/mailmime) will use
        [Include](http://drupal.org/project/include) to auto-download and
        enable the missing files.

    *   If all of the above fails,
        [Mail Mime](http://drupal.org/project/mailmime) will disable itself
        and print a warning to both the screen and the error log.
