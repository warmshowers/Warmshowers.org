
Description:
Smart IP identify visitor's geographical location (longitude/latitude), country, 
region, city and postal code based on the IP address of the user. These information 
will be stored at session variable ($_SESSION) with array key 'smart_ip' and  Drupal 
$user->data object with array key 'geoip_location' of the user but optionally it can  
be disabled (by role) at Smart IP admin page. Other modules can use the function 
smart_ip_get_location($ip_address) that returns an array containing the visitor's 
ISO 3166 2-character country code, longitude, latitude, region, city and postal code. It 
provides a feature for you to perform your own IP lookup and admin spoofing of an arbitrary 
IP for testing purposes.

Maxmind's database is the source of Smart IP database that makes the association between IP 
address and geographical location (longitude/latitude), region, city and postal code. It can 
be found at http://www.maxmind.com/app/geolitecountry it has two versions: a very accurate 
and up to date payable version and a not quite accurate free lite version. Smart IP downloads 
and process the CSV files (GeoLiteCity-Location.csv and GeoLiteCity-Blocks.csv) to store to 
Smart IP database. An optional once a month (Maxmind updates its database every first day of 
a month) automatic update of the Smart IP database is provided or it can be manually updated 
at Smart IP admin page. The database of Maxmind is very huge, the two CSV files size is about 
150MB and the size when stored to SQL database is about 450MB with almost 5 million rows and 
about 600MB additional database space for temporary working table for Smart IP database 
update. The process of downloading the archived CSV files from Maxmind's server, extracting 
the downloaded zip file, parsing the CSV files and storing to the database will took more or 
less eight hours (depends on server's speed). It uses the batch system process. If interrupted 
by an unexpected error, it can recover from where it stopped or the administrator can manually 
continue the broken process at Smart IP admin page.

Another source of Smart IP is the IPInfoDB.com service which also uses Maxmind's database, in 
this case IPInfoDB.com will handle database resource load instead of your server's database. 
By default the use of IPInfoDB.com service as source is enabled. If IPInfoDB.com is desired to  
handle database resource load, it can be configured at Smart IP admin page settings.

Note: The Smart IP database is empty upon initial installation of this module. Either manually 
update the Smart IP database at admin page or wait for the cron to run and update Smart IP 
database automatically for you.

Requirements:
Drupal 7.x

Installation:
1. Copy the extracted smart_ip directory to your Drupal sites/all/modules directory.
2. Login as an administrator. Enable the module at http://www.example.com/?q=admin/modules
3. Set your private file system path at http://www.example.com/?q=admin/config/media/file-system
4. Configure/update Smart IP database/lookup an IP at 
http://www.example.com/?q=admin/config/people/smart_ip.

Support:
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/smart_ip