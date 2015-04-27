<?
/*
 * @file
 * Advanced Poll Field Devel Generator
 * 
 * Adds funcitonality for Devel Generator module to create appropriate content 
 * for the Advanced Poll Field compound field values.
 * 
 */
 
// called by Devel Generator when an Advanced Poll node is generated
function advpoll_field_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_advpoll_field_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _advpoll_field_devel_generate($object, $field, $instance, $bundle);
  }
}

// fills compound fields with appropriate values.
function _advpoll_field_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();

  if ($instance['widget']['type'] == 'advpoll_write_in') {
    $object_field['choice_id'] = dechex(time() * rand(5, 50));
    $object_field['choice'] = devel_create_greeking(4);
    $object_field['write_in'] = 0;        
  }
  return $object_field;
}
