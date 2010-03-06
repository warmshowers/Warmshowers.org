<?php

// $Id: nodewords.api.php,v 1.1.2.8 2009/12/28 21:28:17 kiam Exp $

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
 *     compatible with Nodewords (Nodewords actually uses the meta tags
 *     implemented by a module that supports version 1.1, or 1.3 of
 *     Nodewords API).
 *
 */
function hook_nodewords_api() {
  return array('version' => '1.1');
}

/**
 * This hook declares the meta tags implemented by the module.
 *
 *
 * @return
 *   An array containing the following values:
 *
 *  - tag:context:allowed, tag:context:denied - these indexes define in which
 *    contexts the meta tags are allowed (and denied).
 *
 *  - tag:db:type - if the value of this index is not equal to string, the value
 *    of the meta tag is passed to serialize before to be saved in the database
 *    table used by Nodewords.
 *
 *  - tag:function:prefix - the prefix used when Nodewords looks for some
 *    functions it uses; the actual implementation uses the following functions
 *    (<suffix> stays for the value of this array index):
 *
 *    - <suffix>_form(&$form, $content, $options) - the function is used to
 *      populate the form used to edit the meta tags.
 *
 *    - <suffix>_prepare(&$tags, $content, $options) - the function is used to
 *      populate the array of meta tags before they are output in the page
 *      template.
 *
 *    - <suffix>_settings_form(&$form, $form_id, $options) - the function is
 *      used to populate a settings form; actually, the function is used to
 *      populate the Nodewords settings page ($form_id == 'nodewords_settings_form'),
 *      or the content type settings page ($form_id == 'node_type_form').
 *
 *  - tag:function:parameters - an array passed to the functions used by
 *    Nodewords as $options['parameters'] ($options is the last parameter
 *    passed to the functions.
 *
 *  - tag:template - the string used as template when the meta tag is added to
 *    the HTML tag HEAD; the value can be a constant as defined by the module, or
 *    a string.
 *
 *  - tag:weight - the weight used to order the meta tags before to output them
 *    in HTML; the lighter meta tag will be output first.
 *
 *  - widget:label - the label used as title in the fieldset for the form field
 *    shown in the form to edit the meta tags values.
 *
 *  - widget:permission - the permission associated with the form fields used to
 *    edit the meta tags values; this permission is used only when the meta tag
 *    edit form fields are shown in a form that is accessible not only from the
 *    administrators users (in example, in the node edit form, or in the user
 *    profile form).
 *
 */
function hook_nodewords_tags_info() {
  $tags = array(
    'dc.title' => array(
      'tag:context:denied' => array(NODEWORDS_TYPE_DEFAULT),
      'tag:db:type' => 'string',
      'tag:function:prefix' => 'test_metatags_dc_title',
      'tag:template' => array('dc.title' => NODEWORDS_META),
      'widget:label' => t('Dublin Core title'),
      'widget:permission' => 'edit meta tag Dublin Core TITLE',
    ),
    'location' => array(
      'tag:context:allowed' => array(NODEWORDS_TYPE_NODE, NODEWORDS_TYPE_USER),
      'tag:db:type' => 'array',
      'tag:function:prefix' => 'test_metatags_location',
      'tag:template' => array(
      'geo.position' => NODEWORDS_META,
      'icbm' => NODEWORDS_META,
    ),
    'widget:label' => t('Location'),
      'widget:permission' => 'edit location meta tag',
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
 * @param $arg
 *   the array as obtained from arg().
 *
 * @return
 *   An array containing the type of the object, and an array of IDs.
 */
function hook_nodewords_type_id($arg) {
  if ($arg[0] == 'tracker') {
    return array(
      NODEWORDS_TYPE_TRACKER, array(
        isset($arg[1]) && is_numeric($arg[1]) ? array($arg[1]) : array(-1)
      )
    );
  }
}

/**
 * @} End of "addtogroup Nodewords hooks" .
 */
