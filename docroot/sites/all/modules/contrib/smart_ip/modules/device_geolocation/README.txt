
Description:
Provides visitor's geographical location using client device location source 
that implements W3C Geolocation API whereas the coordinates are geocoded using Google 
Geocoding service. Google Geocoding returns a more detailed location information such 
as: street number, postal code, route, neighborhood, locality, sublocality, establishment, 
administrative area level 1, administrative area level 2, etc. 

Smart IP is the last fallback if W3C Geolocation API failed. Even if the visitors refuses 
to share their location, the geographical information provided by Smart IP will be used 
to know your visitors' geolocation details. A themeable Block content is available to 
show your visitor's geolocation information. Device Geolocation merges its location data 
(collected at Google Geocoding service) with Smart IP visitor's location data storage 
which is in session variable ($_SESSION) with array key 'smart_ip' and Drupal 
$user->data object with array key 'geoip_location'.

Requirements:
Drupal 7.x
Smart IP

Installation:
1. Copy the extracted device_geolocation directory to your Drupal sites/all/modules directory.
2. Login as an administrator. Enable the module at http://www.example.com/?q=admin/modules
as well as the Smart IP module.
3. Configure your visitor's geolocation details block at 
http://www.example.com/?q=admin/structure/block

Support:
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/device_geolocation