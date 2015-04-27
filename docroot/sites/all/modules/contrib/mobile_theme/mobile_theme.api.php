<?php

/**
 * @file
 * Hooks provided by the Mobile Theme module.
 */

/**
 * Defines methods available to detect mobile devices.
 *
 * @return
 *   An associative array whose key is the name of the function, and whose
 *   value is the human-readable name of the function. This function is
 *   called when needed to detect whether on a mobile browser. It should
 *   return TRUE if the user is on a mobile device.
 */
function hook_mobile_theme_detection() {
  return array(
    'my_function_name' => 'My Function Name',
  );
}
