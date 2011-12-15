<?php

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[mailkey].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $subject: The message subject.
 * - $body: The message body in HTML format.
 * - $mailkey: The message identifier.
 * - $recipient: An email address or user object who is receiving the message.
 * - $css: Internal style sheets.
 *
 * @see template_preprocess_mimemail_message()
 */
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" >
    <style type="text/css">
     div {background-color:#fffaf0;}
    </style>
  </head>
  <body id="mimemail-body" <?php if ($mailkey): print 'class="'. $mailkey .'"'; endif; ?>>
    <div id="center">
      <div id="main">

	<img src="http://www.warmshowers.org/sites/all/modules/dev/warmshowers_site/images/WSL_header.jpg" alt="WSL Header" width="725" height="100" />
        
	 <?php print $body ?>

      </div>
    </div>
  </body>
</html>
