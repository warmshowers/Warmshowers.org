#language:en
Feature:I can learn about Warmshowers on the Forum page
  In order to find out more about the Warmshowers community
  As a new or unauthenticated user
  I can read content in the forums

Background:
  Given I am on the Forums page
  And I am an unregistered or unauthenticated user

Scenario: I can view thread titles within a given forum
  And the View Forums tab
  When I click the name of a forum that does not have sub-forums
  Then I will see a list of posts within that forum in reverse-chronological order
  And I will see my path in the breadcrumb trail
  And all author names will appear as WS Member

Scenario: I can view thread titles within a given sub-forum, starting from the main Forums page
  And the View Forums tab
  When I click the name of a sub-forum in the list below its parent forum
  Then I will see a list of posts within that sub-forum in reverse-chronological order
  And I will see my path in the breadcrumb trail
  And all author names will appear as WS Member

Scenario: I can navigate backwards using the breadcrumb trail
  When I click an item in the breadcrumb trail
  Then I will see the corresponding page.
  And I will see my path in the breadcrumb trail
  And all author names will appear as WS Member

Scenario: I can view thread titles within a given sub-forum, starting from the parent forum's page
  And the View Forums tab
  And I have entered the parent forum of my chosen forum
  When I click the name of a sub-forum in the list 
  Then I will see a list of posts within that sub-forum in reverse-chronological order
  And I will see my path in the breadcrumb trail
  And all author names will appear as WS Member

Scenario: I can view the most recent post in a forum
  And the View Forums tab
  When I click the link in the Last Post column of a given forum
  Then I should see the page for the corresponding thread
  And I will see my path in the breadcrumb trail
  And all author names will appear as WS Member

Scenario: I can read posts
  And the View Forums tab
  And I have entered a forum to view
  And I have entered a subforum (where applicable)
  When I click on the link for my post
  Then I should see the page for my chosen post, with responses displayed below in chronological order
  And all author names will appear as WS Member
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

#The other two forums with subforums (Community Experiences and Non-English Forums) do not work this way.  It would probably be better to have all forums of the same type have a similar format.
Scenario: I can collapse the list of subforums
  And I have entered the Website Help and Support forum
  When I click the [-] button in the upper right
  Then I should see see the list of sub-forums collapse
  And see the list of unsorted posts within the parent forum below
  And the symbol change to a [+]

Scenario: I can expand the list of subforums
  And I have entered the Website Help and Support forum
  And I have collapsed the list of subforums
  When I click the [+] button in the upper right
  Then I should see see the list of sub-forums expand
  And see the list of unsorted posts within the parent forum below
  And the symbol change to a [-]

Scenario: I can reach the Login page through forum links
  And I have entered a forum or subforum
  When I click the Login link above the list of posts
  Then I see the login page

Scenario: I can reach the Login page through forum links
  And I have entered a forum or subforum
  When I click the Login link below the list of posts
  Then I see the login page

Scenario: I can reach the Login page through forum links
  And I have entered a forum post or thread
  When I click the Login link at the bottom of a comment
  Then I see the login page.

Scenario: I can reach the Login page through forum links
  And I have entered a forum post or thread
  When I click the Login link at the bottom of a thread
  Then I see the login page.

Scenario: I can reach the Create Account page through forum links
  And I have entered a forum post or thread
  When I click the Register link at the bottom of a comment
  Then I see the Create Account page.

Scenario: I can reach the Create Account page through forum links
  And I have entered a forum post or thread
  When I click the Register link at the bottom of a thread
  Then I see the Create Account page.

Scenario: I can reach the FAQ page
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

Scenario: I can view only Active topics
  When I click the Active Topics tab
  Then I will see the list of currently active topics.

Scenario: I can view only Unanswered topics
  When I click the Unanswered Topics tab
  Then I will see the list of currently unanswered topics.

Scenario: I can view only Active topics
  When I select View Active Forum Topics from the Forum Tools dropdown menu at the bottom right
  Then I will see the list of currently active topics.

Scenario: I can view only Unanswered topics
  When I select View Unanswered Forum Posts from the Forum Tools dropdown menu at the bottom right
  Then I will see the list of currently unanswered topics.

Scenario: I can change the order in which forum posts are displayed
  And I have entered a forum or subforum
  When I select options from the sorting dropdown menus at the bottom left
  And click Sort
  Then I will see posts from that forum sorted according to my choices

Scenario: I can navigate large forums using the pagination buttons
  And I have entered a forum or sub-forum with multiple pages of threads
  When I click on a numbered button, Next button, or Last button at the top right of the list of threads
  Then I will see the corresponding page of threads within that forum.

#The Community Experiences forum has a number of issues.  
#It displays differently than all other forums on the General directory page (no Topics, Posts, or Last Post columns).
#Header collapse/overlap when entered (forumScreenshot06162015.png). 
#Link in the blue forum description area appears truncated "Read more about".