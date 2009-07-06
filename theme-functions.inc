<?php
/**
 * Provides a wrapper for theme_get_setting()
 * that will return either current or a default value
 * provided by omega_vars('variable_name', 'default_value')
 *
 * @param setting name $setting
 * @param default value $d
 * @return value
 */
function ovars($s, $d){
  if ($s) {
    return $s;
  }
  else {
    return $d;
  }
}
