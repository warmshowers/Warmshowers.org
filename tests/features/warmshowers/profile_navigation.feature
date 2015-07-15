#language:en
Feature:Edit account Info
	In order to effectively use the site
	As a registered user
	I can navigate the site starting from the profile page
	
Scenario: Click Update Button
	Given I am on the Profile page
	And the Profile tab
	When I click the Update button
	Then I see the Edit tab

