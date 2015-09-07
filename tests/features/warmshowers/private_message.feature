#language:en
Feature: Send private messages
  In order to interact with other Warmshowers users
  As an authenticated user
  I can send and receive private messages

Background:
  Given I am an authenticated user

@smoke
Scenario: Use the Send Message button
  And I am on another user's profile
  When I click on the "Send Message" button in the "Location" sidebar
  Then I see the "Write New Message" form page
  And I see the text "Write new message to [User]:" at the top of the page

Scenario: Send forum topic author a message
  And I am on the "Reporting bugs or suspected bugs" topic page
  When I click the "Send Author a Message" link at the bottom of the original post
  Then I see the "Write New Message" form page
  And I see the text "Write new message to [User]:" at the top of the page
  And I see the subject field prefilled with the name of the form topic

@smoke
Scenario: Send a private message
  And I am on the "Write New Message" form page
  When I enter text into the "subject" field
  And I enter text into the "message" field
  And I click Send Message
  Then I will see the sent message open in the "Read message" page
  And I will see a modal with "A message has been sent to [user]"
  And the other user will receive the message on their "Message inbox" page
  And the other user will receive an email notifying them of the message

@smoke
Scenario: Read private messages
  And I am on the "Message inbox" page
  When I click on a message subject
  Then I will see the message open in the "Read message" page

Scenario: Read sent private messages
  And I am on the "Sent messages" page
  When I click on a message subject
  Then I will see the message open in the "Read message" page

Scenario: Read unread private messages
  And I am on the "Unanswered Requests" page
  When I click on a message subject
  Then I will see the message open in the "Read message" page

@smoke
Scenario: Reply to a privage message
  And I am viewing a private message on the "Read message" page
  When I enter text into the "Reply Message" field
  And I click the "Send Message" button
  Then I will see the reply below the original message on the "Read message" page
  And I will see a modal with "A message has been sent to [user]"
  And the other user will receive the message on their "Message inbox" page
  And the other user will receive an email notifying them of the message

Scenario: Clear an unsent reply
  And I am viewing a private message on the "Read message" page
  And I have entered text into the "Reply Message" field
  But I have not sent the message
  When I click the "Clear" link at the bottom of the thread
  Then my entered text will no longer appear in the Reply field.

@smoke
Scenario: Attach files to a message
  And I am viewing a private message on the "Read message" page
  And I have entered text into the "Reply Message" field
  When I click on the File Attachments link
  And I click the "Choose File" button
  And I select a file to upload of appropriate type and size
  And I click the "Attach" button
  And I click the "Send Message" button
  Then I will see the reply below the original message on the "Read message" page
  And I will see a modal with "A message has been sent to [user]"
  And a download link for my attachment will appear at the bottom of my sent message

Scenario: Remove an attachment from a message
  And I am viewing a private message on the "Read message" page
  And I have entered text into the "Reply Message" field
  And I have attached a file to my message
  When I check the Delete checkbox next to the file name
  And I click Send Message
  Then I will see the reply below the original message on the "Read message" page
  And I will see a modal with "A message has been sent to [user]"
  And I will not see a download link for my attachment at the bottom of my sent message

Scenario: Forward a private message to another user
  And I am viewing a private message on the "Read message" page
  When I click the "Forward Conversation to Others" link at the bottom of the thread
  And I enter one or more valid Warmshowers names separated by commas in the To: field
  Then I will see a modal with: "This conversation has been forwarded to [user]."

Scenario: Filter Messages
  And I am on the "Message inbox" page
  When I click on the "Filter Messages" link
  And enter a value in the "By subject" field
  And click the "Filter" button
  Then I will see only messages that match my filter term

Scenario: Save a message filter
  And I am on the "Message inbox" page
  When I click on the "Filter Messages" link
  And I enter a value in the "By subject" field
  And I click the "Save filter" button
  And I navigate to the "Home" page
  And I navigate back to the "Message inbox" page
  When I click on the "Filter Messages" link
  Then I will see the "By subject" field prepopulated with my filter term
  And I will see only messages that match my filter term

Scenario: Mark multiple messages as unread
  And I am on the "Message inbox" page
  When I check the "left-hand" checkboxes next to the first two read messages
  And I select "Mark as Unread" from the "Actions" select menu
  Then I will see a modal with:
  """
  Marked [#] thread as unread.
  The previous action can be undone.
  """
  And the messages will appear as unread in the listing

Scenario: Mark multiple messages as read
  And I am on the "Message inbox" page
  When I check the "left-hand" checkboxes next to the first two unread messages
  And I select "Mark as Read" from the "Actions" select menu
  Then I will see a modal with:
  """
  Marked [#] thread as read.
  The previous action can be undone.
  """
  And the message will appear as read in the listing

Scenario: Archive multiple messages
  And I am on the "Message inbox" page
  When I check the "left-hand" checkboxes next to the first two read messages
  And I select "Archive" from the "Actions" select menu
  Then I will see a modal with:
  """
  The messages have been archived.
  The previous action can be undone.
  """
  And I will not see these messages in the "Message inbox" page

Scenario: Sort messages
  And I am on the "Message inbox" page
  When I click the "Last Updated" link in the table header
  Then I will see my messages sorted with the last updated at the top
  And the arrow next to the link will point down

Scenario: Tag multiple messages
  And I am on the "Message inbox" page
  When I check the "left-hand" checkboxes next to the first two read messages
  And I enter the value "tagged" in the input field
  And I click the "Apply Tag" button
  Then I will see the value "tagged" in the "Tags" column
  And I will see a modal with "The previous action can be undone."

Scenario: Modify tags
  And I am viewing a private message on the "Read message" page
  When I click the "Modify Tags" link at the top left
  And I change the value of the input to "modified"
  And I click the "Tag This Conversation" button
  Then I will the "modified" tag appear at the top of the thread
  And a modal with "Your conversation tags have been saved."

Scenario: Remove multiple tags
  And I am on the "Message inbox" page
  When I check the "left-hand" checkboxes next to the first two messages with the "tagged" tag
  And I select "tagged" in the "Remove Tag" select menu
  Then I will no longer see the "tagged" tag next to these messages in the "Tags" column
  And I will see a modal with: 
  """
  The tag has been removed from the selected conversations.
  The previous action can be undone.
  """

Scenario: Delete multiple messages
  And I am on the "Message inbox" page
  When I check the "left-hand" checkboxes next to the first two read messages
  And click the "delete" button
  Then these messages will be deleted
  And I will see a modal with:
  """
  Deleted 1 thread.
  The previous action can be undone.
  """

Scenario: Delete message
  And I am viewing a private message on the "Read message" page
  When I click the "Delete" link in the message header
  Then I see a screen with:
  """
  Are you sure you want to delete this message?
  This action cannot be undone.
  """ 
  When I click the "Delete" button
  Then the message will be deleted
  And I will see a modal with "Message has been deleted."

@fail @smoke
Scenario: Can NOT send an empty private message
  And I am on the "Write New Message" form page
  And I have not entered text into the "subject" field
  And I have not entered text into the "message" field
  And I click the "Send Message" button
  Then I will see a modal with "You must include a subject line with your message."
  And my message will not be sent

@fail
Scenario: Can NOT send an empty reply
  And I am viewing a private message on the "Read message" page
  But I have not entered any text in the "Reply Message" field
  When I click the "Send Message" button
  Then I see a modal with "You must include a message in your reply."
  And my message will not be sent

@fail
Scenario: Can NOT attach an executable file
  And I am on the "Write New Message" form page
  And I have entered text into the "subject" field
  And I have entered text into the "message" field
  When I click on the "File Attachments" link
  And I click the "Choose File" button
  And I select an "executable.exe" file
  And I click the "Attach" button
  Then I will see the message "No file chosen" next to the "Choose File" button
  And my file will not be attached

@fail
Scenario: Can NOT attach large files
  And I am on the "Write New Message" form page
  And I have entered text into the "subject" field
  And I have entered text into the "message" field
  When I click on the "File Attachments" link
  And I click the "Choose File" button
  And I select a "20Mb" file
  And I click the "Attach" button
  When I click the "Send Message" button
  Then I should see a modal informing me that my attachments have exceeded the maximum upload allowance
  And my message will not be sent

Scenario: Can NOT delete unselected messages
  And I am on the "Message inbox" page
  When I click the "delete" button
  Then I will see a modal with:
  """
  You must first select one (or more) messages before you can take that action.
  """
  And no messages will be deleted
