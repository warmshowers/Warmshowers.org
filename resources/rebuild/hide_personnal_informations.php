#!/usr/bin/env drush
<?php

$self_record = drush_sitealias_get_record('@warmshowers.dev');
if (!$self_record) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load your @warmshowers.dev alias.'));
}

drush_log("Updating database to hide sensitive information...",'ok');

$table_user = 'users';  //table containing users. (previously used for testing)
$table_location = 'user_location';  //table containing user's location (previously used for testing)
$table_wsuser = 'wsuser';

$req = "update {$table_location} set
street=CONCAT(oid,'_street'),
city=CONCAT(oid,'_city'),
postal_code=CONCAT(oid,'_postcode')";
drush_invoke_process('@warmshowers.dev','sqlq',array($req));

$req="update {$table_wsuser} set
fullname = CONCAT(uid,'_fullname'),
fax_number =case when fax_number='' then '' else CONCAT(uid,'_fax_number') end,
homephone = case when homephone='' then '' else CONCAT(uid,'_homephone') end,
mobilephone = case when mobilephone='' then '' else CONCAT(uid,'_mobilephone') end,
workphone = case when workphone='' then '' else CONCAT(uid,'_workphone') end";
drush_invoke_process('@warmshowers.dev','sqlq',array($req));

$req="update {$table_user} set data=''";
drush_invoke_process('@warmshowers.dev','sqlq',array($req));
