#language:en
Feature:I can learn more about Warmshowers on the Warmshowers homepage
  In order to learn more about Warmshowers
  As an unauthenticated user
  I can use content and navigation on the Warmshowers Homepage

Background:
  Given I am on the homepage
  And I am an unauthenticated user

Scenario: I can find basic information about Warmshowers Hospitality sharing
  When I click the Frequently-Asked Questions link in the How Does It Work sidebar
  Then I will see the Frequently Asked Questions page

#This feature currently has some strange behavior. This user story describes my best guess about how it's meant to work.
Scenario: I can see a map representing concentrations of Warmshowers users in the US
  When I click on the map
  Then I see a modal with a larger version of this map.

Scenario: I can see photos taken by Warmshowers users
  When I click on the Touring Photos slideshow
  Then I will see a large modal with touring photos.

Scenario: I can use forward/back buttons to see pictures in the Touring Photos slideshow
  And I have opened the large photo modal
  When I click the forward/back buttons at the bottom of the picture
  Then I will see the corresponding photo content within the modal.

Scenario: I can use forward/back buttons to see pictures in the embedded Touring Photos slideshow
  When I click the forward/back buttons at the top left of the picture
  Then I will see the corresponding photo content.

Scenario: I can pause the embedded Touring Photos slideshow
  When I hover over the slideshow
  Then the auto-advance will pause and I can continue looking at one photo.

Scenario: I can find out more about Warmshowers Mobile Apps by clicking on the mobile device image.
  When I click on the picture of mobile devices
  Then I will see the  Mobile Apps page

Scenario: I can find out more about Warmshowers Mobile Apps by clicking on the Mobile Apps Available link.
  When I click on the Mobile Apps Available link
  Then I will see the  Mobile Apps page

