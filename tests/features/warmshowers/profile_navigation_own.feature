#language:en
Feature:I can navigate to various locations starting from my own profile page
	In order to effectively use the site
	As a registered user
	I can navigate the site starting from my profile page
	
Background: 	
	Given I am on my User page
	And the Profile tab

Scenario: I can click tabs to navigate between profile, edit, messages, and feedback pages
	When I click the navigation tabs in the main content area (profile, edit, messages, feedback)
	Then I should see the corresponding content.

Scenario: I can reach the edit tab by clicking the Update button
	When I click the Update button in the location sidebar
	Then I see the Edit tab

Scenario: I can reach the edit tab by clicking the Upload your Picture link
	And I have not uploaded a profile picture
	When I click the "Upload your picture by editing your profile." link in the upper left
	Then I see the Edit tab.

Scenario: I can see a modal with my profile picture
	And I have uploaded a profile picture
	When I click on my profile picture under the WarmShowers Logo
	Then I see a modal with my profile picture

Scenario: I can reach the location page using the Set Location button
	When I click the Set Location button in the location sidebar
	Then I see the Map My Home Location page

Scenario: I can see a large map of my approximate location
	When I click the small map in the location sidebar
	Then I see a modal map with my location and pins showing other nearby users

#Currently this link goes to a page called recommendations_of_me - it should probably direct instead to the feedback tab to improve navigation.
Scenario: I can see my feedback
	When I click on feedback in the green profile summary area
	Then I see the feedback tab

Scenario: I can see my personal website
	And I have entered a personal website url
	When I click the personal website link in the green profile summary area
	Then a new window or browser tab opens to display my website.

Scenario: I can reach the donation page
	When I click on the Donate Now button in the Donation sidebar
	Then I see the donation page.

Scenario: I can see my About Message in my chosen language
	When I select a language from the dropdown menu
	And I click the adjacent button (translate in selected language)
	Then I see my About message translated below