<?php
// $Id: extra_metatags.metatags.inc,v 1.1.2.8 2009/09/21 06:40:42 kiam Exp $

/**
 * @file
 * Support file for the meta tags added by Extra meta tags.
 */

/**
 * @addtogroup extra_metatags
 * @{
 */

/*****************************************************************************
 * 'dc.title' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function extra_metatags_dc_title_form(&$form, $content, $options) {
  $form['dc.title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dublin Core title'),
    '#description' => t('Enter an alternative title. Do not use the value already used for the TITLE tag, or you will probably get warning reports about duplicated titles from the search engines.'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($content['value']),
    '#tree' => TRUE,
  );

  $form['dc.title']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function extra_metatags_dc_title_prepare(&$tags, $content, $options) {
  if (empty($content['value'])) {
    if ($options['type'] == 'node' && count($options['ids']) == 1) {
      if ($node = node_load($options['ids'][0])) {
        $content['value'] = $node->title;
      }
    }
  }

  $tags['dc.title'] = empty($content['value']) ? (!empty($options['default']['dc.title']['value']) ? $options['default']['dc.title']['value'] : '') : $content['value'];
}

/*****************************************************************************
 * Location meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function extra_metatags_location_form(&$form, $content, $options) {
  $form['location'] = array(
    '#type' => 'fieldset',
    '#title' => t('Location coordinates'),
    '#description' => t('Enter first the latitude, and then the longitude.'),
    '#collapsible' => TRUE,
    '#collapsed' => (empty($content['latitude']) || empty($content['longitude'])) ? ((!empty($options['default']['location']['latitude']) && !empty($options['default']['location']['longitude'])) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['location']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => empty($content['latitude']) ? (!empty($options['default']['location']['latitude']) ? $options['default']['location']['latitude'] : '') : $content['latitude'],
    '#element_validate' => array('extra_metatags_location_form_validate'),
    '#size' => 14,
    '#maxlength' => 14,
    '#prefix' => '<div class="container-inline">',
    '#latitude' => TRUE,
  );

  $form['location']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => empty($content['longitude']) ? (!empty($options['default']['location']['longitude']) ? $options['default']['location']['longitude'] : '') : $content['longitude'],
    '#size' => 14,
    '#element_validate' => array('extra_metatags_location_form_validate'),
    '#maxlength' => 14,
    '#suffix' => '</div>',
  );
}

/**
 * Set the meta tag content.
 */
function extra_metatags_location_prepare(&$tags, $content, $options) {
  if ((empty($content['longitude']) || empty($content['longitude'])) && $options['type'] == 'node' && count($options['ids']) == 1 && module_exists('location')) {
    $node = node_load($options['ids'][0]);
    if ($node && isset($node->locations[0]['latitude']) && isset($node->locations[0]['longitude'])) {
      $content['latitude'] = $node->locations[0]['latitude'];
      $content['longitude'] = $node->locations[0]['longitude'];
    }
  }

  if (empty($content['latitude']) || empty($content['longitude'])) {
    $content['latitude'] = !empty($options['default']['location']['latitude']) ? $options['default']['location']['latitude'] : '';
    $content['longitude'] = !empty($options['default']['location']['longitude']) ? $options['default']['location']['longitude'] : '';
  }

  if (!empty($content['longitude']) && !empty($content['longitude'])) {
    $tags['location:geo.position'] = $content['latitude'] . ';' . $content['longitude'];
    $tags['location:icbm'] = $content['latitude'] . ',' . $content['longitude'];
  }
}

/**
 * Validate the values passed as latitude, and longitude.
 */
function extra_metatags_location_form_validate($element, &$form_state) {
  if (isset($element['#latitude'])) {
    if (!empty($element['#value']) && (!is_numeric($element['#value']) || $element['#value'] < -90.0 || $element['#value'] > 90.0)) {
      form_error($element, t('Latitude must be a number between -90.0 and 90.0 (extrems included).'));
    }
  }
  else {
    if (!empty($element['#value']) && (!is_numeric($element['#value']) || $element['#value'] < -180.0 || $element['#value'] > 180.0)) {
      form_error($element, t('Longitude must be a number between -180.0 and 180.0 (extrems included).'));
    }
  }
}

/*****************************************************************************
 * 'PICS' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function extra_metatags_pics_form(&$form, $content, $options) {
  $form['pics-label'] = array(
    '#type' => 'fieldset',
    '#title' => t('PICS labels'),
    '#description' => t('Enter the ICRA PICS labels only, without any extra characters; ICRA PICS labels should be something similar to %icra-pics-example. You can use the online <a href="@icra-label-generator">ICRA label generator</a>.', array('%icra-pics-example' => 'n 0 s 0 v 0 l 0 oa 0 ob 0 oc 0 od 0 oe 0 of 0 og 0 oh 0 c 1', '@icra-label-generator' => 'http://www.icra.org/label/generator/')),
    '#collapsible' => TRUE,
    '#collapsed' => empty($content['value']) ? (!empty($options['default']['pics-label']['value']) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['pics-label']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? (!empty($options['default']['pics-label']['value']) ? $options['default']['pics-label']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function extra_metatags_pics_prepare(&$tags, $content, $options) {
  if (empty($content['value'])) {
    $content['value'] = !empty($options['default']['pics-label']['value']) ? $options['default']['pics-label']['value'] : '';
  }

  if (!empty($content['value'])) {
    if ($options['type'] == 'node' && count($options['ids']) == 1) {
      $tags['pics-label'] = strtr(
        '(pics-1.1 "http://www.icra.org/pics/vocabularyv03/" l gen true for %url r (%pics))',
        array(
          '%url' => url("node/{$options['ids'][0]}", array('absolute' => TRUE)),
          '%pics' => $content['value'],
        )
      );
    }
  }
}

/**
 * @} End of "addtogroup extra_metatags".
 */
