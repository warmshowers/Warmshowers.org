
SMTP Authentication Support module for Drupal 6.x.
This module adds SMTP functionality to Drupal.

REQUIREMENTS
------------
* Access to an SMTP server 
* PHP version 4.0.0 and up.
* The following PHP extensions need to be installed: ereg, hash, date & pcre.
* The PHPMailer package from Codeworx Tech., which can be found here:
  http://phpmailer.worxware.com/index.php?pg=phpmailer
  http://sourceforge.net/projects/phpmailer/files/phpmailer%20for%20php5_6/

  The latest version supported is 2.2.1

* Optional: To connect to an SMTP server using SSL, you need to have the
  openssl package installed on your server, and your webserver and PHP
  installation need to have additional components installed and configured.

INSTALL INSTRUCTIONS
--------------------
See INSTALL.txt

NOTES
-----
This module sends email by connecting to an SMTP server.  Therefore, you need
to have access to an SMTP server for this module to work.

Drupal will often use the email address entered into Administrator -> Site
configuration -> E-mail address as the from address.  It is important for
this to be the correct address and some ISPs will block email that comes from
an invalid address.

Because this module uses the PHPMailer package, it is rather large and may
cause PHP to run out of memory if its memory limit is small.

Connecting to an SMTP server using SSL is possible only if PHP's openssl
extension is working.  If the SMTP module detects openssl is available it
will display the options in the modules settings page.

Sending mail to Gmail requires SSL or TLS.
