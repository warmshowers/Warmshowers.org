<?php
/**
 * Filter by country code
 */
class uc_views_handler_filter_country extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Country');
      $result = db_query("SELECT country_id,country_name FROM {uc_countries} WHERE version > 0 ORDER BY country_name");

      while ($country = db_fetch_array($result)) {
        $options[$country['country_id']] = $country['country_name'];
      }
      if (count($options) == 0) {
        $options[] = t('No countries found.');
      }
      $this->value_options = $options;
    }
  }
}

