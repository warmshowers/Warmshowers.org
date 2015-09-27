#language:en
Feature: Navigate around own profile page
  In order to effectively use the site
  As an authenticated user
  I can navigate the site starting from my profile page
  
Background:   
  Given I am an authenticated user
  And I am on my "Profile" page

@nav
Scenario: Check for correct tab links
  Then I should see the "Profile" tab
  And I should see the "Edit" tab
  And I should see the "Messages" tab
  And I should see the "Feedback" tab
  And I should NOT see the "Orders" tab

@nav
Scenario: Click the Edit tab
  When I click the "Edit" tab
  Then I see the "Profile Edit" page

@nav
Scenario: Click the Messages tab
  When I click the "Messages" tab
  Then I see my "Message Inbox" page

@nav
Scenario: Click the Feedback tab
  When I click the "Feedback" tab
  Then I see the "Feedback View" page

@nav
Scenario: Click the Update link
  When I click the "Update" button in the location sidebar
  Then I see the "Profile Edit" page

@nav
Scenario: Click the Set Location link
  When I click the "Set Location" button in the location sidebar
  Then I see the "Map My Home Location" page

@nav
Scenario: Click the Upload your Picture link
  And I have not uploaded a profile picture
  When I click the "Upload your picture by editing your profile." link in the upper left
  Then I see the "Profile Edit" page

Scenario: View profile picture
  And I have uploaded a profile picture
  When I click on my profile picture under the WarmShowers Logo
  Then I see a modal with my profile picture at full size

@nav
Scenario: View personal website
  And I have entered a personal website url
  When I click the personal website link in the green profile summary area
  Then a new window or browser tab opens to display my website

@nav
Scenario: Click donation link
  Given I have not donated
  When I click on the "Donate Now" button in the Donation sidebar
  Then I see the "Donation" page
