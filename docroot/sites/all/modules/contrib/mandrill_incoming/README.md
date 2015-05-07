# Mandrill Incoming

This module handles incoming email from Mandrill via the Services module.

This module does not currently utilize the Drupal Mandrill module or PHP library.

## Configuration

Enable the module (and [Services](https://drupal.org/project/services), if not enabled), then visit `admin/config/services/mandrill_incoming`.

You'll need to enter the incoming webhook URL and webhook key. Then, specify an email that will be notified if any errors occur while processing messages. Finally, there is an option to continue processing even if Mandrill signature validation fails -- by default, validation errors are ignored.

## Usage

This module won't do much on its own. You need to implement `hook_mandrill_incoming_event` in a custom module to process incoming data. See `mandrill_incoming.api.php` for more information.

