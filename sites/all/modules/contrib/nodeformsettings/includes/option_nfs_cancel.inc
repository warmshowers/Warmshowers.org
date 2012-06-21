<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_cancel(&$form, &$form_state, $settings, $node) {
	// Cancel is Enabled
	if($settings['nfs_cancel']['nfs_cancel_status'] == 0) {
		// Some behaviour is set
		if(isset($settings['nfs_cancel']['nfs_cancel_behaviour'])) {
			switch($settings['nfs_cancel']['nfs_cancel_behaviour']) {
				case 0:
					// Javascript case
					$form['buttons']['cancel'] = array(
						'#type' => 'markup',
						'#value' => '<a class="form-button form-btn form-button-cancel" rel="nofollow" href="javascript: history.go(-1)" title="Cancel" alt="Cancel">' . t('Cancel') . '</a>',
						'#weight' => 51
					);
					break;
				
				case 1:
					// Drupal destination case
					// Patch from http://drupal.org/node/116939
					// Thanks to rkerr http://drupal.org/user/20129 and quicksketch (http://drupal.org/user/35821)
					// Generate a URL for the cancel link.
					if (!isset($_REQUEST['destination']) || $_REQUEST['destination'] == $_GET['q']) {
						$url['path'] = !empty($node->nid) ? 'node/'. $node->nid : '<front>';
					}
					else {
						// parse url to split it up to its components
						$url = substr($url['path'], 1);
					}
					
					$form['buttons']['cancel'] = array(
						'#type' => 'markup',
						'#value' => l(t('Cancel'), $url['path'], array('query' => $url['query'], 'fragment' => $url['fragment'], 'attributes' => array('class' => 'form-button form-button-cancel', 'rel' => 'nofollow'))),
						'#weight' => 51
					);
					break;
			}
		}
	}
		
	return $form;
}

