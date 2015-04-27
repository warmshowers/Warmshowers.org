********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Workflow Module
Author: John VanDyk
Maintainers: Mark Fredrickson <mark.m.fredrickson at gmail dot com>
             John VanDyk drupal.org/user/2375
             Bastlynn http://drupal.org/user/275249
             Nancy Wichmann (NancyDru) http://drupal.org/user/101412
             John Voskuilen (johnv) http://drupal.org/user/591042
Drupal: 7
********************************************************************
CONTENTS:
 - DESCRIPTION
 - INSTALLATION:
 - GETTING STARTED: SETTING UP A WORKFLOW
 - GETTING STARTED: ADDING A WORKFLOW TO A CONTENT TYPE
 - GETTING STARTED: CREATING A NODE
 - ADVANCED SETTINGS
   - Token
   - Views
   - Avaiable hooks

********************************************************************
DESCRIPTION

The workflow module enables you to create arbitrary workflows in
Drupal and associate them with node types.

Workflows are made up of workflow states.
Moving from one state to another is called a transition.
Actions can be associated with transitions.
Tokens are supported when modules Token and Entity_tokens are enabled.

********************************************************************
INSTALLATION

1. Place the entire workflow directory into your Drupal
   sites/all/modules directory (or appropriate alternative).

2. Enable the workflow module by navigating to:

     Administration » Modules

   Enabling the workflow module will create the necessary database
   tables for you.

3. If you wish to use the administrative UI, then enable the
   Workflow UI module. There are several other optional modules
   that you may also enable, if needed.

4. If you want anyone besides the administrative user to be able
   to configure workflows (usually a bad idea), they must be given
   the "administer workflow" access permission:

     Administration » People » Permissions

   When the module is enabled and the user has the "administer
   workflow" permission, a "Workflow" menu should appear in the
   menu system under Configuration -> Workflow.

   You may also grant other Permissions to user roles.

********************************************************************
GETTING STARTED: SETTING UP A WORKFLOW

Let's create a new workflow. Click on Administer -> Configuration ->
Workflow -> Workflow and click on the "Add workflow" tab.

We'll start simple. Call our workflow "Draft-Done" and click Add Workflow.

Now lets add some workflow states to our workflow. Click "add state" and
enter "draft" and click the Add State button. Do the same for "done".

So we've got a workflow with two states, "draft" and "done". Now we
have to tell each state which other states it can move to. With only
two states, this is easy. Click on the "edit" link to edit the workflow
and see its states.

The "From / To -->" column lists all states. To the right are columns
for each state. Within each cell is a list of roles with checkboxes.

This is confusing. It's easiest to understand if you read rows
across from the left. For example, we start with the creation
state. Who may move a node from its creation state to the "draft"
state? Well, the author of the node, for one. So check the "author"
checkbox.

Who may move the node from the "draft" state to the "done" state?
This is up to you. If you want authors to be able to do this,
check the "author" checkbox under the "done" state. If you had
another role, say "editor", that you wanted to give the ability
to decree a node as "done", you'd check the checkbox next to
the "editor" role and not the author role. In this scenario authors
would turn in drafts and editors would say when they are "done".

Be sure to click the Save button to save your settings.

Now we could add an action (previously configured using the trigger
module). Click on the Actions link above your workflow. Add the action
to the transition.

********************************************************************
GETTING STARTED: ADDING A WORKFLOW TO A CONTENT TYPE

Now let's tell Drupal which node types should use this workflow. Go to

  Administration » Structure » Content types » MY_CONTENT_TYPE

Add a field of 'Field Type' Workflow. Let's assign the Draft-Done workflow
to the article node type and click Save.

********************************************************************
GETTING STARTED: CREATING A NODE

Now create a new article by going to Create content -> article. If there
is no sign of a workflow interface here, don't panic. The interface
is only displayed if there is more than one state to which the user
can move the node (why bother the user with a form with only one
selection?) Click Submit to create the article.

You can see the state the node is in and the history of state changes
by clicking on the Workflow tab while viewing a node.

Changing the state to "done" and clicking Submit will fire the action
you set up earlier.

********************************************************************
ADVANCED SETTINGS:

- In the Workflow Field Settings you'll find (amongst other settings)
  an option to choose how to display the available target settings. You
  may choose between 'select list', 'radio buttons' or 'action buttons'.
  The optons show the state labels, or - if you have set them in the
  Workflow Transitions Tab - the transition labels.

- Be sure to enable the TOKEN module and the ENTITY TOKEN module to
  make use of extra tokens. They include all properties of the latest
  transition.

  How to test the available tokens?
  - Enable module 'Token'; use page admin/help/token;
  - Enable module 'Token example'; use page examples/token;
  - Enable module Automatic Entity Label, set a label, and save entity.

  It also contains a token '[node:last-transition:created:seconds]'
  that may be used with Rules to invoke actions, like sending reminder emails,
  when content was NOT updated or a workflow did NOT transition state
  for some time. Using these tokens you won't need PHP snippets.

- If you have WORKFLOW VIEWS enabled, a "Workflow" menu item will appear in the
  navigation menu. This displays on a single page workflow state transition
  forms for all nodes on your system that are subject to workflow. Naturally
  you can modify and extend this View to your heart's content.

- If you want to add functionality programmatically, please check the following
  files:
  - workflow.api.php for all available hooks;
  - workflow.test.inc for some rudimentary coding examples;

- You'll find all settings in one of below pages:
  - /admin/config/workflow/workflow
  - /admin/people/permissions#module-workflow_admin_ui
  - /admin/structure/types/manage/MY_CONTENTY_TYPE/fields/MY_FIELD
