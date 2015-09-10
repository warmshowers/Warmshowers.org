#language:en
Feature: Admin moderation capabilities

  @smoke @mail @rules
  Scenario: Comment moderation email
  Given a [user] posts a comment to a forum post
  And the [user] is not authorized to "post without moderation"
  And I am an authenticated user
  And I have the role of "moderator"
  When the [user] submits a reply to a forum post
  Then I will receive an email
  And I can see the subject line will be "Comment posted by new user [user] - needs approval"
  And I can see the message body will contain a "Comment approval" link

  @smoke
  Scenario: View comment moderation
  Given I am an authenticated user
  And I have the role "moderator"
  When I view the "Comment approval"s page
  Then I will see a filterable view with any unapproved comments in it

  Scenario: Comment moderation
  Given I am an authenticated user
  And I have the role "moderator"
  And I am on the "Comment approval"s page
  When I select the checkbox in the "left-hand" column next to the first comment
  And I select "Publish" from the operations
  And I click the "Execute" button
  Then I will see a green "status" message with the text "Performed Publish on 1 item."
  And I will see the comment has been published

