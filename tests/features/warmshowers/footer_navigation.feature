#language:en
Feature:I can use the footer to navigate the site
  In order to effectively use the site
  As a new or registered user 
  I can navigate the site using the footer

Background: 
  Given I am on any Warmshowers page

Scenario: I can use the Home link to return to the Warmshowers Homepage
  When I click on the Home link in the footer
  Then I see the Warmshowers homepage

Scenario: I can contact Warmshowers admin
  When I click on the Contact Us link in the footer
  Then I see the Contact page
 
Scenario: I can reach the Warmshowers donation page
  When I click on the Donate! link in the footer
  Then I see the Donate page 

Scenario: I can reach the FAQ page
  When I click on the Frequently-Asked Questions link in the footer
  Then I see the FAQ page

Scenario: I can find out about Warmshowers mobile apps
  When I click on the Mobile Apps link in the footer
  Then I see the Mobile Apps page

#If the Links page really hasn't been updated since 2006, it needs some work! 
Scenario: I can see links to Warmshowers-related sites
  When I click on the Links link in the footer
  Then I see the Links page
 
Scenario: I can reach the login form
  And I am not logged in
  When I click the Log In link in the footer
  Then I see the User Account page
  And the Log In tab

Scenario: I can log out of Warmshowers
  And I am logged in
  When I click the Log Out link in the footer
  Then I will be logged out
  And I will see the home page as an unauthenticated user
  
Scenario: I can reach the Create Account page
  And I am not logged in
  When I click on the Sign Up link in the footer
  Then I see the User Account page
  And the Create New Account tab

Scenario: I can reach the Password Recovery page
  And I am not logged in
  When I click on the Password Recovery link in the footer
  Then I see the User Account page
  And the Request New Password tab

Scenario: I can find Warmshowers on Facebook
  And I am logged in to Facebook
  When I click on the Facebook icon in the footer
  Then I a new browser window or tab will open to display the Warmshowers Facebook group page

#I assume this works- don't have a twitter account so it might not, but the url looks right
Scenario: I can find Warmshowers on Twitter
  And I am logged in to Twitter
  When I click on the Twitter icon in the footer
  Then I a new browser window or tab will open to display the Warmshowers Twitter group page

Scenario: I can subscribe to the Warmshowers RSS feed
  When I click the RSS icon in the footer
  Then I see the RSS feed page
  And I can add the URL to my RSS feed reader