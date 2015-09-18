#language:en
Feature: Give feedback to users
  In order to contribute to the Warmshowers community
  As an authenticated user 
  I can use give feedback Warmshowers members

Background:
  Given I am an "authenticated" user

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
  And I click the "Save" button
  Then I see my published feedback
  And a modal with "Feedback Feedback for [user] has been created."

Scenario: Edit feedback
  And I am on a "Feedback edit" form
  When I enter "Feedback has been changed" in the "Please tell about your experience with this member" field
  And I click the "Save" button
  Then I see the "Feedback listing" page
  And I see the text "Feedback has been changed" on the page
  And I see a modal with "Feedback Feedback for [user] has been updated."

Scenario: Edit feedback from the feedback listing
  And I am on another user's "Feedback listing" form
  And I have given feedback for the user
  When I click the "Edit" link to the right of the feedback I have given
  And I enter "Feedback has been changed" in the "Please tell about your experience with this member" field
  And I click the "Save" button
  And I see a modal with "Feedback Feedback for [user] has been updated."
  And I click the "View" link to the right of the feedback page
  Then I see the "Feedback listing" page
  And I see the text "Feedback has been changed" on the page

@smoke
Scenario: Delete feedback
  And I am on a "Feedback edit" form
  When I click the "Delete" button at the bottom of the form
  And I see the message "This action cannot be undone."
  And I click the "Delete" button
  Then I see the "Home" page
  And I see a modal with "Feedback Feedback for [user] has been deleted."

# ISSUE: THIS SCENARIO CAN'T BE REALISTICALLY TESTED WITH VIEWS CACHING ENABLED
Scenario: Delete feedback from the feedback listing
  And I am on another user's "Feedback listing" page
  And I have given feedback for the user
  When I click the "Delete" link to the right of the feedback I have given
  And I see the message "This action cannot be undone."
  And I click the "Delete" button
  Then I see the "Feedback listing" page
  #And my deleted feedback item does not appear in the listing.
  And a modal with "Feedback Feedback for [user] has been deleted."

Scenario: Attach an image to feedback
  And I am on a "Feedback edit" form
  When I click the "Browser image select" button
  And I select a "2MB image" file
  And I click the "Upload" button
  And I enter "Image description summary" into the "Image description" field
  And I click the "Save" button
  Then I see the "Feedback listing" page
  And I will see a thumbnail of the "2MB image" file

Scenario: Remove an image from feedback
  And I am on a "Feedback edit" form
  And I have uploaded an image
  When I click the "Remove" button to the right of the image
  Then I will no longer see the image

Scenario: Reorder feedback images
  And I am on a "Feedback edit" form
  And I have uploaded two images
  When I click the "move cross" to the left of the "first image" name
  And I drag the file down below the "second image"
  And I click the "Save" button
  Then I will see the "second image" shown above the "first image"

Scenario: Feedback hints block
  And I am on the "Create Feedback" form for a user
  Then I see a side bar block with the title "Feedback Hints"

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
