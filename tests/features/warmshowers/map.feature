#language:en
Feature:I can locate available Warmshowers Hosts using the map
  In order to find Warmshowers members in a given area
  As an authenticated user
  I can use the interactive map

Background:
  Given I am an authenticated user

#Is there any rhyme or reason to the initial map focus? It keeps putting me in Michigan?
Scenario: I can use the drag interaction on the map to access information about Warmshowers User distribution
  When I click and drag on the map
  Then I see the map display change accordingly

Scenario: I can use the scroll/compass interaction on the map to access information about Warmshowers User distribution
  When I click a direction on the map compass in the upper left
  Then I see the map display change accordingly

Scenario: I can use the zoom in/out interaction on the map to access information about Warmshowers User distribution
  When I use the vertical slider
  Then I will see the map scale change accordingly

Scenario: I can zoom in on the map using double-click
  When I double click on a map location
  Then the map display will zoom in 

Scenario: I can see a street view map of an approximate location
  When I drag and drop the person icon from the top left of the map to a location of my choice
  Then I will see a street view image of that location

@smoke
Scenario: I can see Warmshowers users near a map location
  When I find a desired location on the map
  Then I will see map markers indicating location and number of hosts nearby

@smoke
Scenario: I can select a specific Warmshowers host using the map
  When I click on a map marker
  Then I see a tooltip with the name, address, picture, and profile link for the user(s)

Scenario: I can view the map in Satellite or standard mode
  When I click the Map or Satellite buttons in the upper right
  Then I will see the map in my chosen mode

Scenario: I can view the map in expanded form
  And I am on the main Warmshowers page
  When I click on the Expand Map link
  Then I see a larger view of the interactive map

Scenario: I can collapse the map to standard size
  And I have expanded the map
  When I click the Collapse Map link in the upper left
  Then I will see the standard authenticated main page again

Scenario: I can dim map markers
  And I am on the main Warmshowers page
  When I click the Dim Map Markers checkbox
  Then I will see the map with reduced-opacity markers

Scenario: I can restore map markers
  And I am on the main Warmshowers page
  And I have checked the Dim Map Markers checkbox
  When I uncheck the Dim Map Markers checkbox
  Then I will see the map with standard-opacity markers

@smoke
Scenario: I can show US Adventure Cycling Routes
  And I am on the main Warmshowers page
  When I check the Load US Adventure Cycling Routes checkbox
  Then I will see the Adventure Cycling routes highlighted on the map in green

Scenario: I can hide US Adventure Cycling Routes
  And I am on the main Warmshowers page
  And I have checked the Load US Adventure Cycling Routes checkbox
  When I uncheck the Load US Adventure Cycling Routes checkbox
  Then I will not see the Adventure Cycling routes 

#If the user fails to select an autocomplete option, search displays some strange behavior strange_map_search_bx.png - better to force a choice or show an explicit error
@smoke
Scenario: I can map Warmshowers members by location using the Search sidebar
  And I am on the main page
  When I select a country in the dropdown menu
  And I enter a city or state/province in the text box
  And I choose from the autocomplete options
  And I click map
  Then I will see the map focus shift to my chosen location with pins indicating nearby users.

@smoke
Scenario: I can submit an exact home location using the map
  And I am on the main page
  When I click the Update Location button
  And I click my exact location on the map
  And I click Submit Location Shown on Map
  Then I see the Map My Home Location page
  And a modal with "Your location has been updated"

@smoke
Scenario: I can submit an exact home location by entering latitude and longitude
  And I am on the main page
  When I click the Update Location button
  And I enter my latitude and longitude in the labeled fields
  And I click Submit Location Shown on Map
  Then I see the Map My Home Location page
  And a modal with "Your location has been updated"

Scenario: I can use only my address for location
  And I am on the Map My Home Location page
  When I click Use Address Only for Map Location
  Then I see my Profile page
  And a modal with "Your location will be determined by your address"

@nav
Scenario: I can reach the Map My Home Location page using the Set Location button
  And I am on my profile page
  When I click the Set Location button in the location sidebar
  Then I see the Map My Home Location page

@map
Scenario: I can see a large map of a user's approximate location
  And I am on a profile page (my own or another user's)
  When I click the small map in the location sidebar
  Then I see a modal map with the user's location and pins showing other nearby users
