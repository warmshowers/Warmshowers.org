<?php
/**
 * @file.
 * Nodewords hooks.
 */

/**
 * The hook is used from nodewords.module to know which API is supported by the
 * the module.
 *
 * @return
 * An array containing the following indexes:
 *
 *   - version - the API version used by the module; basing on this value
 *     Nodewords will take the necessary steps to assure to keep the module
 *     compatible with Nodewords, The minimum API currently supported by the
 *     module is contained in the constant NODEWORDS_MINIMUM_API_VERSION, and
 *     the current API version is contained in the constant
 *     NODEWORDS_API_VERSION.
 */
function hook_nodewords_api() {
  return array('version' => '1.14');
}

/**
 * This hook declares the meta tags implemented by the module.
 *
 * @return
 *   A nested array containing the following values:
 *
 *  - attributes - the tag attributes used when outputting the tag.
 *  - callback - the string used to built the name of the functions called for 
 *    any meta tags operations.
 *  - context - the contexts in which the meta tags are allowed (and denied).
 *  - label - the label used as title in the fieldset for the form field
 *    shown in the form to edit the meta tags values.
 *  - multiple - if set to TRUE, splits the tag value on each line break and
 *    outputs each item as a fully separate copy of the tag; best used when the
 *    form uses a textarea instead of a textfield.
 *  - permission - the permission associated with the form fields used to
 *    edit the meta tags values; this permission is used only when the meta tag
 *    edit form fields are shown in a form that is accessible not only from the
 *    administrators users (in example, in the node edit form, or in the user
 *    profile form).
 *  - templates - the templates used when the meta tag is output.
 *  - weight - the weight used to order the meta tags before to output them;
 *    the lighter meta tag will be output first. See API.txt for a list of the
 *    weights of all included meta tags.
 */
function hook_nodewords_tags_info() {
  $tags = array(
    'dcterms.title' => array(
      'callback' => 'nodewords_extra_dc_title',
      'context' => array(
        'denied' => array(
          NODEWORDS_TYPE_DEFAULT,
        ),
      ),
      'label' => t('Dublin Core title'),
      'permission' => 'edit meta tag Dublin Core TITLE',
      'templates' => array(
        'head' => array(
          'dcterms.title' => NODEWORDS_META,
        ),
      ),
    ),
    'location' => array(
      'callback' => 'nodewords_extra_location',
      'label' => t('Location'),
      'permission' => 'edit location meta tag',
      'templates' => array(
        'geo.position' => NODEWORDS_META,
        'icbm' => NODEWORDS_META,
      ),
    ),
  );

  return $tags;
}

/**
 * The hook is used to alter the metatags content.
 *
 * @param &$output
 *  The array of meta tags values.
 * @param $parameters
 *  An array of parameters. The currently defined are:
 *   * type - the type of object for the page to which the meta
 *     tags are associated.
 *   * ids - the array of IDs for the object associated with the page.
 *   * output - where the meta tags are being output; the parameter value can
 *     'head' or 'update index'.
 */
function hook_nodewords_tags_alter(&$output, $parameters) {
  if (empty($output['abstract']) && $parameters['type'] == NODEWORDS_TYPE_PAGE) {
    $output['abstract'] = t('Node content');
  }
}

/**
 * The hook is used to alter the string containing the metatags output.
 *
 * @param &$output
 *  The string to alter.
 * @param $parameters
 *  An array of parameters. The currently defined are:
 *   * type - the type of object for the page to which the meta
 *     tags are associated.
 *   * ids - the array of IDs for the object associated with the page.
 *   * output - where the meta tags are being output; the parameter value can
 *     'head' or 'update index'.
 */
function hook_nodewords_tags_output_alter(&$output, $parameters) {
  $bool = (
    variable_get('nodewords_add_dc_schema', FALSE) &&
    isset($parameters['output']) &&
    $parameters['output'] == 'head'
  );

  if ($bool) {
    $output = (
      '<link rel="schema.dc" href="http://purl.org/dc/elements/1.1/" />' . "\n" .
      $output
    );
  }
}

/**
 * The hook is used from the module to determinate the type of the object
 * associated with the currently viewed page (node, user, taxonomy term), and
 * the ID of the object.
 *
 * @param &$result
 *   the array used to write the result.
 * @param $arg
 *   the array as obtained from arg().
 */
function hook_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'user') {
    // User page paths: user/$uid.
    if (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_USER;
      $result['id'] = $arg[1];
    }
  }
}

/**
 * @} End of "addtogroup Nodewords hooks" .
 */
