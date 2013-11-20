<?php

function _commentformsettings_settings_form(&$form, $settings) {
	// The module is going to work with or without ctools.
	if(module_exists("ctools")) {
		ctools_include('dependent');
	}
	
	$defaults = commentformsettings_elements_default();
	$options = array(
		0 => t("Enabled"),
		1 => t("Disabled"),
	);
	
	$form['commentformsettings'] = array(
		'#type' => 'fieldset',
		'#title' => t("Comment form settings"),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#group' => 'commentformsettings',
		'#prefix' => '<div id="comment-choices">',
		'#suffix' => '</div>',
	);
	
	$form['commentformsettings']['cfs_author'] = array(
		'#type' => 'radios',
		'#title' => t("Author name"),
		'#options' => $options,
		'#default_value' => isset($settings['cfs_author']) ? $settings['cfs_author'] : $defaults['cfs_author'],
		'#description' => t("Hide the author name input field on the comments form"),
	);
	
	// Option to override the author of a comment
	$form['commentformsettings']['cfs_newadmin'] = array(
		'#type' => 'radios',
		'#title' => t("Allow to override the author of a comment"),
		'#options' => $options,
		'#default_value' => isset($settings['cfs_newadmin']) ? $settings['cfs_newadmin'] : $defaults['cfs_newadmin'],
		'#description' => t("Users with the <em>@perm</em> permission will be able to override the date and author fields.", array('@perm' => 'administer comments'))
	);
	
	$form['commentformsettings']['cfs_preview'] = array(
		'#type' => 'radios',
		'#title' => t("Preview button"),
		'#options' => $options,
		'#default_value' => isset($settings['cfs_preview']) ? $settings['cfs_preview'] : $defaults['cfs_preview'],
		'#description' => t("Hide the Preview button on the comments form. Important: If you set the Preview button to <em>Required</em> you need to show the button here"),
	);
	$form['commentformsettings']['cfs_title'] = array(
		'#type' => 'radios',
		'#title' => t("'Comment' Title"),
		'#options' => $options,
		'#default_value' => isset($settings['cfs_title']) ? $settings['cfs_title'] : $defaults['cfs_title'],
		'#description' => t("Hide the word 'Comment' on the comments form"),
	);
	$form['commentformsettings']['cfs_inputformat'] = array(
		'#type' => 'radios',
		'#title' => t("Input Format Fieldset"),
		'#options' => $options,
		'#default_value' => isset($settings['cfs_inputformat']) ? $settings['cfs_inputformat'] : $defaults['cfs_inputformat'],
		'#description' => t("Enable or disable the Input Format fieldset on comments form"),
	);
	$form['commentformsettings']['cfs_size'] = array(
		'#type' => 'textfield',
		'#title' => t("Textarea size"),
		'#default_value' => isset($settings['cfs_size']) ? $settings['cfs_size'] : $defaults['cfs_size'],
		'#description' => t("Amount of rows to show in the textarea. This setting is not compatible with WYSIWYG editors"),
	);
	$form['commentformsettings']['cfs_submit'] = array(
		'#type' => 'textfield',
		'#title' => t("Submit button title"),
		'#default_value' => isset($settings['cfs_submit']) ? $settings['cfs_submit'] : $defaults['cfs_submit'],
		'#description' => t("Leave the field empty to show de default value"),
	);
	
	$form['commentformsettings']['cfs_pnc'] = array(
		'#tree' => TRUE,
	);
	$form['commentformsettings']['cfs_pnc']['cfs_post_new_comment'] = array(
		'#type' => 'radios',
		'#title' => t("'Post new comment' behaviour"),
		'#options' => array(
			'0' => t('Leave as default'),
			'1' => t("Hide it"),
			'2' => t("Change it"),
		),
		'#description' => t("Drupal shows a legend above the comments, decide what to do with it"),
		'#default_value' => isset($settings['cfs_pnc']['cfs_post_new_comment']) ? $settings['cfs_pnc']['cfs_post_new_comment'] : $defaults['cfs_pnc']['cfs_post_new_comment'],
	);
	$form['commentformsettings']['cfs_pnc']['cfs_post_new_comment_value'] = array(
		'#type' => 'textfield',
		'#title' => t("Change 'Post new comment' value"),
		'#default_value' => isset($settings['cfs_pnc']['cfs_post_new_comment_value']) ? $settings['cfs_pnc']['cfs_post_new_comment_value'] : $defaults['cfs_pnc']['cfs_post_new_comment_value'],
		'#process' => array('ctools_dependent_process'), 
		'#dependency' => array('radio:cfs_pnc[cfs_post_new_comment]' => array(2)), 
	);
	$form['commentformsettings']['cfs_pnc']['cfs_post_new_comment_tag'] = array(
		'#type' => 'select',
		'#title' => t("Change 'Post new comment' tag"),
		'#options' => array(
			'h1' => 'h1',
			'h2' => 'h2',
			'h3' => 'h3',
			'h4' => 'h4',
			'h5' => 'h5',
			'h6' => 'h6',
		),
		'#default_value' => isset($settings['cfs_pnc']['cfs_post_new_comment_tag']) ? $settings['cfs_pnc']['cfs_post_new_comment_tag'] : $defaults['cfs_pnc']['cfs_post_new_comment_tag'],
		'#process' => array('ctools_dependent_process'), 
		'#dependency' => array('radio:cfs_pnc[cfs_post_new_comment]' => array(2)),
		'#description' => t("HTML tag to show"),
	);
	
	// Anonymous comments settings
	// If anonymous users have permissions to comment show the settings
	if(user_access('post comments', drupal_anonymous_user())) {
		drupal_add_js(drupal_get_path("module", "commentformsettings") . "/commentformsettings.js");
		$form['commentformsettings']['anonymous'] = array(
			'#prefix' => '<div id="anonymous-choices">',
			'#suffix' => '</div>',
		);
		$form['commentformsettings']['anonymous']['cfs_anonymousname'] = array(
			'#type' => 'radios',
			'#title' => t("Anonymous comment Name field"),
			'#options' => $options,
			'#default_value' => isset($settings['cfs_anonymousname']) ? $settings['cfs_anonymousname'] : $defaults['cfs_anonymousname'],
			'#description' => t("If disabled, the Comment Form for anonymous users will not show the Name field. You should use this with caution, because you might have comments without knowing who posted them"),
		);
		$form['commentformsettings']['anonymous']['cfs_anonymousmail'] = array(
			'#type' => 'radios',
			'#title' => t("Anonymous comment E-mail field"),
			'#options' => $options,
			'#default_value' => isset($settings['cfs_anonymousmail']) ? $settings['cfs_anonymousmail'] : $defaults['cfs_anonymousmail'],
			'#description' => t("If disabled, the Comment Form for anonymous users will not show the E-mail field."),
		);
		$form['commentformsettings']['anonymous']['cfs_anonymoushomepage'] = array(
			'#type' => 'radios',
			'#title' => t("Anonymous comment Homepage field"),
			'#options' => $options,
			'#default_value' => isset($settings['cfs_anonymoushomepage']) ? $settings['cfs_anonymoushomepage'] : $defaults['cfs_anonymoushomepage'],
			'#description' => t("If disabled, the Comment Form for anonymous users will not show the Homepage field."),
		);	
	}
	
	// Cancel Button
	$form['commentformsettings']['cfs_comment_cancel'] = array(
		'#type' => 'radios',
		'#title' => t("Comment cancel link"),
		'#options' => $options,
		'#default_value' => isset($settings['cfs_comment_cancel']) ? $settings['cfs_comment_cancel'] : $defaults['cfs_comment_cancel'],
		'#description' => t("If comment submission form is set to display on separate page, active a cancel link. This option will not work if the comment submission form is set to display below post or comments"),
	);	
	
	return $form;
}
