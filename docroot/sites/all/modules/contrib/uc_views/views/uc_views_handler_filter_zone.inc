<?php
/**
 * Filter by zone code
 */
class uc_views_handler_filter_zone extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Zone');
      $result = db_query("SELECT zone_id,zone_name FROM {uc_zones} ORDER BY zone_country_id,zone_name");

      while ($zone = db_fetch_array($result)) {
        $options[$zone['zone_id']] = $zone['zone_name'];
      }
      if (count($options) == 0) {
        $options[] = t('No zones found.');
      }
      $this->value_options = $options;
    }
  }
}

