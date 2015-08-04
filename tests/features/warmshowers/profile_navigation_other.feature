  #language:en
Feature:I can navigate to various locations starting from another user's profile page
  In order to effectively use the site
  As a registered user
  I can navigate the site starting from any user's profile page

Background: 	
  Given I am on the Profile page
  And the Profile tab

Scenario: I can click tabs to navigate between a user's profile and feedback pages
  When I click the navigation tabs in the main content area (profile or feedback)
  Then I should see the corresponding content.

Scenario: I can see a modal with a user's profile picture
  And a user has uploaded a profile picture
  When I click on a user's profile picture under the WarmShowers Logo
  Then I see a modal with their picture

Scenario: I can see a large map of a user's approximate location
  When I click the small map in the location sidebar
  Then I see a modal map with the user's approximate location and pins showing other nearby users

#Moved feedback scenarios to feedback.feature

Scenario: I can see a user's personal website
  And the user has entered a personal website url
  When I click the personal website link in the green profile summary area
  Then a new window or browser tab opens to display the user website.

Scenario: I can see a user's About Message in my chosen language
  When I select a language from the dropdown menu
  And I click the adjacent button (translate in selected language)
  Then I see the user's About message translated below

#Send Message scenario moved to private_message.feature

