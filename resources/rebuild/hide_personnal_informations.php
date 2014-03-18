#!/usr/bin/env drush
<?php

drush_log("Updating users and user_location table to hide sensitive information...",'ok');

$self_record = drush_sitealias_get_record('@warmshowers.dev');
if (!$self_record) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load your @warmshowers.dev alias.'));
}

$database_host = $self_record['databases']['default']['default']['host'];
$database_name = $self_record['databases']['default']['default']['database'];
$database_username = $self_record['databases']['default']['default']['username'];
$database_password = '';
if ($self_record['databases']['default']['default']['password']) {
  $database_password = $self_record['databases']['default']['default']['password'];
}

$nb_rec = 500; //amount of rows per update query (table users only)
$table_user = 'users';  //table containing users. (previously used for testing)
$table_location = 'user_location';  //table containing user's location (previously used for testing)
$default_values = array(    //default values when unserializing fails
      "roles" => array("2"),
      "contact" => 0,
      "notcurrentlyavailable" => 1,
      "availability" => "",
      "nearest_large_city" => "",
      "preferred_notice" => "",
      "cost" => "",
      "maxcyclists" => "",
      "motel" => "",
      "campground" => "",
      "bikeshop" => "",
      "comments" => "no comments",
      "services_available" => "0",
      "howdidyouhear" => "",
      "mail_accept" => "0",
      "languagesspoken" => "",
      "URL" => "",
      "comments_per_page" => "",
      "htmlarea_isenabled" => "1",
      "token" => "",
      "admin_compact_mode" => 0,
      "bed" => 0,
      "food" => 0,
      "laundry" => 0,
      "lawnspace" => 0,
      "sag" => 0,
      "shower" => 0,
      "storage" => 0,
      "kitchenuse" => 0,
      "picture_delete" => 0,
      "picture_upload" => "",
      "form_build_id" => "",
      "l10n_client_key" => "",
      "htmlmail_plaintext" => False,
      "l10n_client_disabled" => 0,
      );

global $nb_rows,$err_req,$nb_query,$err_unserialize,$mysqli;
$nb_rows = 0;   //affected rows
$err_req = 0;   //nb of failed queries
$nb_query = 0;  //nb of succesful queries
$err_unserialize = 0;   //nb or errors when unserializing
$mysqli = new mysqli($database_host, $database_username, $database_password, $database_name);

if (mysqli_connect_errno()) {
    drush_log("*Connect failed: %s", mysqli_connect_error(),'error');
    return;
}

function execute_query($req) {
  global $nb_rows,$err_req,$nb_query,$mysqli;
  $mysqli->query($req);
  if($mysqli->error == ''){
    $nb_rows += $mysqli->affected_rows;
    $nb_query += 1;
    return True;
    }
  else {
    $err_req += 1;
    return False;
    }
  }

//UPDATE USERS
$result = $mysqli->query("select uid, data from users");
$req = 'update '.$table_user.' set data = case ';
$req2 = '(';

$i = $nb_rec;
$nb_query = 0;
while($row = $result->fetch_assoc()){
  $data = unserialize($row['data']);
  if (!$data) {
    $err_unserialize += 1;
    $data = $default_values;
  }
  $data['fullname'] = $row['uid']."'s full name";
  $data['fax_number'] = $row['uid']."'s fax number";
  $data['mobilephone'] = $row['uid']."'s mobile phone";
  $data['workphone'] = $row['uid']."'s work phone";
  $data['homephone'] = $row['uid']."'s home phone";
  $data = serialize($data);
  $data = $mysqli->real_escape_string($data);
  $req .= "when uid=".$row['uid']." then '".$data."' ";
  $req2 .= $row['uid'].',';
  $i -= 1;
  if($i == 0) {  //when i=0 it's time to execute the query as it's 500 rows big
    $req = $req.'end where uid in '.substr($req2,0,-1).')';
    execute_query($req);
    $i = $nb_rec;
    $req = 'update '.$table_user.' set data = case ';
    $req2 = '(';
    }
  }

$req = $req.'end where uid in '.substr($req2,0,-1).')';
if ($i != $nb_rec) {
  execute_query($req);
}
drush_log('*update of users completed, '.$nb_rows.' users modified; '.$nb_query.' queries executed and '.$err_req.' queries failed. '.$err_unserialize.' errors of unserializing, default values assigned.','ok');

//UPDATE LOCATION
$nb_rows = 0;
$req = "update {$table_location} set street=CONCAT(oid,'_street'),city=CONCAT(oid,'_city'),postal_code=CONCAT(oid,'_postcode')";

if (execute_query($req,$mysqli,$nb_rows,$err_req,$nb_query)) {
  drush_log('*update of user_location, '.$nb_rows." affected users",'ok');
}
else {
  drush_log('*error updating user_location.','error');
}

$mysqli->close();
?>
