#language:en

Feature: Update account information
  In order to maintain accurate and helpful user info
  As a registered user
  I can edit my account information

Background:
  Given I am an authenticated user
  And I am at the "Profile edit" page

@smoke
Scenario: Select a country from the select menu field
  When I select a country using the dropdown menu
  Then the State/Province dropdown menu will populate with an appropriate list of states or provinces

@rules
Scenario: Change city, state, and country
  When I change my country
  And I select a new state
  And I choose a new city
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And I should see a map of my new location on the profile page

@smoke @rules
Scenario: Change profile picture
  When I click the Choose File button
  And I  a file with size less than 20000 KB
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And I should see my photo on my "Profile" page

@rules
Scenario: Delete profile picture
  When I check the delete picture checkbox
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  Then I should not see my photo on my "Profile" page

@smoke @rules
Scenario: Change password
  When I enter a value in the password field
  And I enter the same value in the confirm password field
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And I see a modal with "The changes have been saved."

@rules
Scenario: Change street address
  When I enter a street address
  And the Not Currently Available checkbox is not selected
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And my street address in the location sidebar
  And a map showing my location in the location sidebar
  And I see a modal with "The changes have been saved."

@smoke @rules
Scenario: Set account to unavailable
  When I check the Not Currently Available box
  And I select a date in the future
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
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

@smoke @rules
Scenario: Set account to available
  When I deselect the Not Currently Available checkbox
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And my hosting information and offerings hould be visible in the main content area
  And my address and phone number should appear in the location sidebar (if entered)
  And I should see a modal with: 
  """
  You have unchecked 'Not Currently Available' so your location will be shown on the map and you may receive guest requests.
  """

@rules
Scenario: Enter referral information
  When I enter a value in the "How you hear about Warmshowers" field
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And a modal indicating that changes are saved

@rules
Scenario: Opt out of nonessential emails
  When I check the box to opt out of nonessential emails
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And a modal indicating that changes are saved

@rules
Scenario: Change default time zone
  When I select a timezone from the dropdown menu
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And I see a modal with "The changes have been saved."

@rules
Scenario: Change hosting information and services
  When I change values for preferred notice, maximum guests, distances to nearby hotels, campgrounds, bike shops, and/or services I am willing to provide to guests in the appropriate form fields/checkboxes
  And I click the "Submit" button
  Then I should be redirected to my "Profile" page
  And I see a modal with "The changes have been saved."
  And if I am available to host, this information will be visible on my profile

@fail @smoke
Scenario: Can NOT change username to another existing username
  When I enter a username with an existing account
  And I click the save button
  Then I will see the Edit Account form with Username field highlighted
  And a modal with "The name [username] is already taken."
  And the change will not be saved

@fail
Scenario: Can NOT change email address to another existing email address
  When I enter an email address with an existing account
  And I click the save button
  Then I will see the Edit Account form with Email field highlighted
  And a modal with "The e-mail address [email] is already registered. Have you forgotten your password?"
  And the change will not be saved

@fail
Scenario: Can NOT change email address to an invalid email address
  When I enter a value that does not match the pattern for an email address in the Email Address field
  And I click the save button
  Then I will see the Edit Account form with Email Address field highlighted
  And a modal with "The e-mail address [value] is not valid."
  And the change will not be saved

@fail @smoke
Scenario: Can NOT submit with non-matching passwords
  When I enter a value in the Password field
  And I enter a different value in the Confirm Password field
  And I click the save button
  Then I will see the Edit Account form with Password and Confirm Password fields highlighted
  And a modal with "The specified passwords do not match."
  And the change will not be saved

@fail
Scenario: Can NOT submit About You with fewer than 15 words
  When I enter fewer than 15 words of text in the About You field
  And I click the save button
  Then I will see the Edit Account form with About You field highlighted
  And a modal with:
  """
  You must write at least 15 words about yourself into this 'About you' field. Please do not type in nonsense because we will read it and you may have your registration delayed.
  """
  And the change will not be saved

@fail @smoke
Scenario: Can NOT submit changes that leave required fields blank
  When I click the save button
  But I have not entered values in all required fields
  Then I will see the Edit Account form with empty field(s) highlighted
  And a modal informing me which values need to be completed
  And the changes will not be saved

@fail
Scenario: Can NOT submit an invalid URL in the Website field
  When I enter a value in the Website field that does not match the pattern for a URL
  And I click the Save button
  Then I will see the Edit Account form with Website field highlighted
  And a modal with "Please enter a valid URL (with http:// on the front) for your website"
  And the change will not be saved
