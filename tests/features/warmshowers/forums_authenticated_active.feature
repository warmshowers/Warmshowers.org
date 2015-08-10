#language:en
Feature:I can interact with other Warmshowers users on the Forums pages
  In order to interact with other Warmshowers users
  As an authenticated user
  I can post and modify content in the forums

Background:
  Given I am on the Forums pages
  And I am an authenticated user

#Private message content moved to private_message.feature

@nav
Scenario: I can reach the New Topic post creation form
  And I have entered a forum or subforum
  When I click the New Topic button at the top left
  Then I will see the post creation form.

@smoke
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

@nav
Scenario: I can access the Edit form for my own posts by using the Edit tab
  And I am viewing a post I created
  When I click the Edit tab
  Then I can see the edit form for my post.

@nav
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

Scenario:I can reorder attachments to a post
  And I have entered the Create Forum Topic form
  And I have entered values for subject, forum, and body
  And I have attached two or more files to my message
  When I click the cross at the left of the file name
  And I drag the file(s) into the desired order
  And I click Submit
  Then my post will be published with attachments listed in desired order.

@smoke
Scenario: I can mark a post as spam
  And I am viewing a spam post
  When I click the Mark This as Spam button
  Then I see the associated thread
  And a modal with:
    """
    Thanks for letting us know. An email has been sent to the administrators so we can get it taken care of.
    You have marked this as spam. You can click again to unmark it if this was a mistake.
    """

@smoke
Scenario: I can mark a post as obsolete
  And I am viewing an obsolete post
  When I click the Mark Obsolete button
  Then I see the associated thread
  And a modal with:
    """
    Thanks for letting us know. An email has been sent to the administrators so we can get it taken care of.
    You have marked this as spam. You can click again to unmark it if this was a mistake.
    """

@smoke
Scenario:I can add a reply to a post using the Post Reply button
  And I am viewing a post to which I would like to reply
  When I click the Post Reply button in the upper left or bottom left of thread 
  And I enter subject text
  And I enter Comment text
  And I click Save
  Then I will see my published reply.

@smoke
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

#Validation/fail scenarios

#Currently, the system will allow sending messages with EITHER subject OR message (or both).  It will not send a message with only an attachment.  The failure modal isn't totally accurate and the validation rules might benefit from tightening a bit here.
@smoke
Scenario: I can NOT publish an empty post or reply
  And I am in the Create Forum Topic or Reply form
  When I do not enter text in the Comment Field
  And I click Save
  Then I will see my incomplete comment with empty field(s) highlighted
  And a modal with "Comment field is required."
  And my comment will not be published.

Scenario: I can NOT publish a new Forum Topic without a subject.
  And I am in the Create Forum Topic area
  But I have not entered a value in the Subject field
  When I click the Submit button
  Then I will see my incomplete comment with Subject field highlighted
  And a modal with "Subject field is required."
  And my comment will not be published.

Scenario: I can NOT publish a new Forum Topic without selecting a Forum.
  And I am in the Create Forum Topic area
  But I have not selected a forum from the dropdown menu
  When I click the Submit button
  Then I will see my incomplete comment with Forums field highlighted
  And a modal with "Forums field is required."
  And my comment will not be published.

@smoke
Scenario: I can NOT attach a file of unapproved file type to a message
  And I am in the Create Forum Topic or Reply form
  And I have completed required fields
  When I click Choose File
  And I select a file of incorrect file type/extension
  Then I will see the message 
  """
  The selected file [filepath] cannot be uploaded. Only files with the following extensions are allowed: png, gif, jpg, jpeg.
  """
  And my file will not be uploaded.

#The system currently DOES allow cumulative totals exceeding the limit. Couldn't find a single file big enough to see if the max limit works on individual file uploads.
#This scenario is a guess about proper functionality.
@smoke
Scenario: I can NOT attach file(s) exceeding the maximum upload size of 20 mb.
  And I am in the Create Forum Topic or Reply form
  And I have completed required fields
  When I click Choose File
  And I select file(s) exceeding the 20 mb limit
  And I click Upload
  And I click the Submit button
  Then I should see a modal informing me that my attachments have exceeded the maximum upload allowance
  And my comment will not be posted

Scenario: I can NOT post a message directly to a container forum.
  And I am in the Create Forum Topic area
  And I have completed the Subject and Body fields
  When I select a container forum (General or Site Leadership) from the Forums dropdown
  And I click Submit
  Then I will see a modal with :
  """
  The item [container] is only a container for forums. Please select one of the forums below it.
  """
  And my comment will not be published.