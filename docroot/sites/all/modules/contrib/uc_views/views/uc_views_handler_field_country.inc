<?php

/**
 * @file
 * Views handler: Return country name for country id
 */

/**
 * Return a country name for country id
 */
class uc_views_handler_field_country extends views_handler_field {
  function render($values) {
    return uc_country_get_by_id($values->{$this->field_alias});
  }
}
