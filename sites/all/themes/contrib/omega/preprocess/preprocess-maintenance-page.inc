<?php
// $Id: preprocess-maintenance-page.inc,v 1.1.2.2 2009/10/12 02:42:28 himerus Exp $
$vars['body_attributes'] = array();

$vars['body_attributes']['id'] = 'maintenance-page';

$classes[] = $vars['body_classes'];

$vars['body_attributes']['class'] = implode(' ', $classes);

$vars['attributes'] = drupal_attributes($vars['body_attributes']);
