********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Workflow Module
Author: John VanDyk
Maintainers: Mark Fredrickson <mark.m.fredrickson at gmail dot com>
             John VanDyk drupal.org/user/2375
Drupal: 6
********************************************************************
DESCRIPTION:

The workflow module enables you to create arbitrary workflows in 
Drupal and associate them with node types.

Workflows are made up of workflow states.

Moving from one state to another is called a transition.

Actions are associated with transitions (actions.module was used
for this in Drupal 5; core actions support is in Drupal 6).

Alex Reisner introduced role-based permissions for workflow states
and generally enhanced this module.

********************************************************************
INSTALLATION:

1. Place the entire workflow directory into your Drupal
   sites/all/modules directory.


2. Enable the workflow module by navigating to:

     Administer > Site building > Modules

   Enabling the workflow module will create the necessary database 
   tables for you.

3. If you want anyone besides the administrative user to be able
   to configure workflows (usually a bad idea), they must be given
   the "administer workflow" access permission:
   
     Administer > User management > Permissions

   When the module is enabled and the user has the "administer
   workflow" permission, a "Workflow" menu should appear in the 
   menu system under Administer -> Site building.

   You may also allow only some users to schedule transitions. Select
   the "schedule workflow transitions" permission to allow transitions.

********************************************************************
GETTING STARTED:

Let's create a new workflow. Click on Administer -> Site building -> 
Workflow and click on the "Add workflow" tab.

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

Now let's tell Drupal which node types should use this workflow. Click
on Administer -> Site building -> Workflow. Let's assign the Draft-Done
workflow to the story node type and click Save Workflow Mapping.

Now we could add an action (previously configured using the trigger
module). Click on the Actions link above
your workflow. Add the action to the transition.

Now create a new story by going to Create content -> Story. If there
is no sign of a workflow interface here, don't panic. The interface
is only displayed if there is more than one state to which the user
can move the node (why bother the user with a form with only one
selection?) Click Submit to create the story.

You can see the state the node is in and the history of state changes
by clicking on the Workflow tab while viewing a node.

Changing the state to "done" and clicking Submit will fire the action
you set up earlier.

********************************************************************