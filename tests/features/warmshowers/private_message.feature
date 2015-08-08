#language:en
Feature:I can exchange private messages with other Warmshowers users
  In order to interact with other Warmshowers users
  As an authenticated user
  I can send and receive private messages

Background:
  Given I am an authenticated user

Scenario: I can use the Send Message button on a user's profile to reach the Write New Message form
  And I am viewing another user's profile
  When I click on the Send Message button in the location sidebar
  Then I see the Write New Message form for the user.

Scenario: I can reach the Write New Message form for the original author of a forum thread
  And I have entered a thread in the forums section
  When I click the Send Author a Message link at the bottom of the original post
  Then I will see the New Message form with the appropriate recipient.

Scenario: I can send a private message
  And I have reached the Write New Message form with a given member as recipient
  When I enter text into the message field
  And I click Send Message
  Then I will see the sent message open in the Messages tab of my profile
  And I will see a modal with "A message has been sent to [user]"
  And the other user will receive the message in their WS inbox
  And the other user will receive an email notifying them of the message

Scenario: I can read private messages I have received
  And I am viewing the Messages tab within my profile
  And Inbox in the submenu
  When I click on the message subject
  Then I will see the message text

Scenario: I can read private messages I have sent
  And I am viewing the Messages tab within my profile
  And Sent Messages in the submenu
  When I click on the message subject
  Then I will see the message text

Scenario: I can read private messages I have received but not replied to
  And I am viewing the Messages tab within my profile
  And Unanswered Requests in the submenu
  When I click on the message subject
  Then I will see the message text

Scenario: I can reply to a privage message
  And I am viewing a private message received from another user
  When I enter text into the Reply field
  And I click Send Message
  Then I will see the reply below the preceding message in the Messages/Read Message tab
  And I will see a modal with "A message has been sent to [user]"
  And the other user will receive the message in their WS inbox
  And the other user will receive an email notifying them of the message

Scenario: I can clear an unsent reply
  And I am viewing an existing message thread
  And I have entered text in the Reply field
  But I have not sent the message
  When I click the Clear link at the bottom of the thread
  Then my entered text will no longer appear in the Reply field.

Scenario: I can attach files to a message
  And I have entered message body text for a new message or reply
  When I click on the File Attachments link
  And I click Choose File
  And I select a file of appropriate type and size
  And I click Attach
  And I click Send Message
  Then I will see the message thread
  And a modal with "A message has been sent to [user]"
  And a download link for my attachment will appear at the bottom of my sent message

#Not sure what, if anything the "list" checkbox does here.
Scenario: I can edit the description of an attachment to an unsent message
  And I have entered message body text for a new message or reply
  And I have attached one or more files to my message
  When I enter a description in the text field
  And I click Send Message
  Then I will see the message thread
  And a modal with "A message has been sent to [user]"
  And the download link for my attachment will carry my description text instead of the filename

Scenario: I can remove an attachment from a message
  And I have entered message body text for a new message or reply
  And I have attached one or more files to my message
  When I check the Delete checkbox next to the file name
  And I click Send Message
  Then the message will be sent without the deleted attachment

Scenario:I can reorder attachments to a message
  And I have entered message body text for a new message or reply
  And I have attached one or more files to my message
  When I click the cross at the left of the file name
  And I drag the file(s) into the desired order
  And I click Send Message
  Then the message will be sent with attachments listed in desired order.

Scenario: I can forward a private message thread to another user
  And I am viewing a private message thread from the Read Message submenu
  When I click the Forward Conversation to Others link at the bottom of the thread
  And I enter one or more valid Warmshowers names separated by commas in the To: field
  Then I will see a modal with:"This conversation has been forwarded to [user]."

#I'm not sure if the Filter by Participant option does anything- wasn't able to get any discernable response but I have a very small sample. Filter by subject does work.
Scenario: I can filter my messages
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I click on the Filter Messages link
  And enter a value in the subject or participant field
  And click the Filter button
  Then I will see only messages that match my filter term

#Save filter doesn't actually do anything that I can identify, and I'm not sure what behavior it SHOULD exhibit, so leaving this scenario incomplete for now.
#Scenario: I can save a message filter for later use
#  And I am viewing the Messages tab (any submenu)
#  When I click on the Filter Messages link
#  And enter a value in the subject or participant field
#  And click the Save Filter button

Scenario: I can mark messages as unread
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I check one or more checkboxes next to message(s)
  And I select Mark as Unread from the Actions dropdown menu
  Then I will see a modal with:
  """
  Marked [#] thread as unread.
  The previous action can be undone.
  """
  And the message will appear as unread in the listing.

Scenario: I can mark messages as read
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I check one or more checkboxes next to message(s)
  And I select Mark as Read from the Actions dropdown menu
  Then I will see a modal with:
  """
  Marked [#] thread as read.
  The previous action can be undone.
  """
  And the message will appear as read in the listing.

#The Archive option currently only appears in the Actions menu in the Inbox submenu, not Sent Messages or All Messages.
Scenario: I can archive messages
  And I am viewing the Messages tab
  And the Inbox submenu
  When I check one or more checkboxes next to message(s)
  And I select Archive from the Actions dropdown menu
  Then I will see a modal with:
  """
  The messages have been archived.
  The previous action can be undone.
  """
  And the message will still appear in the All Messages listing 
  But will not appear in the Inbox.

Scenario: I can sort messages
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I click the Subject, Started, or Last Updated links in the table heading
  Then I will see my messages sorted according to my chosen option
  And the arrow next to the sort mode will indicate ascending or descending order

#The processes/modals for tagging messages should probably be consistent.
Scenario: I can tag messages from the message listing
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I check the checkbox next to a message
  And I enter a tag value in the input field
  And I click Apply Tag
  Then I will see my tag next to the message subject in the Tags column
  And I will see a modal with "The previous action can be undone."

Scenario: I can tag messages from the Read Message view
  And I am viewing a private message thread from the Read Message submenu
  When I click the Tag This Conversation link at the top of the thread
  And I enter a tag value in the input field
  And I click the Tag This Conversation button
  Then I will see my tag appear at the top of the thread
  And a modal with "Your conversation tags have been saved."

Scenario: I can modify tags on a message thread
  And I am viewing a private message thread from the Read Message submenu
  When I click the Modify Tags link at the top left
  And I make changes within the input field
  And I click the Tag This Conversation button
  Then I will see my updated tag(s) appear at the top of the thread
  And a modal with "Your conversation tags have been saved."

Scenario: I can remove tags messages using the Remove Tag dropdown
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I check the checkbox next to a message
  And I select a tag in the Remove Tag dropdown
  Then I will no longer see the removed tag next to the message subject in the Tags column
  And I will see a modal with: 
  """
  The tag has been removed from the selected conversations.
  The previous action can be undone.
  """

#These two delete scenarios are really very different- this should probably be addressed after D7 is stable.
Scenario: I can delete messages from the message listing
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I check the checkbox next to a message
  And click the delete button
  Then the message will be deleted
  And I will see a modal with:
  """
  Deleted 1 thread.
  The previous action can be undone.
  """

Scenario: I can delete messages from the Read Message view
  And I am viewing a private message thread from the Read Message submenu
  When I click the Delete link next to the author and date/timestamp information
  And I see a screen with:
  """
  Are you sure you want to delete this message?
  This action cannot be undone.
  """ 
  And I click Delete
  Then the message will be deleted
  And I will see a modal with "Message has been deleted."

#Validation/Failure Scenarios:

#Currently, the system will allow sending messages with EITHER subject OR message (or both).  It will not send a message with only an attachment.  The failure modal isn't totally accurate and the validation rules might benefit from tightening a bit here.
Scenario: I can NOT send an empty private message
  And I have reached the Write New Message form with a given member as recipient
  When I do not enter text in the Subject or Message Fields
  And I click Send Message
  Then I will see a modal with "You must include a subject line with your message."
  And my message will not be sent.

Scenario: I can NOT send an empty reply
  And I am viewing a private message thread from the Read Message submenu
  But I have not entered any text in the Reply field
  When I click the Send Message button
  Then I see a modal with "You must include a message in your reply."
  And my message will not be sent.

Scenario: I can NOT attach a file of unapproved file type to a message
  And I have entered message body text for a new message or reply
  When I click on the File Attachments link
  And I click Choose File
  And I select a file of incorrect file type/extension
  And I click Attach
  Then I will see the message "No file chosen" next to the Choose File button
  And my file will not be attached.

#The system currently DOES allow cumulative totals exceeding the limit. Couldn't find a single file big enough to see if the max limit works on individual file uploads.
#This scenario is a guess about proper functionality.
Scenario: I can NOT attach file(s) exceeding the maximum upload size of 15 mb.
  And I have entered message body text for a new message or reply
  When I click on the File Attachments link
  And I click Choose File
  And I select file(s) exceeding the 15 mb limit
  And I click Attach
  And I click the Send Message button
  Then I should see a modal informing me that my attachments have exceeded the maximum upload allowance
  And my message will not be sent

Scenario: I can NOT delete messages from the listing without selecting their checkboxes
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I click the delete button
  But I do not select any message checkboxes
  Then I will see a modal with:
  """
  You must first select one (or more) messages before you can take that action.
  """
  And no messages will be deleted

Scenario: I can NOT tag messages from the listing without selecting their checkboxes
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I click the Apply Tag button
  But I do not select any message checkboxes
  Then I will see a modal with:
  """
  You must first select one (or more) messages before you can take that action.
  """
  And no messages will be tagged

Scenario: I can NOT remove tags from messages from the listing without selecting their checkboxes
  And I am viewing the Messages tab (Inbox, Sent, or All submenu)
  When I select a tag from the Remove Tag dropdown
  But I do not select any message checkboxes
  Then I will see a modal with:
  """
  You must first select one (or more) messages before you can take that action.
  """
  And no tags will be removed