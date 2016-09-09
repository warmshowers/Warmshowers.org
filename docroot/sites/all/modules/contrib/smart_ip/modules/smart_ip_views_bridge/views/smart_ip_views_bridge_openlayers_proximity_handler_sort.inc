<?php 

class smart_ip_views_bridge_openlayers_proximity_handler_sort extends openlayers_proximity_handler_sort {
  
  /**
   * Basic options for all sort criteria
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    if ($handlers = $this->view->display_handler->get_handlers('filter')) {
      $options = array();
      foreach ($handlers as $name => $handler) {
      	if ($handler->definition['handler'] == 'smart_ip_views_bridge_openlayers_proximity_handler_filter_circle') {
      	  $options[$name] = $handler->definition['group'] . ': ' . $handler->definition['title'] . ' (' . $handler->admin_summary() . ')';
      	}
      }
      $form['location_provider'] = array(
        '#title' => t('Location provider'),
        '#type' => 'select',
        '#options' => $options,
      );
    }
  }  
}
