#language:en
Feature: Header navigation
  In order to effectively use the site
  As an authenticated or unauthenticated user 
  I can navigate the site using the header

@nav @smoke 
Scenario: Click the Warmshowers Logo
  Given I can see the Warmshowers header
  When I click on the Warmshowers logo
  Then I see the "home" page

@nav
Scenario: Click on the Home menu link
  Given I can see the Warmshowers header
  When I click on the Home link in the header
  Then I see the "home" page

@nav @smoke
Scenario: Click on the My Profile menu link
  Given I am an authenticated user
  When I click on the My Profile link in the header
  Then I see my "Profile" page

@nav @smoke
Scenario: Click on the Sign Up menu link
  Given I am an unauthenticated user
  When I click on the large Sign Up link in the header
  Then I see the "Create New Account" page

@nav
Scenario: Click on the FAQ menu link
  Given I can see the Warmshowers header
  When I click on the FAQ link in the header
  Then I see the "FAQ" page

@nav
Scenario: Click on the Forum menu link
  Given I can see the Warmshowers header
  When I click on the Forums link in the header
  Then I see the "Forums" page

@nav
Scenario: Click on the small Sign Up header link
  Given I am an unauthenticated user
  When I click on the small Sign Up link above Log In in the header
  Then I see the "Create New Account" page

@nav @logout @smoke
Scenario: Click on the logout link
  Given I am an authenticated user
  When I click the Log Out link next to my name in the header
  Then I will be logged out
  And I will see the "Home" page as an unauthenticated user

@nav @smoke
Scenario: Click on the login link
  Given I am an unauthenticated user
  When I click the Log In link
  Then I should see the "Log In" page
