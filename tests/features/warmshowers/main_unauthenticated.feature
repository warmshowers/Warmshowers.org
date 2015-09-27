#language:en
Feature: Homepage for unauthenticated userss
  In order to learn more about Warmshowers
  As an unauthenticated user
  I can use content and navigation on the Warmshowers Homepage

Background:
  Given I am on the homepage
  And I am an "unauthenticated" user

@nav
Scenario: View the how does it work sidebar
  Then I will see the "How Does It Work" sidebar
  And I will see an ordered list with four bullet points:
  """  
  1) Bike tourists and hosts sign up on the site.
  2) The interactive map and other search tools let travelers find hosts on their route.
  3) Travelers send a private message to potential hosts through the site.
  4) Hosts may offer hospitality consisting of a couch, a room, or a place to camp.
  """

@nav
Scenario: View the faq link in the sidebar
  When I click the "Frequently-Asked Questions" link in the "How Does It Work" sidebar
  Then I will see the "Frequently Asked Questions" page

@nav
Scenario: See the map image
  When I click on the "map image"
  Then I see a modal with a larger version of this map

@nav
Scenario: Visit the mobile apps page
  And I can see the "Mobile Apps" block
  When I click on the "Mobile Apps Available" link
  Then I will see the "Mobile Apps" page
