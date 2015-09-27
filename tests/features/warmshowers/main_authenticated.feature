#language:en
Feature: Homepage for authenticated users
  In order to access Warmshowers features
  As an authenticated user
  I can use content and navigation on the Warmshowers Homepage

Background:
  Given I am on the "Home" page
  And I am an "authenticated" user

@nav
Scenario: View profile dashboard button
  When I click the "View Profile" button in the dashboard
  Then I will see my "Profile" page 

@nav
Scenario: Edit profile dashboard button
  When I click the "Edit Profile" button in the dashboard
  Then I will see the "Profile edit" page

@nav
Scenario: Message inbox dashboard button
  When I click the "Messages" button in the dashboard
  Then I will see my "Message inbox" page

@logout @smoke
Scenario: Log Out dashboard button
  When I click the "Log Out" button in the dashboard
  Then I will see the "Home" page as an unauthenticated user

Scenario: Recent feedback block
  Then I see a rotating list of feedback exerpts

@nav
Scenario: Access recent forum posts
  When I click on the "title" of a post in the Forum block
  Then I will see the "Forum topic" page

@nav
Scenario: Create Forum Post
  When I click the "Create a Post" button in the Forum block
  Then I see the "Create Forum Topic" page

@nav
Scenario: Search help page
  When I click the "Need Help Searching" link
  Then I see a the "Searching help" page
