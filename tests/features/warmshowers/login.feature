#language:en
Feature: Logging into the website
  In order to access member-only content
  As an un-authenticated user 
  I can log in to Warmshowers

@smoke
Scenario: Log in with username
  Given I am an unauthenticated user
  And I am on the "User Login" page
  When I enter my Warmshowers username in the "Username or E-mail Address" field
  And I enter a "correct matching password" in the "Password" field
  And I click the "Log In" button
  Then I see the "Home" page as an authenticated user

@smoke
Scenario: Log in with email
  Given I am an unauthenticated user
  And I am on the "User Login" page
  When I enter my Warmshowers-registered email in the "Username or E-mail Address" field
  And I enter a "correct matching password" in the "Password" field
  And I click the "Log In" button
  Then I see the "Home" page as an authenticated user

@smoke
Scenario: Connect to Facebook
  Given I am an unauthenticated user
  And I am on the "User Login" page
  And I have a Facebook account
  And I have not previously used Facebook to login to Warmshowers
  When I click the "Facebook connect" button
  And a new browser window opens with the "Facebook authentication" page
  And a message about what information will be shared with Warmshowers
  And I click the "Okay" button
  Then I see the "Home" page as an authenticated user
  And a modal with "You've connected your account with Facebook."

@smoke
Scenario: Log in with Facebook
  Given I am an unauthenticated user
  And I am on the "User Login" page
  And I have connected my Facebook account with Warmshowers
  When I click the "Facebook connect" button
  Then I see the "Home" page as an authenticated user

@smoke
Scenario: Stay logged in
  Given I am an unauthenticated user
  And I am on the "User Login" page
  AND I have entered my Warmshowers username in the "Username or E-mail Address" field
  And I have entered a "correct matching password" in the "Password" field
  When I check the "Remember Me" checkbox
  And I click the "Log In" button
  And I restart my web browser
  And I go to the "Home" page
  Then I see the "Home" page as an authenticated user

@smoke
Scenario: Automatically log out
  Given I am an authenticated user
  And I am on the "Home" page
  When I restart my web browser
  And I go to the "Home" page
  Then I see the "Home" page as an unauthenticated user

@smoke @fail
Scenario: Log in with an incorrect username
  Given I am an unauthenticated user
  And I am on the "User Login" page
  When I enter an incorrect username in the "Username or Email Address" field
  And I enter a "correct matching password" in the "Password" field
  And I click the "Log In" button
  Then I see the "Log In" page with "Username" field highlighted
  And a modal with:
  """
  Sorry, unrecognized username or password. 
  Have you forgotten your password?
  """

@smoke @fail
Scenario: I can NOT log in to Warmshowers with an incorrect password
  Given I am an unauthenticated user
  And I am on the "User Login" page
  When I enter my Warmshowers username in the "Username or E-mail Address" field
  And I enter an incorrect password in the "Password" field
  And I click the "Log In" button
  Then I see the log in page with "Username" field highlighted
  And a modal with:
  """
  Sorry, unrecognized username or password. 
  Have you forgotten your password?
  """
