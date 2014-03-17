<html><head></head><body>
<?php

//Customizations :
$database="warmshowers";
$server="127.0.0.1";
$db_user="warmshowers";
$db_password="";
$nb_rec=500; //amount of rows per update query
$table_user='users_test';  //table containing users
$table_location='user_location_test';  //table containing user's location
/*$default_values=array(    //default values when unserializing fails
  ["roles"]=> array("2"),
  ["contact"]=> 0,
  ["fullname"]=> "hidden",
  ["notcurrentlyavailable"]=> 1,
  ["availability"]=> "",
  ["fax_number"]=> "",
  ["mobilephone"]=> "",
  ["homephone"]=> "",
  ["workphone"]=> "",
  ["nearest_large_city"]=> "",
  ["preferred_notice"]=> "",
  ["cost"]=> "",
  ["maxcyclists"]=> "",
  ["motel"]=> "",
  ["campground"]=> "",
  ["bikeshop"]=> "",
  ["comments"]=> "no comments",
  ["services_available"]=> "0",
  ["howdidyouhear"]=> "",
  ["mail_accept"]=> "0",
  ["languagesspoken"]=> "",
  ["URL"]=> "",
  ["comments_per_page"]=> "",
  ["htmlarea_isenabled"]=> "1",
  ["token"]=> "",
  ["admin_compact_mode"]=> null,
  ["bed"]=> 0,
  ["food"]=> 0,
  ["laundry"]=> 0,
  ["lawnspace"]=> 0,
  ["sag"]=>0,
  ["shower"]=> 0,
  ["storage"]=> 0,
  ["kitchenuse"]=> 0,
  ["picture_delete"]=> 0,
  ["picture_upload"]=> "",
  ["form_build_id"]=> "",
  ["l10n_client_key"]=> "",
  ["htmlmail_plaintext"]=> False,
  ["l10n_client_disabled"]=> 0);*/
//End customizations

$nb_rows=0;   //affected rows
$err_req=0;   //nb of failed queries
$nb_query=0;  //nb of succesful queries
$err_unserialize=0;   //nb or errors when unserializing
$time=time();

function execute_query($req){
	global $mysqli,$nb_rows,$err_req,$nb_query;
	$mysqli->query($req);
	if($mysqli->error==''){
		$nb_rows+=$mysqli->affected_rows;
		$nb_query+=1;
		return True;
		}
	else {
		$err_req+=1;
		return False;
		}
	}

function exception_error_handler($errno, $errstr, $errfile, $errline ) {
    global $err_unserialize
  ;  $err_unserialize+=1;
}

set_error_handler("exception_error_handler");
error_reporting(E_ALL);

$mysqli = new mysqli($server, $db_user, $db_password, $database);

if (mysqli_connect_errno()) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}

//UPDATE USERS
$r=$mysqli->query("SELECT COUNT(*) as tot FROM ".$table_user);
$nb_rows_users = $r->fetch_assoc()['tot'];

echo '<b>update of table '.$table_user.'</b><br/>';
echo $nb_rows_users.' users in table '.$table_user.'.<br/>';

$result = $mysqli->query("select uid, data from users");
$req='update '.$table_user.' set data = case ';
$req2='(';

$i=$nb_rec;
$nb_query=0;
while($row = $result->fetch_assoc()){
	$data=unserialize($row['data']);
	if (!$data) {
		$data=$row['data'];
  }
	else{
		$data['fullname']='hidden';
		$data['fax_number']='hidden';
		$data['mobilephone']='hidden';
		$data['workphone']='hidden';
		$data['homephone']='hidden';
		$data=serialize($data);
	}
	$data=$mysqli->real_escape_string($data);
	$req.="when uid=".$row['uid']." then '".$data."' ";
	$req2.=$row['uid'].',';
	$i-=1;
	if($i==0){
		$req=$req.'end where uid in '.substr($req2,0,-1).')';
		execute_query($req);
		$i=$nb_rec;
		$req='update '.$table_user.' set data = case ';
		$req2='(';
		}
	}

$req=$req.'end where uid in '.substr($req2,0,-1).')';
if($i!=$nb_rec) execute_query($req);

echo $nb_rows.' users modified; '.$nb_query.' queries executed and '.$err_req.' queries failed.<br/>';
echo $err_unserialize.' errors of unserializing, unchanged data.<br/>';

//UPDATE LOCATION
echo '<b>update of table '.$table_location.'</b><br/>';
$nb_rows=0;
$req="update ".$table_location." set street='hidden',city='hidden',postal_code='hidden'";
if (execute_query($req)) echo $nb_rows." affected rows<br/><br/>";
else echo 'error updating '.$table_location.'.<br/>';

$t=time()-$time;
echo "time taken: ".$t.'s';

$mysqli->close();

?>
</body></html>
