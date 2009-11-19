<?php
// $Id: deadwood.schemaapi.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Generate version upgrade code from 5.x to 6.x.
 *
 * The functions in this file match up with the topics in the roadmaps at
 * http://drupal.org/node/146843 and http://drupal.org/node/114774.
 *
 * The functions herein assume the MySQL database is being used. This affects
 * the type names for table fields. If using a different database, then add
 * an a require_once statement to the database.mysql-common.inc file.
 *
 * Copyright 2008 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Find the module name from a function block.
 *
 * TODO Move this to coversions.inc.
 *
 * @param string $function The function block.
 * @return string The module name.
 */
function deadwood_get_module_name($function, $hook) {
  // Get the module name
  $pattern = '/^function\s*(\w+)_' . $hook . '\s*\(/m';
  if (!preg_match($pattern, $function, $matches)) {
    return;
  }
  return isset($matches[1]) ? $matches[1] : 'module-name';
}

/*
 * Read install hook
 * Parse function name for the module name
 * Parse 'mysqli' case block
 * Write schema hook function
 * Read install hook
 * Replace table creation contents with
 *   // Create tables.
 *   drupal_install_schema('module-name');
 * Read uninstall hook
 * Replace table destruction contents with
 *   // Remove tables.
 *   drupal_uninstall_schema('module-name');
 * Remove any hook_update_N functions
 */

/**
 * Write the new hook_schema() function.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_schema(&$file) {
  $hook = 'install';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $module_name = deadwood_get_module_name($cur, $hook);

  // Check for 'mysqli' case block.
  $pattern = '/case\s*\'mysql[i|]\'\s*:\s*(\w.*?)break;/s';
  if (!preg_match($pattern, $cur, $matches)) {
    return;
  }
  $queries = $matches[1];
  // Use array_shift to eliminate the first element which is blank.
  $queries = explode('db_query', $matches[1]);
  if ($queries[0] == '') {
    array_shift($queries);
  }

  $schema = '';
  $other = array('primary', 'unique', 'index', 'key');
  foreach ($queries as $query) {
    // If a 'mysql' case follows a 'mysqli' case, then the regex returns the
    // 'mysql' line as part of the stuff to be parsed.
    if (!preg_match('/{(.*)}/', $query, $matches)) {
      continue;
    }
    $table_name = $matches[1];

    $schema .= "  \$schema['" . $table_name . "'] = array(\n";
    $schema .= "    'description' => t('TODO'),\n";
    $schema .= "    'fields' => array(\n";

    $fields = explode("\n", $query);
    array_shift($fields); // Remove the create table line.

    // Initialize the index variables.
    $primary = '';
    $uniques = array();
    $indexes = array();

    // Loop on the fields of this table.
    $done = FALSE;
    foreach ($fields as $field) {
      if ($done) {
        continue;
      }
      $field = trim($field);
      // With MySQL definitions, remove blank lines and the last line of table definition.
      if ($field == '' || strpos($field, ')') === 0) {
        $done = TRUE;
        continue;
      }
      $tokens = explode(' ', $field);
      if (count($tokens) < 2) {
        drupal_set_message('Schema field is not defined', 'warning');
        continue;
      }
      // Handle the non-field items.
      if (in_array(strtolower($tokens[0]), $other)) {
        // Get index tokens which may include a multi-field key.
        $tokens = deadwood_get_index_tokens($field);
        while (count($tokens)) {
          $token = strtolower(array_shift($tokens));
          if (!count($tokens)) {
            continue;
          }
          switch ($token) {
            case 'primary' :
              // We can rely on the count because we properly parsed any multi-field keys.
              if (count($tokens) == 2) {
                if (strtolower($tokens[0]) == 'key') {
                  $token = array_shift($tokens);
                }
                else {
                  continue;
                }
              }
              $token = trim(array_shift($tokens), '(),');
              $keys = explode(',', $token); // Handle multi-field keys.
              $keys = deadwood_wrap_keys($keys);
              $primary = $keys;
              break;
            case 'unique' :
              if (count($tokens) == 3) {
                if (strtolower($tokens[0]) == 'key') {
                  $token = array_shift($tokens);
                }
                else {
                  continue;
                }
              }
              if (count($tokens) == 2) {
                $index = strtolower(array_shift($tokens));
              }
              else {
                $index = trim(strtolower($tokens[0]), '(),');
              }
              $token = trim(array_shift($tokens), '(),');
              $keys = explode(',', $token); // Handle multi-field keys.
              $keys = deadwood_wrap_keys($keys);
              $uniques[$index] = $keys;
              break;
            case 'index' :
              if (count($tokens) == 3) {
                if (strtolower($tokens[0]) == 'key') {
                  $token = array_shift($tokens);
                }
                else {
                  continue;
                }
              }
            case 'key' :
              if (count($tokens) == 2) {
                $index = strtolower(array_shift($tokens));
              }
              else {
                $index = trim(strtolower($tokens[0]), '(),');
              }
              $token = trim(array_shift($tokens), '(),');
              $keys = explode(',', $token); // Handle multi-field keys.
              $keys = deadwood_wrap_keys($keys);
              $indexes[$index] = $keys;
              break;
          }
        }
      }
      else {
        // Parse a field definition.
        $name = array_shift($tokens);
        list($type, $size, $precision, $scale, $length) = deadwood_get_db_type(array_shift($tokens));
        $unsigned = FALSE;
        $null = TRUE;
        $default = '';
        while (count($tokens)) {
          $token = trim(strtolower(array_shift($tokens)), ',');
          switch ($token) {
            case 'unsigned' :
              $unsigned = TRUE;
              break;
            case 'not' :
              if (trim(strtolower($tokens[0]), ',') == 'null') {
                $null = FALSE;
                $token = array_shift($tokens);
              }
              break;
            case 'default' :
              if (count($tokens[0])) {
                $default = trim($tokens[0], ',');
                deadwood_get_default($type, $default, $null);
                $token = array_shift($tokens);
              }
              break;
            case 'auto_increment' :
              $type = 'serial'; // Check that type is int?
              break;
            // Keys and indexes could be defined on a field. Do we have any?
//            case 'primary' :
//              break;
          }
        }
        // Write this field to schema.
        $schema .= "      '$name' => array(\n";
        $schema .= "        'description' => t('TODO'),\n";
        $schema .= "        'type' => '$type',\n";
        if ($size != '' && $size != 'normal') {
          $schema .= "        'size' => '$size',\n";
        }
        if ($precision != 0) {
          $schema .= "        'precision' => $precision,\n";
          $schema .= "        'scale' => $scale,\n";
        }
        if ($length != 0) {
          $schema .= "        'length' => $length,\n";
        }
        if ($unsigned) {
          $schema .= "        'unsigned' => $unsigned,\n";
        }
        $schema .= "        'not null' => " . ($null ? 'FALSE' : 'TRUE') . ",\n";
        if ($default != '') {
          $schema .= "        'default' => $default,\n";
        }
        $schema .= "      ),\n"; // Close the array for this field.
      }
    }
    $schema .= "    ),\n"; // Close the fields array.

    // Write the indexes and keys.
    if (count($indexes)) {
      $schema .= "    'indexes' => array(\n";
      foreach ($indexes as $index => $keys) {
        $schema .= "      '$index' => array($keys),\n";
      }
      $schema .= "    ),\n";
    }
    if (count($uniques)) {
      $schema .= "    'unique keys' => array(\n";
      foreach ($uniques as $index => $keys) {
        $schema .= "      '$index' => array($keys),\n";
      }
      $schema .= "    ),\n";
    }
    if ($primary != '') {
      $schema .= "    'primary key' => array($primary),\n";
    }
    $schema .= "  );\n\n"; // Close the array for this table.
  }
  $schema .= "  return \$schema;\n";

  $msg =
"/**
 * Implementation of hook_schema().
 */
function " . $module_name . "_schema() {\n" .
$schema .
"}";

  $from = array();
  $to = array();
  // Find the Id line in the file.
  $from[] = '/^(\/\/ \$Id.*\$\s*$)/m';
  $to[] = "$1\n$msg\n";

  $cur = $file;
  $new = $cur;
  $hook = 'hook_schema';

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

function deadwood_get_index_tokens($field) {
  $keys = substr($field, strpos($field, '(')); // Could strip the '()' now
  $rest = substr($field, 0, strpos($field, '(') - 1);
  $tokens = explode(' ', $rest);
  $tokens = array_merge($tokens, array($keys));
  return $tokens;
}

/**
 * Determine the schema API type of a database field.
 *
 * @param string $type The MySQL field type.
 * @return array containing field type and parameters
 *    array($type, $size, $precision, $scale, $length).
 */
function deadwood_get_db_type($type) {
  static $db_types;

  if (!isset($db_types)) {
    $db_types = array_flip(db_type_map());
  }

  // Extract length, precision and scale parameters.
  // Examples of $type = 'DECIMAL(10,2)' or 'VARCHAR(255)' or 'TEXT(64)'
  // Trim the type value in case it is the only parameter for the field.
  // Example: found a file with 'explanation TEXT,' as a field.
  $parts = explode('(', trim($type, ','));
  $type = strtoupper(array_shift($parts));

  $precision = 0;
  $scale = 0;
  $length = 0;

  if (count($parts) > 0) {
    $parts = explode(',', $parts[0]);
    $parts[count($parts) - 1] = trim($parts[count($parts) - 1], ')');

    switch ($type) {
      case 'DECIMAL' :
        $precision = $parts[0];
        $scale = count($parts > 1) ? $parts[1] : 0;
        break;
      case 'TEXT' :
        $length = $parts[0];
        break;
      case 'VARCHAR' :
        $length = $parts[0];
        break;
    }
  }

  if (array_key_exists($type, $db_types)) {
    list($type, $size) = explode(':', $db_types[$type]);
    return array($type, $size, $precision, $scale, $length);
  }
  return array('unknown', '', 0, 0, 0);
}

/**
 * Clean the default value of any quotation marks if not a text field.
 *
 * @param unknown_type $type
 * @param unknown_type $default
 */
function deadwood_get_default($type, &$default, &$null) {
  switch ($type) {
    case 'text' :
    case 'varchar' :
      break;
    default :
      $default = trim($default, "'\"");
  }
  // Conform to schema module report where 'not null' => FALSE.
  if ($default == 'NULL') {
    $null = TRUE;
    $default = '';
  }
  else {
    $null = 0; // Using FALSE, $null = null?
  }
}

/**
 * Get the index key fields wrapped in quotes and separated by commas.
 *
 * @param array $keys Array of index names and key fields.
 * @return string Index key fields wrapped in quotes and separated by commas.
 */
function deadwood_wrap_keys($keys) {
  foreach ($keys as $key => $value) {
    $value = trim($value);
    $keys[$key] = "'$value'";
  }
  return implode(', ', $keys);
}

/**
 * Update the hook_install() function.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_install(&$file) {
  $hook = 'install';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $module_name = deadwood_get_module_name($cur, $hook);
  $hook = 'hook_install';

  $msg =
"  // Create tables.
  drupal_install_schema('" . $module_name . "');";

  $from = array();
  $to = array();
  // Replace the case statement.
  $from[] = '/^\s*switch\s*\(\$GLOBALS\s*.*?break;\s*}\n/ms';
  $to[] = "$msg\n";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Update the hook_uninstall() function.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_uninstall(&$file) {
  $hook = 'uninstall';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $module_name = deadwood_get_module_name($cur, $hook);
  $hook = 'hook_uninstall';

  $msg =
"  // Remove tables.
  drupal_uninstall_schema('" . $module_name . "');";

  $from = array();
  $to = array();
  // Delete drop table statements.
  $from[] = '/(^\s*db_query\((\'|\")DROP TABLE.*\n)/m'; // May have double or single quote.
  $to[] = "";
  // Add new schema API statement.
  $from[] = '/^(function.*_uninstall.*)$/m';
  $to[] = "$1\n$msg\n";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Delete the hook_upadate_N() functions.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_update(&$file) {
  $hook = 'update_\d+';
  // Process file in chunks.
  $chunks = deadwood_find_hook($hook, $file, TRUE);
  $hook = 'update';
  foreach ($chunks as $chunk) {
    $cur = $chunk;
    $new = '';
    deadwood_save_changes($cur, $new, $file, 'hook_' . $hook . '_N');
  }
}
