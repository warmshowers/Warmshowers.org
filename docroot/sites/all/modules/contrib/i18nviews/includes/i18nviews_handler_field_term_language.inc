<?php

/**
 * Field handler to provide simple renderer that shows term language.
 */
class i18nviews_handler_field_term_language extends views_handler_field_taxonomy {
  function render($values) {
    $languages = locale_language_list();
    $value = isset($languages[$values->{$this->field_alias}]) ? $languages[$values->{$this->field_alias}] : '';
    return $this->render_link($value, $values);
  }
}
