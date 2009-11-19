<?php
// $Id: pathauto.inc 552 2009-10-26 21:29:47Z rfay $

/**
 * @file
 * Miscellaneous functions for Pathauto.
 *
 * @ingroup pathauto
 */

/**
 * Matches Unicode character classes.
 *
 * See: http://www.unicode.org/Public/UNIDATA/UCD.html#General_Category_Values
 *
 * The index only contains the following character classes:
 *   Lu  Letter, Uppercase
 *   Ll  Letter, Lowercase
 *   Lt  Letter, Titlecase
 *   Lo  Letter, Other
 *   Nd  Number, Decimal Digit
 *   No  Number, Other
 *
 * Copied from search.module's PREG_CLASS_SEARCH_EXCLUDE.
 */
define('PREG_CLASS_ALNUM',
'\x{0}-\x{2f}\x{3a}-\x{40}\x{5b}-\x{60}\x{7b}-\x{bf}\x{d7}\x{f7}\x{2b0}-'.
'\x{385}\x{387}\x{3f6}\x{482}-\x{489}\x{559}-\x{55f}\x{589}-\x{5c7}\x{5f3}-'.
'\x{61f}\x{640}\x{64b}-\x{65e}\x{66a}-\x{66d}\x{670}\x{6d4}\x{6d6}-\x{6ed}'.
'\x{6fd}\x{6fe}\x{700}-\x{70f}\x{711}\x{730}-\x{74a}\x{7a6}-\x{7b0}\x{901}-'.
'\x{903}\x{93c}\x{93e}-\x{94d}\x{951}-\x{954}\x{962}-\x{965}\x{970}\x{981}-'.
'\x{983}\x{9bc}\x{9be}-\x{9cd}\x{9d7}\x{9e2}\x{9e3}\x{9f2}-\x{a03}\x{a3c}-'.
'\x{a4d}\x{a70}\x{a71}\x{a81}-\x{a83}\x{abc}\x{abe}-\x{acd}\x{ae2}\x{ae3}'.
'\x{af1}-\x{b03}\x{b3c}\x{b3e}-\x{b57}\x{b70}\x{b82}\x{bbe}-\x{bd7}\x{bf0}-'.
'\x{c03}\x{c3e}-\x{c56}\x{c82}\x{c83}\x{cbc}\x{cbe}-\x{cd6}\x{d02}\x{d03}'.
'\x{d3e}-\x{d57}\x{d82}\x{d83}\x{dca}-\x{df4}\x{e31}\x{e34}-\x{e3f}\x{e46}-'.
'\x{e4f}\x{e5a}\x{e5b}\x{eb1}\x{eb4}-\x{ebc}\x{ec6}-\x{ecd}\x{f01}-\x{f1f}'.
'\x{f2a}-\x{f3f}\x{f71}-\x{f87}\x{f90}-\x{fd1}\x{102c}-\x{1039}\x{104a}-'.
'\x{104f}\x{1056}-\x{1059}\x{10fb}\x{10fc}\x{135f}-\x{137c}\x{1390}-\x{1399}'.
'\x{166d}\x{166e}\x{1680}\x{169b}\x{169c}\x{16eb}-\x{16f0}\x{1712}-\x{1714}'.
'\x{1732}-\x{1736}\x{1752}\x{1753}\x{1772}\x{1773}\x{17b4}-\x{17db}\x{17dd}'.
'\x{17f0}-\x{180e}\x{1843}\x{18a9}\x{1920}-\x{1945}\x{19b0}-\x{19c0}\x{19c8}'.
'\x{19c9}\x{19de}-\x{19ff}\x{1a17}-\x{1a1f}\x{1d2c}-\x{1d61}\x{1d78}\x{1d9b}-'.
'\x{1dc3}\x{1fbd}\x{1fbf}-\x{1fc1}\x{1fcd}-\x{1fcf}\x{1fdd}-\x{1fdf}\x{1fed}-'.
'\x{1fef}\x{1ffd}-\x{2070}\x{2074}-\x{207e}\x{2080}-\x{2101}\x{2103}-\x{2106}'.
'\x{2108}\x{2109}\x{2114}\x{2116}-\x{2118}\x{211e}-\x{2123}\x{2125}\x{2127}'.
'\x{2129}\x{212e}\x{2132}\x{213a}\x{213b}\x{2140}-\x{2144}\x{214a}-\x{2b13}'.
'\x{2ce5}-\x{2cff}\x{2d6f}\x{2e00}-\x{3005}\x{3007}-\x{303b}\x{303d}-\x{303f}'.
'\x{3099}-\x{309e}\x{30a0}\x{30fb}-\x{30fe}\x{3190}-\x{319f}\x{31c0}-\x{31cf}'.
'\x{3200}-\x{33ff}\x{4dc0}-\x{4dff}\x{a015}\x{a490}-\x{a716}\x{a802}\x{a806}'.
'\x{a80b}\x{a823}-\x{a82b}\x{d800}-\x{f8ff}\x{fb1e}\x{fb29}\x{fd3e}\x{fd3f}'.
'\x{fdfc}-\x{fe6b}\x{feff}-\x{ff0f}\x{ff1a}-\x{ff20}\x{ff3b}-\x{ff40}\x{ff5b}-'.
'\x{ff65}\x{ff70}\x{ff9e}\x{ff9f}\x{ffe0}-\x{fffd}');

/**
 * Check to see if there is already an alias pointing to a different item.
 *
 * @param $alias
 *   A string alias (i.e. dst).
 * @param $src
 *   A string that is the internal path.
 * @param $language
 *   A string indicating the path's language.
 * @return
 *   TRUE if an alias exists, FALSE if not.
 */
function _pathauto_alias_exists($alias, $src, $language = '') {
  $alias_pid = db_result(db_query_range("SELECT pid FROM {url_alias} WHERE dst = '%s' AND src <> '%s' AND language = '%s'", array($alias, $src, $language), 0, 1));
  if (function_exists('path_redirect_delete')) {
    // Delete from path_redirect the exact same alias to the same node.
    path_redirect_delete(array('path' => $alias, 'redirect' => $src));

    // If there still is this alias used in path_redirect, then create a different alias
    $redirect_rid = db_result(db_query_range("SELECT rid FROM {path_redirect} WHERE path = '%s'", $alias, 0, 1));
  }
  if ($alias_pid || !empty($redirect_rid)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Returns old alias and pid if there is already an alias
 * pointing to a different item.
 *
 * @param $src
 *   A string that is the internal path.
 * @return
 *   An array with the keys "pid" and "old_alias" containing
 *   the "pid" and old "alias", respectively, of the old alias.
 */
function _pathauto_existing_alias_data($src) {
  $output = array(
    'pid' => '',
    'old_alias' => ''
  );
  $result = db_query("SELECT pid, dst FROM {url_alias} WHERE src='%s'", $src);
  if ($data = db_fetch_object($result)) {
    // The item is already aliased, check what to do...
    switch (variable_get('pathauto_update_action', 2)) {
      // Replace old alias - remember the pid to update
      case 2:
      case 3:
        $output['pid'] = $data->pid;
      // Add new alias in addition to old one
      case 1:
        $output['old_alias'] = $data->dst;
        break;
      // Do nothing
      case 0:
      default:
        break;
    }
  }
  return $output;
}

/**
 * Clean up a string value provided by a module.
 *
 * Resulting string contains only alphanumerics and separators.
 *
 * @param $string
 *   A string to clean.
 * @param $clean_slash
 *   Whether to clean slashes from the given string.
 * @return
 *   The cleaned string.
 */
function pathauto_cleanstring($string, $clean_slash = TRUE) {
  // Default words to ignore
  $ignore_words = array(
    'a', 'an', 'as', 'at', 'before', 'but', 'by', 'for', 'from', 'is', 'in',
    'into', 'like', 'of', 'off', 'on', 'onto', 'per', 'since', 'than', 'the',
    'this', 'that', 'to', 'up', 'via', 'with',
  );

  // Replace or drop punctuation based on user settings
  $separator = variable_get('pathauto_separator', '-');
  $output = $string;
  $punctuation = pathauto_punctuation_chars();
  foreach ($punctuation as $name => $details) {
    $action = variable_get('pathauto_punctuation_'. $name, 0);
    // 2 is the action for "do nothing" with the punctuation
    if ($action != 2) {
      // Slightly tricky inline if which either replaces with the separator or nothing
      $output = str_replace($details['value'], ($action ? $separator : ''), $output);
    }
  }

  // If something is already urlsafe then don't remove slashes
  if ($clean_slash) {
    $output = str_replace('/', '', $output);
  }

  // Optionally remove accents and transliterate
  if (variable_get('pathauto_transliterate', FALSE)) {
    static $i18n_loaded = false;
    static $translations = array();

    if (!$i18n_loaded) {
      $path = drupal_get_path('module', 'pathauto');
      if (is_file($path .'/i18n-ascii.txt')) {
        $translations = parse_ini_file($path .'/i18n-ascii.txt');
      }
      $i18n_loaded = true;
    }

    $output = strtr($output, $translations);
  }

  // Reduce to the subset of ASCII96 letters and numbers
  if (variable_get('pathauto_reduce_ascii', FALSE)) {
    $pattern = '/[^a-zA-Z0-9\/]+/ ';
    $output = preg_replace($pattern, $separator, $output);
  }

  // Get rid of words that are on the ignore list
  $ignore_re = '\b'. preg_replace('/,/', '\b|\b', variable_get('pathauto_ignore_words', $ignore_words)) .'\b';

  if (function_exists('mb_eregi_replace')) {
    $output = mb_eregi_replace($ignore_re, '', $output);
  }
  else {
    $output = preg_replace("/$ignore_re/i", '', $output);
  }

  // Always replace whitespace with the separator.
  $output = preg_replace('/\s+/', $separator, $output);

  // In preparation for pattern matching,
  // escape the separator if and only if it is not alphanumeric.
  if (isset($separator)) {
    if (preg_match('/^[^'. PREG_CLASS_ALNUM .']+$/uD', $separator)) {
      $seppattern = $separator;
    }
    else {
      $seppattern = '\\'. $separator;
    }
    // Trim any leading or trailing separators (note the need to
    $output = preg_replace("/^$seppattern+|$seppattern+$/", '', $output);

    // Replace trailing separators around slashes.
    $output = preg_replace("/$seppattern\/|\/$seppattern/", "/", $output);

    // Replace multiple separators with a single one
    $output = preg_replace("/$seppattern+/", "$separator", $output);
  }

  // Enforce the maximum component length
  $maxlength = min(variable_get('pathauto_max_component_length', 100), 128);
  $output = drupal_substr($output, 0, $maxlength);

  return $output;
}

/**
 * Apply patterns to create an alias.
 *
 * @param $module
 *   The name of your module (e.g., 'node').
 * @param $op
 *   Operation being performed on the content being aliased
 *   ('insert', 'update', 'return', or 'bulkupdate').
 * @param $placeholders
 *   An array whose keys consist of the translated placeholders
 *   which appear in patterns (e.g., t('[title]')) and values are the
 *   actual values to be substituted into the pattern (e.g., $node->title).
 * @param $src
 *   The "real" URI of the content to be aliased (e.g., "node/$node->nid").
 * @param $type
 *   For modules which provided pattern items in hook_pathauto(),
 *   the relevant identifier for the specific item to be aliased
 *   (e.g., $node->type).
 * @param $language
 *   A string specify the path's language.
 * @return
 *   The alias that was created.
 */
function pathauto_create_alias($module, $op, $placeholders, $src, $entity_id, $type = NULL, $language = '') {
  if (($op != 'bulkupdate') and variable_get('pathauto_verbose', FALSE) && user_access('notify of path changes')) {
    $verbose = TRUE;
  }
  else {
    $verbose = FALSE;
  }

  // Retrieve and apply the pattern for this content type
  if (!empty($type)) {
    $pattern = trim(variable_get('pathauto_'. $module .'_'. $type .'_'. $language .'_pattern', ''));
    if (empty($pattern)) {
      $pattern = trim(variable_get('pathauto_'. $module .'_'. $type .'_pattern', ''));
    }
  }
  if (empty($pattern)) {
    $pattern = trim(variable_get('pathauto_'. $module .'_pattern', ''));
  }
  // No pattern? Do nothing (otherwise we may blow away existing aliases...)
  if (empty($pattern)) {
    return '';
  }

  if ($module == 'taxonomy') {
    // Get proper path for term.
    $term_path = taxonomy_term_path(taxonomy_get_term($entity_id));
    if ($term_path != $src) {
      // Quietly alias 'taxonomy/term/[tid]' with proper path for term.
      $update_data = _pathauto_existing_alias_data($src);
      _pathauto_set_alias($src, $term_path, $module, $entity_id, $update_data['pid'], FALSE, $update_data['old_alias'], $language);
      // Set $src as proper path.
      $src = $term_path;
    }
  }

  // Special handling when updating an item which is already aliased.
  $pid = NULL;
  $old_alias = NULL;
  if ($op == 'update' or $op == 'bulkupdate') {
    if (variable_get('pathauto_update_action', 2) == 0) {
      // Do nothing
      return '';
    }
    $update_data = _pathauto_existing_alias_data($src);
    $pid = $update_data['pid'];
    $old_alias = $update_data['old_alias'];
  }

  // Replace the placeholders with the values provided by the module,
  // and optionally lower-case the result
  $alias = str_replace($placeholders['tokens'], $placeholders['values'], $pattern);

  if (variable_get('pathauto_case', 1)) {
    $alias = drupal_strtolower($alias);
  }

  // Two or more slashes should be collapsed into one
  $alias = preg_replace('/\/+/', '/', $alias);

  // Trim any leading or trailing slashes
  $alias = preg_replace('/^\/|\/+$/', '', $alias);

  $maxlength = min(variable_get('pathauto_max_length', 100), 128);
  $alias = drupal_substr($alias, 0, $maxlength);

  // If the alias already exists, generate a new, hopefully unique, variant
  $separator = variable_get('pathauto_separator', '-');
  if (_pathauto_alias_exists($alias, $src, $language)) {
    $original_alias = $alias;
    for ($i = 0; _pathauto_alias_exists(drupal_substr($alias, 0, $maxlength - strlen($i)) . $separator . $i, $src, $language); $i++) {
    }
    // Make room for the sequence number
    $alias = drupal_substr($alias, 0, $maxlength - drupal_strlen($i));
    $alias = $alias . $separator . $i;
    // If verbose is on, alert the user why this happened
    if ($verbose) {
      drupal_set_message(t('The automatically generated alias %original_alias conflicted with an existing alias. Alias changed to %alias.',
        array('%original_alias' => $original_alias, '%alias' => $alias)));
    }
  }

  // Return the generated alias if requested.
  if ($op == 'return') {
    return $alias;
  }

  // If $pid is NULL, a new alias is created - otherwise, the existing
  // alias for the designated src is replaced
  _pathauto_set_alias($src, $alias, $module, $entity_id, $pid, $verbose, $old_alias, $language);

  // Also create a related feed alias if requested, and if supported
  // by the module
  if (drupal_strlen(variable_get('pathauto_'. $module .'_applytofeeds', ''))) {
    $feedappend = variable_get('pathauto_'. $module .'_applytofeeds', '');

    // For forums and taxonomies, the src doesn't always form the base of the rss feed (ie. image galleries)
    if ($module == 'taxonomy' || $module == 'forum') {
      $update_data = _pathauto_existing_alias_data("taxonomy/term/$entity_id/$feedappend");
      _pathauto_set_alias("taxonomy/term/$entity_id/$feedappend", "$alias/feed", $module, $entity_id, $update_data['pid'], $verbose, $update_data['old_alias'], $language);
    }
    else {
      $update_data = _pathauto_existing_alias_data("$src/$feedappend");
      _pathauto_set_alias("$src/$feedappend", "$alias/feed", $module, $entity_id, $update_data['pid'], $verbose, $update_data['old_alias'], $language);
    }
  }

  return $alias;
}

/**
 * Verify if the given path is a valid menu callback.
 *
 * Taken from menu_execute_active_handler().
 *
 * @param $path
 *   A string containing a relative path.
 * @return
 *   TRUE if the path already exists.
 */
function _pathauto_path_is_callback($path) {
  $menu = menu_get_item($path);
  if (isset($menu['path']) && $menu['path'] == $path) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Private function for Pathauto to create an alias.
 *
 * @param $src
 *   The internal path.
 * @param $dst
 *   The visible externally used path.
 * @param $pid
 *   If the item is currently aliased, the pid for that item.
 * @param $verbose
 *   If the admin has enabled verbose, should be TRUE. Else FALSE or NULL.
 * @param $old_alias
 *   If the item is currently aliased, the existing alias for that item.
 * @param $language
 *   The path's language.
 */
function _pathauto_set_alias($src, $dst, $entity_type, $entity_id, $pid = NULL, $verbose = FALSE, $old_alias = NULL, $language = '') {
  // Alert users that an existing callback cannot be overridden automatically
  if (_pathauto_path_is_callback($dst)) {
    if ($verbose && user_access('notify of path changes')) {
      drupal_set_message(t('Ignoring alias %dst due to existing path conflict.', array('%dst' => $dst)));
    }
    return;
  }
  // Alert users if they are trying to create an alias that is the same as the internal path
  if ($src == $dst) {
    if ($verbose && user_access('notify of path changes')) {
      drupal_set_message(t('Ignoring alias %dst because it is the same as the internal path.', array('%dst' => $dst)));
    }
    return;
  }

  // Skip replacing the current alias with an identical alias
  if ($old_alias != $dst) {
    path_set_alias($src, $dst, $pid, $language);

    if (variable_get('pathauto_update_action', 2) == 3 && function_exists('path_redirect_save')) {
      if (!empty($old_alias)) {
        $redirect = array(
          'path' => $old_alias,
          'redirect' => $src,
        );
        path_redirect_save($redirect);
      }
    }
    if ($verbose && user_access('notify of path changes')) {
      if (!empty($redirect)) {
        drupal_set_message(t('Created new alias %dst for %src, replacing %old_alias. %old_alias now redirects to %dst', array('%dst' => $dst, '%src' => $src, '%old_alias' => $old_alias)));
      }
      elseif ($pid) {
        drupal_set_message(t('Created new alias %dst for %src, replacing %old_alias', array('%dst' => $dst, '%src' => $src, '%old_alias' => $old_alias)));
      }
      else {
        drupal_set_message(t('Created new alias %dst for %src', array('%dst' => $dst, '%src' => $src)));
      }
    }
  }
}

/**
 * Generalized function to get tokens across all Pathauto types.
 *
 * @param $object
 *   A user, node, or category object.
 * @return
 *   Tokens for that object formatted in the way that
 *   Pathauto expects to see them.
 */
function pathauto_get_placeholders($type, $object) {
  if (function_exists('token_get_values')) {
    $full = token_get_values($type, $object, TRUE);
    $tokens = token_prepare_tokens($full->tokens);
    $values = pathauto_clean_token_values($full);
    return array('tokens' => $tokens, 'values' => $values);
  }
  // TODO at some point try removing this and see if install profiles have problems again.
  watchdog('Pathauto', 'It appears that you have installed Pathauto, which depends on token, but token is either not installed or not installed properly.');
  return array('tokens' => array(), 'values' => array());
}

/**
 * Clean tokens so they are URL friendly.
 *
 * @param $full
 *   An array of token values that need to be "cleaned" for use in the URL.
 * @return
 *   An array of the cleaned tokens.
 */
function pathauto_clean_token_values($full) {
  foreach ($full->values as $key => $value) {
    // If it's a "path" or "url friendly" token don't remove the "/" character
    if (drupal_substr($full->tokens[$key], -4, 4) === 'path' || drupal_substr($full->tokens[$key], -8, 8) === 'path-raw' || drupal_substr($full->tokens[$key], -5, 5) === 'alias') {
      $full->values[$key] = pathauto_cleanstring($value, FALSE);
    }
    else {
      $full->values[$key] = pathauto_cleanstring($value);
    }
  }
  return $full->values;
}

/**
 * Return an array of arrays for punctuation values.
 *
 * Returns an array of arrays for punctuation values keyed by a name, including
 * the value and a textual description.
 * Can and should be expanded to include "all" non text punctuation values.
 *
 * @return
 *   An array of arrays for punctuation values keyed by a name, including the
 *   value and a textual description.
 */
function pathauto_punctuation_chars() {
  $punctuation = array();

  // Handle " ' ` , . - _ : ; | { [ } ] + = * & % ^ $ # @ ! ~ ( ) ? < > \
  $punctuation['double_quotes']      = array('value' => '"', 'name' => t('Double quotes "'));
  $punctuation['quotes']             = array('value' => "'", 'name' => t("Single quotes (apostrophe) '"));
  $punctuation['backtick']           = array('value' => '`', 'name' => t('Back tick `'));
  $punctuation['comma']              = array('value' => ',', 'name' => t('Comma ,'));
  $punctuation['period']             = array('value' => '.', 'name' => t('Period .'));
  $punctuation['hyphen']             = array('value' => '-', 'name' => t('Hyphen -'));
  $punctuation['underscore']         = array('value' => '_', 'name' => t('Underscore _'));
  $punctuation['colon']              = array('value' => ':', 'name' => t('Colon :'));
  $punctuation['semicolon']          = array('value' => ';', 'name' => t('Semicolon ;'));
  $punctuation['pipe']               = array('value' => '|', 'name' => t('Pipe |'));
  $punctuation['left_curly']         = array('value' => '{', 'name' => t('Left curly bracket {'));
  $punctuation['left_square']        = array('value' => '[', 'name' => t('Left square bracket ['));
  $punctuation['right_curly']        = array('value' => '}', 'name' => t('Right curly bracket }'));
  $punctuation['right_square']       = array('value' => ']', 'name' => t('Right square bracket ]'));
  $punctuation['plus']               = array('value' => '+', 'name' => t('Plus +'));
  $punctuation['equal']              = array('value' => '=', 'name' => t('Equal ='));
  $punctuation['asterisk']           = array('value' => '*', 'name' => t('Asterisk *'));
  $punctuation['ampersand']          = array('value' => '&', 'name' => t('Ampersand &'));
  $punctuation['percent']            = array('value' => '%', 'name' => t('Percent %'));
  $punctuation['caret']              = array('value' => '^', 'name' => t('Caret ^'));
  $punctuation['dollar']             = array('value' => '$', 'name' => t('Dollar $'));
  $punctuation['hash']               = array('value' => '#', 'name' => t('Hash #'));
  $punctuation['at']                 = array('value' => '@', 'name' => t('At @'));
  $punctuation['exclamation']        = array('value' => '!', 'name' => t('Exclamation !'));
  $punctuation['tilde']              = array('value' => '~', 'name' => t('Tilde ~'));
  $punctuation['left_parenthesis']   = array('value' => '(', 'name' => t('Left parenthesis ('));
  $punctuation['right_parenthesis']  = array('value' => ')', 'name' => t('right parenthesis )'));
  $punctuation['question_mark']      = array('value' => '?', 'name' => t('Question mark ?'));
  $punctuation['less_than']          = array('value' => '<', 'name' => t('Less than <'));
  $punctuation['greater_than']       = array('value' => '>', 'name' => t('Greater than >'));
  $punctuation['back_slash']         = array('value' => '\\', 'name' => t('Back slash \\'));

  return $punctuation;
}
