<?php

/*
 * All items in this document are responsible for generating the tab (menu local) pages
 * that are used for administering an individual poll.
 */

/*
 * Generates page for administering individual voters on a poll.  Enables administrator
 * to clear all votes. For users that can view votes but not administer them, the button
 * is not displayed.
 */

function advpoll_votes_page($node) {
  $data = advpoll_get_data($node);

  $output = t('This table lists all the recorded votes for this poll.');
  if ($data->mode == 'unlimited') {
    $output = t('With unlimited voting, a timestamp is used to identify unique votes.  If it is important to identify users by ID or IP, switch to normal voting mode which will use your Voting API settings to record votes.');
  } else if ($data->mode == 'cookie') {
    $output = t('With cookie-based voting, a timestamp is used to identify unique votes while the poll\'s id is set in the cookie to limit votes for a limited time.  If it is important to identify users by ID or IP, switch to normal voting mode which will use your Voting API settings to record votes.');
  } else {
    $output = t('If anonymous users are allowed to vote, they will be identified by the IP address of the computer they used when they voted.');
  }

  $header = array();
  $header[] = array('data' => t('Visitor'), 'field' => 'uid');
  $header[] = array('data' => t('Date'), 'field' => 'timestamp', 'sort' => 'asc');
  $header[] = array('data' => t('Choice'), 'tag');

  $nid = $node->nid;

  $query = db_select('votingapi_vote', 'v')
    ->condition('entity_id', $nid)
    ->extend('PagerDefault')
    ->limit(20)
    ->extend('TableSort')
    ->orderByHeader($header)
    ->fields('v', array(
    'uid',
    'timestamp',
    'tag',
    'vote_source',
    ));

  $results = $query->execute();
  $userObj = null;
  $rows = array();
  foreach ($results as $item) {
    $userID = $item->uid;

    if (!$userID) {
      $userID = $item->vote_source;
    } else {
      $userObj = user_load($userID);
      if ($userObj) {
        $userID = l($userObj->name, 'user/' . $userID);
      }
    }

    $rows[] = array(
      'data' => array(
        $userID,
        format_date($item->timestamp),
        advpoll_match_tag_to_choice($data->choices, $item->tag),
      )
    );
  }

  if ($rows) {
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      )
    );

    $output .= theme('pager', array('tags' => array()));
    if (user_access('administer votes')) {
      $output .= drupal_render(drupal_get_form('advpoll_clear_votes_form'));
    }
  } else {
    $output .= '<hr /><p>' . t('No votes are currently recorded for %title', array('%title' => $node->title)) . '</p>';
  }

  return $output;
}

function advpoll_match_tag_to_choice($choices, $tag) {
  foreach ($choices as $choice) {
    if ($choice['choice_id'] == $tag) {
      return $choice['choice'];
    }
  }
  return;
}

function advpoll_clear_votes_form($form, &$form_state) {
  $nid = (int) check_plain(arg(1));

  $form['reset'] = array(
    '#value' => t('Clear all votes'),
    '#type' => 'submit',
  );
  $form['#action'] = url('node/' . $nid . '/advpoll/votes/clear');
  return $form;
}

function advpoll_electoral_list_page($node) {
  $output = '';

  if (user_access('administer polls')) {
    $output .= drupal_render(drupal_get_form('advpoll_electoral_list_form', $node->nid));
  }

  $output .= '<p>' . t('This table lists all the eligible voters for this poll.') . '</p>';

  $header = array();
  $header[] = array('data' => t('Voter  '), 'field' => 'name');
  if (user_access('administer polls')) {
    $header[] = array('data' => t('Remove'), 'field' => 'uid');
  }
  $query = db_select('advpoll_electoral_list', 'el');
  $query->join('users', 'u', 'el.uid = u.uid');
  $query->condition('el.nid', $node->nid);

  $query->extend('PagerDefault')
    ->limit(20)
    ->extend('TableSort')
    ->orderByHeader($header)
    ->fields('u', array(
      'name',
      'uid',
    ));

  $results = $query->execute();

  $rows = array();
  foreach ($results as $item) {
    $voter = '';
    if (user_access('administer polls')) {
      $voter = l(t('remove'), 'node/' . $node->nid . '/remove/' . $item->uid);
    }

    if ($voter) {
      $rows[] = array(
        'data' => array(
          $item->name,
          $voter,
        )
      );
    } else {
      $rows[] = array(
        'data' => array(
          $item->name,
        )
      );
    }
  }

  if ($rows) {
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      )
    );

    $output .= theme('pager', array('tags' => array()));
  } else {
    $output .= '<hr /><p>' . t('No users have currently been added to the electoral list for %title', array('%title' => $node->title)) . '</p>';
  }

  return $output;
}

/**
 * Build electorial list form.
 */
function advpoll_electoral_list_form($form, &$form_state, $nid) {
  $form['electoral_list'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Administer electoral list'),
    '#collapsible' => TRUE,
    '#weight' => 2,
  );
  $form['electoral_list']['add_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Add user'),
    '#size' => 40,
    '#description' => t('Add an individual user to the electoral list.'),
  );

  // Enable autocompletion if user has required permission.
  if (user_access('access user profiles')) {
    $form['electoral_list']['add_user']['#autocomplete_path'] = 'user/autocomplete';
  }

  // List all roles with "vote on polls" permission, but don't include anonymous users.
  $result = db_query("SELECT r.name, r.rid FROM {role} r LEFT JOIN {role_permission} p ON p.rid = r.rid WHERE p.permission LIKE '%vote on polls%' AND r.rid <> 1 ORDER BY r.name");
  $role_options = array(0 => t('(Select a role)'));

  foreach ($result as $record) {
    $role_options[$record->rid] = $record->name;
  }

  $form['electoral_list']['add_role'] = array(
    '#type' => 'select',
    '#title' => t('Add users by role'),
    '#description' => t('Only roles that have the "vote on polls" permission are listed.'),
    '#options' => $role_options,
  );
  $form['electoral_list']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add to electoral list'),
  );
  $form['electoral_list']['reset'] = array(
    '#type' => 'button',
    '#value' => t('Clear electoral list'),
  );

  $form['nid'] = array('#type' => 'hidden', '#value' => $nid);
  return $form;
}

/*
 * Handles results of submitting electoral administration form.
 */

function advpoll_electoral_list_form_submit($form, &$form_state) {
  $add_user = $form_state['values']['electoral_list']['add_user'];
  $nid = $form_state['values']['nid'];

  // using new db_merge method to avoid having to write multiple checks for insert.
  if ($add_user) {
    $uid = db_query('SELECT uid FROM {users} WHERE name = :user', array(':user' => $add_user))->fetchField();
    db_merge('advpoll_electoral_list')
      ->key(array(
        'nid' => $nid,
        'uid' => $uid,
      ))
      ->fields(array(
        'nid' => $nid,
        'uid' => $uid,
      ))
      ->execute();
    drupal_set_message(t('%user added to electoral list.', array('%user' => $add_user)));
  }

  $add_role = $form_state['values']['electoral_list']['add_role'];
  $roles = $form['electoral_list']['add_role']['#options'];

  if ($add_role) {
    $result = db_query("SELECT u.uid FROM {users} u LEFT JOIN {users_roles} ur ON u.uid = ur.uid WHERE ur.rid = :rid", array(':rid' => $add_role));

    foreach ($result as $record) {
      db_merge('advpoll_electoral_list')
        ->key(array(
          'nid' => $nid,
          'uid' => $record->uid,
        ))
        ->fields(array(
          'nid' => $nid,
          'uid' => $record->uid,
        ))
        ->execute();
    }

    drupal_set_message(t('All users with %role role added to electoral list.', array('%role' => $roles[$add_role])));
  }
}

/**
 * Validate changes to the electoral list.
 */
function advpoll_electoral_list_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Clear electoral list')) {
    if (user_access('administer polls')) {
      $nid = $form_state['values']['nid'];
      $items_deleted = db_delete('advpoll_electoral_list')
        ->condition('nid', $nid)
        ->execute();

      drupal_set_message(t('@items cleared from the Electoral list.', array('@items' => $items_deleted)));
      return;
    }
  }

  $add_user = $form_state['values']['electoral_list']['add_user'];
  if ($add_user) {
    // Check that the user exists
    $result = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $add_user));
    if (!$result) {
      form_set_error('electoral_list][add_user', t('User %user does not exist.', array('%user' => $add_user)));
      return FALSE;
    }
  }
}

/**
 * Remove an individual voter from the electoral list.
 */
function advpoll_remove_voter() {
  $nid = arg(1);
  $uid = arg(3);

  if (is_numeric($nid) && is_numeric($uid)) {
    if ($uid && $node = node_load($nid)) {
      $user = db_query('SELECT name FROM {users} WHERE uid = :uid', array(':uid' => $uid))->fetchField();
      if ($user) {
        $and = db_and()->condition('nid', $nid)->condition('uid', $uid);
        $items_deleted = db_delete('advpoll_electoral_list')
          ->condition($and)
          ->execute();
        drupal_set_message(t('%user removed from the electoral list.', array('%user' => $user)));
      } else {
        drupal_set_message(t('No user found with a uid of %uid.', array('%uid' => $uid)));
      }
    }
  }
  drupal_goto('node/' . $node->nid . '/electoral_list');
}

function advpoll_clear_votes_page($node) {

  $output = drupal_get_form('advpoll_clear_votes_confirm_form', $node->nid, $node->title);

  return $output;
}

/**
 * Display a clear votes confirmation form.
 */
function advpoll_clear_votes_confirm_form($form, &$form_state, $nid, $title) {
  $form = array();
  $form['#nid'] = $nid;
  $confirm_question = t('Are you sure you want to clear all votes for %title?', array('%title' => $title));
  $form['question'] = array('#value' => '<h2>' . $confirm_question . '</h2>');
  $form = confirm_form($form, $confirm_question, 'node/' . $nid . '/admin/votes', t('This will delete all votes that have been cast for %title.', array('%title' => $title)), t('Clear all votes'), t('Cancel'));
  unset($form['#theme']);
  return $form;
}

function advpoll_clear_votes_confirm_form_submit($form, &$form_state) {
  $nid = (int) check_plain(arg(1));
  db_delete('votingapi_vote')->condition('entity_id', $nid)->execute();
  drupal_goto('node/' . $nid);
}

/*
 * Menu callback for Write-in node management tab page.
 */

function advpoll_writeins_page($node) {
  $output = '';

  $output .= drupal_render(drupal_get_form('advpoll_promote_writein_form', $node));
  $output .= drupal_render(drupal_get_form('advpoll_merge_writein_form', $node));

  return $output;
}

/*
 * Generates form used to promote write-in options to normal status on the write-in node
 * management tab page.
 */

function advpoll_promote_writein_form($form, &$form_state, $node) {

  $data = advpoll_get_data($node);
  $count = count($data->choices);

  $options = array();

  for ($i = 0; $i < $count; $i++) {
    if ($data->choices[$i]['write_in']) {
      $options[] = strip_tags($data->choices[$i]['choice']);
    }
  }
  $form['#id'] = 'advpoll-promote_writein';

  if ($options) {

    $form['promote_writein'] = array(
      '#type' => 'fieldset',
      '#title' => t('Promote write-ins'),
      '#description' => t('Write-ins can be converted to regular choices. This is useful if users cannot see past write-ins but you want to promote specific write-ins so that they can be seen by users who vote in the future.'),
    );

    $form['promote_writein']['choices'] = array(
      '#type' => 'checkboxes',
      '#title' => '',
      '#options' => drupal_map_assoc($options),
    );

    $form['promote_writein']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Promote'),
    );
  } else {
    $form['promote_writein'] = array(
      '#type' => 'fieldset',
      '#title' => t('Promote write-ins'),
      '#description' => t('There are currently no write-ins available to promote.'),
    );
  }

  return $form;
}

/*
 * Submit function for advpoll_promote_writein_form.
 */

function advpoll_promote_writein_form_submit($form, &$form_state) {
  $selected = array();
  $nid = $form_state['build_info']['args'][0]->nid;

  $options = $form['promote_writein']['choices']['#options'];

  foreach ($options as $option) {
    if ($form['promote_writein']['choices'][$option]['#checked']) {
      $selected[] = strtolower(strip_tags($option));
    }
  }

  if ($selected) {

    $node = node_load($nid);

    if ($node) {
      $nodeChoices = $node->advpoll_choice[$node->language];
      $updatedChoices = array();
      foreach ($nodeChoices as $item) {
        $choice = strtolower(strip_tags($item['choice']));
        if (in_array($choice, $selected)) {
          $updatedChoices[] = array('choice' => $item['choice'], 'write_in' => 0, 'choice_id' => $item['choice_id']);
        } else {
          $updatedChoices[] = array('choice' => $item['choice'], 'write_in' => $item['write_in'], 'choice_id' => $item['choice_id']);
        }
      }
      $node->advpoll_choice[$node->language] = $updatedChoices;
      node_save($node);
    }
  }
}

/*
 * Provides form for merging write-in section of the Write-ins administration
 * node page.
 */

function advpoll_merge_writein_form($form, &$form_state, $node) {

  $choices = $node->advpoll_choice[$node->language];
  $count = count($choices);

  $options = array();
  $all = array();

  $form['#id'] = 'advpoll-merge_writein';
  for ($i = 0; $i < $count; $i++) {
    if ($choices[$i]['write_in']) {
      $options[] = strip_tags($choices[$i]['choice']);
    } else {
      $all[] = strip_tags($choices[$i]['choice']);
    }
  }

  if ($options && $all) {

    $form['merge_writein'] = array(
      '#type' => 'fieldset',
      '#title' => t('Merge write-ins'),
      '#description' => t('This will delete the write-in and change any votes for it into votes for the selected choice.'),
    );

    $form['merge_writein']['choices'] = array(
      '#type' => 'select',
      '#title' => t('Merge'),
      '#options' => drupal_map_assoc($options),
    );
    $form['merge_writein']['all_choices'] = array(
      '#type' => 'select',
      '#title' => t('into'),
      '#options' => drupal_map_assoc($all),
    );
    $form['merge_writein']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Merge'),
    );
  } else {
    $form['merge_writein'] = array(
      '#type' => 'fieldset',
      '#title' => t('Merge write-ins'),
      '#description' => t('There are currently no write-ins to promote.'),
    );
  }

  return $form;
}

/*
 * Submit function for merging write-in votes.
 * 
 */

function advpoll_merge_writein_form_submit($form, &$form_state) {
  $lang = $form_state['build_info']['args'][0]->language;
  $mergeFrom = strtolower($form['merge_writein']['choices']['#value']);
  $mergeTo = strtolower($form['merge_writein']['all_choices']['#value']);
  $nid = $form_state['build_info']['args'][0]->nid;
  $choices = $form_state['build_info']['args'][0]->advpoll_choice[$lang];

  $fromTag = '';
  $toTag = '';
  // match up unique choice id with selected choice
  foreach ($choices as $choice) {
    if (strtolower(strip_tags($choice['choice'])) == $mergeFrom) {
      $fromTag = $choice['choice_id'];
    } else if (strtolower(strip_tags($choice['choice'])) == $mergeTo) {
      $toTag = $choice['choice_id'];
    }
  }

  // no matches, no merge
  if (!$fromTag || !$toTag) {
    drupal_set_message(t('Unidentifed index for index merging.'));
    return;
  }

  // get votes that belonged to the merge from value
  $criteria = array('entity_id' => $nid, 'tag' => $fromTag);
  $mergeFromVotes = votingapi_select_votes($criteria);

  // transfer merge from votes to new vote object and update votingapi
  foreach ($mergeFromVotes as $vote) {
    $votes = array(
      'entity_type' => 'advpoll',
      'entity_id' => $nid,
      'value' => $vote['value'],
      'tag' => $toTag,
      'uid' => $vote['uid'],
      'vote_source' => $vote['vote_source'],
    );

    votingapi_set_votes($votes);
  }
  // remove votes for merge from tag
  votingapi_delete_votes($mergeFromVotes);

  // remove choice from node.
  $node = node_load($nid);

  if ($node) {
    $nodeChoices = $node->advpoll_choice[$node->language];
    $updatedChoices = array();
    foreach ($nodeChoices as $item) {
      $id = $item['choice_id'];
      if ($id != $fromTag) {
        $updatedChoices[] = $item;
      }
    }
    $node->advpoll_choice[$node->language] = $updatedChoices;
    node_save($node);
  }
}

/*
 * Displays results of poll for results page tab.
 */

function advpoll_results_page($node) {
  drupal_add_css(drupal_get_path('module', 'advpoll') . '/css/advpoll.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  $data = advpoll_get_data($node);
  $output = '';

  if ($node) {
    $output = advpoll_display_results($node->nid, $data);
  }

  return $output;
}