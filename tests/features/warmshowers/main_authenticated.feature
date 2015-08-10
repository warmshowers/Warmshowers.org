#language:en
Feature:I can access important Warmshowers features from the Warmshowers homepage
  In order to access Warmshowers features
  As an authenticated user
  I can use content and navigation on the Warmshowers Homepage

Background:
  Given I am on the homepage
  And I am an authenticated user

@nav
Scenario: I can quickly access my profile using the dashboard buttons
  When I click the View Profile button in the dashboard
  Then I will see my profile page 

@nav
Scenario: I can reach the Edit Profile form using the dashboard buttons
  When I click the Edit Profile button in the dashboard
  Then I will see the edit profile page

@nav
Scenario: I can reach the messages area using the dashboard buttons
  When I click the Messages button in the dashboard
  Then I will see my messages page, with inbox open

@nav
Scenario: I can reach the Map My Home Location page using the dashboard buttons
  When I click the Update Location button in the dashboard
  Then I will see the Map My Home Location page

@logout @smoke
Scenario: I can Log Out using the dashboard buttons
  When I click the Log Out button in the dashboard
  Then I will see the home page as an unauthenticated user.

Scenario: I can see photos taken by Warmshowers users
  When I click on the Touring Photos slideshow
  Then I will see a large modal with touring photos.

@nav
Scenario: I can see information about touring photos
  When I click on the caption link below the slideshow photos
  Then I will see a page about the associated contest entry, comments, and voting.

Scenario: I can read recent feedback
  Then I see a rotating list of feedback exerpts with links to the authors' profiles.

@nav
Scenario: I can quickly access recent forum threads
  When I click on the title of a thread in the Forum block
  Then I will see that thread's page

@nav
Scenario: I can reach the Create a Forum Post form using links on the main page.
  When I click the Create a Post button in the Forum block
  Then I see the Create Forum Topic page

Scenario: I can use forward/back buttons to see pictures in the Touring Photos slideshow
  And I have opened the large photo modal
  When I click the forward/back buttons at the bottom of the picture
  Then I will see the corresponding photo content within the modal.

Scenario: I can pause the embedded Touring Photos slideshow
  When I hover over the slideshow
  Then the auto-advance will pause and I can continue looking at one photo.
  
@nav
Scenario: I can access tips for successfully finding hospitality
  When I click the Need Help Searching link
  Then I see a page with help and tips

#map stuff in map.feature

#search stuff in search.feature