#language:en
Feature: Search for content and users
  In order to access Warmshowers users or content
  As an authenticated user
  I can use the Search feature

@smoke
Scenario: Search site content
  Given I am an "authenticated" user
  And I am on the main "Forums" page
  And I can see the "Search" block
  When I enter "bicycle" in the "Search" text box
  And I click the "Search" button
  Then I will see the "Search content" page
  And I see "bicycle" pre-populated in the "Search" text field
  And I see a list of 10 "search results"
  And I see the word "bicycle" highlighted in bold in the "first search result"

@smoke @fail
Screnario: Can not search content as an unauthenticated user
  Given I am an "unauthenticated" user
  When I visit "/search/node/bicycle"
  Then I see the "Access Denied" page
  And I see a modal with:
  """
  Oops! You don't have access to this page right now. Are you logged in? If not, please log in below.
  """

@smoke
Scenario: Search for members
  Given I am an "authenticated" user
  And I am on the "Home" page
  And I can see the "Search" sidebar
  When I enter "Randy Fay" into the "Search by name, email, town" field
  And I click the "Go" button
  Then I will see the "Search member names and cities" page
  And I see "Randy Fay" pre-populated in the "Search" text field
  And I see a list of "search results"
  And I see the "name" of the all the results contains "Randy Fay"

@smoke @fail
Screnario: Can not search members as an unauthenticated user
  Given I am an "unauthenticated" user
  When I visit "/search/wsuser/Randy Fay"
  Then I see the "Access Denied" page
  And I see a modal with:
  """
  Oops! You don't have access to this page right now. Are you logged in? If not, please log in below.
  """

#map-based search included in map.feature

@smoke
Scenario: Search members by location
  Given I am an "authenticated" user
  And I am on the "Home" page
  And I can see the "Search" sidebar
  When I select "United Kingdom" in the "country" select menu
  And I enter "London" in the "City or State/Province" text field
  And I choose the first "autocomplete" option
  And I click the "list" button
  Then I will see the "Search member names and cities" page
  And I see "London, England|51.50853|-0.12574" pre-populated in the "Search" text field
  And I see a list of 50 "search results"
  And I see the "location" of the all the results contains "London"

Scenario: Search for members, usernames and content
  Given I am an "authenticated" user
  And I am on the "Search content" page
  And I have made a search with the keyword "bicycle"
  And I see a list of 10 "search results"
  When I go to the "Search member names and cities" page
  Then I will see the "Search member names and cities" page
  And I see a list of 50 "search results"
  
