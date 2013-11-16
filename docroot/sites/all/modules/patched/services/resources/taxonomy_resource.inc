<?php
/**
 * @file
 *  Link general taxonomy functionalities to services module.
 */

function _taxonomy_resource_definition() {
  if (!module_exists('taxonomy')) {
    return array();
  }

  return array(
    'taxonomy_term' => array(
      'operations' => array(
        'retrieve' => array(
          'callback' => '_taxonomy_term_resource_retrieve',
          'args' => array(
            array(
              'name' => 'tid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The tid of the taxonomy term to get',
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('view'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'create' => array(
          'callback' => '_taxonomy_term_resource_create',
          'args' => array(
            array(
              'name' => 'term',
              'optional' => FALSE,
              'source' => 'data',
              'description' => 'The taxonomy term array to create',
              'type' => 'array',
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('create'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'update' => array(
          'callback' => '_taxonomy_term_resource_update',
          'args' => array(
            array(
              'name' => 'tid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'description' => 'The unique identifier for this taxonomy term.',
              'type' => 'int',
            ),
            array(
              'name' => 'term',
              'optional' => FALSE,
              'source' => 'data',
              'description' => 'The taxonomy term data to update',
              'type' => 'array',
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('update'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'delete' => array(
          'callback' => '_taxonomy_term_resource_delete',
          'args' => array(
            array(
              'name' => 'tid',
              'optional' => FALSE,
              'source' => array('path' => 0),
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('delete'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'index' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
          'callback' => '_taxonomy_term_resource_index',
          'args' => array(
            array(
              'name' => 'page',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'The zero-based index of the page to get, defaults to 0.',
              'default value' => 0,
              'source' => array('param' => 'page'),
            ),
            array(
              'name' => 'fields',
              'optional' => TRUE,
              'type' => 'string',
              'description' => 'The fields to get.',
              'default value' => '*',
              'source' => array('param' => 'fields'),
            ),
            array(
              'name' => 'parameters',
              'optional' => TRUE,
              'type' => 'array',
              'description' => 'Parameters',
              'default value' => array(),
              'source' => array('param' => 'parameters'),
            ),
            array(
              'name' => 'pagesize',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'Number of records to get per page.',
              'default value' => variable_get('services_taxonomy_term_index_page_size', 20),
              'source' => array('param' => 'pagesize'),
            ),
          ),
          'access arguments' => array('access content'),
        ),
      ),
      'actions' => array(
        'selectNodes' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
          'help'   => t('This method returns all nodes with provided taxonomy id.'),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('view'),
          'access arguments append' => TRUE,
          'callback' => 'taxonomy_service_select_nodes',
          'args'     => array(
            array(
              'name'         => 'tids',
              'type'         => 'string',
              'description'  => t('The vocabulary ids to retrieve, separated by comma.'),
              'source'       => array('data' => 'tids'),
              'optional'     => FALSE,
            ),
            array(
              'name'          => 'fields',
              'type'          => 'string',
              'description'   => t('The fields to retrieve, separated by comma.'),
              'source'        => array('data' => 'fields'),
              'optional'      => TRUE,
              'default value' => array(),
            ),
            array(
              'name'          => 'operator',
              'type'          => 'string',
              'description'   => t('How to interpret multiple IDs in the array. Can be "or" or "and".'),
              'source'        => array('data' => 'operator'),
              'optional'      => TRUE,
              'default value' => 'or',
            ),
            array(
              'name'          => 'depth',
              'type'          => 'int',
              'description'   => t('How many levels deep to traverse the taxonomy tree. Can be a nonnegative integer or "all".'),
              'source'        => array('data' => 'depth'),
              'optional'      => TRUE,
              'default value' => 0,
            ),
            array(
              'name'          => 'pager',
              'type'          => 'int',
              'description'   => t('Whether the nodes are to be used with a pager (the case on most Drupal pages) or not (in an XML feed, for example).'),
              'source'        => array('data' => 'pager'),
              'optional'      => TRUE,
              'default value' => TRUE,
            ),
            array(
              'name'          => 'order',
              'type'          => 'string',
              'description'   => t('The order clause for the query that retrieve the nodes.'),
              'source'        => array('data' => 'order'),
              'optional'      => TRUE,
              'default value' => 'n.sticky DESC, n.created DESC',
            ),
          ),
        ),
      ),
    ),
    'taxonomy_vocabulary' => array(
      'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
      'operations' => array(
        'retrieve' => array(
          'callback' => '_taxonomy_vocabulary_resource_retrieve',
          'args' => array(
            array(
              'name' => 'vid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The vid of the taxonomy vocabulary to get',
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('view'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'create' => array(
          'callback' => '_taxonomy_vocabulary_resource_create',
          'args' => array(
            array(
              'name' => 'vocabulary',
              'optional' => FALSE,
              'source' => 'data',
              'description' => 'The taxonomy vocabulary array to create',
              'type' => 'array',
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('create'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'update' => array(
          'callback' => '_taxonomy_vocabulary_resource_update',
          'args' => array(
            array(
              'name' => 'vid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'description' => 'The unique identifier for this taxonomy vocabulary.',
              'type' => 'int',
            ),
            array(
              'name' => 'vocabulary',
              'optional' => FALSE,
              'source' => 'data',
              'description' => 'The taxonomy vocabulary data to update',
              'type' => 'array',
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('update'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'delete' => array(
          'callback' => '_taxonomy_vocabulary_resource_delete',
          'args' => array(
            array(
              'name' => 'vid',
              'optional' => FALSE,
              'source' => array('path' => 0),
            ),
          ),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('delete'),
          'access arguments append' => TRUE,
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
        ),
        'index' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
          'callback' => '_taxonomy_vocabulary_resource_index',
          'args' => array(
            array(
              'name' => 'page',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'The zero-based index of the page to get, defaults to 0.',
              'default value' => 0,
              'source' => array('param' => 'page'),
            ),
            array(
              'name' => 'fields',
              'optional' => TRUE,
              'type' => 'string',
              'description' => 'The fields to get.',
              'default value' => '*',
              'source' => array('param' => 'fields'),
            ),
            array(
              'name' => 'parameters',
              'optional' => TRUE,
              'type' => 'array',
              'description' => 'Parameters',
              'default value' => array(),
              'source' => array('param' => 'parameters'),
            ),
            array(
              'name' => 'pagesize',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'Number of records to get per page.',
              'default value' => variable_get('services_taxonomy_vocabulary_index_page_size', 20),
              'source' => array('param' => 'pagesize'),
            ),
          ),
          'access arguments' => array('access content'),
        ),
      ),
      'actions' => array(
        'getTree' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/taxonomy_resource'),
          'help'   => t('This method returns a full list of taxonomy terms.'),
          'access callback' => '_taxonomy_resource_access',
          'access arguments' => array('view'),
          'access arguments append' => TRUE,
          'callback' => 'taxonomy_service_get_tree',
          'args'     => array(
            array(
              'name'         => 'vid',
              'type'         => 'int',
              'description'  => t('The vocabulary id to retrieve.'),
              'source'       => array('data' => 'vid'),
              'optional'     => FALSE,
            ),
            array(
              'name'          => 'parent',
              'type'          => 'int',
              'description'   => t('The term ID under which to generate the tree. If 0, generate the tree for the entire vocabulary.'),
              'source'        => array('data' => 'parent'),
              'default value' => 0,
              'optional'      => TRUE,
            ),
            array(
              'name'          => 'maxdepth',
              'type'          => 'int',
              'description'   => t('The number of levels of the tree to return. Leave NULL to return all levels.'),
              'source'        => array('data' => 'maxdepth'),
              'default value' => NULL,
              'optional'      => TRUE,
            ),
          ),
        ),
      ),
    ),
  );
}
/**
 * Return an array of optionally paged tids baed on a set of criteria.
 *
 * An example request might look like
 *
 * http://domain/endpoint/taxonomy_term?fields=tid,name&parameters[tid]=7&parameters[vid]=1
 *
 * This would return an array of objects with only tid and name defined, where
 * tid = 7 and vid = 1.
 *
 * @param $page
 *   Page number of results to return (in pages of 20).
 * @param $fields
 *   The fields you want returned.
 * @param $parameters
 *   An array containing fields and values used to build a sql WHERE clause
 *   indicating items to retrieve.
 * @param $page_size
 *   Integer number of items to be returned.
 * @return
 *   An array of term objects.
 *
 * @see _node_resource_index()
 */
function _taxonomy_term_resource_index($page, $fields, $parameters, $page_size) {
  $query = services_resource_build_index_query('term_data', 't.vid DESC, t.weight DESC, t.name DESC', $page, $fields, $parameters, 't', 'tid', $page_size, 'taxonomy_term');

  $results = array();
  while ($taxonomy = db_fetch_object($query)) {
    $results[] = $taxonomy;
  }
  return services_resource_build_index_list($results, 'taxonomy_term', 'tid');
}

/**
 * Return an array of optionally paged vids baed on a set of criteria.
 *
 * An example request might look like
 *
 * http://domain/endpoint/taxonomy_vocabulary?fields=vid,name&parameters[vid]=2
 *
 * This would return an array of objects with only vid and name defined, where
 * vid = 2.
 *
 * @param $page
 *   Page number of results to return (in pages of 20).
 * @param $fields
 *   The fields you want returned.
 * @param $parameters
 *   An array containing fields and values used to build a sql WHERE clause
 *   indicating items to retrieve.
 * @param $page_size
 *   Integer number of items to be returned.
 * @return
 *   An array of vocabulary objects.
 *
 * @todo
 *   Support node types as parameters.
 *
 * @see _node_resource_index()
 */
function _taxonomy_vocabulary_resource_index($page, $fields, $parameters, $page_size) {
  $query = services_resource_build_index_query('vocabulary', 'v.weight DESC, v.name DESC', $page, $fields, $parameters, 'v', 'vid', $page_size,'taxonomy_term');

  $results = array();
  while ($taxonomy = db_fetch_object($query)) {
    $results[] = $taxonomy;
  }
  return services_resource_build_index_list($results, 'taxonomy_vocabulary', 'vid');
}

/**
 * Return the results of taxonomy_get_term() for a specified term id.
 *
 * @param $tid
 *   Unique identifier for the taxonomy term to retrieve.
 * @return
 *   A term object.
 *
 * @see taxonomy_get_term()
 */
function _taxonomy_term_resource_retrieve($tid) {
  return taxonomy_get_term($tid);
}

/**
 * Create a new taxonomy term based on submitted values.
 *
 * @param $term
 *   Array of values for the taxonomy term.
 * @return
 *   Status constant indicating if term was inserted or updated.
 *
 * @see taxonomy_save_term()
 */
function _taxonomy_term_resource_create($term) {
  // Adds backwards compatability with regression fixed in #1083242
  $term = _services_arg_value($term, 'term');

  return taxonomy_save_term($term);
}

/**
 * Update a taxonomy term based on submitted values.
 *
 * @param $tid
 *   Unique identifier for the taxonomy term to update.
 * @param $term
 *   Array of values for the taxonomy term.
 * @return
 *   Status constant indicating if term was inserted or updated.
 *
 * @see taxonomy_save_term()
 */
function _taxonomy_term_resource_update($tid, $term) {
  // Adds backwards compatability with regression fixed in #1083242
  $term = _services_arg_value($term, 'term');

  $term['tid'] = $tid;
  return _taxonomy_term_resource_create($term);
}

/**
 * Delete a taxonomy term.
 *
 * @param $tid
 *   Unique identifier for the taxonomy term to delete.
 * @return
 *   Status constant indicating deletion.
 *
 * @see taxonomy_del_term()
 */
function _taxonomy_term_resource_delete($tid) {
  return taxonomy_del_term($tid);
}


/**
 * Return the results of taxonomy_vocabulary_load() for a specified vocabulary id.
 *
 * @param $vid
 *   Unique identifier for the taxonomy term to retrieve.
 * @return
 *   A vocabulary object or FALSE.
 *
 * @see taxonomy_vocabulary_load()
 */
function _taxonomy_vocabulary_resource_retrieve($vid) {
  return taxonomy_vocabulary_load($vid);
}

/**
 * Create a new taxonomy vocabulary based on submitted values.
 *
 * Here is a sample vocabulary array, taken from
 * http://drupaldeveloper.in/article/programmatically-create-vocabulary
 *
 * @code
 * $vocabulary = array(
 *   'name'  => t("Name"). // Human readable name of the vocabulary
 *   'description' => t("Description"), // extended description of the vocabulary
 *   'help' => t("help"), // help text
 *   'tags' => 0, // 1 to make this vocabulary free tagging
 *   'multiple' => 0, // 1 to allow multiple selection
 *   'required' => 0, // 1 to make the terms mandatory to be selected
 *   'hierarchy' => 0, // 1 to allow and create hierarchy of the terms within the vocabulary
 *   'relations' => 0, // 1 to set and allow relation amongst multiple terms
 *   'module' => 'mymodule', // provide the module name in which the vocabulary is defined and which is calling this function
 *   'node' => array('story' => 1), // content types to which this vocabulary will be attached to
 *   'weight' => -9, // set the weight to display the vocabulary in the list
 * );
 * @endcode
 *
 * @param $vocabulary
 *   Array of values for the taxonomy vocabulary.
 * @return
 *   Status constant indicating if vocabulary was inserted or updated.
 *
 * @see taxonomy_save_vocabulary()
 */
function _taxonomy_vocabulary_resource_create($vocabulary) {
  // Adds backwards compatability with regression fixed in #1083242
  $vocabulary = _services_arg_value($vocabulary, 'vocabulary');
  taxonomy_save_vocabulary($vocabulary);
  return $vocabulary;
}

/**
 * Update a taxonomy vocabulary based on submitted values.
 *
 * @param $vid
 *   Unique identifier for the taxonomy term to retrieve.
 * @param $vocabulary
 *   Array of values for the taxonomy vocabulary.
 * @return
 *   Status constant indicating if vocabulary was inserted or updated.
 *
 * @see taxonomy_save_vocabulary()
 */
function _taxonomy_vocabulary_resource_update($vid, $vocabulary) {
  // Adds backwards compatability with regression fixed in #1083242
  $vocabulary = _services_arg_value($vocabulary, 'vocabulary');

  $vocabulary['vid'] = $vid;
  return _taxonomy_vocabulary_resource_create($vocabulary);
}

/**
 * Delete a taxonomy vocabulary.
 *
 * @param $vid
 *   Unique identifier for the taxonomy term to retrieve.
 * @return
 *   Status constant indicating deletion.
 *
 * @see taxonomy_del_vocabulary()
 */
function _taxonomy_vocabulary_resource_delete($vid) {
  return taxonomy_del_vocabulary($vid);
}



/**
 * Services interface to taxonomy_get_tree().
 *
 * @see taxonomy_get_tree()
 */
function taxonomy_service_get_tree($vid, $parent = 0, $max_depth = NULL) {
  $terms = taxonomy_get_tree($vid, $parent, -1, $max_depth);
  if (empty($terms)) {
    return services_error('No vocabulary with id: '. $vid .' found.', 404);
  }
  return $terms;
}

/**
 * Services interface to taxonomy_select_nodes().
 *
 * Note that where taxonomy_select_nodes() returns the results
 * of a db_query(), this function returns an array of node objects.
 *
 * @see taxonomy_select_nodes()
 * @return
 *   An array of node objects.
 */
function taxonomy_service_select_nodes($tids, $fields, $operator, $depth, $pager, $order) {
  if (!is_array($tids)) {
    $tids =  explode(',', $tids);
  }
  if (!is_array($fields)) {
    $fields =  explode(',', $fields);
  }
  $result = taxonomy_select_nodes($tids, $operator, $depth, $pager, $order);
  while ($node = db_fetch_object($result)) {
    $nodes[] = services_node_load(node_load($node->nid), $fields);
  }
  if (empty($nodes)) {
    return services_error('No nodes were found with tid(s): '. implode(",", $tids), 404);
  }
  return $nodes;
}

function _taxonomy_resource_access($op = 'view', $args = array()) {
  if (user_access('administer taxonomy') || user_access('get taxonomy tree')) {
    return TRUE;
  }
  // TODO - check perms of user to return ability to interact with terms / vocabulary
}

