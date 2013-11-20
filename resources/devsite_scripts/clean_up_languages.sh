#!/bin/bash

devsite_host=warmshowers.dev
# If the English language version of the site has www on it, put "www." here.
# Otherwise www. will be deleted from the language URLs.
www_prefix=""

#drush vset language_negotiation 0
sed "s/warmshowers.org/$devsite_host/g
s/https/http/g
s/www./$www_prefix/g" languages.sql  | drush sqlc
drush cc all


echo "Now please visit http://$devsite_host/admin/settings/language/i18n/variables and click 'clean up variables' at the bottom. You'll need the line in your settings.php that says 
$conf['user_location_google_key'] = $conf['wsmap_google_key'] = 'AIzaSyAvy-YGOa-_t3q6WuE90ZdU23M1mIdN3TQ';

"
