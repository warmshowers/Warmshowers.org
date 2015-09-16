$Id$

Description:
Smart IP Views Bridge exposes Smart IP visitor's location details to 
Views field (coordinates, country, ISO 3166 2-character country code, 
region, region code (FIPS), city and zip) and filter (country, ISO 3166 
2-character country code, region, region code (FIPS), city and zip).

Requirements:
Drupal 7.x
Smart IP
Views

Installation:
1. Login as an administrator. Enable the module at http://www.example.com/?q=admin/modules
as well as the Smart IP and Views modules.
2. Create your views at http://www.example.com/?q=admin/structure/views

Guide/example of using Smart IP views fields and filters with Location module:
1. Download/install Location module.
2. Add location CCK field to “Page” content type at
http://www.example.com/?q=admin/structure/types/manage/page/fields. Populate 
the field label with “Location” and field name with “location”. Select “Location” 
from Type of data to store.
3. At “Location” field settings > Locative Information > Collection Settings 
select “Allow” for the “City” and “Country” items.
4. Create a “Page” content. Populate the “Title”, “City” and select a country 
(Important: be sure that the city and country matches the geolocation that 
Smart IP has detected based on your IP. To check, enable the device_geolocation 
block - Please refer to the README.txt of device_geolocation module for the instructions).
5. Create your views at http://www.example.com/?q=admin/structure/views
6. Inside your Edit view, add a Filter criteria then select “Location: City” from the 
list (click “Add and configure filter criteria” button).
7. In “Configure filter criterion: Location: City”, populate the “Value:” textfield 
with “smart_ip][location][city” Smart IP Views token and leave the “Is equal to” 
selected in “Operator” dropdown menu (click “Apply” button). 
8. At your Edit view, add a Filter criteria then select “Location: Country” from the 
list (click “Add and configure filter criteria” button).
9. In “Configure filter criterion: Location: Country”, scroll down at the 
bottom of the “Country” list box and select “Smart IP: visitor's country code”. 
And select “Is one of” from “Operator” radio selection (click “Apply” button).
“This will filter contents with the country defined in Location CCK fields (Country 
and City) in ralation to your visitor's country and city detected by Smart IP.”
10. Add a field then select “Smart IP: Country” from the list (click 
“Add and configure fields” button).
11. In “Configure field: Smart IP: Country”, change the “Label” to “Your Country”
then select “Country name” from the “Display style:” dropdown menu (click 
“Update” button). This will display the country of your visitor along with other CCK 
fields you've included in your views.

Support:
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/smart_ip