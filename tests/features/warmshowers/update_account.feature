#language:en

<<<<<<< Updated upstream
Feature:Edit account Info
=======
Feature:I can edit or update my account information
>>>>>>> Stashed changes
	In order to maintain accurate and helpful user info
	As a registered user
	I can edit my account information
	
Background:
	Given I am at the profile page
	And the Edit tab

<<<<<<< Updated upstream
Scenario Outline: Change Personal Info
=======
Scenario Outline: I can add or change my personal information
>>>>>>> Stashed changes
	When I change the value in the <field> field
	And click the save button
	Then I should see a modal about location determination
	And I should see my updated information on the <page> page

	Examples:
	|field		|page	|
	|username	|edit	|
	|username	|profile|	
	|Full Name	|profile|
	|About		|profile|
	|website	|profile|
	|home phone	|profile|
<<<<<<< Updated upstream

#This one does NOT work on the current version of the site (bug)
Scenario: Change Country Choose Province
	When I change my country
	Then the State/Province list should populate with matching entries

Scenario: Change Country
=======
	|language	|profile|
	|phone		|profile|

#This one does NOT work on the current version of the site (bug)
Scenario: I can change my country and see an appropriate list of provinces or states
	When I change my country
	Then the State/Province list should populate with matching entries

Scenario: I can change my city, state, and country
>>>>>>> Stashed changes
	When I change my country
	And I select a new state
	And I choose a new city
	Then I should see a map of my new location on the profile page

<<<<<<< Updated upstream
Scenario: Add a photo
=======
Scenario: I can add or change my profile picture
>>>>>>> Stashed changes
	When I click the Choose File button
	And select a file with size less than 20000 KB
	And click the save button
	Then my photo should appear on the profile page

<<<<<<< Updated upstream
Scenario: Delete a photo
	When I check the delete picture checkbox
	And click the save button
	Then my photo should no longer appear on the profile page
=======
Scenario: I can delete my profile picture
	When I check the delete picture checkbox
	And click the save button
	Then my photo should no longer appear on the profile page

Scenario: I can change my password
	When I enter a value in the password field
	And I enter the same value in the confirm password field
	And I click the save button
	Then I should see the profile page
	And a modal indicating that the changes are saved

Scenario: I can add or change my street address
	When I enter a street address
	And the Not Currently Available checkbox is not selected
	And I click the save button
	Then I should see the profile page 
	And my street address in the location sidebar
	And a map showing my location in the location sidebar
	And a modal indicating that the changes are saved

#The date selector in this field currently allows users to select a date in the past-- this should be fixed.
Scenario: I can set my account to unavailable
	When I check the Not Currently Available box
	And I select a date in the future
	And I click the save button
	Then I should see the Profile page
	And hosting information should read "This member has marked themselves as not currently available for hosting, so their hosting information is not displayed. Expected return --return date--"
	And my address information should not appear in the location sidebar
	And I should see a modal with "You have set your account to 'Not Currently Available' and you will be reminded about this by email from time to time. Please read the FAQ for more information."

Scenario: I can set my account to available
	When I deselect the Not Currently Available checkbox
	And I click the save button
	Then I should see the Profile Page
	And my hosting information and offerings hould be visible in the main content area
	And my address and phone number should appear in the location sidebar (if entered)
	And I should see a modal with "You have unchecked 'Not Currently Available' so your location will be shown on the map and you may receive guest requests."

Scenario: I can enter referral information
	When I enter a value in the How you hear about Warmshowers field
	And I click the save button
	Then I should see the profile page
	And a modal indicating that changes are saved

#The prompt for this option has confusing grammar (you can opt out most of newsletter or donation emails)
Scenario: I can opt out of nonessential emails
	When I check the box to opt out of nonessential emails
	And I click the save button
	Then I should see the profile page
	And a modal indicating that changes are saved
	And I should not receive newsletter and other nonessential email from Warmshowers.

Scenario: I can change my default email language
	When I select a language from the dropdown menu
	And I click the save button
	Then I should see the profile page
	And a modal indicating that changes are saved
	And I should receive emails in my chosen language.

#The fieldset for this option is collapsible by clicking the legend. Is this intentional?
Scenario: I can change my default time zone
	When I select a timezone from the dropdown menu
	And I click the save button
	Then I should see the profile page
	And a modal indicating that changes are saved
	And I should see times using this timezone throughout Warmshowers.
>>>>>>> Stashed changes
