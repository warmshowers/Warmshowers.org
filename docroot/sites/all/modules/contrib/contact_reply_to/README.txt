If you enable this module, the "From" address on contact emails, both site
emails via the contact form and user-to-user emails via the member contact form,
will be "From" the email address configured in site_mail
(admin/settings/site-information). The reply-to header will be set to the
address that Drupal would have used as the From address.

This avoids many spam-classification issues. Many, many mail handlers will
classify as spam a mail that comes from an unauthorized location, as this is
spoofing. What Drupal does by default is spoofing...

Dreamhost actually *prevents* the sending of Drupal emails in many cases,
see http://wiki.dreamhost.com/Sender_Domain_Policy_and_Spoofing#What_is_the_sender_domain_policy.3F
This module should resolve that problem.
