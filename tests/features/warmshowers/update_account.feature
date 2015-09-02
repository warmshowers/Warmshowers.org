#language:en

Feature: Editing account information
  In order to maintain accurate and helpful user info
  As a registered user
  I can edit my account information

Background:
  Given I am an authenticated user
  And I am at the profile edit page

@smoke
Scenario: I can change my personal information
  When I add or change the value in any form field
  And click the save button
  Then I see my profile page with my changes published
  And I see a modal with "The changes have been saved."

@smoke
Scenario: I can change my country
  When I change my country
  Then the State/Province list should populate with matching entries for that country

Scenario: I can change my city, state, and country
  When I change my country
  And I select a new state
  And I choose a new city
  Then I should see a map of my new location on the profile page

@smoke
Scenario: I can change my profile picture
  When I click the Choose File button
  And select a file with size less than 20000 KB
  And click the save button
  Then my photo should appear on the profile page

Scenario: I can delete my profile picture
  When I check the delete picture checkbox
  And click the save button
  Then my photo should no longer appear on the profile page

@smoke
Scenario: I can change my password
  When I enter a value in the password field
  And I enter the same value in the confirm password field
  And I click the save button
  Then I should see the profile page
  And I see a modal with "The changes have been saved."

Scenario: I can change my street address
  When I enter a street address
  And the Not Currently Available checkbox is not selected
  And I click the save button
  Then I should see the profile page 
  And my street address in the location sidebar
  And a map showing my location in the location sidebar
  And I see a modal with "The changes have been saved."

@smoke
Scenario: I can set my account to unavailable
  When I check the Not Currently Available box
  And I select a date in the future
  And I click the save button
  Then I should see the Profile page
  And hosting information should read:
  """
  This member has marked themselves as not currently available for hosting, so their hosting information is not displayed. Expected return [return date]
  """
  And my address information should not appear in the location sidebar
  And I should see a modal with:
  """
  You have set your account to 'Not Currently Available' and you will be reminded about this by email from time to time. 
  Please read the FAQ for more information.
  """

@smoke
Scenario: I can set my account to available
  When I deselect the Not Currently Available checkbox
  And I click the save button
  Then I should see the Profile Page
  And my hosting information and offerings hould be visible in the main content area
  And my address and phone number should appear in the location sidebar (if entered)
  And I should see a modal with: 
  """
  You have unchecked 'Not Currently Available' so your location will be shown on the map and you may receive guest requests.
  """

Scenario: I can enter referral information
  When I enter a value in the How you hear about Warmshowers field
  And I click the save button
  Then I should see the profile page
  And a modal indicating that changes are saved

@mail
Scenario: I can opt out of nonessential emails
  When I check the box to opt out of nonessential emails
  And I click the save button
  Then I should see the profile page
  And a modal indicating that changes are saved
  And I should not receive newsletter and other nonessential email from Warmshowers.

Scenario: I can change my default time zone
  When I select a timezone from the dropdown menu
  And I click the save button
  Then I should see the profile page
  And I see a modal with "The changes have been saved."
  And I should see times using this timezone throughout Warmshowers.

Scenario: I can add or change hosting information and services I am willing to provide to guests
  When I add or change values for preferred notice, maximum guests, distances to nearby hotels, campgrounds, bike shops, and/or services I am willing to provide to guests in the appropriate form fields/checkboxes
  Then I should see the profile page
  And I see a modal with "The changes have been saved."
  And if I am available to host, this information will be visible on my profile.

# Validation/fail scenarios
@smoke
Scenario: I can NOT change my username to one that has already been claimed
  When I enter a username with an existing account
  And I click the save button
  Then I will see the Edit Account form with Username field highlighted
  And a modal with "The name [username] is already taken."
  And the change will not be saved.

Scenario: I can NOT change my email address to one that already has an associated account
  When I enter an email address with an existing account
  And I click the save button
  Then I will see the Edit Account form with Email field highlighted
  And a modal with "The e-mail address [email] is already registered. Have you forgotten your password?"
  And the change will not be saved.

Scenario: I can NOT change my email address to an invalid email address
  When I enter a value that does not match the pattern for an email address in the Email Address field
  And I click the save button
  Then I will see the Edit Account form with Email Address field highlighted
  And a modal with "The e-mail address [value] is not valid."
  And the change will not be saved.

@smoke
Scenario: I can NOT change my password with non-matching values in the Password and Confirm Password fields
  When I enter a value in the Password field
  And I enter a different value in the Confirm Password field
  And I click the save button
  Then I will see the Edit Account form with Password and Confirm Password fields highlighted
  And a modal with "The specified passwords do not match."
  And the change will not be saved.

Scenario: I can NOT change my About You text to contain fewer than 15 words
  When I enter fewer than 15 words of text in the About You field
  And I click the save button
  Then I will see the Edit Account form with About You field highlighted
  And a modal with:
  """
  You must write at least 15 words about yourself into this 'About you' field. Please do not type in nonsense because we will read it and you may have your registration delayed.
  """
  And the change will not be saved.

@smoke
Scenario: I can NOT submit changes that leave required fields blank
  When I click the save button
  But I have not entered values in all required fields
  Then I will see the Edit Account form with empty field(s) highlighted
  And a modal informing me which values need to be completed.
  And the change will not be saved.

Scenario: I can NOT submit an invalid URL in the Website field
  When I enter a value in the Website field that does not match the pattern for a URL
  And I click the Save button
  Then I will see the Edit Account form with Website field highlighted
  And a modal with "Please enter a valid URL (with http:// on the front) for your website"
  And the change will not be saved.
