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

# Run database updates, compile sass and revert features.
echo -e "\033[32;40mRunning database updates...\033[0m"
drush -r ${DEPLOY_TO} updb -y

// Don't want to accidentally do these thigns
#echo -e "\033[32;40mRunning registry rebuild...\033[0m"
#drush -r ${DEPLOY_TO} rr -y
#echo -e "\033[32;40mReverting features...\033[0m"
#drush -r ${DEPLOY_TO} fra -y

echo -e "\033[32;40mCompiling sass...\033[0m"
cd ${DEPLOY_TO}/sites/all/themes/warmshowers_zen && compass compile

# Fin.
echo -e "\033[32;40mClearing all drupal caches...\033[0m"
drush -r ${DEPLOY_TO} cc all -y

if [ -f /etc/init.d/memcached ] ; then
  echo -e "\033[32;40mRestarting memcached...\033[0m"
  sudo service memcached restart
fi

