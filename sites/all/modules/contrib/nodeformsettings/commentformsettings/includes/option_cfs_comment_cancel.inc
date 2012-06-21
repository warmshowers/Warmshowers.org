<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_comment_cancel(&$form, &$form_state, $settings, $node) {
	if($settings['cfs_comment_cancel'] == 0 && arg(0) == "comment" && arg(1) == "reply" && is_numeric(arg(2))) {
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
			'#weight' => 151
		);
	}
	
	return $form;
}

