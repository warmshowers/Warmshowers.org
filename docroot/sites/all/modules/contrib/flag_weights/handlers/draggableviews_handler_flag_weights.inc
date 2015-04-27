<?php

/**
 * @file
 *   Flag weights implementation for draggableviews.
 */

$plugin = array(
	'label' => 'Flag Weights',
	'handler' => array(
		'class' => 'draggableviews_handler_flag_weights',
	),
);

/*
* Implementation using the Flag Weight module.
*/
class draggableviews_handler_flag_weights extends draggableviews_handler {
	//iStryker: set the weight to node weight.  I don't know why its 'node' weight, it should be entity weight I think
	//This will probably change when flag weight changes its naming convention 
	function get($field, $index) {
		$row = $field->view->result[$index];
		return (isset($row->flag_content_node_weight)) ? $row->flag_content_node_weight : 0;
	}
	
  function set($form_state) {
    global $user; // assume $extra['uid] = '***CURRENT_USER***'
    
    $fv = $form_state['values'];
    $view = $form_state['build_info']['args'][0];
    $relationship = $view->relationship['flag_content_rel'];
    $fid = $relationship->definition['extra'][0]['value'];
    $flag = flag_get_flag($relationship->options['flag']);
    
    // For global flags, use uid 0
    $uid = $flag->global ? 0 : $user->uid;
    
    // Save records to our flags table.
    foreach ($fv['draggableviews'] as $item) {
    	// Make sure id is available.
    	if (!isset($item['id'])) {
    		continue;
    	}
    	flag_weights_set_weight($fid, $flag->content_type, $item['id'], $uid, $item['weight']);
    }
  }
}
