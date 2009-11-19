<?php
// $Id: deadwood.formapi.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Generate version upgrade code from 5.x to 6.x.
 *
 * The functions in this file match up with the 18 topics in the roadmap at
 * http://drupal.org/node/144132.
 *
 * Copyright 2008 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Convert calls to hook_validate and submit() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_formAPI(&$file) {
  $hook = 'formAPI';
  drupal_set_message('Start Form API conversions');
  deadwood_convert_validate_submit($file);
  deadwood_convert_form_alter2($file);
  deadwood_convert_submit($file);
  deadwood_convert_submit_params($file);
  deadwood_convert_set_value($file);
  deadwood_convert_multistep($file);
  deadwood_convert_form_base($file);
  deadwood_convert_button_handlers($file);
  deadwood_convert_op_element($file);
  deadwood_convert_retrieve_form($file);
  deadwood_convert_hook_forms($file);
  deadwood_convert_pre_render($file);
  // Add these from observations of API changes not documented in roadmap.
  deadwood_convert_form_builder($file);
  deadwood_convert_get_form($file);
  deadwood_convert_form_callback($file);
  drupal_set_message('Finish Form API conversions');
}

/**
 * Convert calls to hook_validate and submit() for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_validate_submit(&$file) {
  /*
   * TODO Could these module functions have added additional parameters from the core signature?
   * We need to append any additional parameters to the end of the required parameters.
   * Should we search for the string '_form_alter' and place a notification message?
   * This might be present if the module called the hook itself.
   * Are there variables inside this function we could/should rename?
   *
   * There could be multiple occurrences of these functions -- we need a loop.
   */
  $hooks = array('validate', 'submit');
  foreach ($hooks as $hook) {
    // Process file in chunks.
    $chunks = deadwood_find_hook($hook, $file, TRUE);
    foreach ($chunks as $chunk) {
      $cur = $chunk;
      $new = $cur;
      $new = deadwood_convert_fix_validate($new);
      deadwood_save_changes($cur, $new, $file, 'hook_' . $hook);
    }
  }
}

/**
 * Convert calls to hook_validate and submit() for parameter changes.
 *
 * @param string $file The file to convert.
 * @return string The revised file.
 */
function deadwood_convert_fix_validate(&$file) {
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Replace parameters in function signature.
  $from[] = '/^(function.*)(\(.*\))/m';
  $to[] = "$1(\$form, &\$form_state)";
  // Replace form_values with new form_state.
  $from[] = '/\$form_values(|\[)/';
  $to[] = "\$form_state['values']$1";
  // Replace form_id with new form_state.
  $from[] = '/\$form_id/';
  $to[] = '$form_state[\'values\'][\'form_id\']';
  // Replace form[<element>]['#value'] with new form_state['#value'][<element>].
  $from[] = '/\$form(\[\'\w+\'\])\[\'#value\'\]/';
  $to[] = '$form_state[\'values\']$1';

  deadwood_do_conversions($from, $to, $new);
  return $new;
}

/**
 * Add note about new hook_form_$form-id_alter() functions.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_form_alter2(&$file) {
  $hook = 'form_alter2';
  $cur = $file;
  $new = $cur;

  // Check for form_alter functions.
  $pattern = '/(?<!hook)_form_alter\s*(\(.*\))/';
  if (!preg_match($pattern, $file, $matches)) {
    return;
  }

  $msg =
"/* TODO You may want to take advantage of new form-specific alter hooks.
   The hook_form_alter() is complemented by hook_form_\$form-id_alter().
   Optionally, modules can implement form-specific alteration functions rather
   than a single hook_form_alter() with many conditional switch statements.
   This is optional, and is most useful for tidying the code of modules that
   alter many forms to customize a site's operations.
   The new function names are built using the following format:
   [your-module-name]_form_[\$form_id]_alter. */";

  $from = array();
  $to = array();
  // Find the Id line in the file.
  $from[] = '/^(\/\/ \$Id.*\$\s*$)/m';
  $to[] = "$1\n$msg\n";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert return values in calls to hook_submit() for API changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_submit(&$file) {
  $hook = 'submit';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;

  $from = array();
  $to = array();
  // Change return statement to $form_state['redirect'].
  $from[] = '/^(.*)return\s*(.+);/m';
  $to[] = "$1\$form_state['redirect'] = $2;";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Remove parameters to $form['#submit'], $form['#validate'], and
 * $form['#process'].
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_submit_params(&$file) {
  $cur = $file;
  $new = $cur;

  $hooks = array('validate', 'submit', 'process');
  foreach ($hooks as $hook) {
    // Remove parameters to $form['#hook'].
    $search = '/^(\s*)(\$\w+)' . "(\['#$hook'\])\[(.*)\]\s*=\s*array\s*\((.*)\);/m";
    $callback = 'deadwood_fix_submit_params1';
    $chg = preg_replace_callback($search, $callback, $new);
    $new = $chg != '' ? $chg : $new;
    // See fivestar_elements function that defines #process outside of $form
    $search = "/^(\s*)(\'#$hook')\s*=>\s*array\s*\((.*)\)(,|;)$/m";
    $callback = 'deadwood_fix_submit_params2';
    $chg = preg_replace_callback($search, $callback, $new);
    $new = $chg != '' ? $chg : $new;
  }
  $hook = 'submit_params';
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Callback function for converting parameters.
 * $form['#process']['element_process'] = array('abc' => 'xyz', 'def' => '123')
 * $form['#process']['element_process'] = array('xyz', '123')
 * $form['#process']['element_process'] = array()
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_submit_params1($matches) {
  $new = array();
  $new[] = $matches[1] . $matches[2] . $matches[3] . '[] = ' . $matches[4] . ';';

  $i = 1;
  $params = explode(',', $matches[5]);
  foreach ($params as $param) {
    // TODO Instruct the user to name each parameter.
    if (strlen(trim($param))) {
      if (strpos($param, '=>')) {
        $tokens = explode('=>', $param);
        $new[] = $matches[1] . $matches[2] . "['#" . trim($tokens[0], "' ") . "'] = " . trim($tokens[1]) . ',';
      }
      else {
        $new[] = $matches[1] . $matches[2] . "['#" . trim($matches[4], "' ") . "_param_$i'] = " . trim($param) . ',';
      }
      $i++;
    }
  }
  $new = implode("\n", $new);
  return $new;
}

/**
 * Callback function for converting parameters.
 * Code assumes only one $key => array() inside the outer array.
 * '#process' => array('element_process' => array('abc' => 'xyz', 'def' => '123'))
 * '#process' => array('element_process' => array('xyz', '123'))
 * '#process' => array('element_process' => array())
 * '#process' => array('element_process')
 *
 * @param array $matches Function arguments.
 * @return Text of converted function call.
 */
function deadwood_fix_submit_params2($matches) {
  $new = array();

  // Fourth case above -- nothing to change.
  if (!strpos($matches[3], 'array(')) {
    return $matches[0];
  }

  // Get function name.
  $function = substr($matches[3], 0, strpos($matches[3], 'array(') - 1);
  $function = trim($function, '=> ');
  $new[] = $matches[1] . $matches[2] . ' => array(' . $function . ')' . $matches[4];

  // Parse parameters.
  $i = 1;
  $params = substr($matches[3], strpos($matches[3], 'array(') + strlen('array('));
  $params = trim($params, ')');
  $params = explode(',', $params);
  foreach ($params as $param) {
    if (strlen(trim($param))) {
      if (strpos($param, '=>')) {
        $tokens = explode('=>', $param);
        $new[] = $matches[1] . "'#" . trim($tokens[0], "' ") . "' => " . trim($tokens[1]) . ',';
      }
      else {
        $new[] = $matches[1] . "'#" . trim($function, "' ") . "_param_$i' => " . trim($param) . ',';
      }
      $i++;
    }
  }
  $new = implode("\n", $new);
  return $new;
}

/**
 * Revise paramters in calls to form_set_value().
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_set_value(&$file) {
  $hook = 'set_value';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Add $form_state to paramters.
  $from[] = '/(form_set_value)\s*\((.*)\)/';
  $to[] = "$1($2, \$form_state)";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Replace multistep parameter with rebuild paramter.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_multistep(&$file) {
  $hook = 'multistep';
  $cur = $file;
  $new = $cur;

  $msg =
"/* TODO #multistep is gone, use \$form_state instead.
   Move the rebuild statement to the submit function for this form. */";

  $from = array();
  $to = array();
  // Replace multistep parameters.
  $from[] = '/^(.*\$form\[\'#multistep\'\].*$)/m';
  $to[] = "$msg\n\t\$form_state['rebuild'] = TRUE;\n//$1";
  // Comment out redirect parameters.
  $from[] = '/^(.*\$form\[\'#redirect\'\].*$)/m';
  $to[] = "//$1";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

// Not sure how to isolate for #element_validate changes?

/**
 * Add note about elimination of $form['#base'].
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_form_base(&$file) {
  $hook = 'form_base';
  $cur = $file;
  $new = $cur;

  // Check for form_alter functions.
  $pattern = '/\'#base\'/';
  if (!preg_match($pattern, $file, $matches)) {
    return;
  }

  $msg =
"/* TODO \$form['#base'] is gone
   In FormAPI, many forms with different form_ids can share the same validate,
   submit, and theme handlers. This can be done by manually populating the
   \$form['#submit'], \$form['#validate'], and \$form['#theme'] elements with
   the proper function names. */";

  $from = array();
  $to = array();
  // Find the Id line in the file.
  $from[] = '/^(\/\/ \$Id.*\$\s*$)/m';
  $to[] = "$1\n$msg\n";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

// #post_render complements #pre_render

/**
 * Add note about custom handlers on form buttons.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_button_handlers(&$file) {
  $hook = 'form_base';
  $cur = $file;
  $new = $cur;

  // Check for form_alter functions.
  $pattern = '/\$form_values\[\'op\'\]/';
  if (!preg_match($pattern, $file, $matches)) {
    return;
  }

  $msg =
"/* TODO Form buttons can define custom #submit and #validate handlers.
   All forms can have #validate and #submit properties containing lists of
   validation and submission handlers to be executed when a user submits data.
   Previously, if a form featured multiple submission buttons to initiate
   different actions (updating a record versus deleting, for example), it was
   necessary to check the incoming form_values['op'] for the name of the
   clicked button, then execute different code based on its value. Now, it is
   possible to define #validate and #submit properties on each individual form
   button if desired. */";

  $from = array();
  $to = array();
  // Find the Id line in the file.
  $from[] = '/^(\/\/ \$Id.*\$\s*$)/m';
  $to[] = "$1\n$msg\n";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Add note about deprecation of the op element from form_values.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_op_element(&$file) {
  $hook = 'op_element';
  $cur = $file;
  $new = $cur;

  $msg =
"/* TODO The 'op' element in the form values is deprecated.
   Each button can have #validate and #submit functions associated with it.
   Thus, there should be one button that submits the form and which invokes
   the normal form_id_validate and form_id_submit handlers. Any additional
   buttons which need to invoke different validate or submit functionality
   should have button-specific functions. */";

  $from = array();
  $to = array();
  // Comment on deprecated element.
  $from[] = '/^(.*\$form_values\[\'op\'\].*$)/m';
  $to[] = "$msg\n$1";
  // Comment on deprecated element.
  $from[] = '/^(.*\$form_state\[\'values\'\]\[\'op\'\].*$)/m';
  $to[] = "$msg\n$1";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

// Validation handlers can pass information to submit handlers

/**
 * Add note about deprecation of the op element from form_values.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_retrieve_form(&$file) {
  $hook = 'retrieve_form';
  $cur = $file;
  $new = $cur;

  $msg =
"/* TODO drupal_retrieve_form() now accepts a form_state parameter. */";

  $from = array();
  $to = array();
  // Comment on deprecated element.
  $from[] = '/^(.*drupal_retrieve_form)\s*\((.*)\)(.*)$/m';
  $to[] = "$msg\n$1($2, \$form_state)$3";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Change parameters of hook_forms().
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_hook_forms(&$file) {
  $hook = 'forms';
  $cur = deadwood_find_hook($hook, $file);
  $new = $cur;
  $hook = 'hook_forms';

  $from = array();
  $to = array();

  /*
   * The fivestar module has no parameter in its function signature and calls
   * $args = func_get_args();
   * The $args array seems to have another level to it with $args[0][0] being
   * $form_id.
   * So handle two cases.
   */

  // Check for function with no parameters in its signature.
  $pattern = '/function.*\((\s*)\)/';
  if (preg_match_all($pattern, $cur, $matches)) {
    // Add note about reducing index values by one.
    $msg =
"/* TODO Your function did not have \$args in its signature.
   Any \$args[0][n] values have been converted to \$args[n].
   You may need to reduce these indices by one. */";
    $from[] = '/^(function.*\(\s*\).*$)/m';
    $to[] = "$1\n$msg";

    // Replace parameters in function signature.
    $from[] = '/^(function.*)\((\s*)\)/m';
    $to[] = "$1(\$form_id, \$args)";
    // Replace args[0][0] with form_id.
    $from[] = '/(\$args\[0\]\[0\])/m';
    $to[] = "\$form_id";
    // Replace args[0][n] with args[n-1]. Should we leave a note about this?
    $from[] = '/(\$args\[0\]\[(.+?)\])/m';
    $to[] = "\$args[$2]";
  }
  else {
    // Replace parameters in function signature.
    $from[] = '/^(function.*)\((.+)\)/m';
    $to[] = "$1(\$form_id, $2)";
    // Replace args[0] with form_id.
    $from[] = '/(\$args\[0\])/m';
    $to[] = "\$form_id";
  }

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Change parameters to a $form['#pre_render'] function.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_pre_render(&$file) {
  $hook = 'pre_render';
  $cur = $file;
  $new = $cur;

  /*
   * Steps.
   * First, check for ['#pre_render']['my_function'].
   * Second, convert pre_render function from array key to array value.
   * Third, find my_function.
   * Replace parameters in function signature.
   * Replace $form_id with $form['#id'].
   */

  // Check for form_alter functions.
  $pattern = '/\$form' . "\['#$hook']\['(.*)'\]/";
  if (!preg_match_all($pattern, $file, $matches)) {
    return;
  }
  $functions = $matches[1];

  $from = array();
  $to = array();
  // Convert pre_render function from array key to array value.
  $from[] = '/(\$form' . "\['#$hook'])\[(.*)\].*=\s*array\s*\((.*)\);/";
  $to[] = "$1[] = $2;";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);

  // Find each pre_render function.
  foreach ($functions as $function) {
    $cur = deadwood_find_hook($function, $file, FALSE, FALSE);
    $new = $cur;
    $new = deadwood_fix_pre_render($new);
    deadwood_save_changes($cur, $new, $file, $hook . ' = ' . $function);
  }
}

/**
 * Convert calls to pre_render function for parameter changes.
 *
 * @param string $file The file to convert.
 * @return string The revised file.
 */
function deadwood_fix_pre_render(&$file) {
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();
  // Replace parameters in function signature.
  $from[] = '/^(function.*)(\(.*\))/m';
  $to[] = "$1(\$form)";
  // Replace form_id with form[].
  $from[] = '/\$form_id/';
  $to[] = '$form[\'#id\']';

  deadwood_do_conversions($from, $to, $new);
  return $new;
}

/**
 * Convert calls to form_builder function for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_form_builder(&$file) {
  $hook = 'form_builder';
  $cur = $file;
  $new = $cur;

  $msg =
"/* TODO form_builder() now accepts a form_state parameter. */";

  $from = array();
  $to = array();
  // Add $form_state to parameters of function call.
  $from[] = '/^(.*)(form_builder)\s*\((.*)\)(.*)$/m';
  $to[] = "$msg\n\t\$form_state = array();\n$1$2($3, \$form_state)$4";

  deadwood_do_conversions($from, $to, $new);
  deadwood_save_changes($cur, $new, $file, $hook);
}

/**
 * Convert calls to drupal_get_form function for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_get_form(&$file) {
  $hook = 'get_form';
  $cur = $file;
  $new = $cur;

  /*
   * Steps.
   * First, check for calls to drupal_get_form('my_function', ...).
   * Second, add $form_state to parameters of these function calls.
   * Third, find my_function.
   * Add $form_state to parameters in function signature.
   */

  // Check for calls to drupal_get_form.
  $pattern = '/drupal_get_form\s*\(\s*\'(\w+)\'\s*,.*\)/';
  if (!preg_match_all($pattern, $file, $matches)) {
    return;
  }
  $functions = array_unique($matches[1]);

  // Find each form function.
  foreach ($functions as $function) {
    $cur = $file;
    $new = $cur;
    $from = array();
    $to = array();
    // Add $form_state to parameters in function calls and function signature.
    $from[] = "/($function)\s*\((.*)\)/";
    $to[] = "$1(\$form_state, $2)";
    // Add '&' before $form_state in function signature.
    $from[] = "/^(function\s*$function)\s*\((.*)\)/m";
    $to[] = "$1(&$2)";

    deadwood_do_conversions($from, $to, $new);
    deadwood_save_changes($cur, $new, $file, $hook . ' = ' . $function);
  }
}

/**
 * Convert calls to form functions for parameter changes.
 *
 * @param string $file The file to convert.
 */
function deadwood_convert_form_callback(&$file) {
  $hook = 'form_callback';
  $cur = $file;
  $new = $cur;

  /*
   * Steps.
   * First, check for form callback functions with parameters.
   * Second, add $form_state to parameters in these function signatures.
   */

  // Check for form callback functions called by drupal_get_form or related.
  // This excludes functions defined as a page callback in the menu hook.
  $callbacks = '(page arguments|callback)';
  $inbetween = '(]\s*=\s*|\s*=>\s*)';
  $pattern = "/'$callbacks'$inbetween'(\w+)'/";
  if (!preg_match_all($pattern, $file, $matches)) {
    return;
  }
  $functions = array_unique($matches[3]);

  // Find each form function.
  foreach ($functions as $function) {
    $cur = $file;
    $new = $cur;

    $from = array();
    $to = array();
    // Add $form_state to parameters in function signature.
    $from[] = "/^(function\s*$function)\s*\((.+)\)/m";
    $to[] = "$1(&\$form_state, $2)";

    deadwood_do_conversions($from, $to, $new);
    deadwood_save_changes($cur, $new, $file, $hook . ' = ' . $function);
  }
}
