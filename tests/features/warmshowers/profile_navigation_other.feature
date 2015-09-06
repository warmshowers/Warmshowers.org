#language:en
Feature: Navigate around a users's profile page
  In order to effectively use the site
  As a registered user
  I can navigate the site starting from any user's profile page

Background: 	
  Given I am on the another user's "Profile" page

@fail smoke
Scenario: Check profiles are restricted
  Given I am an unauthenticated user
  Then I should see the "Access Denied" Page
  And a modal window should open with the message:
  """
  Oops! You don't have access to this page right now. Are you logged in? If not, please log in below.
  """

@nav
Scenario: View a user's feedback page
  Given I am an authenticated user
  When I click the "Feedback" tab
  Then I should see the "Feedback View" page

Scenario: View a user's picture
  And a user has uploaded a profile picture
  When I click on a user's profile picture under the WarmShowers Logo
  Then I see a modal with their picture

@nav
Scenario: View a user's personal website
  And the user has entered a personal website url
  When I click the personal website link in the green profile summary area
  Then a new window opens to display the user's website

