<?php

/**
 * @file
 * Theme functions for the uc_file module.
 */

/**
 * Themes the download table at the user account page.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @return string
 *   The HTML output.
 *
 * @ingroup themeable
 */
function theme_uc_file_hook_user_file_downloads($variables) {
  $form = $variables['form'];

  $header = array(
    array('data' => t('Remove'    )),
    array('data' => t('Filename'  )),
    array('data' => t('Expiration')),
    array('data' => t('Downloads' )),
    array('data' => t('Addresses' )),
  );
  $rows = array();
  $output = '';

  foreach (element_children($form['file_download']) as $key) {

    if (!isset($form['file_download'][$key]['addresses_in'])) {
      continue;
    }

    $file_download = &$form['file_download'][$key];

    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($file_download['remove'])),

        array('data' => drupal_render($file_download['filename'])),

        array(
          'data' =>
            drupal_render($file_download['expires']) . ' <br />' .

            '<div class="duration">' .
              drupal_render($file_download['time_polarity']) .
              drupal_render($file_download['time_quantity']) .
              drupal_render($file_download['time_granularity']) .
            '</div>',
        ),

        array(
          'data' =>
            '<div class="download-table-index">' .
              drupal_render($file_download['downloads_in']) . '/' . drupal_render($file_download['download_limit']) .
            '</div>',
        ),

        array(
          'data' =>
            '<div class="download-table-index">' .
              drupal_render($file_download['addresses_in']) . '/' . drupal_render($file_download['address_limit']) .
            '</div>',
        ),
      ),
      'class' => array('download-table-row'),
    );
  }

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'download-table'),
    'empty' => t('No files can be downloaded by this user.'),
  ));
  $output .= drupal_render_children($form);

  return $output;
}
