#language:en
Feature: Language negotiation
  In order to access Warmshowers content in my language
  As a user
  I can access content in my language

Background:
  Given I am on the "English" language site

@smoke
Scenario: Change language to French
  And I am on the "Contact" page
  When I select "French" from the language dropdown menu in the header
  Then I see the "page title" changed to "Contactez-nous"

@smoke
Scenario: Change language to Spanish
  And I am on the "Home" page
  When I select "Spanish" from the language dropdown menu in the header
  Then I see the "page title" changed to "Contacto"

@smoke
Scenario: Choose language for Warmshowers emails
  And I have set "English" as my "main language"
  And I am an "authenticated" user
  And I am on the "Profile edit" page
  When I select "French (Francais)" from the Language dropdown menu
  And I click the "Save" button
  Then I see a modal with "The changes have been saved."

@mail @smoke
Scenario: Receive Warmshowers emails in choosen language
  And I have set "French" as my "main language"
  When I receive a "Warmshowers notification email" in my inbox
  Then I see it written in "French"

Scenario: Translate profile page
  And I am on the another user's "Profile" page
  When I select a "Francais" from the language dropdown menu
  Then I see the user's About message translated into French
