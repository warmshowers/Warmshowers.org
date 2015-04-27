<?php

/*
 * @file
 * Advanced Poll Pages Include
 * 
 * All items in this document are responsible for generating the tab (menu local) pages
 * that are used for administering an individual poll.
 */

/*
 * Generates page for administering individual voters on a poll.  Enables administrator
 * to clear all votes. For users that can view votes but not administer them, the button
 * is not displayed.
 * 
 * @param $node
 * An advpoll node
 * @return
 * Markup displayed by menu callback for this page. 
 */

function advpoll_votes_page($node) {
  $data = advpoll_get_data($node);

  $output = t('This table lists all the recorded votes for this poll.');
  if ($data->mode == 'unlimited') {
    $output = t('With unlimited voting, a timestamp is used to identify unique votes.  If it is important to identify users by ID or IP, switch to normal voting mode which will use your Voting API settings to record votes.');
  } elseif ($data->mode == 'cookie') {
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
  $user_obj = NULL;
  $rows = array();
  foreach ($results as $item) {
    $user_id = $item->uid;

    if (!$user_id) {
      $user_id = $item->vote_source;
    } else {
      $user_obj = user_load($user_id);
      if ($user_obj) {
        $user_id = l($user_obj->name, 'user/' . $user_id);
      }
    }

    $rows[] = array(
      'data' => array(
        $user_id,
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
      $rendered_form = drupal_get_form('advpoll_clear_votes_form');
      $output .= drupal_render($rendered_form);
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

/*
 * Clear votes form element for the vote display page.
 */

function advpoll_clear_votes_form($form, &$form_state) {
  $nid = (int) check_plain(arg(1));

  $form['reset'] = array(
    '#value' => t('Clear all votes'),
    '#type' => 'submit',
  );
  $form['#action'] = url('node/' . $nid . '/advpoll/votes/clear');
  return $form;
}

/*
 * Displays contents of electoral list page.

 * @param $node
 * An advpoll node
 * @return
 * Markup displayed by menu callback for this page. 
 */

function advpoll_electoral_list_page($node) {
  $output = '';

  if (user_access('administer polls')) {
    $rendered_form = drupal_get_form('advpoll_electoral_list_form', $node->nid);
    $output .= drupal_render($rendered_form);
  }

  $output .= '<p>' . t('This table lists all the eligible voters for this poll.') . '</p>';

  $header = array();
  $header[] = array('data' => t('Voter') . ' ', 'field' => 'name');
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
 * Form for adding or removing users on the electoral list page.
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

  // Check to see if basic authorized users (2) have permission to vote.  If this is the case, 
  // new roles added after auth was selected in permissions may not show up in list. This check 
  // ensures that all roles get added to the list if auth users are included.
  $auth = db_query("SELECT r.name, 
                      r.rid 
                      FROM {role} r 
                      LEFT JOIN {role_permission} p ON p.rid = r.rid 
                      WHERE p.permission LIKE '%vote on polls%' 
                      AND r.rid = 2 ORDER BY r.name")->fetchField();
  // List all roles with "vote on polls" permission, but don't include anonymous users.
  if ($auth) {
    $result = db_query("SELECT r.name, 
                      r.rid 
                      FROM {role} r 
                      WHERE r.rid <> 1 ORDER BY r.name");
  } else {
    $result = db_query("SELECT r.name, 
                      r.rid 
                      FROM {role} r 
                      LEFT JOIN {role_permission} p ON p.rid = r.rid 
                      WHERE p.permission LIKE '%vote on polls%' 
                      AND r.rid <> 1 ORDER BY r.name");
  }
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
    $uid = db_query('SELECT uid 
                     FROM {users} 
                     WHERE name = :user', array(':user' => $add_user))->fetchField();

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
    $result = db_query("SELECT u.uid 
                        FROM {users} u 
                        LEFT JOIN {users_roles} ur ON u.uid = ur.uid 
                        WHERE ur.rid = :rid", array(':rid' => $add_role));

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
    $result = db_query("SELECT uid 
                        FROM {users} 
                        WHERE name = :name", array(':name' => $add_user));
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
      $user = db_query('SELECT name 
                        FROM {users} 
                        WHERE uid = :uid', array(':uid' => $uid))->fetchField();

      if ($user) {
        db_delete('advpoll_electoral_list')
          ->condition('nid', $nid)
          ->condition('uid', $uid)
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
  db_delete('votingapi_vote')
    ->condition('entity_id', $nid)
    ->condition('entity_type', 'advpoll')
    ->execute();
  drupal_goto('node/' . $nid);
}

/*
 * Display Write-in node management tab page.
 * 
 * @param $node
 * And advpoll node
 */

function advpoll_writeins_page($node) {
  $output = '';

  $rendered_form = drupal_get_form('advpoll_promote_writein_form', $node);
  $output .= drupal_render($rendered_form);
  $rendered_form = drupal_get_form('advpoll_merge_writein_form', $node);
  $output .= drupal_render($rendered_form);

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
      $lang = $node->language;

      if (!isset($node->advpoll_choice[$lang])) {
        $lang = 'und';
      }

      $node_choices = $node->advpoll_choice[$lang];
      $updated_choices = array();
      foreach ($node_choices as $item) {
        $choice = strtolower(strip_tags($item['choice']));
        if (in_array($choice, $selected)) {
          $updated_choices[] = array('choice' => $item['choice'], 'write_in' => 0, 'choice_id' => $item['choice_id']);
        } else {
          $updated_choices[] = array('choice' => $item['choice'], 'write_in' => $item['write_in'], 'choice_id' => $item['choice_id']);
        }
      }
      $node->advpoll_choice[$lang] = $updated_choices;
      node_save($node);
    }
  }
}

/*
 * Provides form for merging write-in section of the Write-ins administration
 * node page.
 */

function advpoll_merge_writein_form($form, &$form_state, $node) {
  $lang = $node->language;

  if (!isset($node->advpoll_choice[$node->language])) {
    $lang = 'und';
  }

  $choices = $node->advpoll_choice[$lang];
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

  if (!isset($form_state['build_info']['args'][0]->advpoll_choice[$lang])) {
    $lang = 'und';
  }

  $merge_from = strtolower($form['merge_writein']['choices']['#value']);
  $merge_to = strtolower($form['merge_writein']['all_choices']['#value']);
  $nid = $form_state['build_info']['args'][0]->nid;
  $choices = $form_state['build_info']['args'][0]->advpoll_choice[$lang];

  $from_tag = '';
  $to_tag = '';
  // match up unique choice id with selected choice
  foreach ($choices as $choice) {
    if (strtolower(strip_tags($choice['choice'])) == $merge_from) {
      $from_tag = $choice['choice_id'];
    } elseif (strtolower(strip_tags($choice['choice'])) == $merge_to) {
      $to_tag = $choice['choice_id'];
    }
  }

  // no matches, no merge
  if (!$from_tag || !$to_tag) {
    drupal_set_message(t('Unidentifed index for index merging.'));
    return;
  }

  // get votes that belonged to the merge from value
  $criteria = array('entity_id' => $nid, 'tag' => $from_tag);
  $merge_fromVotes = votingapi_select_votes($criteria);

  // transfer merge from votes to new vote object and update votingapi
  foreach ($merge_fromVotes as $vote) {
    $votes = array(
      'entity_type' => 'advpoll',
      'entity_id' => $nid,
      'value' => $vote['value'],
      'tag' => $to_tag,
      'uid' => $vote['uid'],
      'vote_source' => $vote['vote_source'],
    );

    votingapi_set_votes($votes);
  }
  // remove votes for merge from tag
  votingapi_delete_votes($merge_fromVotes);

  // remove choice from node.
  $node = node_load($nid);

  if ($node) {
    $lang = $node->language;

    if (!isset($node->advpoll_choice[$node->language])) {
      $lang = 'und';
    }
    $node_choices = $node->advpoll_choice[$lang];
    $updated_choices = array();
    foreach ($node_choices as $item) {
      $id = $item['choice_id'];
      if ($id != $from_tag) {
        $updated_choices[] = $item;
      }
    }
    $node->advpoll_choice[$lang] = $updated_choices;
    node_save($node);
  }
}

/*
 * Display the poll's results for users with appropriate permissions
 * 
 * @param $node
 * And advpoll node
 */

function advpoll_results_page($node) {
  drupal_add_css(drupal_get_path('module', 'advpoll') . '/css/advpoll.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  $data = advpoll_get_data($node);
  $output = '';

  if ($node) {
    $output = advpoll_display_results($node->nid, $data, 1);
  }

  return $output;
}