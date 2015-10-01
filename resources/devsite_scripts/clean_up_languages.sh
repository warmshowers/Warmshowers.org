#!/bin/bash

webroot=~/workspace/wsupg/docroot
source_host=warmshowers.org
devsite_host=wsupg.dev
# If the English language version of the site has www on it, put "www." here.
# Otherwise www. will be deleted from the language URLs.
www_prefix=""

drush sqlc -r $webroot "UPDATE languages SET domain=REPLACE(domain, '$source_host', '$devsite_host');"
drush sqlc -r $webroot "UPDATE languages SET domain=REPLACE(domain, 'www.$source_host', '$www_prefix$devsite_host') WHERE language='en_working';"

drush cc all

