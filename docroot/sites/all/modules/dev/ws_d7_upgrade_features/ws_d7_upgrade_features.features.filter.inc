<?php
/**
 * @file
 * ws_d7_upgrade_features.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function ws_d7_upgrade_features_filter_default_formats() {
  $formats = array();

  // Exported format: Filtered HTML.
  $formats[1] = array(
    'format' => 1,
    'name' => 'Filtered HTML',
    'cache' => 1,
    'status' => 1,
    'weight' => -1,
    'filters' => array(
      'filter_html' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <b> <i> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd><img> <div> <p>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_autop' => array(
        'weight' => 1,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 99,
        ),
      ),
    ),
  );

  // Exported format: Full HTML (WYSIWYG).
  $formats[3] = array(
    'format' => 3,
    'name' => 'Full HTML (WYSIWYG)',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_htmlcorrector' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: Full html with line breaks.
  $formats[4] = array(
    'format' => 4,
    'name' => 'Full html with line breaks',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_autop' => array(
        'weight' => 1,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: markdown.
  $formats[5] = array(
    'format' => 5,
    'name' => 'markdown',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_markdown' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_autop' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -45,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: Email Filter.
  $formats[6] = array(
    'format' => 6,
    'name' => 'Email Filter',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_autop' => array(
        'weight' => -10,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_html' => array(
        'weight' => -9,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <b> <i> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd><img> <div> <table> <tr> <td> <th>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_url' => array(
        'weight' => -8,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 100,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -7,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: plain text old.
  $formats[7] = array(
    'format' => 7,
    'name' => 'plain text old',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_html' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_html_escape' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_autop' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: Plain text.
  $formats[8] = array(
    'format' => 8,
    'name' => 'Plain text',
    'cache' => 1,
    'status' => 1,
    'weight' => 1,
    'filters' => array(
      'filter_html_escape' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => 1,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'filter_autop' => array(
        'weight' => 2,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
