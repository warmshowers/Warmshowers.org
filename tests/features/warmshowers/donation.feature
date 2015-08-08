#language:en

Feature: I can donate to Warmshowers.org
  In order to sustain Warmshowers
  As an authenticated user
  I can donate to Warmshowers

Background: 
  Given I am an authenticated user

#INCOMPLETE SCENARIO

#@test
#Scenario: I can donate to Warmshowers using debit or credit card using a single payment
#  And I am on the Donation page
#  When I select the radio button for my chosen one-time donation amount
#  And I click the Give Now button in the corresponding row
#  And I see the Payment Information screen with total matching my chosen amount
#  And I select the radio button for Debit or Credit card
#  And I enter my card number
#  And I enter my card's expiration date using the dropdown menus
#  And I enter my card's cvv
#  And I click Send your Donation
#  Then 
#  And my card will be charged
#  And I will receive a confirmation/receipt email

#@test
#Scenario: I can donate to Warmshowers using Paypal using a single payment
#  And I am on the Donation page
#  When I select the radio button for my chosen one-time donation amount
#  And I click the Give Now button in the corresponding row
#  And I see the Payment Information screen total matching my chosen amount
#  And I select the radio button for Paypal
#  And I click Send your Donation
#  And
#  Then 
#  And my Paypal account will be debited
#  And I will receive a confirmation/receipt email

#It's a bit odd that the process for one-time donations is identical to repeat donations.  If I were signing up, I would want clarifying information like when the debits will be taken, how many repeats I authorize, etc.

#@test
#Scenario: I can donate to Warmshowers using debit or credit card using a repeat payment
#  And I am on the Donation page
#  When I select the radio button for my chosen repeating donation amount
#  And I click the Give Now button in the corresponding row
#  And I see the Payment Information screen with total matching my chosen amount
#  And I select the radio button for Debit or Credit card
#  And I enter my card number
#  And I enter my card's expiration date using the dropdown menus
#  And I enter my card's cvv
#  And I click Send your Donation
#  Then 
#  And
#  And my card will be charged
#  And I will receive a confirmation/receipt email

#@test
#Scenario: I can donate to Warmshowers using Paypal using a repeat payment
#  And I am on the Donation page
#  When I select the radio button for my chosen repeating donation amount
#  And I click the Give Now button in the corresponding row
#  And I see the Payment Information screen total matching my chosen amount
#  And I select the radio button for Paypal
#  And I click Send your Donation
#  And
#  Then 
#  And my Paypal account will be debited
#  And I will receive a confirmation/receipt email

@test
Scenario: I can donate to Warmshowers at the free Someday level
  And I am on the Donation page
  When I click on the Someday button
  And I see the Payment Information screen with Free Order selected
  And I click the Send Your Donation button
  Then I see the Thanks for Your Support page with future donor specific message

@test
Scenario: I can donate to Warmshowers at the free Hosting Only level
  And I am on the Donation page
  When I click on the Hosting Only button
  And I see the Payment Information screen with Free Order selected
  And I click the Send Your Donation button
  Then I see the Thanks for Your Support page with hosting-specific message

#Tried this out with the free donation level and didn't appear to receive a confirmation email, so not sure if those comments went anywhere or not
@test
Scenario: I can send a comment with my Warmshowers donation
  And I am on the Payment Information page
  And I have completed required fields
  When I enter comment text in the input field
  And I click Send your Donation
  Then my comments will be sent to Warmshowers
  And will appear in my order confirmation email

@contact
Scenario: I can email Warmshowers admin using the link on the Donation page.
  And I am on the Donation page
  And I have a mail client installed and configured
  When I click on the wsl@warmshowers.org link
  Then my mail client will pop up with a new message and the warmshowers address as recipient.

Scenario: I can find out about CVV numbers
  And I am on the Payment Information page
  When I click the What's the CVV? link
  Then I will see a pop-up explaining CVV and showing where to find it

@nav
Scenario: I can edit my email address using the link on the Payment Information page
  And I am on the Payment Information Page
  When I click the Edit link next to my email address
  Then I will see my Profile page
  And the Edit tab

@nav
Scenario: I can access the contact form for site admin using the link on the Donation page
  And I am on the Donation page
  When I click on the Contact Form link 
  Then I see the contact form to reach site admin.

@nav
Scenario: I can find out about Warmshowers' nonprofit status by using the link on the Donation page.
  And I am on the Donation page
  When I click on the Tax-deductible in the USA link
  Then I see the main FAQ page 
  And the Is my Donation Tax-deductible? question is expanded.

@nav
Scenario: I can view Warmshowers' 501c3 letter
  And I am on the Donation page
  When I click on The IRS Letter link
  Then I will see the relevant forum thread
  And the PDF attached to the first post

@nav
Scenario: I can verify Warmshowers' 501c3 status using the link on the Donation page
  And I am on the Donation page
  When I click on the IRS Website link
  Then I will see a new browser tab or window with the IRS listing for Warmshowers.

@nav
Scenario: I can view Warmshowers leadership documents by using the Leadership Forum link on the Donation page
  And I am on the Donation page
  When I click on the Leadership Forum link
  Then I will see the Leadership Council forum page

@nav
Scenario: I can view Warmshowers' 2015 Annual report using the link on the Donation page
  And I am on the Donation page
  When I click on the January 2015 Annual Report link
  Then I will see the relevant forum post

@nav
Scenario: I can view Warmshowers' 2015 Budget using the link on the Donation page
  And I am on the Donation page
  When I click on the 2015 Budget link
  Then I will see the relevant forum post

@nav
Scenario: I can view Warmshowers' Articles of Incorporation and Bylaws using the link on the Donation page
  And I am on the Donation page
  When I click on the Articles of Incorporation and Bylaws link
  Then I will see the relevant forum post

@nav
Scenario: I can view Warmshowers Board of Directors information using the link on the Donation page
  And I am on the Donation page
  When I click on the  Board of Directors Information link
  Then I will see the relevant forum post

#Scenarios related to the Donation/Membership level FAQ page
@nav
Scenario: I can find out about donation levels by using a link on the Donation page.
  And I am on the Donation page
  When I click on the  Donation Level FAQs (top of page) OR  Donation/Membership FAQ (bottom of page) link
  Then I see the Frequently Asked Questions - Donation levels page

@nav
Scenario: I can access the answer to a given Donation FAQ item
  And I am on the Donation FAQ page
  And my chosen question is collapsed
  When I click on the link for my question
  Then I should see the answer text expanded below the question link.

@nav
Scenario: I can close expanded items in the Donation FAQ for clear viewing
  And I am on the Donation FAQ page
  And a question is in expanded form
  When I click on the question link of the expanded item
  Then I should see the item return to collapsed form.