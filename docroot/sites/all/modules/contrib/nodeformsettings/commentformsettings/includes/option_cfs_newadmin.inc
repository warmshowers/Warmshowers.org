<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_newadmin(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_newadmin'] == 0 && empty($form['cid']['value']) && user_access('administer comments')) {
		global $user;
    // Wrap the fields in a fieldset.
		
		$form['admin'] = array(
			'#type' => 'fieldset',
      '#title' => t('Administration'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -2,
    );

    // Unset the root author fields so they don't conflict.
    unset($form['_author']);
    unset($form['author']);

		// Only allow registered users to be used.
		$form['admin']['author'] = array(
			'#type' => 'textfield',
			'#title' => t('Authored by'),
			'#size' => 30,
			'#maxlength' => 60,
			'#autocomplete_path' => 'user/autocomplete',
			'#default_value' => $user->name,
			'#weight' => -1,
		);

		// Date field.
		$form['admin']['date'] = array(
			'#type' => 'textfield',
			'#parents' => array('date'),
			'#title' => t('Authored on'),
			'#size' => 20,
			'#maxlength' => 25,
			'#default_value' => format_date(time(), 'custom', 'Y-m-d H:i O'),
			'#weight' => -1,
		);

		// Status field.
		$form['admin']['status'] = array(
			'#type' => 'radios',
			'#parents' => array('status'),
			'#title' => t('Status'),
			'#default_value' => 0,
			'#options' => array(t('Published'), t('Not published')),
			'#weight' => -1,
		);
  
	}
	
	return $form;
}

