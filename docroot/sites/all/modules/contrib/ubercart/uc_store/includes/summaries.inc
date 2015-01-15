<?php

/**
 * @file
 * Provides summaries of forms and fieldsets.
 */

/**
 * Summarizes the elements in a form array.
 *
 * Recursively builds summaries of items nested in fieldsets if applicable.
 *
 * @param $form
 *   The form array to summarize.
 * @return
 *   An array of summary information, structured for theme_item_list().
 */
function summarize_form($form) {
  $items = array();

  // Loop through each element in $form.
  foreach (element_children($form) as $key) {
    // Set the callback for the summary to the default if not specified.
    if (!isset($form[$key]['#summary callback'])) {
      // Default filter forms to have no summary.
      if (isset($form[$key]['#element_validate']) && in_array('filter_form_validate', (array) $form[$key]['#element_validate'])) {
        $form[$key]['#summary callback'] = 'summarize_null';
      }
      else {
        $form[$key]['#summary callback'] = 'summarize_element';
      }
    }

    // Setup the arguments array, always passing in the form array.
    $args = array($form[$key]);

    // Append the arguments specified by the element.
    if (isset($form[$key]['#summary arguments'])) {
      $args = array_merge($args, $form[$key]['#summary arguments']);
    }

    // Fetch the result from the summary callback.
    $result = call_user_func_array($form[$key]['#summary callback'], $args);

    // Check the type of the result...
    if (is_array($result)) {
      // Arrays get merged in so summaries can include multiple items.
      $items = array_merge($items, $result);
    }
    elseif (!empty($result)) {
      // Otherwise add a non-empty result to the array as a new value.
      $items[] = $result;
    }
  }

  return $items;
}

/**
 * Summarize an individual element using its specified #summary if possible.
 *
 * @param $form
 *   The element's array in the form array.
 * @param $title
 *   TRUE or FALSE indicating whether or not to include the element's #title in
 *     the summary text.
 * @return
 *   A summary string for the element or a summary array for a fieldset.
 */
function summarize_element($form, $title = FALSE) {
  // Check to see if the form array contained a #summary.
  if (isset($form['#summary'])) {
    // If so, decide whether to display it with or without the element's #title.
    if ($title) {
      return t('!title: !summary', array('!title' => $form['#title'], '!summary' => $form['#summary']));
    }
    else {
      return $form['#summary'];
    }
  }
  elseif (isset($form['#type'])) {
    // Otherwise, use a sensible default based on the field type.
    switch ($form['#type']) {
      case 'fieldset':
        return array(
          array(
            'data' => $form['#title'] .':',
            'children' => summarize_form($form),
          ),
        );

      case 'textfield':
        return $form['#title'] .': '. check_plain($form['#default_value']);

      case 'select':
        if (!empty($form['#multiple']) and is_array($form['#default_value'])) {
          $options = array();
          foreach ($form['#default_value'] as $value) {
            if (isset($form['#options'][$value])) {
              $options[] = $form['#options'][$value];
            }
          }
          // Return an item list of the selected options.
          return array(array(
            'data' => $form['#title'] . ':',
            'children' => $options,
          ));
        }
        // Else, fall through.
      case 'radios':
        return $form['#title'] .': '. $form['#options'][$form['#default_value']];

      case 'checkbox':
        if ($form['#default_value'] && isset($form['#title'])) {
          return $form['#title'];
        }
        else {
          return;
        }
    }

    // If we didn't have a default action, check for a callback.
    $callback = 'summarize_'. $form['#type'];

    if (function_exists($callback)) {
      return $callback($form);
    }
  }
}

/**
 * Return an empty summary string.
 */
function summarize_null($form) {
  return array();
}

/**
 * Returns the summary string for a checkbox element based on its current value.
 *
 * @param $true
 *   The summary string to use if the checkbox has been checked.
 * @param $false
 *   The summary string to use if the checkbox has not been checked.
 * @return
 *   The correct supplied string based on the current value of the checkbox.
 */
function summarize_checkbox($form, $true, $false) {
  if ($form['#default_value']) {
    return $true;
  }
  else {
    return $false;
  }
}

/**
 * Summarize the form pages that are children of the specified path.
 *
 * @param $path
 *   The menu path to start from when checking for children forms.
 * @param $trim
 *   When set to TRUE, summary data will only be included in the return array
 *   when the summary actually has items.
 * @param $only_parent
 *   When set to TRUE, forms will only be included from the given $path,
 *   no recursion will be done
 * @return
 *   An array of data representing a form page summary including keys for the
 *   page's 'path', and edit 'href', a summary 'title' and 'items'.
 */
function summarize_child_form_pages($path, $trim = FALSE, $only_parent = FALSE) {
  $summaries = array();

  // Fetch and loop through any child menu items from the database.
  $accessor = "LIKE '%s/%%'";

  // If no_recur is TRUE, only look for the parent
  if ($only_parent) {
    $accessor = "= '%s'";
  }

  $result = db_query("SELECT path FROM {menu_router} WHERE path ". $accessor ." ORDER BY weight", $path);

  while ($row = db_fetch_array($result)) {
    $item = menu_get_item($row['path']);
    // Only allow items the user can access.
    if ($item['access'] === FALSE) {
      continue;
    }

    if ($item['page_callback'] == 'drupal_get_form') {
      if ($item['type'] == MENU_DEFAULT_LOCAL_TASK) {
        $parent = menu_get_item($item['tab_parent']);
        $href = $parent['href'];
      }
      else {
        $href = $item['href'];
      }

      $form_id = $item['page_arguments'][0];

      if (!function_exists($form_id)) {
        require_once($item['file']);
      }

      $form_state = array('storage' => NULL, 'submitted' => FALSE);
      $form = drupal_retrieve_form($form_id, $form_state);
      drupal_prepare_form($form_id, $form, $form_state);

      $summary_items = summarize_form($form);

      if (!$trim || $trim && count($summary_items) > 0) {
        $summaries[] = array(
          'path' => url($item['path']),
          'href' => $href,
          'title' => $item['title'],
          'items' => $summary_items,
        );
      }
    }
  }

  return $summaries;
}

/**
 * Theme a summaries array into a handy div for use with some JS enhancement.
 *
 * @param $summaries
 *   An array of summary arrays each containing the following keys:
 *   - path - the path of the settings form the array is summarizing.
 *   - title - the title of the settings form.
 *   - href - the actual URL of the summary's form page.
 *   - items - an array of items formatted for theme_item_list().
 * @param $link
 *   TRUE or FALSE indicating whether or not to display an edit icon linking to
 *     the settings form the summary represents.
 * @return
 *   The HTML output of the summary.
 * @ingroup themeable
 */
function theme_summary_overview($summaries, $link = TRUE) {
  // Add some Ubercart specific JS for modifying summaries.
  drupal_add_js(drupal_get_path('module', 'uc_store') .'/includes/summaries.js');
  drupal_add_js(array('editIconPath' => base_path() . drupal_get_path('module', 'uc_store') .'/images/order_edit.gif'), 'setting');

  $output = '';

  foreach ($summaries as $summary) {
    // Add a containing div for the summary overview.
    $output .= '<div id="'. $summary['path'] .'" class="summary-overview">';

    // Add a div for the header containing the title and edit link.
    $output .= '<div class="summary-header"><span class="summary-title">'
             . check_plain($summary['title']) .': </span>';

    if ($link) {
      $output .= ' <span class="summary-link">'. l(t('edit'), $summary['href'])
                .'</span>';
    }

    $output .= '</div>';

    // Add in the list for the summary items.
    $output .= theme('item_list', $summary['items']);

    // Close out the summary overview div.
    $output .= '</div>';
  }

  return $output;
}
