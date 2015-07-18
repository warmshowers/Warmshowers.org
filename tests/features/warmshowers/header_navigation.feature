#language:en
Feature:I can use the header to navigate the site
	In order to effectively use the site
	As a new or registered user 
	I can navigate the site using the header

Scenario: I can use the logo to return to the Warmshowers Homepage
	Given I am on any Warmshowers page
	When I click on the Warmshowers Towel logo
	Then I see the Warmshowers homepage

#There is currently one exception to this- when "Expand Map" is clicked, the Home, My Profile, FAQ, and Forums links disappear from the header.  The size of the header does not change, so not sure why these links can't/shouldn't stay. 
Scenario: I can use the Home link to return to the Warmshowers Homepage
	Given I am on any Warmshowers page
	When I click on the Home link in the header
	Then I see the Warmshowers homepage

Scenario: I can choose a language to see the site
	Given I am on any Warmshowers page
	When I select a language from the dropdown menu
	Then I see the site text translated into my chosen language

#There is currently one exception to this- when "Expand Map" is clicked, the Home, My Profile, FAQ, and Forums links disappear from the header.  The size of the header does not change, so not sure why these links can't/shouldn't stay. 
Scenario: I can reach my profile page
	Given I am a registered user
	And I am logged in
	When I click on the My Profile link in the header
	Then I see my User page
	And the profile tab

#There is currently one exception to this- when "Expand Map" is clicked, the Home, My Profile/Sign Up, FAQ, and Forums links disappear from the header.  The size of the header does not change, so not sure why these links can't/shouldn't stay. 

Scenario: I can reach the Create Account page
	Given I am a new user
	When I click on the large Sign Up link in the header
	Then I see the User Account page
	And the Create New Account tab


Scenario: I can reach the Create Account page
	Given I am a new user
	When I click on the small Sign Up link above Log In in the header
	Then I see the User Account page
	And the Create New Account tab

#There is currently one exception to this- when "Expand Map" is clicked, the Home, My Profile, FAQ, and Forums links disappear from the header.  The size of the header does not change, so not sure why these links can't/shouldn't stay. 
Scenario: I can reach the FAQ page
	Given I am on any Warmshowers page
	When I click on the FAQ link in the header
	Then I see the FAQ page

#There is currently one exception to this- when "Expand Map" is clicked, the Home, My Profile, FAQ, and Forums links disappear from the header.  The size of the header does not change, so not sure why these links can't/shouldn't stay. 
Scenario: I can reach the Forums Area
	Given I am on any Warmshowers page
	When I click on the Forums link in the header
	Then I see the Forums entry page
	And the View Forums tab

Scenario: I can log out
	Given I am a registered user
	And I am logged in
	When I click the Log Out link next to my name in the header
	Then I will be logged out
	And I will see the home page as an unauthenticated user

Scenario: I can reach the login form
	Given I am a registered user
	And I am not logged in
	When I click the Log In link
	Then I should see the User Account page
	And the Log In tab