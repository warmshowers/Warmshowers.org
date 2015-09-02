#language:en
Feature: Logging into the website
  In order to access member-only content
  As an un-authenticated user 
  I can log in to Warmshowers

Background:
  Given I am an authenticated user

@smoke
Scenario: I can log in to Warmshowers using my username
  And I am on the User Account page
  And the Log in tab
  When I enter my Warmshowers username in the Username or E-mail Address field
  And I enter a correct matching password in the Password field
  And I click the Log In button
  Then I see the main page as an authenticated user.

@smoke
Scenario: I can log in to Warmshowers using my email address
  And I am on the User Account page
  And the Log in tab
  When I enter my Warmshowers-registered email in the Username or E-mail Address field
  And I enter a correct matching password in the Password field
  And I click the Log In button
  Then I see the main page as an authenticated user.

@smoke
Scenario: I can connect my Warmshowers account to Facebook
  And I am on the User Account page
  And the Log in tab
  And I have a Facebook account
  And I have not previously used Facebook to login to Warmshowers
  When I click the Facebook connect button
  And a new browser tab or window opens with Facebook
  And a message about what information will be shared with Warmshowers
  And I click Okay
  Then I see the main Warmshowers page as an authenticated user
  And a modal with "You've connected your account with Facebook."

@smoke
Scenario: I can login to Warmshowers using Facebook
  And I am on the User Account page
  And the Log In tab
  And I have connected my Facebook account with Warmshowers
  When I click the Facebook Connect button
  Then I see the main Warmshowers page as an authenticated user

@smoke
Scenario: I can have Warmshowers remember my login information
  And I am on the User Account page
  And the Log In tab
  And I have entered correct values for username/email address and password
  When I check the Remember Me checkbox
  And I click the Log In button
  Then I see the main Warmshowers page as an authenticated user
  And I will be logged in automatically on my next visit

#Validation/Fail Scenarios
@smoke
Scenario: I can NOT log in to Warmshowers with an incorrect user name or email address
  And I am on the User Account page
  And the Log in tab
  When I enter an incorrect username or email address in the Username or Email Address field
  And I enter a correct password in the Password field
  And I click the Log In button
  Then I see the log in page with username field highlighted
  And a modal with:
  """
  Sorry, unrecognized username or password. 
  Have you forgotten your password?
  """

@smoke
Scenario: I can NOT log in to Warmshowers with an incorrect password
  And I am on the User Account page
  And the Log in tab
  When I enter a correct username or email address in the Username or Email Address field
  And I enter an incorrect password in the Password field
  And I click the Log In button
  Then I see the log in page with username field highlighted
  And a modal with:
  """
  Sorry, unrecognized username or password. 
  Have you forgotten your password?
  """
