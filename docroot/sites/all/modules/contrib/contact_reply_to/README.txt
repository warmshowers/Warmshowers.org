If you enable this module, the "From" address on contact emails, both site
emails via the contact form and user-to-user emails via the member contact form,
will be "From" the email address configured in site_mail
(admin/config/system/site-information). The reply-to header will be set to the
address that Drupal would have used as the From address.

You can configure the addition of additional information to the subject and/or
the body of the email at admin/config/system/contact_reply_to.

This avoids many spam-classification issues. Many, many mail handlers will
classify as spam a mail that comes from an unauthorized location, as this is
spoofing. What Drupal does by default is spoofing...

Yahoo will no longer accept emails with spoofed from: headers, and other
providers seem to be moving this way.

Dreamhost actually *prevents* the sending of Drupal emails in many cases,
see http://wiki.dreamhost.com/Sender_Domain_Policy_and_Spoofing#What_is_the_sender_domain_policy.3F
This module should resolve that problem.
