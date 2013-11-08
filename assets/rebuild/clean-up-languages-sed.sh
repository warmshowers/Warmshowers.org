#!/bin/bash

devsite_host=warmshowers.dev
# If the English language version of the site has www on it, put "www." here.
# Otherwise www. will be deleted from the language URLs.
www_prefix=""

sed "s/warmshowers.org/$devsite_host/g
s/https/http/g
s/www./$www_prefix/g" languages.sql | drush @warmshowers.dev sqlc
