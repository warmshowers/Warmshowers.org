This module adds the autocomplete=off attribute to selected key user forms.
On a browser that respects this setting, it means that the browser will not
try to autocomplete the password on the user login forms, or the whole
user edit form.

Many browsers, including current Chrome as of this writing and LastPass
do not respect this setting. But it can:

1. Offer some security on the login form
2. Prevent naive users from getting the browser-fill on the first password
   form and not the second, making them angry and confused.

To configure, install and enable the module, then visit
admin/settings/no_autocomplete to choose which feature you'd like to enable.


