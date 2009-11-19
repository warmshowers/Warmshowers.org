<?php
// $Id: deadwood.conversions.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Generate version upgrade code from 5.x to 6.x.
 *
 * The functions in this file match up with the 80 topics in the roadmap at
 * http://drupal.org/node/114774. As the topics sometimes overlap, there is
 * a corresponding redundancy in the functions. The hope for the 7.x version
 * of this module is that the 7.x roadmap will be written without any
 * overlap in topics.
 *
 * This includes 6 topics in the menu API roadmap at
 * http://drupal.org/node/103114.
 *
 * Copyright 2008 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

module_load_include('inc', 'deadwood', 'deadwood.formapi');
module_load_include('inc', 'deadwood', 'deadwood.schemaapi');
module_load_include('inc', 'deadwood', 'deadwood.notifies');

/**
 * Convert files in a directory.
 *
 * @param string $dirname Module input directory containing the code files to be updated.
 * @param string $newdirname Module output directory where the converted code files will be written.
 * @param array $params Keyed array with run-time parameters. Should contain
 * these keys:
 *    'extensions' => array() // The file types to convert based on extension
 *    'conversions' => array() // The conversions to apply
 * @param boolean $recursive Indicates whether to recurse the subdirectories of $dirname.
 */
function deadwood_convert_dir($dirname, $newdirname, $params = array(), $recursive = TRUE) {
  global $filename;
  static $ignore = array('.', '..', 'CVS', '.svn');

  // Create an output directory we can write to.
  if (!is_dir($newdirname)) {
    mkdir($newdirname);
  }
  else {
    deadwood_clean_directory($newdirname);
  }

  // Extract the file types to convert based on extension.
  $extensions = array();
  if (isset($params) && count($params) && array_key_exists('extensions', $params)) {
    $extensions = $params['extensions'];
  }

  $dir = opendir($dirname);
  while ($filename = readdir($dir)) {
    if (!in_array($filename, $ignore)) {
      if (is_dir($dirname . '/' . $filename)) {
        $newfilename = $filename;
        // Handle conversion item #79.
        if ($filename == 'po') {
          $newfilename = 'translations';
        }
        if ($recursive) {
          deadwood_convert_dir($dirname . '/' . $filename, $newdirname . '/' . $newfilename, $params, $recursive);
        }
      }
      elseif (in_array(pathinfo($filename, PATHINFO_EXTENSION), $extensions)) {
        copy($dirname . '/' . $filename, $newdirname . '/' . $filename);
        drupal_set_message('Converting the file => ' . $filename);
        deadwood_convert_file($newdirname . '/' . $filename, $params);
      }
      else {
        copy($dirname . '/' . $filename, $newdirname . '/' . $filename);
      }
    }
  }
}

/**
 * Convert a file.
 *
 * @param string $filename The name of the file to convert.
 * @param array $params Keyed array with run-time parameters. Should contain
 * these keys:
 *    'extensions' => array() // The file types to convert based on extension
 *    'conversions' => array() // The conversions to apply
 */
function deadwood_convert_file($filename, $params = array()) {
  if (!file_exists($filename)) {
    return FALSE;
  }

  // Read the file and copy the contents.
  $cur = file_get_contents($filename);
  $new = $cur;

  // Extract the conversions to apply.
  $conversions = array();
  if (isset($params) && count($params) && array_key_exists('conversions', $params)) {
    $conversions = $params['conversions'];
  }

  // Process conversions on file.
  $is_info_file = pathinfo($filename, PATHINFO_EXTENSION) == 'info';
  $is_schema_file = pathinfo($filename, PATHINFO_EXTENSION) == 'install';
  foreach ($conversions as $conversion) {
    if (function_exists($function = 'deadwood_convert_' . $conversion)) {
      $is_info_conversion = strpos($conversion, 'info') === 0;
      $is_schema_conversion = in_array($conversion, array('hook_schema', 'hook_install', 'hook_uninstall', 'hook_update'));
      // Only run info changes on info files
      // Only run install changes on install files
      if ((!$is_info_file && !$is_info_conversion && !$is_schema_file && !$is_schema_conversion) ||
          ($is_info_file && $is_info_conversion) ||
          ($is_schema_file && $is_schema_conversion)) {
        call_user_func_array($function, array(&$new));
      }
    }
  }

  // Write the new file.
  if ($new != $cur) {
    if (file_put_contents($filename, $new) === FALSE) {
      drupal_set_message('File could not be written');
    }
    drupal_set_message('Replaced the file');
  }
}

/**
 * Find the text of a particular function.
 *
 * @param string $hook
 *   By default, the suffix of the function name to find.
 *   Example: passing $hook = 'menu' will find a function whose name ends in '_menu'.
 *   When $hook_is_suffix = FALSE, then $hook is the entire function name to find.
 * @param string $file The file to search.
 * @param boolean $match_all When TRUE, find all functions with $hook in the name.
 * @param boolean $hook_is_suffix The $hook is only the suffix of the function name.
 * @return string The function text.
 */
function deadwood_find_hook($hook, $file, $match_all = FALSE, $hook_is_suffix = TRUE) {
  // Construct pattern based on function parameters.
  $prefix = $hook_is_suffix ? '\w+_' : '';
  $pattern  = '/^function\s*';
//  $pattern .= $hook_is_suffix ? '\w+_' : '';
  $pattern .= $prefix . $hook . '\s*\(.*?(?=(\/\*\*|^function|\z))/ms';

  if ($match_all) {
    preg_match_all($pattern, $file, $matches, PREG_PATTERN_ORDER);
    // This block should be unnecessary with the changes to pattern above.
    if (!isset($matches[0][0])) {
      // Check to see if the function name exists.
      $pattern = '/^function\s*' . $prefix . $hook . '\s*\(/m';
      preg_match($pattern, $file, $matches);
      if (!isset($matches[0])) {
        return array();
      }
      // Find last function in file.
      $pattern = '/^function\s*' . $prefix . $hook . '.*\z/ms';
      preg_match_all($pattern, $file, $matches, PREG_PATTERN_ORDER);
      drupal_set_message('Primary search failed to find function text for _' . $hook .'. Resorting to secondary pattern to find function.');
    }
    return isset($matches[0]) ? $matches[0] : array();
  }
  else {
    preg_match($pattern, $file, $matches);
    // This block should be unnecessary with the changes to pattern above.
    if (!isset($matches[0])) {
      // Check to see if the function name exists.
      $pattern = '/^function\s*' . $prefix . $hook . '\s*\(/m';
      preg_match($pattern, $file, $matches);
      if (!isset($matches[0])) {
        return '';
      }
      // Find last function in file.
      $pattern = '/^function\s*' . $prefix . $hook . '.*\z/ms';
      preg_match($pattern, $file, $matches);
      drupal_set_message('Primary search failed to find function text for _' . $hook .'. Resorting to secondary pattern to find function.');
    }
    return isset($matches[0]) ? $matches[0] : '';
  }
}

/**
 * Loop on from and to arrays, converting the text of the subject string.
 *
 * @param string $from The pattern to search for.
 * @param string $to The string to replace the pattern with.
 * @param string $subject The string to search and replace.
 */
function deadwood_do_conversions($from, $to, &$subject) {
  for ($i = 0; $i < count($from); $i++) {
    $subject = preg_replace($from[$i], $to[$i], $subject);
  }
}

/**
 * Save the changes back to the file.
 *
 * @param string $cur The string to search for in $file.
 * @param string $new The replacement string.
 * @param string $file The text being replaced (all of or part of a file).
 * @param string $hook The hook being modified.
 */
function deadwood_save_changes($cur, $new, &$file, $hook) {
  if ($new != $cur) {
    $file = str_replace($cur, $new, $file);
    drupal_set_message('Completed conversions for ' . $hook);
  }
}

/**
 * Convert menu function.
 * See: http://drupal.org/node/103114.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_menu(&$file) {
  $hook = 'menu';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;

  $msg1 = "/* TODO\n" .
          "   Non menu code that was placed in hook_menu under the '!\$may_cache' block\n" .
          "   so that it could be run during initialization, should now be moved to hook_init.\n" .
          "   Previously we called hook_init twice, once early in the bootstrap process, second\n" .
          "   just after the bootstrap has finished. The first instance is now called boot\n" .
          "   instead of init.\n" .
          "   \n" .
          "   In Drupal 6, there are now two hooks that can be used by modules to execute code\n" .
          "   at the beginning of a page request. hook_boot() replaces hook_init() in Drupal 5\n" .
          "   and runs on each page request, even for cached pages. hook_init() now only runs\n" .
          "   for non-cached pages and thus can be used for code that was previously placed in\n" .
          "   hook_menu() with \$may_cache = FALSE:\n" .
          "   \n" .
          "   Dynamic menu items under a '!\$may_cache' block can often be simplified\n" .
          "   to remove references to arg(n) and use of '%<function-name>' to check\n" .
          "   conditions. See http://drupal.org/node/103114.\n" .
          "   \n" .
          "   The title and description arguments should not have strings wrapped in t(),\n" .
          "   because translation of these happen in a later stage in the menu system.\n" .
          "*/\n";

  $from = array();
  $to = array();
  // hook_menu() no longer takes any parameters (remove $may_cache).
  $from[] = '/_menu\(\$may_cache/m';
  $to[] = '_menu(';
  $from[] = '/^(.*?if \((!|)\$may_cache\).*)$/m';
  $to[] = "$msg1$1";
  // callback becomes page callback.
  $from[] = '/\'callback\'/m';
  $to[] = '\'page callback\'';
  // callback arguments becomes page arguments.
  $from[] = '/\'callback arguments\'/m';
  $to[] = '\'page arguments\'';
  // access becomes access callback and access arguments.
  $from[] = '/\'access\'\s*=>\s*user_access/m';
  $to[] = '\'access arguments\' => array';
  $from[] = '/\'access\'/m';
  $to[] = '\'access arguments\'';
  $from[] = '/=\s*user_access\s*\(\'/m';
  $to[] = '= array(\'';
  // title and description arguments should not have strings wrapped in t().
//  $from[] = '/\'title\' => t\(\'(.*)\'\)([|,])$/m';
//  $to[] = '\'title\' => \'$1\'$2';
//  $from[] = '/\'description\' => t\(\'(.*)\'\)([|,])$/m';
//  $to[] = '\'description\' => \'$1\'$2';
  // There may be other t() references on these lines. Add a message?
  //  t\((('|)(\w+)('|))\)
//  $from[] = '/(\'title\' =>.*\W)t\(((\'|)([\w ]+)(\'|))\)/';
  $from[] = '/(\'title\' =>.*\W)t\((.*)\)/';
  $to[] = '$1$2';
  $from[] = '/(\'description\' =>.*\W)t\((.*)\)/';
  $to[] = '$1$2';
  // Repeat these as they may occur multiple times on a line (probably a way to do this with one call).
//  $from[] = '/(\'title\' =>.*\W)t\(((\'|)([\w ]+)(\'|))\)/';
  $from[] = '/(\'title\'\s*=>.*\W)t\((.*)\)/';
  $to[] = '$1$2';
  $from[] = '/(\'description\'\s*=>.*)\Wt\((.*)\)/';
  $to[] = '$1$2';
  // Menu index strings with wildcards may be simplified
  $from[] = '/(\'path\'\s*=>.*)arg\(\d+\)(.*)$/m'; // '/(\'path\' =>.*)\..*arg\(\d+\).*\.(.*)$/m';
  $to[] = '$1\'%\'$2';
  // value of path is the new index for $items. Do last due to above search for 'path.'
  // Some code the 'path' to start on the line after 'array('; others on the same line.
  // Same inconsistency exists in the database table statements where some put the first
  // field on the 'CREATE TABLE' line.
  $from[] = '/^(\s*)\$items\[\]\s*=\s*array\s*\(\s*\'path\'\s*=>\s*(.+),\s*(\n\s*)/m';
  $to[] = '$1$items[$2] = array($3';

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert calls to url() and l().
 *
 * See: http://drupal.org/node/114774#url.
 * Adapted from the script at http://drupal.org/files/issues/replace.php_.txt.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_url(&$file) {
  $hook = 'url';
  $msg = "/* TODO\n" .
         "   Please manually fix the parameters on the l() or url() function on the next line.\n" .
         "   Typically, this was not changed because of a function call inside an array call like\n" .
         "   array('title' => t('View user profile.'))." .
         "*/\n";

  $callbacks = array('deadwood_fix_l', 'deadwood_fix_url');
  foreach ($callbacks as $callback) {
    $cur = $file;
    $new = '';

    // Process file in chunks to avoid PCRE bug?
    $pattern = $callback == 'deadwood_fix_l' ? '/(?=(?<!ion )(?<![a-zA-Z_\'\"])l\()/' : '/(?=(?<!ion )(?<![a-zA-Z_\'\"])url\()/';
    $chunks = preg_split($pattern, $cur);

    $search = $callback == 'deadwood_fix_l' ? '@(?<!ion )(?<![a-zA-Z_\'\"])l\((([^()]*|\((?2)?\))+)\)@' : '@(?<!ion )(?<![a-zA-Z_\'\"])url\((([^()]*|\((?2)?\))+)\)@';

    foreach ($chunks as $chunk) {
      $chg = preg_replace_callback($search, $callback, $chunk);
      $new .= $chg != '' ? $chg : $msg . $chunk;
    }

    deadwood_save_changes($cur, $new, $file, $hook);
  }
}

/**
 * Callback function for converting l() references.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_l($matches) {
  $args = deadwwod_get_args($matches[1]);

  // Two arguments, no change.
  if (count($args) <= 2) {
    return $matches[0];
  }

  $options = "array(";
  if (isset($args[2]) && $args[2][1] != 'array()' && $args[2][1] != 'NULL') {
    $options .= "'attributes' => " . $args[2][1] . ', ';
  }
  if (isset($args[3]) && $args[3][1] != "''" && $args[3][1] != '""' && $args[3][1] != 'NULL') {
    $options .= "'query' => " . $args[3][1] . ', ';
  }
  if (isset($args[4]) && $args[4][1] != "''" && $args[4][1] != '""' && $args[4][1] != 'NULL') {
    $options .= "'fragment' => " . $args[4][1] . ', ';
  }
  if (isset($args[5]) && $args[5][1] != "FALSE") {
    $options .= "'absolute' => TRUE, ";
  }
  if (isset($args[6]) && $args[6][1] != "FALSE") {
    $options .= "'html' => TRUE, ";
  }
  $options = rtrim($options, ', ');

  $return = 'l('. $args[0][1] . ', ' . $args[1][1] . ', ' . $options  . '))';
  return $return;
}

/**
 * Callback function for converting url() references.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_url($matches) {
  $args = deadwwod_get_args($matches[1]);

  // One argument, no change.
  if (count($args) <= 1) {
    return $matches[0];
  }

  // Ignore the url() call from l().
  if ($args[1][1] == '$options') return $matches[0];

  $options = "array(";
  if (isset($args[1]) && $args[1][1] != "''" && $args[1][1] != '""' && $args[1][1] != 'NULL') {
    $options .= "'query' => " . $args[1][1] . ', ';
  }
  if (isset($args[2]) && $args[2][1] != "''" && $args[2][1] != '""' && $args[2][1] != 'NULL') {
    $options .= "'fragment' => " . $args[2][1] . ', ';
  }
  if (isset($args[3]) && $args[3][1] != "FALSE") {
    $options .= "'absolute' => TRUE, ";
  }
  $options = rtrim($options, ', ');

  $return = 'url(' . $args[0][1] . ', ' . $options  . '))';
  return $return;
}

/**
 * Helper function for parsing arguments.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwwod_get_args($string) {
  preg_match_all('` *(([^(),]+|\((([^()]*|\((?3)?\))+)\))+) *(,|$)`', $string, $matches, PREG_SET_ORDER);
  return $matches;
}

/**
 * Convert calls to taxonomy_node_get_terms(), taxonomy_node_get_terms_by_vocabulary(),
 * taxonomy_node_save and taxonomy_node_delete().
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_taxonomy(&$file) {
  $hook = 'taxonomy';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  /*
   * TODO Add this text if table name is found.
   * The {term_node} table now has a vid (version id) column, and (tid, nid, vid) is now the primary key.
   * Any direct queries against {term_node} should be checked, and probably modified to use the node
   * revision (vid) field instead of nid.
   *
  */
  // taxonomy_node_get_terms() now takes a full $node object, not just a nid (node id).
  $from[] = '/taxonomy_node_get_terms\s*\(\$(nid|node->nid)/';
  $to[] = 'taxonomy_node_get_terms($node';
//  $from[] = '/taxonomy_node_get_terms\(([^\$node]+)/m';
//  $to[] = '_menu(';
  // taxonomy_node_get_terms_by_vocabulary() now takes a full $node object, not just a nid (node id).
  $from[] = '/taxonomy_node_get_terms_by_vocabulary\s*\(\$(nid|node->nid)/m';
  $to[] = 'taxonomy_node_get_terms_by_vocabulary($node';
  // taxonomy_node_save() now takes a full $node object, not just a nid (node id).
  $from[] = '/taxonomy_node_save\s*\(\$(nid|node->nid)/m';
  $to[] = 'taxonomy_node_save($node';
  // taxonomy_node_delete() now takes a full $node object, not just a nid (node id).
  $from[] = '/taxonomy_node_delete\s*\(\$(nid|node->nid)/m';
  $to[] = 'taxonomy_node_delete($node';
  // taxonomy_node_delete_revision() has been added to delete all taxonomy terms from the current revision of the given $node object.

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert module loads to use new module_load_include().
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_module_load(&$file) {
  $hook = 'module_load';
  $cur = $file;
  $new = $cur;

  $callback = 'deadwood_fix_module_load';
  $functions = array('require_once', 'require', 'include_once', 'include');
  foreach ($functions as $function) {
    // replace function calls using drupal_get_path.
    $search = "/$function\s*(\(|)drupal_get_path\s*\((.+),\s*(.+)\)\s*\.\s*(.+?)(\)|);/";
    $chg = preg_replace_callback($search, $callback, $new);
    $new = $chg != '' ? $chg : $new;
    // replace function calls not using drupal_get_path.
    // This uses an opportunistic 'inc' as extension.
    $search = "/$function\s*(\(|)(.+?)\s*\.\s*(.+?)(\)|);/";
    $chg = preg_replace_callback($search, $callback, $new);
    $new = $chg != '' ? $chg : $new;
  }

  $from = array();
  $to = array();
  // remove '_xx' from function name.
  $from[] = '/module_load_include_xx/';
  $to[] = 'module_load_include';

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Callback function for converting module loads.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_module_load($matches) {
  // If not 5 or 6 arguments, no change.
  if (count($matches) <= 4) {
    return $matches[0];
  }

  $all = trim($matches[count($matches) - 2], "'/);");
  $ext = substr($all, strpos($all, '.') + 1);
  $name = substr($all, 0, strpos($all, '.'));
  $module = $matches[3];
  if (count($matches) == 5) {
    global $filename;
    $module = pathinfo($filename, PATHINFO_FILENAME);
  }
  $new = "module_load_include_xx('$ext', '$module', '$name');";

  return $new;
}

/**
 * Convert calls to hook_form_alter() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_form_alter(&$file) {
  /*
   * TODO These module functions could have added additional parameters from the core signature.
   * We need to append any additional parameters to the end of the required parameters.
   * Should we search for the string '_form_alter' and place a notification message?
   * This might be present if the module called the hook itself.
   * Are there variables inside this function we could/should rename?
   */
  $hook = 'form_alter';
  $pattern = '/(?<!hook)_form_alter\s*(\(.*\))/';
  // No need for preg_match_all as a module should only define this function once?
  if (!preg_match($pattern, $file, $matches)) {
    return;
  }
  $cur = $matches[0];
  $new = $cur;

  $search = $matches[1];
  $replace = '(&$form, &$form_state, $form_id)';
  $new = str_replace($search, $replace, $new);

  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert calls to hook_link_alter() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_link_alter(&$file) {
  /*
   * TODO These module functions could have added additional parameters from the core signature.
   * We need to append any additional parameters to the end of the required parameters.
   * Should we search for the string '_link_alter' and place a notification message?
   * This might be present if the module called the hook itself.
   */
  $hook = 'link_alter';
  $pattern = '/_link_alter\s*(\(.*\))/';
  // No need for preg_match_all as a module should only define this function once?
  if (!preg_match($pattern, $file, $matches)) {
    return;
  }
  $cur = $matches[0];
  $new = $cur;

  $search = $matches[1];
  $replace = '(&$links, $node)';
  $new = str_replace($search, $replace, $new);

  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert calls to hook_profile_alter() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_profile_alter(&$file) {
  /*
   * TODO These module functions could have added additional parameters from the core signature.
   * We need to append any additional parameters to the end of the required parameters.
   * Should we search for the string '_profile_alter' and place a notification message?
   * This might be present if the module called the hook itself.
   */
  $hook = 'profile_alter';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;

  $from = array();
  $to = array();
  // Change function paramters.
  $from[] = '/_profile_alter\s*(\(.*\))/';
  $to[] = '_profile_alter(&$account)';
  // Change variable references.
  $from[] = '/\$fields\[/';
  $to[] = '$account->content[';

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert calls to hook_mail_alter() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_mail_alter(&$file) {
  /*
   * TODO These module functions could have added additional parameters from the core signature.
   * We need to append any additional parameters to the end of the required parameters.
   * Should we search for the string '_mail_alter' and place a notification message?
   * This might be present if the module called the hook itself.
   */
  $hook = 'mail_alter';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;

  $from = array();
  $to = array();
  // Change function paramters.
  $from[] = '/_mail_alter\s*(\(.*\))/';
  $to[] = '_mail_alter(&$message)';
  // Change variable references.
  $from[] = '/([^&])\$(mailkey|to|subject|body|from|headers)/';
  $to[] = '$1$message[\'$2\']';

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Add note about need to register theme functions in hook_theme().
 * Create entries to register theme functions in hook_theme().
 * These will need to be manually combined from all project files.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_theme(&$file) {
  $hook = 'theme';
  $cur = $file;
  $new = $cur;

  global $filename;

  $name = pathinfo($filename, PATHINFO_FILENAME);
  $msg =
"/* TODO Implement the hook_theme registry. Combine all theme registry entries
   into one hook_theme function in each corresponding module file.
function $name" . "_theme() {
  return array(";

  // Find all theme functions and add to hook_theme.
  $search = '/^function\s*theme_(\w+)\s*\((.*?)\s*\)\s*{$/ms';
  preg_match_all($search, $new, $matches, PREG_SET_ORDER);
  $text = $msg . "\n";
  foreach ($matches as $match) {
    $text .= "    '$match[1]' => array(\n";
    $text .= "      'file' => '$filename',\n";
    $text .= "      'arguments' => array(\n";
    $text .= deadwood_get_theme_params($match[2]);
    $text .= "      ),\n";
    $text .= "    ),\n";
  }
  $text .= "  );\n";
  $text .= "}; */\n";

  // Create function body and add theme entries. Write at top of file.
  $from[] = '/^(\/\/ \$Id.*\$\s*$)/m';
  $to[] = "$1\n$text\n";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Callback function for writing theme registry entries.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_get_theme_params($matches) {
  $new = array();
  $params = explode(',', $matches);
  foreach ($params as $param) {
    $tokens = explode('=', $param);
    $new[] = "        '" . trim($tokens[0], '$ ') . "' => " . (isset($tokens[1]) ? trim($tokens[1]) : 'NULL') . ",\n";
  }
  $new = implode("", $new);
  return $new;
}

/**
 * Convert calls to watchdog() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_watchdog(&$file) {
  $hook = 'watchdog';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Parameters of watchdog() changed
  // remove the t(.., array(.., function())) reference inside the watchdog() call
  $from[] = '/(watchdog\(.*),\s*t\((.*array.*?\)\))\)/';
  $to[] = "\$1, $2"; // TODO Are the '\' characters intended or leftover from above???
  // remove the t(.., array()) reference inside the watchdog() call
  $from[] = '/(watchdog\(.*),\s*t\((.*array.*?\))\)/';
  $to[] = "\$1, $2";
  // remove the t() reference inside the watchdog() call
  $from[] = '/(watchdog\(.*),\s*t\((.*?)\)/';
  $to[] = "\$1, $2";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert syntax for .info files.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_info1(&$file) {
  $hook = 'info1';
  $cur = $file;
  $new = $cur;

  // New syntax for .info files
  $search = '/(dependencies)\s*=\s*(.+)/';
  $callback = 'deadwood_fix_dependencies';
  $chg = preg_replace_callback($search, $callback, $new);
  $new = $chg != '' ? $chg : $new;

  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Specify core compatibility in .info files.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_info2(&$file) {
  $hook = 'info2';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Core compatibility now specified in .info files
  $from[] = '/\z/';
  $to[] = "\ncore = 6.x";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Callback function for converting dependencies.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_dependencies($matches) {
  // Two arguments, no change.
  if (count($matches) <= 2) {
    return $matches[0];
  }

  $deps = explode(' ', $matches[2]);
  $new = array();
  foreach ($deps as $dep) {
    $new[] = $matches[1] . '[] = ' . $dep;
  }
  $new = implode("\n", $new);
  return $new;
}

/**
 * Convert calls to cache_set() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_cache1(&$file) {
  $hook = 'cache1';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Changes to cache_set parameter order
  // remove a serialize() reference inside the cache_set() call
  // switch order of parameters 2 and 3
  $from[] = '/cache_set\s*\(\s*(.+),\s*(.+),\s*(serialize\s*\(|)(.+)(\)|)\)/';
  $to[] = "cache_set($1, $4, $2)";
  // remove 'cache' as the table parameter since it is defaulted
  $from[] = '/cache_set\s*\(\s*(.+),\s*(.+),\s*(\'cache\')\)/';
  $to[] = "cache_set($1, $2)";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Remove (un)serialize calls with cache_get() and cache_set().
 * This is not a perfect split on the conversion since we remove
 * the serialize reference in cache1 above.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_cache2(&$file) {
  $hook = 'cache2';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Cache set and get automatically (un)serialize complex data types
  $from[] = '/unserialize\s*\(\s*cache_get/';
  $to[] = "cache_get";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace calls to $_SERVER['REMOTE_ADDR'] with new function.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_ip_address(&$file) {
  $hook = 'ip_address';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // New ip_address() function when working behind proxies
  $from[] = '/\$_SERVER\[\'REMOTE_ADDR\'\]/';
  $to[] = "ip_address()";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to file_check_upload().
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_upload1(&$file) {
  $hook = 'upload1';
  $cur = $file;
  $new = $cur;

  $msg =
"/* TODO Modify the validators array to suit your needs.
   This array is used in the revised file_save_upload */
  \$validators = array(
    'file_validate_is_image' => array(),
    'file_validate_image_resolution' => array('85x85')),
    'file_validate_size' => array(30 * 1024),
  );";

  $from = array();
  $to = array();
  // file_check_upload() merged into file_save_upload()
  // file_save_upload() has new parameter
  $from[] = '/^(.*)(file_check_upload)\s*\((.*)\)/m';
  $to[] = "$msg\n\n$1file_save_upload($2, \$validators)";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to {file_revisions} table.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_upload2(&$file) {
  $hook = 'upload2';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // The {file_revisions} table is now {upload}
  $from[] = '/{file_revisions}/';
  $to[] = "{upload}";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to custom_url_rewrite with new functions.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_custom_url(&$file) {
  $hook = 'custom_url';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // custom_url_rewrite('alias', ..) is now custom_url_rewrite_outbound
  // Change function_exists calls.
  $from[] = "/^(\s*if.*exists)(\('custom_url_rewrite'\))(.*\('alias.*?;)/ms";
  $to[] = "$1('custom_url_rewrite_outbound')$3";
  // Change function calls.
  $from[] = "/(custom_url_rewrite)(\('alias', )/";
  $to[] = "$1_outbound(";
  // custom_url_rewrite('source', ..) is now custom_url_rewrite_inbound
  // Change function_exists calls.
  $from[] = "/^(\s*if.*exists)(\('custom_url_rewrite'\))(.*\('source.*?;)/ms";
  $to[] = "$1('custom_url_rewrite_inbound')$3";
  // Change function calls.
  $from[] = "/(custom_url_rewrite)(\('source', )/";
  $to[] = "$1_inbound(";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace paramters for hook_help.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_help(&$file) {
  $hook = 'help';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $hook = 'hook_help';

  $from = array();
  $to = array();

  // Save the parameter name to use in next search.
  $old_param = '$section';
  $pattern = "/_help\((.+)\)/";
  preg_match($pattern, $cur, $matches);
  if (isset($matches[1])) {
    $old_param = $matches[1];
  }
  // hook_help() parameters are changed
  // Change function parameter signature.
  $from[] = "/_help\((.+)\)/";
  $to[] = "_help(\$path, \$arg)";
  // Change function body.
  $from[] = "/\\$old_param/";
  $to[] = "\$path";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to taxonomy_get_vocabulary with taxonomy_vocabulary_load.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_taxonomy_vocabulary(&$file) {
  $hook = 'taxonomy_vocabulary';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // taxonomy_get_vocabulary() changed to taxonomy_vocabulary_load().
  $from[] = '/taxonomy_get_vocabulary\s*\(/';
  $to[] = 'taxonomy_vocabulary_load(';

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to hook_init with hook_boot.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_init(&$file) {
  $hook = 'hook_init';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // hook_init() changed to hook_boot().
  $from[] = '/hook_init\s*\(/';
  $to[] = 'hook_boot(';
  // Added a message to deadwood_convert_menu about new hook_init.

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to global comment variables with per node type references.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_comment_settings(&$file) {
  $hook = 'comment_settings';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // add node type when setting comment variables.
  $from[] = "/variable_set\s*\('comment(.*)'/";
  $to[] = "variable_set('comment$1_' . \$node->type";
  // add node type when getting comment variables.
  $from[] = "/variable_get\s*\('comment(.*)'/";
  $to[] = "variable_get('comment$1_' . \$node->type";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Comment out references to dangerous_skip_check form property.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_dangerous_skip_check(&$file) {
  $hook = 'dangerous_skip_check';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // add node type when setting comment variables.
  $from[] = "/^(.*'#DANGEROUS_SKIP_CHECK'.*)$/m";
  $to[] = "//$1 // This property has been removed from core.";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to access_control with permissions.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_access_control(&$file) {
  $hook = 'access_control';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // Access control renamed to permissions.
  $from[] = '/(user_access\s*\(.*)administer access control/';
  $to[] = "$1administer permissions";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to locale_refresh_cache with cache_clear_all.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_locale_cache(&$file) {
  $hook = 'locale_cache';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // Access control renamed to permissions.
  $from[] = '/locale_refresh_cache\s*\(\)/';
  $to[] = "cache_clear_all('locale:', 'cache', TRUE)";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace references to admin/logs with admin/reports.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_admin_logs(&$file) {
  $hook = 'admin_logs';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // admin/logs renamed to admin/reports.
  $from[] = '/admin\/logs/';
  $to[] = "admin/reports";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert parameters in references to drupal_mail.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_drupal_mail(&$file) {
  $hook = 'drupal_mail';
  $cur = $file;
  $new = $cur;

  // convert parameters in references to drupal_mail.
  $callback = 'deadwood_fix_drupal_mail';
  $search = "/^((.*)(drupal_mail)\s*\((.+?),\s*(.+?),\s*(.+?),\s*(.+?)(,\s*([^,]*?)|)(,\s*(.*?)|)\))(.*?)$/m";
  $chg = preg_replace_callback($search, $callback, $new);
  $new = $chg != '' ? $chg : $new;

  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Callback function for converting drupal_mail parameters.
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_drupal_mail($matches) {
  // If not 5 arguments, then error.
  if (count($matches) < 5) {
    return $matches[0];
  }

  $cur = $matches[0];
  $beg = $matches[2];
  $function = $matches[3];
  $key = $matches[4];
  $to = $matches[5];
  $subject = $matches[6];
  $body = $matches[7];
  $from = $matches[9];
  $headers = $matches[11];
  $end = $matches[12];

  $msg =
"$beg/* TODO Create a hook_mail(\$key, &\$message, \$params) function to generate
$beg" . "the message body when called by drupal_mail. */
$beg\$account = array(); // Set this as needed
$beg\$language = user_preferred_language(\$account);
$beg\$object = array(); // Replace this as needed
$beg\$context['subject'] = \$subject;
$beg\$context['body'] = \$body;";

  if ($headers != '') {
    $msg .= "\n$beg\$context['headers'] = $headers;";
  }
  $msg .= "\n$beg\$params = array('account' => \$account, 'object' => \$object, 'context' => \$context);";

  global $filename;
  $module = pathinfo($filename, PATHINFO_FILENAME);

  if ($from != '') {
    $new = "//$cur\n$msg\n$beg$function('$module', $key, $to, \$language, \$params, $from);";
  }
  else {
    $new = "//$cur\n$msg\n$beg$function('$module', $key, $to, \$language, \$params);";
  }

  return $new;
}

/**
 * Convert parameters in references to user_authenticate.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_user_authenticate(&$file) {
  $hook = 'user_authenticate';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // user_authenticate changed parameters.
  $from[] = '/(user_authenticate)\s*\(\s*(.*),\s*(.*?)\)/';
  $to[] = "$1(array('name' => $2, 'pass' => $3)";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Add a parameter in references to hook_access.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_access(&$file) {
  $hook = 'access';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $hook = 'hook_access';

  $from = array();
  $to = array();

  // hook_access() added a parameter.
  $from[] = '/(_access)\s*\(\s*(.*?)\)/';
  $to[] = "$1($2, \$account)";
  // remove global user reference.
  $from[] = '/^.*(global\s*\$user;)$/m';
  $to[] = "";
  // rename user to account.
  $from[] = '/\$user/';
  $to[] = "\$account";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Remove javacript inclusions in hook_footer.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_footer(&$file) {
  $hook = 'footer';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $hook = 'hook_footer';

  $msg = "/* TODO It's no longer possible to add javascript to header in hook_footer(). */";

  $from = array();
  $to = array();

  // Can't add javascript to header in hook_footer.
  $from[] = '/^(.*drupal_add_js\s*\(.*)$/m';
  $to[] = "$msg\n//  $1";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}
