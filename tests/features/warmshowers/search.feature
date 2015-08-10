#language:en
Feature:I can search the Warmshowers site for content or user information
  In order to access Warmshowers users or content
  As an authenticated user
  I can use the Search feature

Background:
  Given I am an authenticated user

@smoke
Scenario: I can search site content
  And I am on any forum page
  When I enter a search term in the text box labeled Search
  And I click the Search button
  Then I will see the search page with a list of search results corresponding to my search term in the Site Content tab

@smoke
Scenario: I can find Warmshowers members by name, email, or town using the Search sidebar
  And I am on the main page or search page
  When I enter a name, email address, or town in the labeled input
  Then I will see the search page with a list of search results in the Member Names and Cities tab

#map-based search included in map.feature

@smoke
Scenario: I can find Warmshowers members by location using the Search sidebar
  And I am on the main page or search page
  When I select a country in the dropdown menu
  And I enter a city or state/province in the text box
  And I choose from the autocomplete options
  And I click list
  Then I will see the search page with a list of search results in the Member Names and Cities tab

#When I enter a username in the "Search by name, email, or town" field I still get main results in the Member Names and Cities tab.  The matching usernames are also displayed in the usernames tab, but this seems pretty redundant.
@smoke
Scenario: I can use fields on the search page to find specific content
  And I am on the search page
  When I enter a search term in the text field in any tab
  And I click search
  Then I will see a list of search results for that tab populate below the search field.
  And I can see other results for the same term by switching tabs.

