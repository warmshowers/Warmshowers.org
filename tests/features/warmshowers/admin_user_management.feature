#language:en
Feature: Admin management of users

  @rules @mail @smoke
  Scenario: Receive user account notifications
  Given a [user] is on the "Create Account" page
  And I am an authenticated user
  And I have the role of "moderator"
  When the [user] submits the form with validated data
  Then a new user account will be created
  And I will receive an email
  And I can see the subject line will be "WS new user: [user]"
  And I can see the message body will contain a summary of the account and a "Profile edit" link
