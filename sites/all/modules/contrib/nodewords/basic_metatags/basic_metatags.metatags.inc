<?php
// $Id: basic_metatags.metatags.inc,v 1.1.2.18 2009/09/22 18:27:55 kiam Exp $

/**
 * @file
 * Support file for the meta tags added by Basic meta tags.
 */

/**
 * @addtogroup basic_metatags
 * @{
 */

/*****************************************************************************
 * 'abstract' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_abstract_form(&$form, $content, $options) {
  $form['abstract'] = array(
    '#type' => 'fieldset',
    '#title' => t('Abstract'),
    '#description' => t('Enter a short abstract. Typically it is one sentence.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($content['value']) ? (!empty($options['default']['abstract']['value']) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['abstract']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? (!empty($options['default']['abstract']['value']) ? $options['default']['abstract']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function basic_metatags_abstract_prepare(&$tags, $content, $options) {
  $tags['abstract'] = empty($content['value']) ? (!empty($options['default']['abstract']['value']) ? $options['default']['abstract']['value'] : '') : $content['value'];
}

/*****************************************************************************
 * 'canonical' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_canonical_form(&$form, $content, $options) {
  $form['canonical'] = array(
    '#type' => 'fieldset',
    '#title' => t('Canonical URL'),
    '#description' => t('Canonical URLs are used from the search engines, and allow them to not report duplicate titles for HTML pages that are accessible from different URLs. Use a relative URL without the initial slash; canonical URLs that point to a different domain are normally not accepted.'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($content['value']),
    '#tree' => TRUE,
  );

  $form['canonical']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#element_validate' => array('basic_metatags_canonical_form_validate'),
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function basic_metatags_canonical_prepare(&$tags, $content, $options) {
  if (empty($content['value'])) {
    $path = '';

    switch ($options['type']) {
      case 'frontpage':
        $content['value'] = '<front>';
        break;

      case 'node':
        if (count($options['ids']) == 1) {
          $path = 'node/' . $options['ids'][0];
        }
        break;

      case 'page':
        if (count($options['ids']) == 1 && !empty($options['ids'][0])) {
          $path = $options['ids'][0];
        }
        break;

      case 'term':
        if (count($options['ids']) == 1) {
          $path = 'taxonomy/term/' . $options['ids'][0];
        }
        break;

      case 'tracker':
        if (count($options['ids']) == 1) {
          if ($options['ids'][0] = -1) {
            $path = 'tracker';
          }
          elseif (is_numeric($options['ids'][0])) {
            $path = 'user/' . $options['ids'][0] . '/track';
          }
        }
        break;

      case 'user':
        if (count($options['ids']) == 1) {
          $path = 'user/' . $options['ids'][0];
        }
        break;
    }

    if ($path) {
      $content['value'] = drupal_get_path_alias($path);
    }
  }
  elseif (!empty($content['value']) && strpos($content['value'], '/') === 0) {
    $content = drupal_substr($content['value'], 1);
  }

  $tags['canonical'] = !empty($content['value']) ? check_url(url($content['value'], array('absolute' => TRUE))) : '';
}

function basic_metatags_canonical_form_validate($element, &$form_state) {
  if (!empty($element['#value']) && preg_match('%^(http://|https://|ftp://|mailto:|smb://|afp://|file://|gopher://|news://|ssl://|sslv2://|sslv3://|tls://|tcp://|udp://)%', $element['#value'])) {
    form_error($element, t('Canonical URL must be a relative URL.'));
  }
}

/*****************************************************************************
 * 'copyright' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_copyright_form(&$form, $content, $options) {
  $form['copyright'] = array(
    '#type' => 'fieldset',
    '#title' => t('Copyright'),
    '#description' => t('Enter a short copyright statement.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($content['value']) ? (!empty($options['default']['copyright']['value']) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['copyright']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? (!empty($options['default']['copyright']['value']) ? $options['default']['copyright']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function basic_metatags_copyright_prepare(&$tags, $content, $options) {
  $tags['copyright'] = empty($content['value']) ? (!empty($options['default']['copyright']['value']) ? $options['default']['copyright']['value'] : '') : $content['value'];
}

/*****************************************************************************
 * 'description' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_description_form(&$form, $content, $options) {
  $form['description'] = array(
    '#type' => 'fieldset',
    '#title' => t('Description'),
    '#description' => t('Enter a description. Limit your description to about 20 words, with a maximum of %count characters. It should not contain any HTML tags or other formatting.', array('%count' => variable_get('nodewords_max_size', 255))) . (variable_get('basic_metatags_use_teaser', FALSE) ? ' ' . t('When you leave this field empty, the teaser will be used as description.') : ''),
    '#collapsible' => TRUE,
    '#collapsed' => empty($content['value']) ? (!empty($options['default']['description']['value']) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['description']['value'] = array(
    '#type' => 'textarea',
    '#default_value' => empty($content['value']) ? (!empty($options['default']['description']['value']) ? $options['default']['description']['value'] : '') : $content['value'],
    '#cols' => 60,
    '#rows' => 6,
  );
}

/**
 * Set the meta tag content.
 */
function basic_metatags_description_prepare(&$tags, $content, $options) {
  if (empty($content['value']) && count($options['ids']) == 1) {
    switch ($options['type']) {
      case 'term':
        $term = nodewords_get_term($options['ids'][0]);
        if ($term) {
          $content['value'] = $term->description;
        }
        break;

      case 'vocabulary':
        // TODO: probably we have to do a db_rewrite_sql() query here so access is restricted
        $voc = taxonomy_vocabulary_load($options['ids'][0]);
        if ($voc) {
          $content['value'] = $voc->description;
        }
        break;
    }
  }

  $tags['description'] = empty($content['value']) ? (!empty($options['default']['description']['value']) ? $options['default']['description']['value'] : '') : $content['value'];
}

/*****************************************************************************
 * 'keywords' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_keywords_form(&$form, $content, $options) {
  $form['keywords'] = array(
    '#type' => 'fieldset',
    '#title' => t('Keywords'),
    '#description' => t('Enter a comma separated list of keywords. Avoid duplication of words as this will lower your search engine ranking.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($content['value']) ? (!empty($options['default']['keywords']['value']) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['keywords']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($content['value']) ? (!empty($options['default']['keywords']['value']) ? $options['default']['keywords']['value'] : '') : $content['value'],
    '#size' => 60,
    '#element_validate' => array('basic_metatags_keywords_form_validate'),
    '#maxlength' => variable_get('nodewords_max_size', 255),
  );
}

/**
 * Set the meta tag content.
 */
function basic_metatags_keywords_prepare(&$tags, $content, $options) {
  if (empty($content['value'])) {
    $content['value'] = (!empty($options['default']['keywords']['value']) ? $options['default']['keywords']['value'] : '');
  }

  if ($options['type'] == 'node' && module_exists('taxonomy') && count($options['ids']) == 1) {
    $node = node_load($options['ids'][0]);
    foreach (taxonomy_node_get_terms($node) as $term) {
      if (in_array($term->vid, variable_get('nodewords_keyword_vids', array()))) {
        if (empty($content['value'])) {
          $content['value'] = $term->name;
        }
        else {
          $content['value'] .= ',' . $term->name;
        }
      }
    }
  }
  elseif ($options['type'] == 'term') {
    $terms = array();

    foreach ($options['ids'] as $id) {
      $term = taxonomy_get_term($id);
      if ($term) {
        $terms[] = $term->name;
      }
    }

    if (count($terms)) {
      $content['value'] = implode(',', $terms);
    }
  }

  $tags['keywords'] = nodewords_unique($content['value']);
}

/**
 * Validate the values passed as keywords.
 */
function basic_metatags_keywords_form_validate($element, &$form_state) {
  if (trim($element['#value']) == ',') {
    form_error($element, t('Keywords must be a comma separated list of words.'));
  }
}

/*****************************************************************************
 * 'Revision-After' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_revisit_after_form(&$form, $content, $options) {
  $options = array(
    '1 day' => '1 day',
    '5 days' => '5 days',
    '10 days' => '10 days',
    '20 days' => '20 days',
    '30 days' => '1 month',
    '60 days' => '2 months',
  );

  $form['revisit-after'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revisit after'),
    '#description' => t('The Revisit-After META tag defines how often a search engine or spider should come to your website for re-indexing. This tag is used for websites that change their content and on a regular basis. This tag can also be beneficial in boosting your rankings if search engines display results based on the most recent submissions.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($content['value']) ? (!empty($options['default']['revisit-after']['value']) ? TRUE : FALSE) : TRUE,
    '#tree' => TRUE,
  );

  $form['revisit-after']['value'] = array(
    '#type' => 'select',
    '#default_value' => empty($content['value']) ? (!empty($options['default']['revisit-after']['value']) ? $options['default']['revisit-after']['value'] : '1') : $content['value'],
    '#options' => $options,
    '#multiple' => FALSE,
  );
}

/**
 * Set the meta tag content.
 */
function basic_metatags_revisit_after_prepare(&$tags, $content, $options) {
  $tags['revisit-after'] = empty($content['value']) ? (!empty($options['default']['revisit-after']['value']) ? $options['default']['revisit-after']['value'] : '1 day') : $content['value'];
}

/*****************************************************************************
 * 'robots' meta tag.
 ****************************************************************************/

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function basic_metatags_robots_form(&$form, $content, $options) {
  $field_options = array(
    'noindex' => t('NOINDEX'),
    'nofollow' => t('NOFOLLOW'),
    'noarchive' => t('NOARCHIVE'),
    'noopd' => t('NOOPD'),
    'nosnippet' => t('NOSNIPPET'),
    'noydir' => t('NOYDIR'),
  );

  $form['robots'] = array(
    '#type' => 'fieldset',
    '#title' => t('Robots'),
    '#description' => t('The ROBOTS meta tag offers a simple mechanism to indicate to web robots and crawlers whether the page should be indexed (INDEX or NOINDEX) and whether links on the page should be followed (FOLLOW or NOFOLLOW).'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($content['value']) ? count(array_filter($content['value'])) : 0,
    '#tree' => TRUE,
  );

  $form['robots']['value'] = array(
    '#type' => 'checkboxes',
    '#default_value' => empty($content['value']) ? array() : $content['value'],
    '#options' => $field_options,
    '#checkall' => TRUE,
  );

  if ($options['type'] != 'default') {
    $form['robots']['use_default'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the default value'),
      '#default_value' => !empty($content['use_default']),
    );

    if (!empty($options['default']['robots']['value'])) {
      $default_values = array_keys(array_filter($options['default']['robots']['value']));
    }

    if (!empty($default_values)) {
      $description = t(
        'The default value is currently %default_values.',
        array('%default_values' => implode(', ', $default_values))
      );
    }
    else {
      $description = t('The default value is currently not set.');
    }

    $form['robots']['use_default']['#description'] = $description;
  }
}

/**
 * Set the meta tag content.
 */
function basic_metatags_robots_prepare(&$tags, $content, $options) {
  if (empty($content['value']) && !empty($content['use_default'])) {
    $content['value'] = !empty($options['default']['robots']['value']) ? $options['default']['robots']['value'] : $default_value;
  }

  if (!empty($content['value'])) {
    $value = array_keys(array_filter($content['value']));
    if (!empty($value)) {
      $tags['robots'] = implode(', ', $value);
    }
  }
}

/**
 * @} End of "addtogroup basic_metatags".
 */
