<?php
// $Id: coder_style.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * This include file implements tests for the Drupal Standards as defined
 * at http://drupal.org/coding-standards
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder.
 */

function coder_array_ticks() {
  $some_array[FOO_BAR] = $baz; // This is ok.
  $some_array[foo] = $baz; // This is not.
}

function coder_test_tab() {
  // Tab in	comment - is this ok?
  $var = 'tab in	quote'; // Is this ok?
	$var = 'tab error';
}

function coder_test_stdclass() {
  $var = new stdClass(); // This is ok.
  $var = new StdClass(); // This is not.
  $var = new stdclassToo(); // Should be camelcase rule.
}

function coderCamelCase() {
  $camelCaseVar = 'whatever'; // Camel case functions and vars not allowed.
  $var = $obj->camelCase; // But camel case objects elements are.
  new camcelCase(); // Is ok.
}

function coder_test_two_errors_on_same_line() {
  if('test=' . $test == 'test='){ // There are 3 errors on this line.
  }
}

function coder_test_embedded_php() {
  ?>This is embedded php and should Not trigger a camelCase error.<?php
  ?>This second embedded php and should Not trigger
a camelCase error.<?php
}

function coder_hex_number() {
  $var = 0xFF; // Should NOT be camelcase.
}

function coder_multiline_quote() { // From Drupal5 block.module.
  return t('<p>Blocks are boxes of content that may be rendered into certain regions of your web pages, for example, into sidebars. They are usually generated automatically by modules, but administrators can create blocks manually.</p>
<p>Only enabled blocks are shown. You can position blocks by specifying which area of the page they should appear in (e.g., a sidebar).  Highlighted labels on this page show the regions into which blocks can be rendered. You can specify where within a region a block will appear by adjusting its weight.</p>
<p>If you want certain blocks to disable themselves temporarily during high server loads, check the "Throttle" box. You can configure the auto-throttle on the <a href="@throttle">throttle configuration page</a> after having enabled the throttle module.</p>
<p>You can configure the behaviour of each block (for example, specifying on which pages and for what users it will appear) by clicking the "configure" link for each block.</p>', array('@throttle' => url('admin/settings/throttle')));
}

function coder_break() {
  print '<br>'; // Should generate an error, and will throw performance warning.
  ?><br><?php // Should also generate an error.
}

function coder_heredoc() {
  $var = <<< __EOD__
<br><!-- a php error and not a camelCase error -->
<B><!-- an uppercase XHTML error -->
__EOD__;
  $var = <<<__EOD__
<br><!-- and again -->
__EOD__;
}

function coder_dot() {
  if ($file = file_check_upload($fieldname . '_upload')) { // Not ok.
  }
  $v .= 'bugger'; // Ok.
  $a = $v .'bugger'; // Ok.
  $a = $v ."bugger"; // Ok, but will throw performance warning.
  $a = $v.'bugger'; // Not ok.
  $a = $some_func().'bugger'; // Not ok.

  $a = 1.0 * .1 * 1. * (0.1) * (1.) * (.1) * (1.0); // Ok.
}

function coder_array_indexes() {
  $a[hello] = 'this is bad';
  $a['hello'] = 'this is no';
}

function coder_trailing_spaces() {
  $left = 'this is bad';   
  $left = 'this is ok';
}

function coder_control_structures() {
  if($a == 1){ }
  if ($a == 1) {
  }else {
  }
  if ($a == 1) { $b = 2;
  }
  if ($a == 1) {$b = 2;
  }
}

function coder_lowercase_true() {
  $a = TRUE; // Ok.
  $atrue = "true"; // Ok.
  $a = true; // Not ok.
  $a =true; // Not ok.
  if ($a == true) { // Not ok.
    return false; // Not ok.
  }
}

// Should generate an error about the trailing php close.
?>
