#language:en
Feature: Read content on the Forums pages
  In order to stay informed about Warmshowers issues, discussions, and users 
  As an authenticated user
  I can read content in the forums

Background:
  Given I am an authenticated user

@nav
Scenario: View topic titles within a forum
  Given I am on the main "Forums" page
  When I click the "General discussion" forum page
  Then I will see a "List of topics" within that forum in reverse-chronological order
  And I will see the main "Forums" forum link in the breadcrumb trail

@nav
Scenario: Navigate backwards using the breadcrumb trail
  Given I am on the "General discussion" forum page
  When I click on the main "Forums" page link in the breadcrumb trail
  Then I will see the main "Forums" page
  And I will see the main "Forums" page link removed from the breadcrumb trail change

@nav
Scenario: View sub-forums within a forum
  Given I am on the main "Forums" page
  When I click the "Website Help and Support" forum page
  Then I will see a "List of forums" within that forum
  And I will see the main "Forums" forum link in the breadcrumb trail

@nav
Scenario: View topic titles within a sub-forum
  Given I am on the "Website Help and Support" forum page
  When I click the "Future Features and Initiatives" sub-forum page
  Then I will see a "List of topics" within that sub-forum in reverse-chronological order
  And I will see the "Website Help and Support" forum link in the breadcrumb trail

Scenario: Identify Forums with new content
  Given I am on the main "Forums" page
  When I look at the "left-hand" column
  Then I will see a yellow sun icon if the forums has new content.

@nav
Scenario: View new posts on a forum
  Given I am on the main "Forums" page
  And a forum in the forums list has new posts since my last visit
  When I click on the "X new" link in the "Topics" column for a forum
  Then I will see the "List of topics" with the page focus on the new posts

@nav
Scenario: View new replies to a post
  Given I am on the "General discussion" forum page
  And a post in the "List of posts" has new replies since my last visit
  When I click on the "view X new" link in the Replies column
  Then I will see the topic with focus on the first unread reply

@nav
Scenario: View the most recent post in a forum
  Given I am on the main "Forums" page
  When I click the "Title" link in the "Last Post" column for the "Website Help and Support" forum
  Then I should see the page for the corresponding topic
  And I will see the "Website Help and Support" forum link in the breadcrumb trail

@nav
Scenario: View author's profile
  Given I am on the "General discussion" forum page
  When I click an "Author" link in the "Topic" column
  Then I should see the user "Profile" page for the author of that post

@nav
Scenario: Read posts
  Given I am on the "General discussion" forum page
  When I click on the link for a post
  Then I should see the topic page for my chosen
  And I see posts displayed below in chronological order
  And I will see the "General discussion" link in the breadcrumb trail

Scenario: Collapse the list of forums
  Given I am on the main "Forums" page
  When I click the "[-]" button in the upper right of the "List of forums"
  Then I should see see the "List of forums" collapse
  And the button text change to "[+]"

Scenario: Expand the list of forums
  Given I am on the main "Forums" page
  And the "List of forums" is collapsed
  When I click the "[+]" button in the upper right
  Then I should see see the "List of forums" expand
  And the button text change to "[-]"

@nav
Scenario: View Active topics
  Given I am on the main "Forums" page
  When I click the "Active topics" tab
  Then I will see the list of currently active topics.

@nav
Scenario: View Unanswered topics
  Given I am on the main "Forums" page
  When I click the "Unanswered topics" tab
  Then I will see the list of currently unanswered topics.

@nav
Scenario: View New & Updated topics
  Given I am on the main "Forums" page
  When I click the "New & Updated topics" tab
  Then I will see a list of topics with content added since my last visit.

Scenario: Mark all forum content as read
  Given I am on the main "Forums" page
  When I select "Mark All Forums Read" from the Forum Tools select menu at the bottom right
  Then I will see a modal with "All forum content been marked as read"

Scenario: Narrow topics by forum
  Given I am on the "Active topics" page
  When I select one or more forums from the list
  And I click Apply
  Then I will see only active topics within my chosen forums listed below

Scenario: Change the topic order
  Given I am on the "General discussion" forum page
  When I select "Topics" and "Down" from the sort select menus at the bottom left
  And I click Sort
  Then I will see only topics with the newest at the top

@nav
Scenario: Use the pagination buttons
  Given I am on the "General discussion" forum page
  When I click on a "page 5" in the "pagination" menu in the bottom left
  Then I will see the fifth page of results with older topics
  And the "first" and "previous" links will be added to the "pagination" menu

@nav
Scenario: Return to the top
  Given I am on the "Reporting bugs or suspected bugs" topic page
  And I have scrolled to the bottom of the page
  When I click the Top button at the bottom left of a post
  Then I will go to the first post in the topic

@nav
Scenario: View the first unread message in a topic
  And I have entered a topic with unread items
  When I click the "First Unread" button
  Then the page focus will change to the first unread post in the topic

@nav
Scenario: View the last post in a topic 
  And I have entered a topic
  When I click on the "Last Post" button
  Then I will go to the last reply to the original post

