#language:en
Feature: Give feedback to users
  In order to contribute to the Warmshowers community
  As an authenticated user 
  I can use give feedback Warmshowers members

Background:
  Given I am an authenticated user

@nav
Scenario: See feedback link
  And I am on another user's "Profile" page
  When I click on the "Feedback" link in the green "User stats" summary area
  Then I see the "Feedback" page

@nav
Scenario: See feedback tab
  And I am on another user's "Profile" page
  When I click the feedback tab
  Then I see feedback by and about the user

Scenario: View feedback for a user
  And I am on another user's "Feedback" page
  Then I see a "List of feedback" for the user

Scenario: View feedback by a user
  And I am on another user's "Feedback" page
  When I click on the link "View feedback [user] has given"
  Then I see a "List of feedback" given by the user

@nav
Scenario: Access the Create Feedback form
  And I am on another user's "Profile" page
  When I click on the "Provide Feedback" button in the "Actions" sidebar
  Then I see the "Create Feedback" form for the user

@smoke @mail @rules
Scenario: Create positive feedback
  And I am on the "Create Feedback" form for a user
  When I select "Positive" from the "Overall experience with [user]" select menu
  And I enter at least 10 words of text in the "Please tell about your experience with this member" input field
  And I select the "Guest" from the "Feedback is for" radio fields
  And I click the "Submit" button
  Then I see my published feedback
  And a modal with "Feedback Feedback for [user] has been created."
  And I will receive an email notification with:
  """
  An email has been sent to [user] letting them know about your feedback.
  """
  And the user will receive an email notification with:
  """
  No idea what the message is meant to be.
  """

@smoke @mail @rules
Scenario: Create negative feedback
  And I am on the "Create Feedback" form for a user
  When I select "Negative" from the "Overall experience with [user]" select menu
  And I enter at least 10 words of text in the "Please tell about your experience with this member" input field
  And I select the "Guest" from the "Feedback is for" radio fields
  And I click the "Submit" button
  Then I see my published feedback
  And a modal with "Feedback Feedback for [user] has been created."

Scenario: Edit feedback from item view
  And I am on a "Feedback post" page 
  When I click the "Edit" tab
  And make desired changes to form fields
  And I click the "Submit" button
  Then I see my feedback updated
  And a modal with "Feedback Feedback for [user] has been updated."

Scenario: Edit feedback from the feedback listing
  And I am on another user's "Feedback" page
  When I click the "Edit" link to the right of the feedback item I want to change
  And I click the "Submit" button
  Then I see my updated published feedback 
  And a modal with "Feedback Feedback for [user] has been updated."

Scenario: I can view a feedback item I created from the feedback listing
  And I am viewing the feedback listing page for a user
  When I click the View link to the right of the feedback item I want to see
  Then I see the item view for the published feedback.

Scenario: I can delete feedback I created from the feedback listing
  And I am viewing the feedback listing page for a user
  When I click the Delete link to the right of the feedback item I want to remove
  And I see the message "This action cannot be undone."
  And I click the Delete button
  Then I see the updated feedback listing for the user
  And my deleted feedback item does not appear in the listing.
  And a modal with "Feedback Feedback for [user] has been deleted."

#Deleted feedback is removed from the feedback list, but still appears on the profile page and the front page.
Scenario: I can delete feedback I created from the edit form
  And I am viewing the edit feedback for for a feedback item I created
  And the user has more than one item of feedback
  When I click the Delete button at the bottom of the form
  And I see the message "This action cannot be undone."
  And I click the Delete button
  Then I see the updated feedback listing for the user
  And a modal with "Feedback Feedback for [user] has been deleted."
  But my deleted feedback item does not appear in the listing.

#Not sure having a separate functionality for empty feedback is necessary/helpful. Probably should direct to the blank feedback page instead.
Scenario: I can delete feedback I created from the edit form when it is a user's only feedback
  And I am viewing the edit feedback for for a feedback item I created
  And the user has only one feedback item
  When I click the Delete button at the bottom of the form
  And I see the message "This action cannot be undone."
  And I click the Delete button
  Then I see the main page
  And a modal with "Feedback Feedback for [user] has been deleted."

Scenario: I can select an input format for my feedback
  And I am viewing the create or edit feedback form
  When I click the Input Format link
  Then I can select the Filtered HTML or Plain Text radio button
  And I can read more about formatting options by clicking the More Information about Formatting Options link

Scenario: I can view changes when editing a feedback item
  And I have created a thread-opening post
  And I have entered the edit form for that post
  When I click View Changes
  Then I will see a side-by-side list of changes made by edits to the post.

Scenario: I can preview a feedback item before submitting
  And I am in the Create Feedback or Edit Feedback area
  And I have completed required fields
  When I click the Preview button
  Then I will see a preview of my post above the Create/Reply form
  And if I have uploaded an image to my post I will see fields for additional information. 

Scenario: I can attach an image to a feedback item
  And I have entered the Create Feedback or Edit Feedback form
  When I click Choose File
  And I select a file of appropriate type and size
  And I click Upload
  Then I will see a thumbnail of my image
  And I will see a field for Description text.

Scenario: I can add information to an image uploaded to a feedback item
  And I have entered the Create Feedback or Edit Feedback form
  And I have uploaded an image
  When I see the text field for Description text
  And I enter text as desired
  And I submit my post
  Then my Description text will appear as a tooltip when a user hovers over my uploaded image.

Scenario: I can remove an image from a feedback item
  And I have entered the Create Feedback or Edit Feedback form
  And I have uploaded an image
  When I see the text field for Description text
  And I click the Remove button
  Then the image will no longer appear in the Image table

Scenario: I can add additional images to a feedback item
  And I have entered the Create Feedback or Edit Feedback form
  And I have uploaded files to fill up default spaces in the Image table
  When I click the Add Another Item button
  Then I will see an additional row in the Image table

Scenario:I can reorder attachments to a post
  And I have entered the Create Feedback or Edit Feedback form
  And I have entered values for required fields
  And I have attached two or more files to my feedback item
  When I click the cross at the left of the file name
  And I drag the file(s) into the desired order
  And I click Submit
  Then my post will be published with images shown in desired order.

#Sidebar navigation
@nav
Scenario: I can access the contact form for site admin through the Feedback form
  And I have entered the Create Feedback or Edit Feedback form
  When I click on the Via the Contact Form link in the sidebar
  Then I see the contact form to reach site admin.

@nav
Scenario: I can read about what to do about negative interactions
  And I have entered the Create Feedback or Edit Feedback form
  When I click on the  "What if I have a problem with a Warmshowers host or guest" link in the sidebar
  Then I see the FAQ item on this subject.

#Validation/Failure Scenarios:
@smoke
Scenario: I can NOT publish feedback with less than 10 words of text
  And I am on the Create Feedback form for a user
  When I select a feedback type from the dropdown menu
  And I enter fewer than 10 words of text in the experience input field
  And I select the role for the user I am offering feedback about using the radio buttons
  And I click Submit
  Then I see the feedback form
  And a modal with:
  """
  The Please tell about your experience with this member of your Feedback is too short. You need at least 10 words.
  """
  And my feedback is not published

@smoke
Scenario: I can NOT upload a file with incorrect filetype
  And I am in the Create Feedback or Edit Feedback Area
  And I have completed required fields
  When I click Choose File
  And I select a file of incorrect filetype
  Then I will see the upload area highlighted in red
  And the message:
  """
  The selected file [file path] cannot be uploaded. Only files with the following extensions are allowed: jpg, jpeg.
  """

#The system currently DOES allow cumulative totals exceeding the limit. Couldn't find a single file big enough to see if the max limit works on individual file uploads.
#This scenario is a guess about proper functionality.
@smoke
Scenario: I can NOT attach file(s) exceeding the maximum upload size of 15 mb.
  And I have entered values for required files
  When I click Choose File
  And I select file(s) exceeding the 15 mb limit
  And I click Upload
  And I click the Submit button
  Then I should see a modal informing me that my attachments have exceeded the maximum upload allowance
  And my feedback will not be published
