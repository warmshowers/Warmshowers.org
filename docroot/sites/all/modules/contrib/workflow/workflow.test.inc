<?php

/**
 * @file
 * Contains test functions.
 */

/**
 * Test functions.
 *
 * To test caches, load same object twice in a page (see d.o. issue #1572466).
 */
function _workflow_test_entity_workflow_crud() {
  $ws = workflow_load_multiple();
  $count1 = count($ws);
  $workflow = workflow_create('test' . REQUEST_TIME);

  $workflow->save();

  // Test cache: $w3 must be OK, too.
  $w2 = workflow_load($workflow->wid);
  $w3 = workflow_load($workflow->wid);
  if ($w2 != $w3) {
    // error.
  }

  // Test Cache: number of workflows must be OK.
  $ws = workflow_load_multiple();
  $count2 = count($ws);

  $workflow->delete();
  $ws = workflow_load_multiple();
  $count3 = count($ws);

  if ($count1 === $count3 && ($count2 - $count1) == 1) {
    drupal_set_message(t('workflow->create/save/delete OK'));
  }
  else  {
    drupal_set_message(t('workflow->create/save/delete: error'));
  }

  $workflow = workflow_create('test' . REQUEST_TIME);
  $s1 = $workflow->createState('first');
  $s2 = $workflow->createState('second');
  $s3 = $workflow->createState('third');
  $s1->save();
  $s2->save();
  $s2->save();
  $s3->save();
}

function _workflow_test_entity1() {
  $workflow = NULL;

  // Create a workflow.
  dpm('--- Create Workflow ---');
  dpm($workflow, '--- test workflow_create() pre');
  $workflow = workflow_create('test' . REQUEST_TIME);
  dpm($workflow, '--- test workflow_create() pre_save');
  $workflow->save();
  dpm($workflow, '--- test workflow_create() post_save');

  // Create States for the workflow.
  dpm('--- Create States ---');
  dpm($workflow->getStates(TRUE), '--- test getStates() pre');
  dpm($workflow, '--- test getStates() pre');
  $s1 = $workflow->createState('first');
  dpm($workflow->getStates(TRUE), '--- test getStates() post s1');
  dpm($workflow, '--- test getStates() post s1');
  $s2 = $workflow->createState('second');
  $s3 = $workflow->createState('third');
//  $s1->save();
//  $s2->save();
//  $s3->save();
  dpm($workflow->getStates(TRUE), '--- test getStates() post');
  dpm($workflow, '--- test getStates() post');
  dpm($s3->getWorkflow(), '--- test getStates() post');

  // Create Transitions for the workflow.
  dpm('--- Create Transitions ---');
  dpm('--- Create Transition s1->s2 ---');
  dpm($workflow->getTransitions(), '--- test getTransitions() pre');
  $roles = array(-1, 5);
  $t1 = $workflow->createTransition($s1->sid, $s2->sid);
  $t1->label = 'transitions t1';
  $t1->roles = $roles;
  // The transition was initially saved in createState,
  // but without roles and name.
  $t1->save();
  dpm($t1, '-- test show new Transition');
  dpm($workflow->getTransitions(), '-- test getTransitions() post 1');

  dpm('--- Create Transition s1->s2 ---');
  dpm($workflow->getTransitions(), '--- test getTransitions() pre');
  $roles = array(-1, 6);
  $t2 = $workflow->createTransition($s2->sid, $s3->sid);
  $t2->label = 'transitions t2';
  $t2->roles = $roles;
  // The transition was initially saved in createState,
  // but without roles and name.
  $t2->save();
  dpm($t2, '-- test show new Transition');
  dpm($workflow->getTransitions(), '-- test getTransitions() post 2');

  dpm('--- Show resulting Workflow ---');
  dpm($workflow);
  dpm('todo: Test if workflow, states and transitions are properly shown in Admin UI.');
  dpm('--- Show resulting Workflow after workflow_load---');
  // Refresh the workflow, and show again.
  $workflow2 = workflow_load($workflow->wid);
  dpm($workflow2);

//  dpm($t->uri());
//  dpm($t->label());

  // Remove the workflow and its components.
  dpm('--- Delete resulting Workflow ---');
  $workflow->delete();
  dpm($workflow);
  dpm('todo: Test if workflow, states and transitions are properly deleted.');
}

function _workflow_test_entity2() {
  $w = workflow_load('test');
  dpm($w);
  dpm($w->getTransitions());
  return;

  $ts = entity_load('WorkflowConfigTransition', array(6));
  $t = reset($ts);
  $t->roles += array(3 => 3);
  entity_save('WorkflowConfigTransition', $t);
}

function _workflow_test_entity3() {
  $workflow = workflow_load_single(1);
  dpm($workflow->getStates());
dpm($workflow->loadTransitions());
//  $t1 = $workflow->createTransition($s1->sid, $s2->sid);
//  $t1->save();
//  dpm($t1);
//  $t2 = $workflow->createTransition('third', 'second');
//  $t2->save();
//  dpm($t2);
//  dpm($t->uri());
//  dpm($t->label());
// $workflow->delete();

//  $ts = entity_load('WorkflowConfigTransition', array(6));
//  $t = reset($ts);
//  $t->roles += array(3 => 3);
//  entity_save('WorkflowConfigTransition', $t);
}

function _workflow_test_entity_allowable() {
  global $user;

  $workflow = workflow_create('workflow_test_' . REQUEST_TIME);
  $workflow->save();
  $s1 = $workflow->createState('to be opened');
  $s2 = $workflow->createState('open');
  $s3 = $workflow->createState('closed');
  $s1->save();
  $s2->save();
  $s3->save();
  $roles = array(1, 2); // anon, auth user.
  $t1 = $t = $workflow->createTransition(WORKFLOW_CREATION_STATE_NAME, 'to be opened');
  $t->roles = $roles;
  $t->save();
  $t2 = $t = $workflow->createTransition('to be opened', 'open');
  $t->roles = $roles;
  $t->save();
  $t3 = $t = $workflow->createTransition('open', 'closed');
  $t->roles = $roles;
DPM($t3);
  $t->save();
DPM($t3);
  $t4 = $t = $workflow->createTransition('closed', 'open');
  $t->roles = $roles;
  $t->save();

  dpm($s2->getOptions('', NULL, '', $user, FALSE));
  // returns TRUE if the role is allowed to do the transition.
  $rc = $t3->isAllowed($role = 2);
  dpm($t3);
  dpm('this result must be TRUE: ' . (int) $rc);
  $rc = $t3->isAllowed($role = 3);
  dpm('this result must be FALSE: ' . (int) $rc);

  $rc = $workflow->GetTransitionsBySid($s3->sid);
  dpm($rc, 'allowed transitions from ' . $s3->label());

  $ts = $workflow->getTransitions();
  dpm($ts);
  $ts = $workflow->getTransitionsBySid($s3->sid);
  dpm($ts);

  $workflow->delete();
}

function _workflow_test_metadata() {

  // Find the first, arbitrary workflow.
  $workflows = workflow_load_multiple();
  $workflow = reset($workflows);
dpm($workflow);

  // Add a Workflow wrapper;
  $wrapper = $workflow_wrapper = entity_metadata_wrapper('Workflow', $workflow);
dpm(t('---- Workflow: showing objects ---'));
dpm($wrapper);

  // Get properties of the Workflow.
dpm(t('---- Workflow: showing getters ---'));
dpm($wrapper->wid->label() . ' => ' . $wrapper->wid->value() );
dpm($wrapper->name->label() . ' => ' . $wrapper->name->value() );
dpm($wrapper->label->label() . ' => ' . $wrapper->label->value() );
dpm($wrapper->status->value() );
dpm($wrapper->tab_roles->value() );
//dpm($wrapper->states->value() );
dpm($wrapper->states->optionsList() );
//dpm($wrapper->transitions->value() );
dpm($wrapper->options->optionsList() );
  // Set properties of the Workflow.
  //dpm($wrapper->label->set('ssdfdss') );
  // ...

  // Get properties of States.
dpm(t('---- start of WorkflowState getters ---'));
  $states = $workflow->getStates();
  $state = reset($states);
  $state_wrapper = entity_metadata_wrapper('WorkflowState', $state);
dpm($states);
dpm(  $state_wrapper->sid->value()  );
dpm(  $state_wrapper->wid->value()  );
dpm(  $state_wrapper->weight->value()  );
//dpm(  $state_wrapper->state->value()  );
dpm(  $state_wrapper->status->value()  );

  //dpm(  $wrapper->author->mail->value()  );
  //dpm(  $wrapper->title->value(array('sanitize' => TRUE))  );

  // Get the transitions.
dpm(t('---- start of WorkflowConfigTransition getters ---'));
  $transitions = $state->getTransitions();
dpm($transitions);
  $options = $state->getOptions('', NULL, '', NULL);
dpm($options);
  $transition = reset($transitions);
dpm($transition);

  $transition_wrapper = entity_metadata_wrapper('WorkflowConfigTransition', $transition);
dpm(  $transition_wrapper  );
//dpm(  $transition_wrapper->old_sid->value()  );
dpm(  $transition_wrapper->old_state->value()  );
dpm(  $transition_wrapper->old_state->value()->sid  );

}

/**
 * Test all tokens
 */
function _workflow_test_tokens($entity_id = 76, $entity_type = 'node', $field_name = NULL) {
  global $user;

// $entity_id = 76;

  if ($entity_id) {
    $node = entity_load_single($entity_type, $entity_id);
  }
  else {
    // Create a node.
    $node = new stdClass();
    $node->title = "A new Node " . REQUEST_TIME;
    $node->type = "WorkfowField";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or for example 'en' if locale is enabled.
    $node->uid = $user->uid;
    $node->status = 1; //(1 or 0): published or not
    $node->promote = 0; //(1 or 0): promoted to front page
    $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write
//  // Term reference (taxonomy) field
  $node->field_workflow[$node->language][]['value'] = 1;
//  // Entity reference field
//  $node->field_customer_nid[$node->language][] = array(
//    'target_id' => $form_state['values']['entity id'],
//    'target_type' => 'node',
//  );
    // 'node' is default,
    // Other possible values are "user" and  "taxonomy_term".
    $node = node_submit($node); // Prepare node for saving
    node_save($node);
  }

  $t_greetings = "
    Hello [current-user:name]!
    <br/>node type + id + title = [node:content-type] + [node:nid] + [node:title]
  ";

  $t_node_all = '
 <br />	Workflow last transition	 = 	[node:last-transition]	    	Last workflow state transition of content.
 <br />	Comment comment	 = 	[node:last-transition:comment]	    	Workflow executed transition "comment" property.
 <br />	Created Medium format	 = 	[node:last-transition:created:medium]	    	A date in "medium" format. (Fri, 05/30/2014 - 10:59)
 <br />	Created Raw timestamp	 = 	[node:last-transition:created:raw]	    	A date in UNIX timestamp format (1401440380)
 <br />	Created Seconds-since	 = 	[node:last-transition:created:seconds]	    	A date in "seconds ago" format (604800). Use it for easy scheduling workflow transitions.
 <br />	Delta    	 = 	[node:last-transition:delta]	    	Workflow executed transition "delta" property.
 <br />	Entity_type	 = 	[node:last-transition:entity-type]	    	Workflow executed transition "entity_type" property.
 <br />	Field_name	 = 	[node:last-transition:field-name]	    	Workflow executed transition "field_name" property.
 <br />	label    	 = 	[node:last-transition:label]	    	Workflow executed transition "label" property.
 <br />	Language	 = 	[node:last-transition:language]	    	Workflow executed transition "language" property.

 <br />	New state	 = 	[node:last-transition:new-state]	    	The new state.
 <br />	  Original workflow state	 = 	[node:last-transition:new-state:original]	    	The original workflow state data if the workflow state is being updated or saved.
 <br />	  State ID	 = 	[node:last-transition:new-state:sid]	    	The State ID.
 <br />	  State label	 = 	[node:last-transition:new-state:label]	    	The state label.
 <br />	  Status	 = 	[node:last-transition:new-state:status]	    	Workflow state "status" property.
 <br />	  Sysid   	 = 	[node:last-transition:new-state:sysid]	    	Workflow state "sysid" property.
 <br />	  URL    	 = 	[node:last-transition:new-state:url]	    	The URL of the workflow state.
 <br />	  Weight	 = 	[node:last-transition:new-state:weight]	    	Workflow state "weight" property.
 <br />	  Wid    	 = 	[node:last-transition:new-state:wid]	    	Workflow state "wid" property.

 <br />	Nid	 = 	[node:last-transition:nid]	    	Workflow executed transition "nid" property.
 <br />	Old_sid	 = 	[node:last-transition:old-sid]	    	Workflow executed transition "old_sid" property.
 <br />	Revision_id = 	[node:last-transition:revision-id]	    	Workflow executed transition "revision_id" property.
 <br />	Sid	 = 	[node:last-transition:sid]	    	Workflow executed transition "sid" property.
 <br />	Stamp	 = 	[node:last-transition:stamp]	    	Workflow executed transition "stamp" property.
 <br />	Uid	 = 	[node:last-transition:uid]	    	Workflow executed transition "uid" property.

 <br />	Old state	 = 	[node:last-transition:old-state]	    	The old state.
 <br />	  Original workflow state	 = 	[node:last-transition:old-state:original]	    	The original workflow state data if the workflow state is being updated or saved.
 <br />	  State ID	 = 	[node:last-transition:old-state:sid]	    	The State ID.
 <br />	  State label	 = 	[node:last-transition:old-state:label]	    	The state label.
 <br />	  Status	 = 	[node:last-transition:old-state:status]	    	Workflow state "status" property.
 <br />	  Sysid  	 = 	[node:last-transition:old-state:sysid]	    	Workflow state "sysid" property.
 <br />	  URL   	 = 	[node:last-transition:old-state:url]	    	The URL of the workflow state.
 <br />	  Weight	 = 	[node:last-transition:old-state:weight]	    	Workflow state "weight" property.
 <br />	  Wid   	 = 	[node:last-transition:old-state:wid]	    	Workflow state "wid" property.

 <br />	User	 = 	[node:last-transition:user]	    	The user that executed the transition.
 <br />	  Created	 = 	[node:last-transition:user:created]	    	The date the user account was created.
 <br />	  Default theme	 = 	[node:last-transition:user:theme]	    	The user"s default theme.
 <br />	  Edit URL	 = 	[node:last-transition:user:edit-url]	    	The URL of the account edit page.
 <br />	  Email	 = 	[node:last-transition:user:mail]	    	The email address of the user account.
 <br />	  Last access	 = 	[node:last-transition:user:last-access]	    	The date the user last accessed the site.
 <br />	  Last login	 = 	[node:last-transition:user:last-login]	    	The date the user last logged in to the site.
 <br />	  Name	 = 	[node:last-transition:user:name]	    	The login name of the user account.
 <br />	  Original user	 = 	[node:last-transition:user:original]	    	The original user data if the user is being updated or saved.
 <br />	  Picture	 = 	[node:last-transition:user:picture]	    	The picture of the user.
 <br />	  Roles	 = 	[node:last-transition:user:roles]	    	The user roles associated with the user account.
 <br />	  Status	 = 	[node:last-transition:user:status]	    	Whether the user is active or blocked.
 <br />	  URL	 = 	[node:last-transition:user:url]	    	The URL of the account profile page.
 <br />	  User ID	 = 	[node:last-transition:user:uid]	    	The unique ID of the user account.

 <br />	Workflow	 = 	[node:last-transition:Workflow]	    	Workflow the state belongs to.
 <br />	  Workflow ID	 = 	[node:last-transition:Workflow:wid]	    	The ID used to identify this workflow internally.
 <br />	  Module	 = 	[node:last-transition:Workflow:module]	    	Workflow "module" property.
 <br />	  Options	 = 	[node:last-transition:Workflow:options]	    	Workflow "options" property.
 <br />	  Original workflow	 = 	[node:last-transition:Workflow:original]	    	The original workflow data if the workflow is being updated or saved.
 <br />	  States of this Workflow	 = 	[node:last-transition:Workflow:states]	    	States of this Workflow
 <br />	  Status	 = 	[node:last-transition:Workflow:status]	    	Workflow "status" property.
 <br />	  Tab_roles	 = 	[node:last-transition:Workflow:tab-roles]	    	Workflow "tab_roles" property.
 <br />	  Transitions of this Workflow	 = 	[node:last-transition:Workflow:transitions]	    	Transitions of this Workflow
 <br />	  URL	 = 	[node:last-transition:Workflow:url]	    	The URL of the workflow.
 <br />	  Workflow ID	 = 	[node:last-transition:Workflow:wid]	    	The unique ID of the workflow applied to this node.
 <br />	  Workflow label	 = 	[node:last-transition:Workflow:label]	    	The workflow applied to this node.

 <br />	  Workflow executed transition ID	 = 	[node:last-transition:hid]	    	The unique ID of the workflow executed transition.

  ';

// This text is to test only a few tokens, for better inspection.
  $t_node_sub = '
 <br />	User	 = 	[node:last-transition:user]	    	The user that executed the transition.
 <br />	label    	 = 	[node:last-transition:label]	    	Workflow executed transition "label" property.
  ';

// todo : test old-state
  $data = array();
  $data['node'] = $node;
  $data['entity'] = $node;
  $data['entity_type'] = 'node';

  // Display the tokenized text.
  dpm($node);
  dpm(token_replace(  $t_greetings, $data));
  dpm(token_replace(  $t_node_all, $data));
//  dpm(token_replace(  $t_node_sub, $data));

}

function _workflow_test_tokens_term($entity_id = '2', $entity_type = 'taxonomy_term', $field_name = NULL) {
  $token_type = 'term';

  global $user;

  if ($entity_id) {
    $node = entity_load_single($entity_type, $entity_id);
  }
  else {
    // Create a term.
  }

  $t_greetings = "
    Hello [current-user:name]!
    <br/>node type + id + title = [term:content-type] + [term:tid] + [term:title]
  ";

  $t_node_all = '
 <br />	Workflow last transition	 = 	[term:last-transition]	    	Last workflow state transition of content.
 <br />	Comment comment	 = 	[term:last-transition:comment]	    	Workflow executed transition "comment" property.
 <br />	Created	                 = 	[term:last-transition:created]	    	The date the transition was created.
 <br />	Created Medium format	 = 	[term:last-transition:created:medium]	    	A date in "medium" format. (Fri, 05/30/2014 - 10:59)
 <br />	Created Raw timestamp	 = 	[term:last-transition:created:raw]	    	A date in UNIX timestamp format (1401440380)
 <br />	Created Seconds-since	 = 	[term:last-transition:created:seconds]	    	A date in "seconds ago" format (604800). Use it for easy scheduling workflow transitions.
 <br />	Delta    	 = 	[term:last-transition:delta]	    	Workflow executed transition "delta" property.
 <br />	Entity_type	 = 	[term:last-transition:entity-type]	    	Workflow executed transition "entity_type" property.
 <br />	Field_name	 = 	[term:last-transition:field-name]	    	Workflow executed transition "field_name" property.
 <br />	label    	 = 	[term:last-transition:label]	    	Workflow executed transition "label" property.
 <br />	Language	 = 	[term:last-transition:language]	    	Workflow executed transition "language" property.

 <br />	New state	 = 	[term:last-transition:new-state]	    	The new state.
 <br />	  Original workflow state	 = 	[term:last-transition:new-state:original]	    	The original workflow state data if the workflow state is being updated or saved.
 <br />	  State ID	 = 	[term:last-transition:new-state:sid]	    	The State ID.
 <br />	  State label	 = 	[term:last-transition:new-state:label]	    	The state label.
 <br />	  Status	 = 	[term:last-transition:new-state:status]	    	Workflow state "status" property.
 <br />	  Sysid   	 = 	[term:last-transition:new-state:sysid]	    	Workflow state "sysid" property.
 <br />	  URL    	 = 	[term:last-transition:new-state:url]	    	The URL of the workflow state.
 <br />	  Weight	 = 	[term:last-transition:new-state:weight]	    	Workflow state "weight" property.
 <br />	  Wid    	 = 	[term:last-transition:new-state:wid]	    	Workflow state "wid" property.

 <br />	Nid	 = 	[term:last-transition:nid]	    	Workflow executed transition "nid" property.
 <br />	Old_sid	 = 	[term:last-transition:old-sid]	    	Workflow executed transition "old_sid" property.
 <br />	Revision_id = 	[term:last-transition:revision-id]	    	Workflow executed transition "revision_id" property.
 <br />	Sid	 = 	[term:last-transition:sid]	    	Workflow executed transition "sid" property.
 <br />	Stamp	 = 	[term:last-transition:stamp]	    	Workflow executed transition "stamp" property.
 <br />	Uid	 = 	[term:last-transition:uid]	    	Workflow executed transition "uid" property.

 <br />	Old state	 = 	[term:last-transition:old-state]	    	The old state.
 <br />	  Original workflow state	 = 	[term:last-transition:old-state:original]	    	The original workflow state data if the workflow state is being updated or saved.
 <br />	  State ID	 = 	[term:last-transition:old-state:sid]	    	The State ID.
 <br />	  State label	 = 	[term:last-transition:old-state:label]	    	The state label.
 <br />	  Status	 = 	[term:last-transition:old-state:status]	    	Workflow state "status" property.
 <br />	  Sysid  	 = 	[term:last-transition:old-state:sysid]	    	Workflow state "sysid" property.
 <br />	  URL   	 = 	[term:last-transition:old-state:url]	    	The URL of the workflow state.
 <br />	  Weight	 = 	[term:last-transition:old-state:weight]	    	Workflow state "weight" property.
 <br />	  Wid   	 = 	[term:last-transition:old-state:wid]	    	Workflow state "wid" property.

 <br />	User	 = 	[term:last-transition:user]	    	The user that executed the transition.
 <br />	  Created	 = 	[term:last-transition:user:created]	    	The date the user account was created.
 <br />	  Default theme	 = 	[term:last-transition:user:theme]	    	The user"s default theme.
 <br />	  Edit URL	 = 	[term:last-transition:user:edit-url]	    	The URL of the account edit page.
 <br />	  Email	 = 	[term:last-transition:user:mail]	    	The email address of the user account.
 <br />	  Last access	 = 	[term:last-transition:user:last-access]	    	The date the user last accessed the site.
 <br />	  Last login	 = 	[term:last-transition:user:last-login]	    	The date the user last logged in to the site.
 <br />	  Name	 = 	[term:last-transition:user:name]	    	The login name of the user account.
 <br />	  Original user	 = 	[term:last-transition:user:original]	    	The original user data if the user is being updated or saved.
 <br />	  Picture	 = 	[term:last-transition:user:picture]	    	The picture of the user.
 <br />	  Roles	 = 	[term:last-transition:user:roles]	    	The user roles associated with the user account.
 <br />	  Status	 = 	[term:last-transition:user:status]	    	Whether the user is active or blocked.
 <br />	  URL	 = 	[term:last-transition:user:url]	    	The URL of the account profile page.
 <br />	  User ID	 = 	[term:last-transition:user:uid]	    	The unique ID of the user account.

 <br />	Workflow	 = 	[term:last-transition:Workflow]	    	Workflow the state belongs to.
 <br />	  Workflow ID	 = 	[term:last-transition:Workflow:wid]	    	The ID used to identify this workflow internally.
 <br />	  Module	 = 	[term:last-transition:Workflow:module]	    	Workflow "module" property.
 <br />	  Options	 = 	[term:last-transition:Workflow:options]	    	Workflow "options" property.
 <br />	  Original workflow	 = 	[term:last-transition:Workflow:original]	    	The original workflow data if the workflow is being updated or saved.
 <br />	  States of this Workflow	 = 	[term:last-transition:Workflow:states]	    	States of this Workflow
 <br />	  Status	 = 	[term:last-transition:Workflow:status]	    	Workflow "status" property.
 <br />	  Tab_roles	 = 	[term:last-transition:Workflow:tab-roles]	    	Workflow "tab_roles" property.
 <br />	  Transitions of this Workflow	 = 	[term:last-transition:Workflow:transitions]	    	Transitions of this Workflow
 <br />	  URL	 = 	[term:last-transition:Workflow:url]	    	The URL of the workflow.
 <br />	  Workflow ID	 = 	[term:last-transition:Workflow:wid]	    	The unique ID of the workflow applied to this node.
 <br />	  Workflow label	 = 	[term:last-transition:Workflow:label]	    	The workflow applied to this node.

 <br />	  Workflow executed transition ID	 = 	[term:last-transition:hid]	    	The unique ID of the workflow executed transition.
  ';

// This text is to test only a few tokens, for better inspection.
  $t_node_sub = '
 <br />	Workflow	 = 	[term:last-transition:Workflow]	    	Workflow the state belongs to.
  ';

  $data = array();
  // $data['node'] = $node;
  $data['term'] = $node;
  $data['entity'] = $node;
  $data['entity_type'] = $entity_type;
  $data['token_type'] = $token_type;
  $options['sanitize'] = FALSE;
  $options['clear'] = TRUE;
  $options['language'] = NULL;
//dpm($node);
//dpm($data);
  // Display the tokenized text.
//  dpm(token_replace($t_greetings, $data));
  dpm(token_replace($t_node_all, $data, $options));
}

/**
 * Test, taken from CommentTokenReplaceTestCase::testCommentTokenReplacement
 */
/*
function WorkflowCommentTokenReplacement() {
  global $language;
  $url_options = array(
    'absolute' => TRUE,
    'language' => $language,
  );

  $this->drupalLogin($this->admin_user);

  // Set comment variables.
  $this->setCommentSubject(TRUE);

  // Create a node and a comment.
  $node = $this->drupalCreateNode(array('type' => 'article'));
  $parent_comment = $this->postComment($node, $this->randomName(), $this->randomName(), TRUE);

  // Post a reply to the comment.
  $this->drupalGet('comment/reply/' . $node->nid . '/' . $parent_comment->id);
  $child_comment = $this->postComment(NULL, $this->randomName(), $this->randomName());
  $comment = comment_load($child_comment->id);
  $comment->homepage = 'http://example.org/';

  // Add HTML to ensure that sanitation of some fields tested directly.
  $comment->subject = '<blink>Blinking Comment</blink>';
  $instance = field_info_instance('comment', 'body', 'comment_body');

  // Generate and test sanitized tokens.
  $tests = array();
  $tests['[comment:cid]'] = $comment->cid;
  $tests['[comment:hostname]'] = check_plain($comment->hostname);
  $tests['[comment:name]'] = filter_xss($comment->name);
  $tests['[comment:mail]'] = check_plain($this->admin_user->mail);
  $tests['[comment:homepage]'] = check_url($comment->homepage);
  $tests['[comment:title]'] = filter_xss($comment->subject);
  $tests['[comment:body]'] = _text_sanitize($instance, LANGUAGE_NONE, $comment->comment_body[LANGUAGE_NONE][0], 'value');
  $tests['[comment:url]'] = url('comment/' . $comment->cid, $url_options + array('fragment' => 'comment-' . $comment->cid));
  $tests['[comment:edit-url]'] = url('comment/' . $comment->cid . '/edit', $url_options);
  $tests['[comment:created:since]'] = format_interval(REQUEST_TIME - $comment->created, 2, $language->language);
  $tests['[comment:changed:since]'] = format_interval(REQUEST_TIME - $comment->changed, 2, $language->language);
  $tests['[comment:parent:cid]'] = $comment->pid;
  $tests['[comment:parent:title]'] = check_plain($parent_comment->subject);
  $tests['[comment:node:nid]'] = $comment->nid;
  $tests['[comment:node:title]'] = check_plain($node->title);
  $tests['[comment:author:uid]'] = $comment->uid;
  $tests['[comment:author:name]'] = check_plain($this->admin_user->name);

  // Test to make sure that we generated something for each token.
  $this->assertFalse(in_array(0, array_map('strlen', $tests)), 'No empty tokens generated.');

  foreach ($tests as $input => $expected) {
    $output = token_replace($input, array('comment' => $comment), array('language' => $language));
    $this->assertEqual($output, $expected, format_string('Sanitized comment token %token replaced.', array('%token' => $input)));
  }

  // Generate and test unsanitized tokens.
  $tests['[comment:hostname]'] = $comment->hostname;
  $tests['[comment:name]'] = $comment->name;
  $tests['[comment:mail]'] = $this->admin_user->mail;
  $tests['[comment:homepage]'] = $comment->homepage;
  $tests['[comment:title]'] = $comment->subject;
  $tests['[comment:body]'] = $comment->comment_body[LANGUAGE_NONE][0]['value'];
  $tests['[comment:parent:title]'] = $parent_comment->subject;
  $tests['[comment:node:title]'] = $node->title;
  $tests['[comment:author:name]'] = $this->admin_user->name;

  foreach ($tests as $input => $expected) {
    $output = token_replace($input, array('comment' => $comment), array('language' => $language, 'sanitize' => FALSE));
    $this->assertEqual($output, $expected, format_string('Unsanitized comment token %token replaced.', array('%token' => $input)));
  }

  // Load node so comment_count gets computed.
  $node = node_load($node->nid);

  // Generate comment tokens for the node (it has 2 comments, both new).
  $tests = array();
  $tests['[node:comment-count]'] = 2;
  $tests['[node:comment-count-new]'] = 2;

  foreach ($tests as $input => $expected) {
    $output = token_replace($input, array('node' => $node), array('language' => $language));
    $this->assertEqual($output, $expected, format_string('Node comment token %token replaced.', array('%token' => $input)));
  }
}
 */
