<?php

function i18nviews_views_data() {
  $data['node']['content_negotiation'] = array(
    'group' => t('i18n'),
    'title' => t('Content negotiation'),
    'help' => t('Removes the nodes that are not valid according to the content selection mode'),
    'filter' => array(
      'handler' => 'content_negotiation_filter_handler',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers
 */
function i18nviews_views_handlers() {
  return array(
    'handlers' => array(
      'content_negotiation_filter_handler' => array(
        'parent' => 'views_handler_filter',
        'file' => 'includes/content_negotiation_filter_handler.inc'
      ),
    ),
  );
}
