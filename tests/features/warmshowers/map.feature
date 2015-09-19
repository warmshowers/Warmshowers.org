#language:en
Feature: Map search
  In order to find Warmshowers members in a given area
  As an authenticated user
  I can use the interactive map

Background:
  Given I am an "authenticated" user

@smoke
Scenario: View map markers
  Then I will see "map markers" indicating location of hosts

@nav
Scenario: Drag the map
  And I am on the "Home" page
  And I can see the map loaded
  When I click and drag on the map
  Then I see the map move
  And I see new "map markers" appear

@nav
Scenario: Scroll into the map
  And I am on the "Home" page
  And I can see the map loaded
  When I click the "+" button on the map
  Then I see the map "zoom in"
  And I see the "map markers" reload

@smoke
Scenario: Select a host
  And I am on the "Home" page
  And I can see the map loaded
  When I click on a "map marker"
  Then I see a tooltip with the "name", "address", "picture", and "profile link" for the user

Scenario: Expand the map
  And I am on the "Home" page
  And I can see the map loaded
  When I click on the "Expand Map" link
  Then I see the map expand to fill the page

Scenario: Collapse the map
  And I am on the "Home" page
  And I can see the map loaded
  And I see the map is expanded
  When I click the "Collapse Map" link in the upper left
  Then I will see the map reduced to it's original size

Scenario: Dim map markers
  And I am on the "Home" page
  And I can see the map loaded
  When I click the "Dim Map Markers" checkbox
  Then I will see the map with "reduced-opacity markers"

Scenario: Restore dimmed map markers
  And I am on the "Home" page
  And I can see the map loaded
  And I have checked the "Dim Map Markers" checkbox
  When I uncheck the "Dim Map Markers" checkbox
  Then I will see the map with "standard-opacity markers"

Scenario: Show US Adventure Cycling Routes
  And I am on the "Home" page
  And I can see the map loaded
  When I check the "Load US Adventure Cycling Routes" checkbox
  Then I will see the Adventure Cycling routes highlighted on the map in green

Scenario: Hide US Adventure Cycling Routes
  And I am on the "Home" page
  And I can see the map loaded
  And I have checked the "Load US Adventure Cycling Routes" checkbox
  When I uncheck the "Load US Adventure Cycling Routes" checkbox
  Then I will not see the "US Adventure Cycling" routes 

@smoke
Scenario: Map search with search sidebar
  And I am on the "Home" page
  And I can see the map loaded
  When I select "United Kingdom" in the "Country" dropdown menu
  And I enter "London" in the "City or State/Province" field
  And I click on the "first autocomplete option"
  And I click on the "Map" button
  Then I will see the map focus shift to "London"

@smoke @bug
Scenario: Map search with search sidebar without autocomplete
  And I am on the "Home" page
  And I can see the map loaded
  When I select "United Kingdom" in the "Country" dropdown menu
  And I enter "London" in the "City or State/Province" field
  And I click on the "Map" button
  Then I will see the map focus shift to the "best-guess location" for "London"

@smoke
Scenario: Update location on the map
  And I am on the "Map My Home Location" page
  And I click my exact location on the map
  And I click "Submit location shown on map" button
  Then I see the "Map My Home Location" page
  And I see a modal with "Your location has been updated"

Scenario: Update location using latitude and longitude
  And I am on the "Map My Home Location" page
  And I enter "51.500000" in the "latitude" field
  And I enter "-0.20000" in the "longitude" field
  And I click "Submit location shown on map" button
  Then I see the "Map My Home Location" page
  And I see a modal with "Your location has been updated"

@smoke
Scenario: Update location use address geolocation
  And I am on the "Map My Home Location" page
  When I click the "Use Address Only for Map Location" button
  Then I see my "Profile" page
  And I see a modal with:
  """
  Your location will be determined by your address
  User location geocoded with accuracy 2
  Your map location has been determined from your address, but please check it using the Set Map Location tab
  """

Scenario: Hide user from map search
  And I have set my account as "unavailable"
  When I navigate to "my home location" on the map
  Then I will not see a "map marker" indicating "my home location"

Scenario: Show user in map search
  And I have set my account as "available"
  When I navigate to "my home location" on the map
  Then I will see a "map marker" indicating "my home location"
