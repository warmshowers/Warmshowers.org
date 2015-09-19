#language:en
Feature: Resetting a password

Background:
  Given I am an authenticated user
  And I am not logged in

@smoke
Scenario: Request new password
  And I am on the "Password reset" page
  When I enter my "email address" in the "Username or E-mail Address" field
  And I click the "Email New Password" button
  Then I see the "Log In" page
  And I see a modal with "Further instructions have been sent to your e-mail address."
  And I receive an email with a password token

@smoke
Scenario: Reset password
  And I have received an email with a "password token"
  When I click on the "password reset" link in the email
  And I see the page with:
  """
  Reset password
  This is a one-time login for [user] and will expire on [date].
  """
  And I click the "Log In" button
  Then I will be logged in
  And I will see the my "Profile" page
  And a modal with:
  """
  You have just used your one-time login link. It is no longer necessary to use this link to login. Please change your password.
  """

# Message needs checking please
@smoke
Scenario: Change password
  And I have clicked the "One-time login" button
  When I enter a new password in the "password" field
  And I click the "Save" button
  Then I see a modal with:
  """
  Your new password has been saved."
  """

@nav
Scenario: Navigate to email whitelist help page
  And I am on the "Password reset" page
  When I click on the "Details on How to Do it are Here" link
  Then I will see the "Spam Filters" page with directions.

@nav
Scenario: Navigate to Password reset page following failed login
  And I am on the "User login" page
  And I enter an incorrect username in the "Username or Email Address" field
  And I click the "Log In" button
  When I click the "Have you Forgotten your Password?" link
  Then I see the "Password reset" page
  
@smoke @fail
Scenario: Request password with invalid email address
  And I am on the "Password reset" page
  When I enter an invalid "email address" in the "Username or E-mail Address" field
  And I click the "Email New Password" button
  Then I see a modal with:
  """
  Sorry, [incorrect username/e-mail address] is not recognized as a user name or an e-mail address.
  """

