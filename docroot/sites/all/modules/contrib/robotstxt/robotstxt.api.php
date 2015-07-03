<?php

/**
 * @file
 * Hooks provided by the robotstxt module.
 */

/**
 * Add additional lines to the site's robots.txt file.
 *
 * @return
 *   An array of strings to add to the robots.txt.
 */
function hook_robotstxt() {
  return array(
    'Disallow: /foo',
    'Disallow: /bar',
  );
}
