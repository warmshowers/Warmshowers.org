#language:en
Feature: Use the contact form
  As an authenticated or unauthenticated user
  I can contact Warmshowers

  # Should the sender receive an email confirmation?
  @smoke @mail
  Scenario: Sending an email through the contact form
    Given I am on the page "contact"
    When I enter values in all required fields
    And I enter an email address in the Your Email Address field
    And I click the Send Email button
    Then I see the main Warmshowers page
    And a modal with "Your message has been sent"
    And an email will be sent to Warmshowers Admin with my message

  @smoke
  Scenario: Submit contact form with empty fields
    Given I am on the page "contact"
    When I click the Send Email button
    Then I see the Contact page with empty field highlighted
    And a modal with "[empty field] field is required."
    And my message will not be sent

  Scenario: Submit contact form with an invalid email address
    Given I am on the page "contact"
    And I have entered values in all required fields
    When I enter a value that does not follow the correct pattern for an email address in the Your Email Address field
    And I click the Send Email button
    Then I see the Contact page with Email Address field highlighted
    And a modal with "You must enter a valid e-mail address."
    And my message will not be sent
