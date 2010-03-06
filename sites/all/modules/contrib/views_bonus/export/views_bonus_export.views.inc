<?php
// $Id: views_bonus_export.views.inc,v 1.9 2010/01/08 20:55:21 neclimdul Exp $
/**
 * @file
 * Views include file with views hooks.
 */

/**
 * Implementation of hook_views_plugins().
 */
function views_bonus_export_views_plugins() {
  $path = drupal_get_path('module', 'views_bonus_export');
  return array(
    'style' => array(
      'views_bonus_export' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'views_bonus_plugin_style_export',
        'path' => $path,
      ),
      'views_csv' => array(
        'title' => t('CSV file'),
        'help' => t('Display the view as a comma seperated list.'),
        'path' => $path,
        'handler' => 'views_bonus_plugin_style_export_csv',
        'parent' => 'views_bonus_export',
        'theme' => 'views_bonus_export_csv',
        'theme file' => 'views_bonus_export.theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        // Views Bonus Export element that will be used to set additional headers when serving the feed.
        'export headers' => array('Content-type: text/csv; charset=utf-8'),
        // Views Bonus Export element mostly used for creating some additional classes and template names.
        'export feed type' => 'csv',
      ),
      'views_doc' => array(
        'title' => t('DOC file'),
        'help' => t('Display the view as a doc file.'),
        'path' => $path,
        'handler' => 'views_bonus_plugin_style_export_doc',
        'parent' => 'views_bonus_export',
        'theme' => 'views_bonus_export_doc',
        'theme file' => 'views_bonus_export.theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'export headers' => array('Content-Type: application/msword'),
        'export feed type' => 'doc',
      ),
      'views_txt' => array(
        'title' => t('TXT file'),
        'help' => t('Display the view as a txt file.'),
        'path' => $path,
        'handler' => 'views_bonus_plugin_style_export_txt',
        'parent' => 'views_bonus_export',
        'theme' => 'views_bonus_export_txt',
        'theme file' => 'views_bonus_export.theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'export headers' => array('Content-Type: text/plain'),
        'export feed type' => 'txt',
      ),
      'views_xls' => array(
        'title' => t('XLS file'),
        'help' => t('Display the view as a xls file.'),
        'path' => $path,
        'handler' => 'views_bonus_plugin_style_export_xls',
        'parent' => 'views_bonus_export',
        'theme' => 'views_bonus_export_xls',
        'theme file' => 'views_bonus_export.theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'export headers' => array('Content-Type: application/vnd.ms-excel'),
        'export feed type' => 'xls',
      ),
      'views_xml' => array(
        'title' => t('XML file'),
        'help' => t('Display the view as a txt file.'),
        'path' => $path,
        'handler' => 'views_bonus_plugin_style_export_xml',
        'parent' => 'views_bonus_export',
        'theme' => 'views_bonus_export_xml',
        'theme file' => 'views_bonus_export.theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'export headers' => array('Content-Type: text/xml'),
        'export feed type' => 'xml',
      ),
    ),
  );
}
