#language:en

Feature:Edit account Info
	In order to maintain accurate and helpful user info
	As a registered user
	I can edit my account information
	
Background:
	Given I am at the profile page
	And the Edit tab

Scenario Outline: Change Personal Info
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

#This one does NOT work on the current version of the site (bug)
Scenario: Change Country Choose Province
	When I change my country
	Then the State/Province list should populate with matching entries

Scenario: Change Country
	When I change my country
	And I select a new state
	And I choose a new city
	Then I should see a map of my new location on the profile page

Scenario: Add a photo
	When I click the Choose File button
	And select a file with size less than 20000 KB
	And click the save button
	Then my photo should appear on the profile page

Scenario: Delete a photo
	When I check the delete picture checkbox
	And click the save button
	Then my photo should no longer appear on the profile page