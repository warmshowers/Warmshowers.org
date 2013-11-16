<?php

/**
 * Implementation of hook_views_data().
 */
function i18nviews_views_data() {
  $data['node']['content_negotiation'] = array(
    'group' => t('Node translation'),
    'title' => t('Content negotiation'),
    'help' => t('Removes the nodes that are not valid according to the content selection mode.'),
    'filter' => array(
      'handler' => 'i18nviews_content_negotiation_filter_handler',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function i18nviews_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'i18nviews') . '/includes',
    ),
    'handlers' => array(
      // field handlers.
      'i18nviews_handler_field_taxonomy' => array(
         'parent' => 'views_handler_field_taxonomy',
      ),
      'i18nviews_handler_field_term_description' => array(
         'parent' => 'views_handler_field_markup',
      ),
      'i18nviews_handler_field_term_node_tid' => array(
         'parent' => 'views_handler_field_term_node_tid',
      ),
      'i18nviews_handler_field_term_language' => array(
        'parent' => 'views_handler_field_taxonomy',
      ),
      // argument handlers.
      'i18nviews_handler_argument_term_language' => array(
        'parent' => 'views_handler_argument',
      ),
      'i18nviews_handler_argument_term_node_tid' => array(
        'parent' => 'views_handler_argument_term_node_tid',
      ),
      'i18nviews_handler_argument_term_node_tid_depth' => array(
        'parent' => 'views_handler_argument_term_node_tid_depth',
      ),
      // filter handlers.
      'i18nviews_content_negotiation_filter_handler' => array(
        'parent' => 'views_handler_filter',
      ),
      'i18nviews_handler_filter_term_language' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'i18nviews_handler_filter_term_node_tid' => array(
         'parent' => 'views_handler_filter_term_node_tid',
      ),
    ),
  );
}


/**
 * Implemenation of hook_views_data_alter().
 */
function i18nviews_views_data_alter(&$data) {
  $data['term_data']['name']['field']['handler'] = 'i18nviews_handler_field_taxonomy';
  $data['term_data']['description']['field']['handler'] = 'i18nviews_handler_field_term_description';
  $data['term_node']['tid']['field']['handler'] = 'i18nviews_handler_field_term_node_tid';
  $data['term_node']['tid']['argument']['handler'] = 'i18nviews_handler_argument_term_node_tid';
  $data['term_node']['tid']['filter']['handler'] = 'i18nviews_handler_filter_term_node_tid';
  $data['node']['term_node_tid_depth']['argument']['handler'] = 'i18nviews_handler_argument_term_node_tid_depth';

  // Add translation information to the term_data table.
  $data['term_data']['language'] = array(
    'group' => t('Term translation'),
    'title' => t('Language'),
    'help' => t('The language the term is in.'),
    'field' => array(
      'handler' => 'i18nviews_handler_field_term_language',
     'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'i18nviews_handler_filter_term_language',
    ),
    'argument' => array(
      'handler' => 'i18nviews_handler_argument_term_language',
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function i18nviews_views_plugins() {
  $path = drupal_get_path('module', 'i18nviews') . '/includes';
  return array(
    'module' => 'i18nviews',
    'localization' => array(
      'i18nstrings' => array(
        'title' => t('Views translation module'),
        'help' => t("Use the locale system as implemented by the Views translation module."),
        'handler' => 'views_plugin_localization_i18nstrings',
        'path' => $path,
      ),
    ),
    'argument validator' => array(
      'i18n_taxonomy_term' => array(
        'title' => t('Taxonomy term (i18n)'),
        'help' => t("Use the locale to interpret arguments."),
        'handler' => 'i18nviews_plugin_argument_validate_i18n_taxonomy_term',
        'path' => $path,
        'parent' => 'taxonomy_term',
      ),
    ),
  );
}
