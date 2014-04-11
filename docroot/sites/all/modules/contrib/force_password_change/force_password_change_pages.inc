<?php

// $Id: force_password_change_pages.inc,v 1.1.4.3 2011/01/17 05:50:29 hakulicious Exp $

/**
 * This function creates the settings page. It is a callback function for drupal_get_form()
 */
function force_password_change_settings($form_state)
{
	$form['first_time_login_password_change'] = array
	(
		'#type' => 'checkbox',
		'#title' => t('Force password change on first-time login'),
		'#default_value' => variable_get('first_time_login_password_change', 0),
	);
	$form['force_password_change_login_or_init'] = array
	(
		'#type' => 'radios',
		'#title' => t('Check for pending password change'),
		'#options' => array(0 => t('on every page load'), 1 => t('on login only')),
		'#default_value' => variable_get('force_password_change_login_or_init', 0),
		'#description' => t('Select when to check if a user has a pending password change. Checking on every page load adds a little overhead to every page load, but is the most secure method. Checking on login will only check if a change is pending when the user first logs in, but on sites where users may stay logged in for lengthy periods of time, it may be a long time before they are forced to change their password.'),
	);
	$all_roles = user_roles(TRUE);
	foreach($all_roles as $rid => $role)
	{
		if($rid > 2)
		{
			$user_count = db_result(db_query('SELECT COUNT(uid) FROM {users_roles} WHERE rid = %d', $rid));
			$pending_count = db_result
			(
				db_query
				(
					'SELECT COUNT(u.uid) ' .
					'FROM {users} AS u ' .
					'JOIN {users_roles} AS ur ' .
					'ON ur.uid = u.uid ' .
					'WHERE u.force_password_change = 1 AND u.uid > 0 AND ur.rid = %d',
					$rid
				)
			);
		}
		elseif($rid == 2)
		{
			$user_count = db_result(db_query('SELECT COUNT(uid) FROM {users} WHERE uid > 0'));
			$pending_count = db_result(db_query('SELECT COUNT(uid) FROM {users} WHERE force_password_change = 1 AND uid > 0'));
		}
		$roles[$rid] = $role;
		$stats[$rid] = array
		(
			'user_count' => $user_count,
			'pending_count' => $pending_count
		);
	}
	$form['roles'] = array
	(
		'#type' => 'checkboxes',
		'#options' => $roles,
		'#title' => t('Force users in the following roles to change their password'),
		'#description' => t('Users who are not signed in will be required to change their password immediately upon sign in. Users who are currently signed in will be required to change their password upon their next page click, but after changing their password will be redirected back to the page they were attempting to access.') . '<br />' . t('Note: When you return to this page, no roles will be selected. This is because this setting is a trigger, not a persistant state.'),
	);
	$expiry_data = db_query('SELECT rid, expiry, weight from {force_password_change_expiry} ORDER BY weight ASC, rid ASC');
	$expiry = array();
	while($data = db_fetch_array($expiry_data))
	{
		$expiry[$data['rid']] = array
		(
			'expiry' => $data['expiry'],
			'weight' => $data['weight'],
		);
	}
	$form['expiry_data'] = array
	(
		'#type' => 'value',
		'#value' => $expiry,
	);
	$form['expiry'] = array
	(
		'#type' => 'fieldset',
		'#title' => t('Password Expiry'),
		'#collapsible' => TRUE,
	);

	$form['expiry']['header'] = array
	(
		'#value' => '<p>' . t('Select the amount of time after which you would like users in a role to be automatically forced to change their password. Any users who do not change their password in this amount of time will be forced to change their password on their next login/page load. If you do not wish passwords to expire for a certain role, leave/set the value for that role to zero.') . '</p>',
	);
	$form['expiry']['expire_password'] = array
	(
		'#type' => 'checkbox',
		'#title' => t('Enable password expiration'),
		'#default_value' => variable_get('expire_password', TRUE),
		'#description' => t('When this box is checked, passwords will be set to expire according to the rules set out below. If this box is un-checked, password expiry will be disabled, and the password expiry options below will be ignored.'),
	);
	$form['expiry']['table'] = array
	(
		'#tree' => TRUE,
		'#theme' => 'force_password_change_expiry',
	);
	$time_period = array
	(
		'hours',
		'days',
		'weeks',
		'years',
	);
	$heaviest_weight = 0;
	if(count($expiry))
	{
		foreach($expiry as $rid => $data)
		{
			$form['expiry']['table'][$rid]['role'] = array
			(
				'#value' => $roles[$rid],
			);
			if($data['expiry'] != '' && $data['expiry'])
			{
				$expires = $data['expiry'];
				$year = 60 * 60 * 24 * 365;
				if($expires % $year === 0)
				{
					$time_period_default = 3;
					$time_quantity_default = $expires / $year;
				}
				else
				{
					$week = 60 * 60 * 24 * 7;
					if($expires % $week === 0)
					{
						$time_period_default = 2;
						$time_quantity_default = $expires / $week;
					}
					else
					{
						$day = 60 * 60 * 24;
						if($expires % $day === 0)
						{
							$time_period_default = 1;
							$time_quantity_default = $expires / $day;
						}
						else
						{
							$hour = 60 * 60;
							$time_period_default = 0;
							if($expires % $hour === 0)
							{
								$time_quantity_default = $expires / $hour;
							}
							else
							{
								$time_quantity_default = 0;
							}
						}
					}
				}
			}
			else
			{
				$time_period_default = 0;
				$time_quantity_default = 0;
			}
			$form['expiry']['table'][$rid]['time_quantity'] = array
			(
				'#type' => 'textfield',
				'#default_value' => $time_quantity_default,
			);
			$form['expiry']['table'][$rid]['time_period'] = array
			(
				'#type' => 'select',
				'#options' => $time_period,
				'#default_value' => $time_period_default,
			);
			$form['expiry']['table'][$rid]['weight'] = array
			(
				'#type' => 'weight',
				'#delta' => count($roles),
				'#default_value' => ($data['weight'] != '') ? $data['weight'] : 0,
			);
			$heaviest_weight = ($data['weight'] != '') ? $data['weight'] : 0;
		}
	}
	foreach($roles as $rid => $r)
	{
		if(!isset($form['expiry']['table'][$rid]))
		{
			$heaviest_weight++;
			$form['expiry']['table'][$rid]['role'] = array
			(
				'#value' => $r,
			);
			$form['expiry']['table'][$rid]['time_quantity'] = array
			(
				'#type' => 'textfield',
				'#default_value' => 0,
			);
			$form['expiry']['table'][$rid]['time_period'] = array
			(
				'#type' => 'select',
				'#options' => $time_period,
			);
			$form['expiry']['table'][$rid]['weight'] = array
			(
				'#type' => 'weight',
				'#delta' => count($roles),
				'#default_value' => $heaviest_weight,
			);
		}
	}
	$form['expiry']['footer'] = array
	(
		'#value' => '<p>' . t('Drag and drop the rows to set the priority for password expiry. The roles with the highest priority should be placed at the top of the list. If a user is a member of more than one role, then the time after which their password expires will be determined by whichever of their roles has the highest priority. Any other roles will be ignored. On this note, placing the authenticated user role above any other role will effectively nullify the expiry date for those roles, since all members are authenticated users.') . '</p>',
	);
	$form['stats'] = array
	(
		'#type' => 'value',
		'#value' => $stats,
	);
	$form['change_password_url'] = array
	(
		'#type' => 'textfield',
		'#title' => t('Path to password change URL'),
		'#description' => t('Only change this if you have implemented a module that changes the user password URL from user/[UID]/edit to something else. Use !uid in place of the user id') . '<br />' . t('WARNING: if you set this URL to an invalid URL, you could break your site, so backup your files first, and double check the path before setting it.'),
		'#default_value' => variable_get('change_password_url', 'user/!uid/edit'),
	);

	$form['submit'] = array
	(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	return $form;
}

/**
 * Submit function for the settings form
 */
function force_password_change_settings_submit($form, &$form_state)
{
	$selected_roles = array();
	variable_set('change_password_url', $form_state['values']['change_password_url']);
	variable_set('first_time_login_password_change', $form_state['values']['first_time_login_password_change']);
	variable_set('force_password_change_login_or_init', $form_state['values']['force_password_change_login_or_init']);
	foreach($form_state['values']['roles'] as $role)
	{
		if($role > 2)
		{
			$uids = array();
			$db_uids = db_query
			(
				'SELECT uid ' .
				'FROM {users_roles} ' .
				'WHERE rid = %d',
				$role
			);
			while($uid = db_fetch_array($db_uids))
			{
				$uids[] = $uid['uid'];
			}
			$uid_list = implode(', ', $uids);
			if(strlen($uid_list))
			{
				db_query
				(
					'UPDATE {users} ' .
					'SET force_password_change = 1 ' .
					'WHERE uid IN (%s)',
					$uid_list
				);
				db_query
				(
					'UPDATE {force_password_change_users} ' .
					'SET last_force = %d ' .
					'WHERE uid IN (%s)',
					time(),
					$uid_list
				);
			}
			$selected_roles[] = $role;
		}
		elseif($role == 2)
		{
			db_query
			(
				'UPDATE {users} ' .
				'SET force_password_change = 1 '
			);
			db_query
			(
				'UPDATE {force_password_change_users} ' .
				'SET last_force = %d ',
				time()
			);
			$selected_roles[] = $role;
		}
	}
	if(count($selected_roles))
	{
		db_query
		(
			'UPDATE {force_password_change_roles} ' .
			'SET last_force = %d ' .
			'WHERE rid IN (%s) ',
			time(),
			implode(', ', $selected_roles)
		);
		$roles = user_roles(TRUE);
		$list = '<ul>';
		foreach($selected_roles as $sr)
		{
			$list .= '<li>' . $roles[$sr] . '</li>';
		}
		$list .= '</ul>';
		if($form_state['values']['force_password_change_login_or_init'])
		{
			drupal_set_message(t('Users in the following roles will be required to change their password on their next login: !list', array('!list' => $list)), 'status');
		}
		else
		{
			drupal_set_message(t('Users in the following roles will be required to immediately change their password: !list', array('!list' => $list)), 'status');
		}
	}
	
	$delete_expiries = array();
	variable_set('expire_password', $form_state['values']['expire_password']);
	$add_query = 'INSERT INTO {force_password_change_expiry} (rid, expiry, weight) VALUES ';
	$add_query2 = '';
	$arguments = array();
	foreach($form_state['values']['table'] as $rid => $expiry)
	{
		$multiplier = array(60 * 60, 60 * 60 * 24, 60 * 60 * 24 * 7, 60 * 60 * 24 * 365);
		$time_period = $expiry['time_quantity'] * $multiplier[$expiry['time_period']];
		if(isset($form_state['values']['expiry_data'][$rid])) // update
		{
			db_query('UPDATE {force_password_change_expiry} SET expiry = %d, weight = %d WHERE rid = %d', $time_period, $expiry['weight'], $rid);
		}
		else // create
		{
			if(strlen($add_query2))
			{
				$add_query2 .= ',';
			}
			$add_query2 .= '(%d, %d, %d)';
			$arguments[] = $rid;
			$arguments[] = $time_period;
			$arguments[] = $expiry['weight'];
		}
	}
	if(count($delete_expiries))
	{
		db_query('DELETE FROM {force_password_change_expiry} WHERE rid IN (%s)', implode(',', $delete_expiries));
	}
	if(strlen($add_query2))
	{
		db_query($add_query . $add_query2, $arguments);
	}
}

/**
 * Details page for a particular role, as it relates to the Force Password Change module
 */
function force_password_change_list($rid)
{
	$roles = user_roles(TRUE);
	if($rid < 2 || !$rid)
	{
		drupal_set_title(t("Error: role doesn't exist"));
		return t("Error: role doesn't exist.");
	}
	drupal_set_title(t('Force Password Change Details for !role', array('!role' => $roles[$rid])));
	$output = '<p><strong>' . t('Last forced password change for this role:') . '</strong> ';
	$last_change = db_result(db_query('SELECT last_force FROM {force_password_change_roles} WHERE rid = %d', $rid));
	$output .= ($last_change != '') ? format_date($last_change, 'small') : t('Never');
	$output .= '</p>';
	$pending_header = array
	(
		array('data' => t('Username'), 'field' => 'u.name', 'sort' => 'asc'),
		array('data' => t('Last Forced Password Change'), 'field' => 'fpcu.last_force'),
		array('data' => t('Last Password Change'), 'field' => 'fpcu.last_password_change'),
	);
	
	$pending_query = 'SELECT u.uid, u.name, u.created, fpcu.last_force, fpcu.last_password_change ' .
						'FROM {users} AS u ' .
						'JOIN {force_password_change_users} AS fpcu ' .
						'ON fpcu.uid = u.uid ';
	if($rid > 2)
	{
		$pending_query .= 'JOIN {users_roles} AS ur ' .
							'ON ur.uid = u.uid ';
	}
	$pending_query .= 'WHERE u.force_password_change = 1 ';
	if($rid > 2)
	{
		$pending_query .= ' AND ur.rid = ' . $rid . ' ';
	}
	$pending_query .= tablesort_sql($pending_header);
	$pending_user_data = pager_query
	(
		$pending_query,
		100,
		0
	);
	
	$rows = array();
	$force_password_change_installation_date = variable_get('force_password_change_installation_date', 0);
	$forced_uids = variable_get('force_password_change_first_time_uids', array());
	while($pending_user = db_fetch_array($pending_user_data))
	{
		$row = array();
		$row[] = l($pending_user['name'], 'user/' . $pending_user['uid']);
		if($pending_user['last_force'] != '')
		{
			$last_force = format_date($pending_user['last_force'], 'small');
		}
		elseif(variable_get('first_time_login_password_change', FALSE) && $pending_user['created'] > $force_password_change_installation_date)
		{
			$last_force = t('Their first login');
		}
		else
		{
			if(count($forced_uids) && isset($forced_uids[$pending_user['uid']]))
			{
				$last_force = t('Their first login');
			}
			elseif($pending_user['last_password_change'] != '')
			{
				$last_force = t('Their first login');
			}
			else
			{
				$last_force = t('Never');
			}
		}
		$row[] = $last_force;
		$row[] = ($pending_user['last_password_change'] != '') ? format_date($pending_user['last_password_change'], 'small') : t('Never');
		$rows[] = $row;
	}
	if(!count($rows))
	{
		$row = array();
		$row[] = array('data' => t('No users found'), 'colspan' => 3);
		$rows[] = $row;
	}
	$output .= '<h3>' . t('Users with a pending password change') . '</h3>' . theme('table', $pending_header, $rows) . theme('pager', array(), 100, 0);

	$nonpending_header = array
	(
		array('data' => t('Username'), 'field' => 'u.name', 'sort' => 'asc'),
		array('data' => t('Last Forced Password Change'), 'field' => 'fpcu.last_force'),
		array('data' => t('Last Password Change'), 'field' => 'fpcu.last_password_change'),
	);
	$nonpending_query = 'SELECT u.uid, u.name, u.created, fpcu.last_force, fpcu.last_password_change ' .
						'FROM {users} AS u ' .
						'JOIN {force_password_change_users} AS fpcu ' .
						'ON fpcu.uid = u.uid ';
	if($rid > 2)
	{
		$nonpending_query .= 'JOIN {users_roles} AS ur ' .
							'ON ur.uid = u.uid ';
	}
	$nonpending_query .= 'WHERE u.force_password_change = 0 ';
	if($rid > 2)
	{
		$nonpending_query .= ' AND ur.rid = ' . $rid . ' ';
	}
	$nonpending_query .= tablesort_sql($nonpending_header);
	$nonpending_user_data = pager_query
	(
		$nonpending_query,
		100,
		1
	);
	$rows = array();
	while($nonpending_user = db_fetch_array($nonpending_user_data))
	{
		$row = array();
		$row[] = l($nonpending_user['name'], 'user/' . $nonpending_user['uid']);
		if($nonpending_user['last_force'] != '')
		{
			$last_force = format_date($nonpending_user['last_force'], 'small');
		}
		elseif(variable_get('first_time_login_password_change', FALSE) && $nonpending_user['created'] > $force_password_change_installation_date)
		{
			$last_force = t('Their first login');
		}
		else
		{
			if(count($forced_uids) && isset($forced_uids[$nonpending_user['uid']]))
			{
				$last_force = t('Their first login');
			}
			elseif($nonpending_user['last_password_change'] != '')
			{
				$last_force = t('Their first login');
			}
			else
			{
				$last_force = t('Never');
			}
		}
		$row[] = $last_force;
		$row[] = ($nonpending_user['last_password_change'] != '') ? format_date($nonpending_user['last_password_change'], 'small') : t('Never');
		$rows[] = $row;
	}
	if(!count($rows))
	{
		$row = array();
		$row[] = array('data' => t('No users found'), 'colspan' => 3);
		$rows[] = $row;
	}
	$output .= '<h3>' . t('Users without a pending password change') . '</h3>' . theme('table', $nonpending_header, $rows) . theme('pager', array(), 100, 1);
	
	$output .= drupal_get_form('force_password_change_single_role', $rid);
	
	if($_GET['return'] == 'settings')
	{
		$output .= '<p>' . l(t('Back to settings page'), 'admin/user/force_password_change') . '</p>';
	}
	elseif($_GET['return'] == 'roles')
	{
		$output .= '<p>' . l(t('Back to roles page'), 'admin/user/roles') . '</p>';
	}
	
	return $output;
}

/**
 * Form to force a password change on a single role. Used on the details page
 */
function force_password_change_single_role($form_state, $rid)
{
	$form['force_password_change'] = array
	(
		'#type' => 'checkbox',
		'#title' => t('Force users in this role to change their password'),
		'#description' => t('Users who are not signed in will be required to change their password immediately upon sign in. Users who are currently signed in will be required to change their password upon their next page click, but after changing their password will be redirected back to the page they were attempting to access.') . '<br />' . t('Note: When you return to this page, this box will be unchecked. This is because this setting is a trigger, not a persistant state.'),
		'#weight' => -1,
	);
	$form['rid'] = array
	(
		'#type' => 'value',
		'#value' => $rid,
	);
	$form['submit'] = array
	(
		'#type' => 'submit',
		'#value' => t('Force Password Change'),
	);
	return $form;
}

/**
 * Submission function for the form that forces a password change on a single role

 */
function force_password_change_single_role_submit($form, &$form_state)
{
	$uids = array();
	$db_uids = db_query
	(
		'SELECT uid ' .
		'FROM {users_roles} ' .
		'WHERE rid = %d',
		$form_state['values']['rid']
	);
	while($uid = db_fetch_array($db_uids))
	{
		$uids[] = $uid['uid'];
	}
	$uid_list = implode(', ', $uids);
	if(strlen($uid_list))
	{
		db_query
		(
			'UPDATE {users} ' .
			'SET force_password_change = 1 ' .
			'WHERE uid IN (%s)',
			$uid_list
		);
		db_query
		(
			'UPDATE {force_password_change_users} ' .
			'SET last_force = %d ' .
			'WHERE uid IN (%s)',
			time(),
			$uid_list
		);
	}
	db_query
	(
		'UPDATE {force_password_change_roles} ' .
		'SET last_force = %d ' .
		'WHERE rid = %d ',
		time(),
		$form_state['values']['rid']
	);
	drupal_set_message(t('Users in this role will be required to immediately change their password'));	
}

/**
 * Theme function that effectively rebuilds the add new role form (Administer -> User management -> Roles)
 * This function adds a link from each role the force password change details page for that role
 */
function theme_force_password_change_new_role_form($form)
{
	$header = array(t('Name'), array('data' => t('Operations'), 'colspan' => 3));
	foreach (user_roles() as $rid => $name)
	{
		$edit_permissions = l(t('edit permissions'), 'admin/user/permissions/'. $rid);
		if (!in_array($rid, array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID)))
		{
			$rows[] = array($name, l(t('edit role'), 'admin/user/roles/edit/'. $rid), $edit_permissions, l(t('Force Password Change'), 'force_password_change/list/' . $rid, array('query' => 'return=roles')));
		}
		else
		{
			if($rid == DRUPAL_AUTHENTICATED_RID)
			{
				$rows[] = array($name, t('locked'), $edit_permissions, l(t('Force Password Change'), 'force_password_change/list/' . $rid, array('query' => 'return=roles')));
			}
			else
			{
				$rows[] = array($name, t('locked'), $edit_permissions, '---');
			}
		}
	}
	$rows[] = array(drupal_render($form['name']), array('data' => drupal_render($form['submit']), 'colspan' => 3));
	
	$output = drupal_render($form);
	$output .= theme('table', $header, $rows);
	
	return $output;
}

/**
 * Theme function for the settings page. This is used to place the stats for a role beside the checkbox, without having to 
 * include the stats in the label
 */
function theme_force_password_change_settings($form)
{
	$path = drupal_get_path('module', 'force_password_change');
	drupal_add_css($path . '/css/force_password_change_settings.css', 'module', 'all', FALSE);
	$output = drupal_render($form['first_time_login_password_change']);
	$temp_output .= '<div id="force_password_roles">';
	foreach(element_children($form['roles']) as $rid)
	{
		$temp_output .= drupal_render($form['roles'][$rid]);
		$temp_output .= '<p>(' . t('Users in role: !user_count | Users with pending forced password change: !pending_count | !details', array('!user_count' => $form['stats']['#value'][$rid]['user_count'], '!pending_count' => $form['stats']['#value'][$rid]['pending_count'], '!details' => l(t('Details'), 'force_password_change/list/' . $rid, array('query' => 'return=settings')))) . ')</p>';
	}
	$temp_output .= '</div>';
	$output .= drupal_render($form['roles']) . $temp_output;
	return $output . drupal_render($form);
}

/**
 * Theme function for password expiry on the settings page. This is used add the tabledrag for the expiry data
 */
function theme_force_password_change_expiry($form)
{
	drupal_add_tabledrag('password_expiry_table', 'order', 'sibling', 'weight-group');
	$header = array(t('Role'), t('Expire password after:'), t('Weight'));
	$rows = array();
	foreach(element_children($form) as $key)
	{
		$element = &$form[$key];
		$element['weight']['#attributes']['class'] = 'weight-group';
		$row = array();
		$row[] = drupal_render($element['role']);
		$row[] = drupal_render($element['time_quantity']) . drupal_render($element['time_period']);
		$row[] = drupal_render($element['weight']);
		$rows[] = array('data' => $row, 'class' => 'draggable');
	}
	$output = theme('table', $header, $rows, array('id' => 'password_expiry_table'));
	return $output . drupal_render($form);
}