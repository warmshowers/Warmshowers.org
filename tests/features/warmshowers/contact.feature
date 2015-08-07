#language:en
Feature:I can contact Warmshowers admin
  In order to communicate with Warmshowers admin
  As an authenticated or unauthenticated user 
  I can contact Warmshowers admin

Background:
  Given I have an email address

#Should the sender receive an email confirmation?
@test
Scenario: I can use the contact form to send an email to Warmshowers admin
  And I am on the Contact page
  When I enter values in all required fields
  And I enter an email address in the Your Email Address field
  And I click the Send Email button
  Then I see the main Warmshowers page
  And a modal with "Your message has been sent"
  And an email will be sent to Warmshowers Admin with my message.

@nav
Scenario: I can access the FAQ page using the link on the Contact page
  And I am on the Contact page
  When I click on the Read the FAQ First link
  Then I will see the FAQ page

@nav
Scenario: I can access the Help Forum page using the link on the Contact page
  And I am on the Contact page
  When I click on the Help Forum link
  Then I will see the Website Help and Support Forum page

#Validation/Fail Scenarios
@test
Scenario: I can NOT sent a message with an invalid email address
  And I am on the Contact page
  And I have entered values in all required fields
  When I enter a value that does not follow the correct pattern for an email address in the Your Email Address field
  And I click the Send Email button
  Then I see the Contact page with Email Address field highlighted
  And a modal with "You must enter a valid e-mail address."
  And my message will not be sent

@test
Scenario: I can NOT send a message without completing required form fields
  And I am on the Contact page
  And I have NOT entered values in all form fields
  When I click the Send Email button
  Then I see the Contact page with empty field highlighted
  And a modal with "[empty field] field is required."
  And my message will not be sent.