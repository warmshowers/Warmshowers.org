#!/bin/bash

webroot=~/workspace/wsupg/docroot
devsite_host=wsupg.dev
# If the English language version of the site has www on it, put "www." here.
# Otherwise www. will be deleted from the language URLs.
www_prefix=""

#drush vset language_negotiation 0
sed "s/warmshowers.org/$devsite_host/g
s/https/http/g
s/www./$www_prefix/g" languages.sql  | drush sqlc -r $webroot
drush cc all


"
