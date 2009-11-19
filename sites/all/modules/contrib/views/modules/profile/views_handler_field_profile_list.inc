<?php
// $Id: views_handler_field_profile_list.inc,v 1.1 2008/09/03 19:21:29 merlinofchaos Exp $
/**
 * Field handler display a profile list item.
 */
class views_handler_field_profile_list extends views_handler_field_prerender_list {
  /**
   * Break up our field into a proper list.
   */
  function render($value) {
    $field = $value->{$this->field_alias};
    $this->items[$field] = array();
    foreach (split("[,\n\r]", $field) as $item) {
      if ($item != '' && $item !== NULL) {
        $this->items[$field][] = $item;
      }
    }
    return parent::render($value);
  }
}
