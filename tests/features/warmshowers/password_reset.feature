#language:en
Feature:I can log in to Warmshowers
  In order to access member-only content
  As an authenticated user 
  I can log in to Warmshowers

Background:
  Given I am an authenticated user and not logged in. 

@smoke
Scenario: I can request a new password
  And I am on the User Account page
  And the Request New Password tab
  When I enter my Warmshowers username or email address in the Username or E-mail Address field
  And I click the Email New Password button
  Then I see the User Account/Log In page
  And a modal with "Further instructions have been sent to your e-mail address."
  And I will receive an email with a password token.

@smoke
Scenario: I can use a password token to rest my Warmshowers password
  And I have received an email with a password token
  When I click on the link in my email
  And I see the page with:
  """
  Reset password
  This is a one-time login for [user] and will expire on [date].
  """
  And I click the Log In button
  Then I will be logged in
  And I will see the Edit Profile page
  And a modal with:
  """
  You have just used your one-time login link. It is no longer necessary to use this link to login. Please change your password.
  """
  And I can change my password.

@nav
Scenario: I can find out how to whitelist Warmshowers in my email settings by using the link on the Request Password page
  And I am on the User Account page
  And the Request New Password tab
  When I click on the Details on How to Do it are Here link
  Then I will see the Spam Filters page with directions.

@nav
Scenario: I can access the Request Password page using the link in the failed login modal
  And I have entered an incorrect username or password on the Log In page
  When I click the Have you Forgotten your Password? link
  Then I see the User Account page
  And the Request New Password tab.
  
#Validation/Fail Scenarios
Scenario: I can NOT request a password using an invalid username or email address
  And I am on the User Account page
  And the Request New Password tab
  When I enter an invalid username or password
  And I click the Email New Password button
  Then I see the Request Password page with username field highlighted
  And a modal with:
  """
  Sorry, [incorrect username/password] is not recognized as a user name or an e-mail address.
  """

