<?php

/**
 * Define all settings for nodes
 */
function _nodeformsettings_settings_form(&$form, $settings) {
	// The module is going to work with or without ctools.
	if(module_exists("ctools")) {
		ctools_include('dependent');
	}
	$defaults = nodeformsettings_elements_default();
	$options = array(
		0 => t("Enabled"),
		1 => t("Disabled"),
	);
	
	$form['nodeformsettings'] = array(
		'#type' => 'fieldset',
		'#title' => t("Node form settings"),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#group' => 'nodeformsettings',
	);
	
	// To set height of body textarea (1 for textfield)
	$form['nodeformsettings']['nfs_submission_body_rows'] = array(
		'#title' => t("Body field size"),
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => isset($settings['nfs_submission_body_rows']) ? $settings['nfs_submission_body_rows'] : $defaults['nfs_submission_body_rows'],
		'#description' => t("Amount of rows to determine the height of the body field. Enter 1 to get a textfield instead of a textarea.")
	);
	
	// Hide the split summary button
	$form['nodeformsettings']['nfs_splitsummary'] = array(
		'#title' => t("Split Summary Button"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_splitsummary']) ? $settings['nfs_splitsummary'] : $defaults['nfs_splitsummary'],
	);
	
	// Hide the Input Form Fieldset
	$form['nodeformsettings']['nfs_inputformat'] = array(
		'#title' => t("Input Form Fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_inputformat']) ? $settings['nfs_inputformat'] : $defaults['nfs_inputformat'],
	);
	
	// Hide the Revision log message
	$form['nodeformsettings']['nfs_revisionlog'] = array(
		'#title' => t("Revision log message"),
		'#type' => 'radios',
		'#options' => array(
			'0' => t("Enabled (if revisions are enabled)"),
			'1' => t("Only show when user has <a href=\"!url\">View Revisions permission</a>", array('!url' => url('admin/user/permissions', array('fragment' => 'edit-1-view-revisions-wrapper')))),
			'2' => t("Disabled (for all users)")),
		'#default_value' => isset($settings['nfs_revisionlog']) ? $settings['nfs_revisionlog'] : $defaults['nfs_revisionlog'],
	);
	
	// Hide the Author Information Fieldset
	$form['nodeformsettings']['nfs_author_information'] = array(
		'#title' => t("Author Information fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_author_information']) ? $settings['nfs_author_information'] : $defaults['nfs_author_information'],
	);
	
	// Hide the Path Fieldset
	$form['nodeformsettings']['nfs_path'] = array(
		'#title' => t("Path fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_path']) ? $settings['nfs_path'] : $defaults['nfs_path'],
	);
	
	// Hide the Menu Fieldset
	$form['nodeformsettings']['nfs_menu'] = array(
		'#title' => t("Menu fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_menu']) ? $settings['nfs_menu'] : $defaults['nfs_menu'],
	);
	
	// Hide the Publishing options Fieldset
	$form['nodeformsettings']['nfs_publishingoptions'] = array(
		'#title' => t("Publishing Options fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_publishingoptions']) ? $settings['nfs_publishingoptions'] : $defaults['nfs_publishingoptions'],
	);
	
	// Hide the Comments options Fieldset
	$form['nodeformsettings']['nfs_comments'] = array(
		'#title' => t("Comments Options fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_comments']) ? $settings['nfs_comments'] : $defaults['nfs_comments'],
	);
	
	// Hide the Taxonomy Fieldset
	// Code taken from http://drupal.org/node/739148#comment-3031146 by @goron
	$form['nodeformsettings']['nfs_taxonomy'] = array(
		'#title' => t("Taxonomy fieldset"),
		'#type' => 'radios',
		'#options' => $options,
		'#default_value' => isset($settings['nfs_taxonomy']) ? $settings['nfs_taxonomy'] : $defaults['nfs_taxonomy'],
		'#description' => t("If disabled, it will remove the fieldset and show all the elements in the fieldset as if they are not in a fieldset. Enable it to show the fieldset"),
	);
	
	// Hide the Preview button
	$form['nodeformsettings']['nfs_preview'] = array(
		'#title' => t("Preview Button"),
		'#type' => 'radios',
		'#options' => array(
			0 => t("Enabled"),
			1 => t("Disabled"),
			2 => t("Force preview"),
		),
		'#default_value' => isset($settings['nfs_preview']) ? $settings['nfs_preview'] : $defaults['nfs_preview'],
		'#disabled' => FALSE,
	);
	
	if(variable_get('node_preview', 1)) {
		$form['nodeformsettings']['nfs_preview']['#description'] = t('If you want to disable the Preview button, or Force it, go to !url and change the "Preview Post" option to "Optional"', array('!url' => l(t('Post settings'), 'admin/content/node-settings')));
	}
	
	// Show a Cancel button
	$form['nodeformsettings']['nfs_cancel'] = array(
		'#tree' => TRUE,
	);
	
	$form['nodeformsettings']['nfs_cancel']['nfs_cancel_status'] = array(
		'#title' => t("Cancel Button"),
		'#type' => 'radios',
		'#options' => $options,
		'#description' => t("Enable or disable a Cancel button"),
		'#default_value' => isset($settings['nfs_cancel']['nfs_cancel_status']) ? $settings['nfs_cancel']['nfs_cancel_status'] : $defaults['nfs_cancel']['nfs_cancel_status'],
	);
	
	$form['nodeformsettings']['nfs_cancel']['nfs_cancel_behaviour'] = array(
		'#title' => t("Cancel Button Behaviour"),
		'#type' => 'select',
		'#options' => array(
			0 => t("Back one page using Javascript"),
			1 => t("Using previous destination")
		),
		'#default_value' => isset($settings['nfs_cancel']['nfs_cancel_behaviour']) ? $settings['nfs_cancel']['nfs_cancel_behaviour'] : $defaults['nfs_cancel']['nfs_cancel_behaviour'],
		'#dependency' => array('radio:nfs_cancel[nfs_cancel_status]' => array(0)),
		'#process' => array('ctools_dependent_process'),
	);
	
	// Change the submit button text value
	$form['nodeformsettings']['nfs_submit'] = array(
		'#title' => t("Submit button value"),
		'#type' => 'textfield',
		'#default_value' => isset($settings['nfs_submit']) ? $settings['nfs_submit'] : $defaults['nfs_submit'],
	);
	
	// Option to hide the node title
	$form['nodeformsettings']['nfs_hide_node_title'] = array(
		'#type' => 'radios',
		'#title' => t("Node title"),
		'#options' => array(0 => t("Show the node title"), 1 => t("Hide the node title")),
		'#description' => t("This setting controlls wether or not the node title is displayed when viewing the node. The title field is going to appear on the 'create node' form regardless of these settings. To avoid its display and automatically create nodetitles ou can use the module !url", array('!url' => l(t('Automatic Nodetitles'), 'http://drupal.org/project/auto_nodetitle', $options = array('attributes' => array('target' => '_blank'))))),
		'#default_value' => isset($settings['nfs_hide_node_title']) ? $settings['nfs_hide_node_title'] : $defaults['nfs_hide_node_title'],
	);
	
	// Change the title of the form when creating a node (node/add)
	$form['nodeformsettings']['nfs_title_create'] = array(
		'#title' => t('Page title when creating a node'),
		'#type' => 'textfield',
		'#default_value' => isset($settings['nfs_title_create']) ? $settings['nfs_title_create'] : $defaults['nfs_title_create'],
		'#description' => t('Available variable: !node_type.'),
	);
	
	// Change the title of the form when editing a node (node/edit)
	$form['nodeformsettings']['nfs_title_edit'] = array(
		'#title' => t('Page title when editing a node'),
		'#type' => 'textfield',
		'#default_value' => isset($settings['nfs_title_edit']) ? $settings['nfs_title_edit'] : $defaults['nfs_title_edit'],
		'#description' => t('Available variables: !node_title, !node_type.'),
	);
		
	return $form;
}