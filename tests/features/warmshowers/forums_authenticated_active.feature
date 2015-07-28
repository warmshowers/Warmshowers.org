#language:en
Feature:I can interact with other Warmshowers users on the Forums pages
  In order to interact with other Warmshowers users
  As an authenticated user
  I can post and modify content in the forums

Background:
  Given I am on the Forums pages
  And I am an authenticated user

Scenario: I can send a private message to the original author of a forum thread
  And I have entered a thread
  When I click the Send Author a Message link at the bottom of the original post
  Then I will see the New Message form with the appropriate recipient.

Scenario: I can reach the New Topic post creation form
  And I have entered a forum or subforum
  When I click the New Topic button at the top left
  Then I will see the post creation form.

Scenario: I can write and submit a post to a new topic
  And I am on the post creation form
  When I enter a subject
  And I select an appropriate forum from the dropdown list
  And enter my comments in the Body field
  And I click the Submit button
  Then I will see my new thread
  And a modal with "Forum topic [title] has been created."

Scenario: I can edit my post
  And I am viewing a post I created
  When I click the Edit button
  And I see the edit form
  And I make desired changes
  And I click the Submit button
  Then I will see my edited post
  And a modal with "Forum topic [topic title] has been updated."

Scenario: I can access the Edit form for my own posts by using the Edit tab
  And I am viewing a post I created
  When I click the Edit tab
  Then I can see the edit form for my post.

Scenario: I can view my post with the view tab
  And I am editing a post I created
  When I click the view tab
  Then I can see the published form of my post.

Scenario: I can select an input format for my post
  And I am viewing the create or edit post form
  When I click the Input Format link
  Then I can select the Filtered HTML or Plain Text radio button
  And I can read more about formatting options by clicking the More Information about Formatting Options link

Scenario: I can view changes when editing a post
  And I have created a thread-opening post
  And I have entered the edit form for that post
  When I click View Changes
  Then I will see a side-by-side list of changes made by edits to the post.

Scenario: I can preview a post before submitting
  And I am in the Create Forum or Reply form area
  And I have completed required fields
  When I click the Preview button
  Then I will see a preview of my post above the Create/Reply form
  And if I have uploaded an image to my post I will see fields for additional information.

Scenario: I can leave a shadow copy
  And I have created a thread-opening post
  And I have entered the edit form for that post
  And I have changed the forum in which to publish my post
  When I select the Leave Shadow Copy checkbox
  Then a link to the original published forum will be included in the new location

Scenario: I can delete my post
  And I am viewing or editing a thread-opening post I created
  When I click the Delete button
  And I see a warning page about deleting
  And I click Delete again
  Then I will see the main page 
  And a modal with the message "Forum topic [Title] has been deleted."
  And my post will no longer appear in the forums

#The list checkbox has no description and it's not clear what it is for.  If this is a useful feature, it should have some clarifying text.
Scenario: I can attach an image to a forum post
  And I have entered the Create Forum Topic form
  When I click Choose File
  And I select a file of appropriate type and size
  And I click Upload
  Then I will see a thumbnail of my image
  And I will see fields for Description and Title text.

#The Description text appears to not work correctly.  It doesn't appear in the published post or anywhere else that I can discern.
Scenario: I can add information to an image uploaded to a post
  And I have entered the Create Forum Topic form
  And I have uploaded an image
  When I see the text fields for Description and Title text
  And I enter text as desired
  And I submit my post
  Then my Title text will appear as a tooltip when a user hovers over my uploaded image.

Scenario: I can remove an image from a post
  And I have entered the Create Forum Topic form
  And I have uploaded an image
  When I see the text fields for Description and Title text
  And I click the Remove button
  Then the image will no longer appear in the Image table

Scenario: I can add additional images to a post
  And I have entered the Create Forum Topic form
  And I have uploaded files to fill up default spaces in the Image table
  When I click the Add Another Item button
  Then I will see an additional row in the Image table

Scenario: I can mark a post as spam
  And I am viewing a spam post
  When I click the Mark This as Spam button
  Then I see the associated thread
  And a modal with:
    """
    Thanks for letting us know. An email has been sent to the administrators so we can get it taken care of.
    You have marked this as spam. You can click again to unmark it if this was a mistake.
    """

Scenario: I can mark a post as obsolete
  And I am viewing an obsolete post
  When I click the Mark Obsolete button
  Then I see the associated thread
  And a modal with:
    """
    Thanks for letting us know. An email has been sent to the administrators so we can get it taken care of.
    You have marked this as spam. You can click again to unmark it if this was a mistake.
    """

Scenario:I can add a reply to a post using the Post Reply button
  And I am viewing a post to which I would like to reply
  When I click the Post Reply button in the upper left or bottom left of thread 
  And I enter subject text
  And I enter Comment text
  And I click Save
  Then I will see my published reply.

Scenario:I can add a reply to a post using the Reply button
  And I am viewing a post to which I would like to reply
  When I click the Reply button at the bottom right of a comment
  And I enter subject text
  And I enter Comment text
  And I click Save
  Then I will see my published reply.
  And I will receive notification of all replies to my comment

Scenario: I can opt out of notifications on a thread to which I have replied
  And I am in the reply form for an existing thread
  When I unclick the Notify Me When New Comments are Posted checkbox (checked by default)
  And I click Save
  Then I will not receive notification of new posts to this thread

Scenario: I can change my notification settings for a thread to which I have replied
  And I am in the reply form for an existing thread
  And I have left the Notify Me When New Comments are Posted checkbox checked
  When I select the All Comments radio button (not selected by default)
  And I click Save
  Then I will receive notification of all new replies to this thread (not just replies to my comment)


