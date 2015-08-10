#language:en

Feature: I can access Warmshowers in supported languages
  In order to access Warmshowers content in my language
  As an authenticated or unauthenticated user
  I can access Warmshowers content in my language

Scenario: I can choose a language to see the site
  Given I am on any Warmshowers page
  When I select a language from the dropdown menu in the header
  Then I see the site text translated into my chosen language

Scenario: I can let other Warmshowers users know what languages I speak
  Given I am on the Create New Account or Edit Account page
  When I enter language(s) in the Languages Spoken field
  And I complete required fields
  And I submit the form
  Then my languages will be visible in the green summary area of my profile page.

@mail
Scenario: I can select a language in which to receive Warmshowers emails
  Given I am on the Create New Account or Edit Account page
  When I select or change a language from the Language dropdown menu
  And I complete required fields
  And I submit the form
  Then email communication from Warmshowers will be written in my chosen language.

#Currently doesn't actually require button click- not sure if the button should just be removed, or if this is an unexpected behavior?
Scenario: I can translate my About Message to my chosen language
  Given I am viewing a profile page (my own or another user's)
  When I select a language from the dropdown menu
  And I click the adjacent Translate To button
  Then I see the user's About message translated below

#I was not able to figure out what this selector does - possibly non-latin character selector? Or something that is not visible to the user. Leaving incomplete for now.

#Scenario: I can indicate the language of my forum post
#  Given I am an authenticated user
#  And I am in the Forums area
#  And the Create Forum Topic area
#  And I have completed required fields
#  When I select a language from the dropdown menu
#  And I click Submit
#  Then