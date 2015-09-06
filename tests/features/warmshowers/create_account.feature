#language:en

Feature: Creating an account
  In order to become a WarmShowers member
  As a new user
  I can create an account

@fail @smoke
Scenario: Can NOT create an account using existing username
  Given I am on the Create Account page
  When I enter a username with an existing account
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Username field highlighted
  And a modal with "The name [username] is already taken."
  And the account will not be created

@fail @smoke
Scenario: Can NOT create a new account using an existing email address
  Given I am on the Create Account page
  When I enter an email address with an existing account
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Email field highlighted
  And a modal with "The e-mail address [email] is already registered. Have you forgotten your password?"
  And the account will not be created

@fail @smoke
Scenario: Can NOT create an account without a valid email address
  Given I am on the Create Account page
  When I enter a value that does not match the pattern for an email address in the Email Address field
  And I enter the same value in the Confirm Email Address field
  And I complete all other required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Email Address field highlighted
  And a modal with "The e-mail address [value] is not valid."
  And the account will not be created

@fail @smoke
Scenario: Can NOT create an account with non-matching Email Addresses
  Given I am on the Create Account page
  When I enter an email address in the Email Address field
  And I enter a different email address in the Confirm Email Address field
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Email Address and Confirm Email Address fields highlighted
  And a modal with "Your e-mail address and confirmed e-mail address must match."
  And the account will not be created

@fail @smoke
Scenario: Can NOT create an account with non-matching Passwords
  Given I am on the Create Account page
  When I enter a value in the Password field
  And I enter a different value in the Confirm Password field
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Password and Confirm Password fields highlighted
  And a modal with "The specified passwords do not match."
  And the account will not be created

@fail @smoke
Scenario: Can NOT create an account with fewer than 15 words of About You text
  Given I am on the Create Account page
  When I enter fewer than 15 words of text in the About You field
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with About You field highlighted
  And a modal with 
  """
  You must write at least 15 words about yourself into this 'About you' field. Please do not type in nonsense because we will read it and you may have your registration delayed.
  """
  And the account will not be created

@fail @smoke
Scenario: Can NOT create an account without completing all required fields
  Given I am on the Create Account page
  When I fail to complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with empty field(s) highlighted
  And a modal informing me which values need to be completed.
  And the account will not be created

Scenario: Can NOT submit an invalid URL in the Website field
  Given I am on the Create Account page
  When I enter a value in the Website field that does not match the pattern for a URL
  And I complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Website field highlighted
  And a modal with "Please enter a valid URL (with http:// on the front) for your website"
  And the account will not be created

@smoke @mail
Scenario: Complete the registration form
  Given I am at the Create New Account page
  When I enter an available username in the Username field
  And I enter a valid email address in the Email Address field
  And I reenter the email address in the Confirm Email Address field
  And I enter a password in the Password field
  And I reenter the correct password in the Confirm Password field
  And I select a country using the dropdown menu
  And I enter a city/town in the City/Town field
  And I enter a state/province using the dropdown menu
  And I enter a full name in the Full Name field
  And I enter at least 15 words of text about myself
  And I submit the form
  Then I should see a modal with:
  """
  A validation e-mail has been sent to your e-mail address. In order to gain full access to the site, you will need to follow the instructions in that message.
  """
  And I should see the New Member Validation Instructions page
  And I should receive a validation email

@smoke
Scenario: Validate an account
  Given I have completed the registration form
  And I receive a validation email
  When I click on the validation link
  Then I should see the Welcome, New Member page
  And a modal with:
  """
  You have successfully validated your e-mail address.
  We are asking every member to choose a donation level (there are free options). Please choose a donation level Thanks!
  You have not uploaded a picture yet. Please upload a picture to improve your chances to find hosts or guests. Upload your picture by editing your profile.
  """
