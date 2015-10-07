#!/bin/sh


# deploy_config.sh can contain:
# DEPLOY_TO="/var/www/warmshowers.org/docroot"
# FILES_DIR_GROUP=www-data

DEPLOY_TO=/var/www/warmshowers.org/docroot
FILES_DIR_GROUP=www-data

if [ -f ./deploy_config.sh ] ; then
  . ./deploy_config.sh
fi

echo "Deploying to $DEPLOY_TO"

# Ensure files directory is writable.
sudo chgrp -R $FILES_DIR_GROUP ${DEPLOY_TO}/files
sudo chmod -R ug+rw $DEPLOY_TO/files

# Remove robots.txt file, we let the robotstxt module handle this.
rm -f ${DEPLOY_TO}/robots.txt

# Run database updates, compile sass and revert features.
echo -e "\033[32;40mRunning database updates...\033[0m"
drush -r ${DEPLOY_TO} updb -y
echo -e "\033[32;40mRunning registry rebuild...\033[0m"
drush -r ${DEPLOY_TO} rr -y
echo -e "\033[32;40mCompiling sass...\033[0m"
cd ${DEPLOY_TO}/sites/all/themes/warmshowers_zen && compass compile
echo -e "\033[32;40mReverting features...\033[0m"
drush -r ${DEPLOY_TO} fra -y

# Fin.
echo -e "\033[32;40mClearing all drupal caches...\033[0m"
drush -r ${DEPLOY_TO} cc all -y

if [ -f /etc/init.d/memcached ] ; then
  echo -e "\033[32;40mRestarting memcached...\033[0m"
  sudo service memcached restart
fi

