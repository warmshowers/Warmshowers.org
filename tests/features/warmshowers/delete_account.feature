#language:en

Feature: Delete user account
  In order to leave the Warmshowers community
  As a registered user
  I can delete my own account

@smoke
Scenario: Cancel my own account
  Given I am an "authenticated" user
  And I am on my "Account cancellation" page
  And I see the title "Are you sure you want to cancel your account?"
  And I click the "Cancel account" button
  Then I am logged out
  And I see the "Home" page
  And I can not login with my username and password

@smoke @fail
Scenario: Cancel another user's account
  Given I am an "authenticated" user
  And I am on another user's "Account cancellation" page
  Then I see the title "Access Denied"
  And I can not see the "Cancel account" button
