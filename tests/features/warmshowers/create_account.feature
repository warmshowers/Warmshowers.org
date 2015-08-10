#language:en

Feature: I can create an account
  In order to become a WarmShowers member
  As a new user
  I can create an account

@smoke @mail
Scenario:I can complete and submit the registration form
  Given I am at the User Account page
  And on the Create New Account tab
  When I enter an available username in the Username field
  And a valid email address in the Email Address field
  And reenter the email address in the Confirm Email Address field
  And enter a password in the Password field
  And reenter the correct password in the Confirm Password field
  And select a country using the dropdown menu
  And enter a city/town in the City/Town field
  And enter a state/province using the dropdown menu
  And enter a full name in the Full Name field
  And enter at least 15 words of text about myself
  Then I should see a modal with:
  """
  A validation e-mail has been sent to your e-mail address. In order to gain full access to the site, you will need to follow the instructions in that message.
  """
  And I should see the New Member Validation Instructions page
  And I should receive a validation email

@smoke
Scenario: After submitting the registration form, I can confirm my email address using the link I receive.
  Given I receive a validation email
  When I click on the validation link
  Then I should see the Welcome, New Member page
  And a modal with:
  """
  You have successfully validated your e-mail address.
  We are asking every member to choose a donation level (there are free options). Please choose a donation level Thanks!
  You have not uploaded a picture yet. Please upload a picture to improve your chances to find hosts or guests. Upload your picture by editing your profile.
  """

#Currently this does not work correctly.
@smoke
Scenario: When I select a country from the dropdown menu, I can see an appropriate list of states or provinces
  Given I am on the Create Account page
  When I select a country using the dropdown menu
  Then the State/Province dropdown menu will populate with an appropriate list of states or provinces.

#Registration optional fields
Scenario: I can include a street address with my registration
  Given I am on the Create Account page
  When I enter a street address in the Street Address field
  And I complete all required fields  
  And I click the Create New Account button
  Then my account will be created
  And if I am available to host, my address will be visible on my profile page.

Scenario: I can include a zip/postal code with my registration
  Given I am on the Create Account page
  When I enter a postal code in the Postal Code field
  And I complete all required fields  
  And I click the Create New Account button
  Then my account will be created
  And if I am available to host, my postal code will be visible on my profile page. 

Scenario: I can include a phone number with my registration
  Given I am on the Create Account page
  When I enter a phone number in the Home Phone,Mobile Phone, or Work Phone field
  And I complete all required fields  
  And I click the Create New Account button
  Then my account will be created
  And my phone number will be visible on my profile page. 

Scenario: I can include hosting information about preferred notice, maximum guests, distances to nearby hotels, campgrounds, bike shops, and services I am willing to provide to guests with my registration
  Given I am on the Create Account page
  When I enter values for preferred notice, maximum guests, distances to nearby hotels, campgrounds, bike shops, and/or services I am willing to provide to guests in the appropriate form fields/checkboxes
  And I complete all required fields
  And I click the Create New Account button
  Then my account will be created
  And if I am available to host, this information will be visible on my profile.

Scenario: I can include a website in my registration
  Given I am on the Create Account page
  When I enter a valid URL in the Website field
  And I complete all required fields
  And I click the Create New Account button
  Then my account will be created
  And my website url will be displayed in the green summary area of my profile page.

Scenario: I can enter referral information
  When I enter a value in the How you hear about Warmshowers field
  And I complete all required fields
  And I click the Create New Account button
  Then my account will be created
  And Warmshowers will receive my referral data

#The date selector in this field currently allows users to select a date in the past-- this should be fixed.
Scenario: I can set my account to unavailable
  Given I am on the Create Account page
  When I check the Not Currently Available box
  And I select a date in the future
  And I complete all required fields
  And I click the Create New Account button
  Then my account will be created
  And hosting information on my profile page should read:
  """
  This member has marked themselves as not currently available for hosting, so their hosting information is not displayed. Expected return --return date--
  """
  And my address and post code information (if entered) should not appear in the location sidebar on my profile page

#Language scenarios are in language.feature

@mail
Scenario: I can opt out of nonessential emails
  Given I am on the Create Account page
  When I check the box to opt out of nonessential emails
  And I complete all required fields
  And I click the Create New Account button
  Then my account will be created
  And I should not receive newsletter and other nonessential email from Warmshowers.

#Validation/Fail Scenarios
@smoke
Scenario: I can NOT create an account using a username that has already been claimed
  Given I am on the Create Account page
  When I enter a username with an existing account
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Username field highlighted
  And a modal with "The name [username] is already taken."
  And the account will not be created.

@smoke
Scenario: I can NOT create a new account using an email address that already has an associated account
  Given I am on the Create Account page
  When I enter an email address with an existing account
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Email field highlighted
  And a modal with "The e-mail address [email] is already registered. Have you forgotten your password?"
  And the account will not be created.

@smoke
Scenario: I can NOT create an account without a valid email address
  Given I am on the Create Account page
  When I enter a value that does not match the pattern for an email address in the Email Address field
  And I enter the same value in the Confirm Email Address field
  And I complete all other required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Email Address field highlighted
  And a modal with "The e-mail address [value] is not valid."
  And the account will not be created.

@smoke
Scenario: I can NOT create an account with non-matching values in the Email Address and Confirm Email Address fields
  Given I am on the Create Account page
  When I enter an email address in the Email Address field
  And I enter a different email address in the Confirm Email Address field
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Email Address and Confirm Email Address fields highlighted
  And a modal with "Your e-mail address and confirmed e-mail address must match."
  And the account will not be created.

@smoke
Scenario: I can NOT create an account with non-matching values in the Password and Confirm Password fields
  Given I am on the Create Account page
  When I enter a value in the Password field
  And I enter a different value in the Confirm Password field
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Password and Confirm Password fields highlighted
  And a modal with "The specified passwords do not match."
  And the account will not be created.

@smoke
Scenario: I can NOT create an account with fewer than 15 words of text in the About You section
  Given I am on the Create Account page
  When I enter fewer than 15 words of text in the About You field
  And complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with About You field highlighted
  And a modal with 
  """
  You must write at least 15 words about yourself into this 'About you' field. Please do not type in nonsense because we will read it and you may have your registration delayed.
  """
  And the account will not be created.

@smoke
Scenario: I can NOT create an account without completing all required fields
  Given I am on the Create Account page
  When I fail to complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with empty field(s) highlighted
  And a modal informing me which values need to be completed.
  And the account will not be created.

Scenario: I can NOT submit an invalid URL in the Website field
  Given I am on the Create Account page
  When I enter a value in the Website field that does not match the pattern for a URL
  And I complete all required fields
  And I click the Create New Account button
  Then I will see the Create Account form with Website field highlighted
  And a modal with "Please enter a valid URL (with http:// on the front) for your website"
  And the account will not be created.