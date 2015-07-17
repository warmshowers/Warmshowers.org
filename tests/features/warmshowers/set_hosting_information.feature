#language:en

Feature:I can or edit my hosting information
	In order to support users in making good host/guest matches
	As a registered user
	I can edit my hosting information
	
Background:
	Given I am at the profile page
	And the Edit tab
	And the Not Currently Available box is unchecked


Scenario Outline: I can add or change my  hosting information
	When I enter a value in the <field> field
	And I click the save button
	Then I should see the profile page
	And my <field> value, labeled under Hosting Information
	And a modal indicating that changes have been saved.

	Examples:
	|field				|
	|Preferred Notice	|
	|Max Guests			|
	|Dist to motel		|	
	|Dist to campground	|
	|Dist to bikeshop	|

Scenario: I can select services to offer a guest
	When I select services I may be able to offer
	And I click the save button
	Then I should see the profile page
	And my selected options in an unordered list titled "this host may offer" under Hosting Information
	And a modal indicating that changes have been saved.


