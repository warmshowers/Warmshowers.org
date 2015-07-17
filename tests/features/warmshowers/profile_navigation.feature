#language:en
<<<<<<< Updated upstream
Feature:Edit account Info
=======
Feature:I can navigate to various locations starting from the profile page
>>>>>>> Stashed changes
	In order to effectively use the site
	As a registered user
	I can navigate the site starting from the profile page
	
<<<<<<< Updated upstream
Scenario: Click Update Button
	Given I am on the Profile page
	And the Profile tab
	When I click the Update button
	Then I see the Edit tab

=======
Background: 	
	Given I am on the Profile page
	And the Profile tab

Scenario: I can click tabs to navigate between profile, edit, messages, and feedback pages
	When I click the navigation tabs in the main content area
	Then I should see the corresponding content.

Scenario: I can reach the edit tab by clicking the Update button
	When I click the Update button in the location sidebar
	Then I see the Edit tab

Scenario: I can reach the edit tab by clicking the Upload your Picture link
	And I have not uploaded a profile picture
	When I click the "Upload your picture by editing your profile." link in the upper left
	Then I see the Edit tab.

Scenario: I can reach the location page using the Set Location button
	When I click the Set Location button in the location sidebar
	Then I see the Map My Home Location page

Scenario: I can see a large map of my approximate location
	When I click the small map in the location sidebar
	Then I see a modal map with my location and pins showing other nearby users

#Currently this link goes to a page called recommendations_of_me - it should probably direct instead to the feedback tab to improve navigation.
Scenario: I can see my feedback
	When I click on feedback in the profile header
	Then I see the feedback tab
>>>>>>> Stashed changes
