Things to consider doing when bringing in a database from the live site:
* Turn on reroute_email (and configure it) so no email can be sent out.
* Use the script here change_user_email.sh to change the user emails so they can't be used in any way
* use cleanup_languages.sh to fix the languages and rebuild the cache. You probably have to change the setting of what datatabase is going to load.
* Configure outgoing email if necessary (smtp settings, admin/settings/smtp)
* Clean up variables at admin/settings/language/i18n/variables
* Make sure you have a settings.php that will work. You may need to turn off the memcache settings, and you may need to use the standard google maps key that is in warmshowers.thefays.us
