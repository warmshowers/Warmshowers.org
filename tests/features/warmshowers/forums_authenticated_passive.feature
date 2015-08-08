#language:en
Feature:I can stay informed about Warmshowers by reading content on the Forums pages
  In order to stay informed about Warmshowers issues, discussions, and users 
  As an authenticated user
  I can read content in the forums

Background:
  Given I am on the Forums pages
  And I am an authenticated user

Scenario: I can view thread titles within a given forum
  And the View Forums tab
  When I click the name of a forum that does not have sub-forums
  Then I will see a list of posts within that forum in reverse-chronological order
  And I will see my path in the breadcrumb trail

Scenario: I can view thread titles within a given sub-forum, starting from the main Forums page
  And the View Forums tab
  When I click the name of a sub-forum in the list below its parent forum
  Then I will see a list of posts within that sub-forum in reverse-chronological order
  And I will see my path in the breadcrumb trail

Scenario: I can navigate backwards using the breadcrumb trail
  When I click an item in the breadcrumb trail
  Then I will see the corresponding page.
  And I will see my path in the breadcrumb trail

#Search box stories moved to search.feature


Scenario: I can easily identify Forums or Subforums with new content since my last view
  And I am on a page listing forum or subforum listings
  When I look at the left-hand column
  Then I will see a sun icon that accurately reflects new content.

#Currently this is a bit off- it seems to put the bottom of the new post at the top of the screen, so you need to scroll up to actually see what you were looking for.
Scenario: I can view new posts on a forum by using the link in the topics column
    And I am on a page listing forum or subforum listings
    When I click on a link in the Topics column ("# new")
    Then I will see the list of threads with the page focus on the new post(s)

Scenario: I can view new replies to a post by using the link in the Replies column
    And I am on a page listing threads (forum/subforum, aggregate)
    When I click on a link in the Replies column ("# new")
    Then I will see the thread with focus on first unread reply

Scenario: I can view thread titles within a given sub-forum, starting from the parent forum's page
  And the View Forums tab
  And I have entered the parent forum of my chosen forum
  When I click the name of a sub-forum in the list 
  Then I will see a list of posts within that sub-forum in reverse-chronological order
  And I will see my path in the breadcrumb trail

Scenario: I can view the most recent post in a forum
  And the View Forums tab
  When I click the link in the Last Post column of a given forum
  Then I should see the page for the corresponding thread
  And I will see my path in the breadcrumb trail

Scenario: I can view the user profile of a post author
  When I click an author link 
  Then I should see the user profile page for the author of that post

Scenario: I can read posts
  And the View Forums tab
  And I have entered a forum to view
  And I have entered a subforum (where applicable)
  When I click on the link for my post
  Then I should see the page for my chosen post, with responses displayed below in chronological order
  And I will see my path in the breadcrumb trail

Scenario: I can collapse the list of forums
  When I click the [-] button in the upper right
  Then I should see see the list of forums collapse
  And the symbol change to a [+]

Scenario: I can expand the list of forums
  And I have collapsed the list of forums
  When I click the [+] button in the upper right
  Then I should see see the list of forums expand
  And the symbol change to a [-]

#!!The Community Experiences and Non-English forums DO collapse for authenticated users, but not for unauthenticated.
#Only Website Help and Support shows the list of posts below the list of subforums, however.
Scenario: I can collapse the list of subforums
  And I have entered a forum with subforums
  When I click the [-] button in the upper right
  Then I should see see the list of sub-forums collapse
  And see the list of unsorted posts within the parent forum below #currently only works for Website Help & Support
  And the symbol change to a [+]

Scenario: I can expand the list of subforums
  And I have entered a forum with subforums
  And I have collapsed the list of subforums
  When I click the [+] button in the upper right
  Then I should see see the list of sub-forums expand
  And see the list of unsorted posts within the parent forum below #currently only works for Website Help & Support
  And the symbol change to a [-]

Scenario: I can reach the Login page through forum links
  And I have entered a forum or subforum
  When I click the Login link above the list of posts
  Then I see the login page

Scenario: I can reach the FAQ page using forum links
  And the View Forums tab
  When I click on the Frequently-Asked Questions link in the Website Help Forum description
  Then I see the FAQ page

Scenario: I can view only Admin/Governance forums
  And the View Forums tab
  When I click on the Site Administrators/Design/Governance/Volunteers link
  Then I will see only forums in the admin/governance category

Scenario: I can view only General forums
  And the View Forums tab
  When I click on the General link
  Then I will see only forums in the general category

Scenario: I can view only Active topics by using the tab navigation
  When I click the Active Topics tab
  Then I will see the list of currently active topics.

Scenario: I can view only Unanswered topics by using the tab navigation
  When I click the Unanswered Topics tab
  Then I will see the list of currently unanswered topics.

Scenario: I can view only New & Updated topics by using the tab navigation
  When I click the New & Updated topics tab
  Then I will see a list of topics with content added since my last visit.

Scenario: I can view only Active topics by using Forum Tools
  And I am on the main Forums page
  When I select View Active Forum Topics from the Forum Tools dropdown menu at the bottom right
  Then I will see the list of currently active topics.

Scenario: I can view only Unanswered topics by using Forum Tools
  And I am on the main Forums page
  When I select View Unanswered Forum Posts from the Forum Tools dropdown menu at the bottom right
  Then I will see the list of currently unanswered topics.

Scenario: I can view only New & Updated topics by using Forum Tools
  And I am on the main Forums page
  When I select New & Updated topics from the Forum Tools dropdown menu at the bottom right
  Then I will see a list of topics with content added since my last visit.

Scenario: I can mark all forum content as read by using Forum Tools
  And I am on the main Forums page
  When I select Mark All Forums Read from the Forum Tools dropdown menu at the bottom right
  Then I will see a modal with "All forum content been marked as read"

Scenario: I can narrow active topics by forum or subforum
  And I am on the Active Forum Topics tab
  When I select one or more forums and/or subforums from the list
  And I click Apply
  Then I will see only active topics within my chosen forums/subforums listed below.

Scenario: I can narrow New & Updated topics by forum or subforum
  And I am on the New & Updated Topics tab
  When I select one or more forums and/or subforums from the list
  And I click Apply
  Then I will see only new & updated topics within my chosen forums/subforums listed below.

Scenario: I can change the order in which forum posts are displayed
  And I have entered a forum or subforum
  When I select options from the sorting dropdown menus at the bottom left
  And click Sort
  Then I will see posts from that forum sorted according to my choices

Scenario: I can narrow unanswered topics by forum or subforum
  And I am on the Unaswered Forum Topics tab
  When I select one or more forums and/or subforums from the list
  And I click Apply
  Then I will see only unanswered topics within my chosen forums/subforums listed below.

Scenario: I can navigate large forums using the pagination buttons
  And I have entered a forum or sub-forum with multiple pages of threads
  When I click on a numbered button, Next button, or Last button at the top right of the list of threads
  Then I will see the corresponding page of threads within that forum.

Scenario: I can navigate aggregate lists using the pagination buttons
  And I have entered an aggregated list (Active, New/Updated, Unanswered tabs)
  When I click on a numbered button, Next button, or Last button at the bottom of the list of threads
  Then I will see the corresponding page of threads within that forum.

Scenario: I can return to the top of a thread using the Top button
  And I have entered a thread
  When I click the Top button at the bottom left of a post/reply
  Then the page focus will change to the first post in the thread

Scenario: I can view the first unread message in a thread
  And I have entered a thread with unread items
  When I click the First Unread button
  Then the page focus will change to the first unread post in the thread

#This button directs to the last "post" in a thread, but not the last content (which may be a reply or other lower-tier item) which seems confusing.
Scenario: I can view the last top-level post in a thread 
  And I have entered a thread
  When I click on the Last Post button
  Then the page focus will change to the latest reply to the original post (but not to more recent lower level comments)

