<?php


/**
 * Implementation of hook_views_plugins().
 */
function views_periodic_execution_views_plugins() {
  return array(
    'display' => array(
      'views_periodic_execution' => array(
        'title' => t('Periodic execution'),
        'help' => t("The view will be executed periodically."),
        'handler' => 'views_periodic_execution_plugin_display',
        'theme' => 'views_view',
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Periodic execution'),
        // Don't alow direct instances of this display:
        'no ui' => TRUE,
      ),
    ),
  );
}
