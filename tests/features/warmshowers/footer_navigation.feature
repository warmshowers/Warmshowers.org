#language:en
Feature:I can use the footer to navigate the site
  In order to effectively use the site
  As a new or registered user 
  I can navigate the site using the footer

Background: 
  Given I am on any Warmshowers page

@nav
Scenario: I can use the Home link in the footer to return to the Warmshowers Homepage
  When I click on the Home link in the footer
  Then I see the Warmshowers homepage

@nav
Scenario: I can reach the Contact form using the link in the footer
  When I click on the Contact Us link in the footer
  Then I see the Contact page

@nav 
Scenario: I can reach the Warmshowers donation page using the link in the footer
  When I click on the Donate! link in the footer
  Then I see the Donate page 

@nav
Scenario: I can reach the FAQ page using the link in the footer
  When I click on the Frequently-Asked Questions link in the footer
  Then I see the FAQ page

@nav
Scenario: I can find out about Warmshowers mobile apps using the link in the footer
  When I click on the Mobile Apps link in the footer
  Then I see the Mobile Apps page

@nav
Scenario: I can use the link in the footer to access a page of links to Warmshowers-related sites
  When I click on the Links link in the footer
  Then I see the Links page

@nav 
Scenario: I can reach the login form using the link in the footer
  And I am an unauthenticated user
  When I click the Log In link in the footer
  Then I see the User Account page
  And the Log In tab

@logout
Scenario: I can log out of Warmshowers using the link in the footer
  And I am an authenticated user
  When I click the Log Out link in the footer
  Then I will be logged out
  And I will see the home page as an unauthenticated user

@nav  
Scenario: I can reach the Create Account page using the link in the footer
  And I am an unauthenticated user
  When I click on the Sign Up link in the footer
  Then I see the User Account page
  And the Create New Account tab

@nav
Scenario: I can reach the Password Recovery page using the link in the footer
  And I am not logged in
  When I click on the Password Recovery link in the footer
  Then I see the User Account page
  And the Request New Password tab

@nav
Scenario: I can find Warmshowers on Facebook using the link in the footer
  And I am logged in to Facebook
  When I click on the Facebook icon in the footer
  Then I a new browser window or tab will open to display the Warmshowers Facebook group page

#I assume this works- don't have a twitter account so it might not, but the url looks right
@nav
Scenario: I can find Warmshowers on Twitter using the link in the footer
  And I am logged in to Twitter
  When I click on the Twitter icon in the footer
  Then I a new browser window or tab will open to display the Warmshowers Twitter group page

Scenario: I can subscribe to the Warmshowers RSS feed using the link in the footer
  When I click the RSS icon in the footer
  Then I see the RSS feed page
  And I can add the URL to my RSS feed reader