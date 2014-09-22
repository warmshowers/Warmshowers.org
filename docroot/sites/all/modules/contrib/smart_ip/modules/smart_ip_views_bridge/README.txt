$Id$

Description:
Smart IP Views Bridge exposes Smart IP visitor's location details to 
Views field (coordinates, country, ISO 3166 2-character country code, 
region, region code (FIPS), city and zip) and filter (country, ISO 3166 
2-character country code, region, region code (FIPS), city and zip).

Requirements:
Drupal 6.x
Smart IP
Views 2

Installation:
1. Login as an administrator. Enable the module at http://www.example.com/?q=admin/build/modules
as well as the Smart IP and Views modules.
2. Create your views at http://www.example.com/?q=admin/build/views

Guide/example of using Smart IP views fields and filters with Location module:
1. Download/install Location module.
2. Add location CCK field to “Page” content type (be sure to select 
“Allow” at the “City” from “Collection settings”).
3. Create a “Page” content. Populate the “Title”, “City” and select a country.
4. Create your views at http://www.example.com/?q=admin/build/views
5. Inside your Edit view, add a filter then select “Location: Country” and 
“Location: City” from the list (click “Add” button).
6. In “Defaults: Configure filter Location: Country”, scroll down at the 
bottom of the “Country” list and select “Smart IP: visitor's country code”. 
And select “Is” from “Operator:” radio selection. 
7. In “Defaults: Configure filter Location: City”, populate the “Value:” textfield 
with “smart_ip][location][city” Smart IP Views token and leave the “Is equal to” 
selected in “Operator:” dropdown menu.
“This will filter contents with the country defined in Location CCK fields (Country 
and City) in ralation to your visitor's country and city.”
8. Add a field then select “Smart IP: Country” from the list (click “Add” button).
9. In “Defaults: Configure field Smart IP: Country”, select “Country name” from the 
“Display style:” dropdown menu (click “Update” button). This will display the 
country of your visitor along with other CCK fields you've included in your views.

Support:
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/smart_ip