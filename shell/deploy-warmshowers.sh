#!/bin/sh

DEPLOY_TO=/var/www/docroot

# Ensure files directory is writable.
chmod -R 0775 ${DEPLOY_TO}/sites/default/files

# Remove robots.txt file, we let the robotstxt module handle this.
rm -f ${DEPLOY_TO}/robots.txt

# Run database updates, compile sass and revert features.
echo -e "\033[32;40mRunning database updates...\033[0m"
drush -r ${DEPLOY_TO} updb -y
echo -e "\033[32;40mRunning registry rebuild...\033[0m"
drush -r ${DEPLOY_TO} rr -y
echo -e "\033[32;40mCompiling sass...\033[0m"
cd ${DEPLOY_TO}/sites/all/themes/warmshowers_zen && compass compile --production
echo -e "\033[32;40mReverting features...\033[0m"
drush -r ${DEPLOY_TO} fra -y

# Fin.
echo -e "\033[32;40mClearing all drupal caches...\033[0m"
drush -r ${DEPLOY_TO} cc all -y

