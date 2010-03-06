<?php
// $Id: views_bonus_export.theme.inc,v 1.5 2009/10/01 17:45:40 neclimdul Exp $
/**
 * @file
 * Theme related functions for processing our output style plugins.
 * Turns out that views basically requires you to have a theme include but
 * Drupal isn't really all that consistent actually including it. Since we
 * absolutely must have our preprocessing run the code has been moved to the
 * main module file. However views still requires this file to exist or
 * throws errors.
 *
 * Drupal bug: http://drupal.org/node/591804
 * Views bug: http://drupal.org/node/593336
 */
