#language:en

Feature: I can create an account
	In order to become a WarmShowers member
	As a new user
	I can create an account

Scenario Outline: As a new user, I can view the account creation page in my language
	Given I am at the <page> page
	And I have selected <lang> language
	When I see the Sign Up link in my language
	And I click the <size> Sign Up link in my chosen language
	Then I see the User Account page in my chosen language
	And the Create New Account tab in my chosen language

	Examples:
		|page			|lang 		|size	|
		|landing		|English	|large	|
		|landing		|English	|small	|
		|FAQ			|English	|large	|
		|Forums			|English	|small	|
		|landing		|Francais	|large	|
		|landing		|Espanol	|small	|

#Need to edit these examples with longer "about" text and proper dummy accounts.
Scenario Outline: As a new user, I can complete and submit the registration form
	Given I am at the User Account page
	And on the Create New Account tab
	When I enter <username> in the Username field
	And <email> in the email address field
	And reenter the correct email address in the confirm email address field
	And enter <password> in the password field
	And reenter the correct password in the confirm password field
	And select a <country>
	And enter a <city>/town
	And enter a <state>/province
	And enter a <fullname>
	And enter at least 15 words of <aboutText>
	Then I should see a modal with "A validation e-mail has been sent to your e-mail address. In order to gain full access to the site, you will need to follow the instructions in that message."
	And I should see the New Member Validation Instructions page
	And I should receive a validation email

	Examples:
		|username 	|email			 	|password		|country		|city		|state	|fullname		|aboutText			|
		|Spector 	|spector@aol.com 	|f00d			|United States	|Denver		|CO		|Inspector Spacetime|"I'm a dog and I love food" |
		|Mabs 		|Melissa@yahoo.com	|crochet		|Malaysia		|Kuala Lumpur|Kuala Lumpur	|Melissa Jones		|"I teach music in an international school"	|
		|Joe 		|js1234@gmail.com	|Wh0@Dood		|Singapore	|Cagayan de Oro City	|NOT LISTED		|Joe TheCat		|"I love to play the trumpet and teach English"		|
		|Grendal 	|gilgamesh@nunya.net	|eP1k	|South Korea	|Paju		|Gyeonggi-do |Jim Jones		|"I don't know what I'm doing here"|	
					

Scenario: After submitting the registration form, I can confirm my email address using the link I receive.
	Given I receive a validation email
	When I click on the validation link
	Then I should see the Welcome, New Member page
	And a modal with "You have successfully validated your e-mail address." and "We are asking every member to choose a donation level (there are free options). Please choose a donation level Thanks!" and "You have not uploaded a picture yet. Please upload a picture to improve your chances to find hosts or guests. Upload your picture by editing your profile."