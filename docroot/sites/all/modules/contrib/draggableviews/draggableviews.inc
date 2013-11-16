<?php
// $Id: draggableviews.inc,v 1.7.2.26 2009/03/27 19:11:44 sevi Exp $

/**
 * @file
 * Draggableviews processing functions.
 * Rough summary of what functions in this file do:
 *  - Fetch views-style-plugin information
 *  - Analyze structure
 *  - Build/rebuild hierarchy
 */

/**
 * Fetch views-style-plugin information.
 * Collect all known information in a handy array.
 *
 * @param $view
 *   The views object
 *
 * @return
 *   A structured array containing the extracted draggableviews settings, additional
 *   field information, all nodes the view returned and the view object itself.
 *   array(
 *     'view' => view,
 *
 *     'order' => array(
 *       'field' => array(
 *         'field_name'  => field_name,
 *         'field_alias' => field_alias,
 *         'handler'     => handler,
 *       ),
 *       'visible'       => TRUE/FALSE,
 *     ),
 *
 *     'hierarchy' => array(
 *       'field' => array(
 *         'field_name'  => field_name,
 *         'field_alias' => field_alias,
 *         'handler'     => handler,
 *       ),
 *       'visible' => TRUE/FALSE,
 *     ),
 *
 *     'depth_limit' => depth_limit,
 *
 *     'types' = array(
 *       node_type1 => "root"/"leaf",
 *       node_type2 => "root"/"leaf",
 *       ..
 *     ),
 *
 *     'expand_links' = array(
 *       'show'              => TRUE/FALSE,
 *       'default_collapsed' => TRUE/FALSE,
 *       'by_uid'            => TRUE/FALSE,
 *     ),
 *
 *     'view_window_extensions' = array(
 *       'extension_top'    => 3,
 *       'extension_bottom' => 3,
 *     ),
 *
 *     'locked' => TRUE/FALSE,
 *
 *     'depth'  => 0,
 *
 *     'default_on_top' => TRUE/FALSE,
 *
 *     'nodes' => array(
 *       nid1 => array(
 *         'order' => array(
 *           0 => value,
 *           1 => value,
 *           ..
 *         ),
 *         'parent' => value,
 *       ),
 *       ..
 *     ),
 *   );
 */
function _draggableviews_info($view, $info = NULL) {
  $options = $view->style_plugin->options;
  $fields  = $view->field;
  $results = $view->result;

  // if there is already an info array just rebuild the nodes array and skip this section
  if (!isset($info)) {
    $info = array();

    // extract draggableviews settings.
    if ($options['tabledrag_order']['field'] != 'none') {
      if ($handler = _draggableviews_init_handler($options['tabledrag_order'], $view)) {
        $info['order'] = array(
          'field' => array(
            'handler'     => $handler,
            'field_name'  => $options['tabledrag_order']['field'],
            'field_alias' => $fields[$options['tabledrag_order']['field']]->field_alias,
          ),
          'visible' => strcmp($options['tabledrag_order_visible']['visible'], 'visible') == 0 ? TRUE : FALSE,
        );
      }
      else {
        drupal_set_message(t('Draggableviews: Handler <i>') . $options['tabledrag_order']['handler'] . t('</i> could not be found.'), 'error');
        unset($info['order']);
        return $info;
      }
      $info['order']['visible'] = strcmp($options['tabledrag_order_visible']['visible'], 'visible') == 0 ? TRUE : FALSE;

      if ($options['tabledrag_hierarchy']['field'] != 'none') {
        if ($handler = _draggableviews_init_handler($options['tabledrag_hierarchy'], $view)) {
          $info['hierarchy'] = array(
            'field' => array(
              'handler'     => $handler,
              'field_name'  => $options['tabledrag_hierarchy']['field'],
              'field_alias' => $fields[$options['tabledrag_hierarchy']['field']]->field_alias,
            ),
            'visible' => strcmp($options['tabledrag_hierarchy_visible']['visible'], 'visible') == 0 ? TRUE : FALSE,
          );

          $info['depth_limit'] = $options['draggableviews_depth_limit'];
          if (!isset($info['depth_limit'])) $info['depth_limit'] = 10;

          if (isset($options['tabledrag_types'])) {
            foreach ($options['tabledrag_types'] as $type) {
              $info['types'][$type['node_type']] = $type['type'];
            }
          }

          $info['expand_links'] = array(
            'show'              => strcmp($options['tabledrag_expand']['expand_links'], 'expand_links') == 0 ? TRUE : FALSE,
            'default_collapsed' => strcmp($options['tabledrag_expand']['collapsed'], 'collapsed') == 0 ? TRUE : FALSE,
            'by_uid'            => strcmp($options['tabledrag_expand']['by_uid'], 'by_uid') == 0 ? TRUE : FALSE,
          );
        }
        else {
          drupal_set_message(t('Draggableviews: Handler <i>') . $options['tabledrag_hierarchy']['handler'] . t('</i> could not be found.'), 'error');
        }
      }

      $info['view_window_extensions'] = $options['draggableviews_extensions'];
      if (!isset($info['view_window_extensions']['extension_top'])) $info['view_window_extensions']['extension_top'] = 3;
      if (!isset($info['view_window_extensions']['extension_bottom'])) $info['view_window_extensions']['extension_bottom'] = 3;

      $info['locked'] = strcmp($options['tabledrag_lock']['lock'], 'lock') == 0 ? TRUE : FALSE;
    }
  }

  $info['depth'] = 0;

  $info['default_on_top'] = $options['draggableviews_default_on_top'] == 1 ? TRUE : FALSE;

  // Hold a reference of the view object itself.
  $info['view'] = &$view;

  // Get all nodes and their properties.
  $info['nodes'] = array();
  if (isset($info['order']) && isset($results) && count($results) > 0) {
    // loop through all resulting nodes
    foreach ($results as $row) {
      if (is_numeric($row->{$info['order']['field']['field_alias']})) {
        $info['nodes'][$row->nid]['order'][0] = (int)($row->{$info['order']['field']['field_alias']});
      }
      else {
        // Default position of new nodes.
        $info['nodes'][$row->nid]['order'][0] = $info['default_on_top'] == 1 ? -1 : $view->total_rows;
      }
      if (isset($info['hierarchy'])) {
        $info['nodes'][$row->nid]['parent'] = (int)($row->{$info['hierarchy']['field']['field_alias']});
      }
    }
  }

  return $info;
}

/*
 * Quick Check Structure
 *
 * I used the word "Quick" because only visible nodes and only the very top hierarchy level will be checked. If the
 * return is TRUE we can be sure that the structure will be displayed correctly. But errors that don't affect the
 * current output can not be detected. Enhanced checks will be done when we have to rebuild a broken structure.
 *
 * We check for the following:
 *  - Wrong order values: The order values must constantly increase by 1, independent of the hierarchy level.
 *  - Parent mismatch: The parent_nid must equal with the nid we memorized before we entered the current hierarchy level.
 *
 * @param $inputs
 *   The structured info array. Look at _draggableviews_info(..) to learn more.
 *
 * return
 *   TRUE if structure is valid
 */
function _draggableviews_quick_check_structure($info) {
  // Calculate views page offset.
  $pager  = $info['view']->pager;
  $offset = $pager['items_per_page'] * $pager['current_page'] + $pager['offset'];

  // Call function in checking mode (renumber = FALSE). The order value must begin with $offset.
  return _draggableviews_ascending_numbers($info, $offset, FALSE);
}

/**
 * Build hierarchy
 *
 * Although there shouldn't be any structure based errors after submit
 * broken parents can be detected and repaired. All other structure based errors
 * can not be detected. If there are any they will be detected by _draggableviews_quick_check_structure(..)
 * and finally repaired by _draggableviews_analyze_structure(..).
 *
 * @param $info
 *   The structured information array
 */
function _draggableviews_build_hierarchy(&$info) {
  $nodes = &$info['nodes'];
  $input = &$info['input'];

  foreach ($nodes as $nid => $prop) {
    // get depth
    if (($depth = _draggableviews_get_hierarchy_depth($nid, $input, $info)) === FALSE) {
      // Error! The hierarchy structure is broken and could
      // look like the following: (we're currently processing X)
      // A
      //   --X
      // --D
      //
      // The next steps:
      //  1) bring it down to the root level
      //  2) Set order fields to the minimum

      $input[$nid]['parent'] = 0;

      // We gracefully sidestep the order-loop
      $depth = -1;
    }

    // Let's take a look at the following expample, to understand
    // what is beeing done.
    //
    // A
    // --B
    // --C
    //   --X
    // --D
    // E
    // Imagine we're currently processing X:
    //
    // We know that X is in depth=3, so we save the received
    // weight value in the 3rd order field of node X.
    //
    // The 2nd order field must inherit the received weight of
    // node C (the next parent). And the 1st order field must
    // inherit the received weight of node A (the parent of C).
    //
    // When we finally order the view by weight1, weight2, weight3 then
    // weight1 and weight2 from node X will always equal with
    // those from node A and B, and weight3 defines the order of the 3rd level.

    $temp_nid = $nid;

    for ($i = $depth; $i >= 0; $i--) {
      // we're operating top-down, so we determine the parents nid by the way

      $nodes[$nid]['order'][$i] = $input[$temp_nid]['order'][0];

      if (isset($info['hierarchy']) && $i > 0) {
        if (!($temp_nid = $input[$temp_nid]['parent'])) {
          // this line should never be reached assumed the depth
          // was calculated correctly.
          drupal_set_message(t('Undefined State called in draggableviews_build_hierarchy(..)'), 'error');
          break;
        }
      }
    }

    if (isset($info['hierarchy'])) {
      // Simply set the parent value
      $nodes[$nid]['parent'] = $input[$nid]['parent'];
    }

    // Now set the next level to the minimum value. Otherwise
    // it could happen that a child appears above its parent.
    // The ? can be anything, unfortunately also > 5
    //
    // --A (3,5)
    // B   (3,?)
    //
    // To guaranteer that the ? is always the lowest, we choose
    // the minimum.

    $depth = ($depth == -1) ? 0 : $depth;

    $nodes[$nid]['order'][$depth + 1] = $info['order']['field']['handler']->get_minimum_value();
    $nodes[$nid]['depth']             = $depth;
  }

  // Last but not least sort nodes and finally assign ascending numbers. This is necessary since this module supports paging.
  _draggableviews_sort_nodes($nodes);
  // calculate views page offset
  $pager  = $info['view']->pager;
  $offset = $pager['items_per_page'] * $pager['current_page'] + $pager['offset'];
  _draggableviews_ascending_numbers($info, $offset, TRUE);
}

/**
 * Rebuild hierarchy
 *
 * This function is called when the structure is broken.
 *
 * @param $info
 *   The structured information array. Look at _draggableviews_info(..) to learn more.
 */
function _draggableviews_rebuild_hierarchy(&$info) {
  // We backup the page settings and restore them after completing all operations.
  $backup_pager = $info['view']->pager;

  if ($backup_pager['items_per_page'] > 0) {
    // We have to make sure that there's no hidden node with an order value that refers to the current page.
    // If the items to display per page are limitated we load the entire view.

    //@todo: We reduce the fields to a minimum because of performance issues.

    _draggableviews_reload_info($info, 0, 0, 0);
  }

  // Calculate depth values.
  // Nodes with broken parents will be brought down to the root level.
  // These depth values will be used for both theming and repairing broken structures.
  _draggableviews_calculate_depths($info);

  // Detect and repair ordering errors.
  // The nodes order values have to equal with the parents order values.
  _draggableviews_check_order($info);

  // The last issue we have to deal with is the order itself. Probably there
  // are more many with the same order value what could lead to display errors.
  // We just sort the nodes by their current order values and subsequent we assign ascending numbers.
  _draggableviews_sort_nodes($info['nodes']);
  _draggableviews_ascending_numbers($info, 0, TRUE);

  // Save hierarchy.
  _draggableviews_save_hierarchy($info);

  // The structure should be valid now.
  // Nonetheless let's make a final check for debugging reasons.
  if (!_draggableviews_quick_check_structure($info)) {
    drupal_set_message("Draggableviews: Rebuilding structure didn't work. The structure is broken.", "error");
  }
}

/**
 * (CHECK/WRITE) Ascending Numbers.
 *
 * This function is used for both renumbering and checking.
 * Order values have to start with $offset.
 *
 * @param $info
 *   The structured info array. Look at _draggableviews_info(..) to learn more.
 * @param $offset
 *   Where we start to count.
 * @param $renumber
 *   Renumber or check.
 *
 * @return
 *   TRUE if no errors found.
 */
function _draggableviews_ascending_numbers(&$info, $offset = 0, $renumber = FALSE) {
  // We need to hold
  //  1)  the last nid,
  //  2)  the last order value,
  //  3)  the parent's nid of each hierarchy level,
  //  3a) the current depth (hierarchy level),
  $last_nid        = -1;
  $last_order      = $offset;
  $last_parent_nid = array(0);
  $depth           = 0;

  foreach ($info['nodes'] as $nid => $values) {
    $parent_nid = isset($info['hierarchy']) ? $values['parent'] : 0;

    // Let's take a look at the following expample, to understand what is beeing done.
    // The order value of the parent's level always equals with the parent's value.
    //
    // A     (0 - -) First node of level0.
    // --B   (0 1 -) First node of level1. But we continue counting.
    //   --C (0 1 2) First node of level2. But we still continue counting.
    // X     (3 - -) Now we leave 2 levels (level2->level0). We still have to continue counting.
    //
    // Imagine we are currently processing X. We are leaving 2 levels and we need to determine the
    // last order value that was used. First we check how many levels are beeing left:
    $leave_hierarchy_levels = 0;
    for ($i = 0; $i < $depth; $i++) {
      if ($parent_nid == $last_parent_nid[$i]) {
        // Found the level we go to. Calculate the number of levels that are beeing left.
        $leave_hierarchy_levels = $depth - $i;
        break;
      }
    }
    if ($leave_hierarchy_levels > 0) {
      // We leave some hierarchy levels. We simply inherit the order value of the level we come from.
      $depth -= $leave_hierarchy_levels;
      // Remove obsoleted values from stack.
      for ($i = 0; $i < $leave_hierarchy_levels; $i++) {
        array_pop($last_parent_nid);
      }
    }
    else if ($parent_nid == $last_nid) {
      // We are entering a new level. This is the first child of the last node.
      array_push($last_parent_nid, $last_nid);
      $depth++;
    }
    else if ($parent_nid != $last_parent_nid[$depth]) {
      // This node is neither a member of any previous hierarchy level
      // nor a child of the last node (opening a new level)
      // nor a member of the current hierarchy level
      // There's something wrong!
      if ($renumber) {
        drupal_set_message("Something wrong in _draggableviews_ascending_numbers (". ($renumber ? 'WRITE' : 'CHECK') .").", "error");
      }
      return FALSE;
    }

    // This function is used for both renumbering and checking the hierarchy. Decide what we have to do:
    if ($renumber) {
      // Assign order value.
      $info['nodes'][$nid]['order'][0] = $last_order;
    }
    else {
      // Check structure.
      $order = $values['order'][0];
      if ($order != $last_order) {
        // This would cause troubles with paging.
        // We better initiate a rebuild!
        return FALSE;
      }
    }

    $last_nid = $nid;
    $last_order++;
  }

  return TRUE;
}

/**
 * Extend View Window
 *
 * In order to make it possible to drag from one page to another we have to extend the visible window of each page.
 * We also have to make sure that parent nodes appear with all their children (..and child nodes with their parent).
 * The view will be re-executed with the calculated range, based on the suggested range.
 *
 * @param $info
 *   The structured info array. Look at _draggableviews_info(..) to learn more.
 * @param $extension_top
 * @param $extension_bottom
 *
 * @return
 *   An array containing the new, calculated range.
 */
function _draggableviews_extend_view_window(&$info, $reload = FALSE) {
  // Check page settings.
  if ($info['pager']['items_per_page'] <= 0) {
    // Nothing to do. Just return the actual range.
    return array(
      'first_index' => 0,
      'last_index' => count($info['view']->result) - 1,
    );
  }

  // Notice that we have two sets of pager information:
  // - The "should-be" values at $info['pager']
  // - The real, current values at $view->pager.
  // For building the new view we always use the "should-be" pager information.

  $items_per_page = $info['pager']['items_per_page'];
  $current_page   = $info['pager']['current_page'];
  $offset         = $info['pager']['offset'];

  $first_index = $items_per_page * $current_page;
  $last_index  = $items_per_page * ($current_page + 1) - 1;

  $nodes = $info['nodes'];

  // If the new index is out of range we'll use the last existing index. Calculate this index:
  $current_first_index = $info['view']->pager['items_per_page'] * $info['view']->pager['current_page'] + $info['view']->pager['offset'];
  $total_index = $current_first_index + count($nodes) - 1;

  if ($first_index > $total_index) {
    // $first_index is out of range.
    $first_index = 0;
    $last_index  = $items_per_page -1;
  }

  // Check permissions and lock.
  if (!user_access('Allow Reordering') || $info['locked']) {
    // Don't extend view window (= suggestion).
    $extension_top    = 0;
    $extension_bottom = 0;
  }
  else {
    $extension_top    = $info['view_window_extensions']['extension_top'];
    $extension_bottom = $info['view_window_extensions']['extension_bottom'];
  }

  // Extend on top:
  $first_index -= $extension_top;
  if ($first_index < 0) {
    // Be careful: $first_index < 0. So this is a subtraction!
    $extension_top += $first_index;
    if ($extension_top < 0) $extension_top = 0;
    $first_index = 0;
  }
  // Extend on bottom:
  $last_index += $extension_bottom;
  if ($last_index > $total_index) {
    $extension_bottom = $extension_bottom - ($last_index - $total_index);
    if ($extension_bottom < 0) $extension_bottom = 0;
    $last_index = $total_index;
  }

  $add_to_extension_top    = 0;
  $add_to_extension_bottom = 0;

  if (isset($info['hierarchy'])) {
    // There are possibly some child nodes that would be cut of their parents. To avoid this we localize
    // the next parents (if needed) and use their index for the range. To analyze the nodes array we slice out
    // all nodes before (the B's) and all nodes after (the a's) the current page (the -'s):
    //
    // B B B B B B B B B - - - - - - - - A A A A A A A A A A.
    //
    // Slice out all nodes before (currently not shown):
    $nodes_before  = array_slice($nodes, 0, $first_index, TRUE);
    // Slice out all nodes after  (currently not shown):
    $nodes_after   = array_slice($nodes, $last_index + 1, NULL, TRUE);
    // Slice out all nodes that are currently shown:
    $nodes_current = array_slice($nodes, $first_index, $last_index - $first_index + 1, TRUE);

    $first_node = current($nodes_current);
    if ($first_node['parent'] > 0) {
      // The first node of the current page is a child node. Look for the next parent
      // that is on the root level. We can't use foreach() because we iterate the wrong way.
      $temp_node = end($nodes_before);
      while ($temp_node !== FALSE) {
        $add_to_extension_top++;
        if ($temp_node['parent'] == 0) break;
        $temp_node = prev($nodes_before);
      }
    }
    // Check for possible children of the last node.
    foreach ($nodes_after AS $temp_node) {
      if ($temp_node['parent'] == 0) break;
      $add_to_extension_bottom++;
    }

    // Update range.
    $first_index -= $add_to_extension_top;
    $last_index  += $add_to_extension_bottom;
  }

  if ($reload) {
    // Re-execute the view and reload the info array. We need to add 1 to the number of items
    // to show (e.g. show from index 5 to index 5: show 5-5+1=1 item).
    _draggableviews_reload_info($info, $last_index - $first_index + 1, 0, $offset + $first_index);
  }

  // Mark extended nodes as "extension".
  $nodes = &$info['nodes'];
  for ($i = 0; $i < $extension_top + $add_to_extension_top; $i++) {
    $nid = key($nodes);
    $nodes[$nid]['extension'] = TRUE;
    next($nodes);
  }
  end($nodes);
  for ($i = 0; $i < $extension_bottom + $add_to_extension_bottom; $i++) {
    $nid = key($nodes);
    $nodes[$nid]['extension'] = TRUE;
    prev($nodes);
  }

  return array(
    'first_index' => $first_index,
    'last_index' => $last_index,
  );
}

/**
 * Click Sort
 *
 * The nodes are already in the right order.
 * Simply assign ascending values.
 * Re-execute view and refresh info array.
 *
 * @param $info
 *   The structured info array. Look at _draggableviews_info(..) to learn more.
 */
function _draggableviews_click_sort(&$info) {
  // Check permissions.
  if (!user_access('Allow Reordering')) {
    drupal_set_message(t('You are not allowed to reorder nodes.'), 'error');
    return;
  }

  // Check if structure is locked.
  if ($info['locked']) {
    drupal_set_message(t('The structure is locked.'), 'error');
    return;
  }

  // If the items to display per page are limited we load the entire view.
  $pager = $info['view']->pager;
  if ($pager['items_per_page'] > 0) {
    _draggableviews_reload_info($info, 0, 0, 0);
  }

  // First bring all child nodes down to the root level. We can't keep the hierarchy information
  // because the nodes have been mixed up thoroughly.
  if (isset($info['hierarchy'])) {
    foreach ($info['nodes'] AS $nid => $values) {
      $info['nodes'][$nid]['parent'] = 0;
      $info['nodes'][$nid]['depth']  = 0;
    }
  }
  // Assign ascending numbers
  _draggableviews_ascending_numbers($info, 0, TRUE);
  _draggableviews_save_hierarchy($info);

  // Re-execute view with original page settings.
  _draggableviews_reload_info($info, $pager['items_per_page'], $pager['current_page'], $pager['offset']);
}

/**
 * Save Hierarchy
 *
 * @param $info
 *   The structured information array. Look at _draggableviews_info(..) to learn more.
 */
function _draggableviews_save_hierarchy($info) {
  // Loop through all nodes.
  foreach ($info['nodes'] as $nid => $prop) {
    if (isset($info['hierarchy'])) {
      $value   = $prop['parent'];
      $handler = $info['hierarchy']['field']['handler'];
      $handler->save($nid, $value);
    }

    $value   = $prop['order'][0];
    $handler = $info['order']['field']['handler'];
    $handler->save($nid, $value);
  }
}

/**
 * Calculate Depths
 *
 * Nodes with broken parents will be brought down to the root level.
 *
 * @param $info
 *   The structured information array. Look at _draggableviews_info(..) to learn more.
 *
 * @return
 *   TRUE if no errors occured.
 */
function _draggableviews_calculate_depths(&$info) {
  $error = FALSE;

  // Loop through all rows the view returned.
  foreach ($info['nodes'] as $nid => $prop) {
    if ($info['nodes'][$nid]['parent'] < 0) $info['nodes'][$nid]['parent'] = 0;
    
    // Determine hierarchy depth of current row.
    $depth = _draggableviews_get_hierarchy_depth($nid, $info['nodes'], $info);

    if ($depth === FALSE) {
      $error = TRUE;
      $depth = 0;
      $info['nodes'][$nid]['parent'] = 0;
    }

    $info['nodes'][$nid]['depth'] = $depth;

    // Move the order value to the calculated level.
    $info['nodes'][$nid]['order'][$depth] = $info['nodes'][$nid]['order'][0];

    // Set the next level to the minimum value. Otherwise
    // it could happen that a child appears above its parent.
    $info['nodes'][$nid]['order'][$depth + 1] = $info['order']['field']['handler']->get_minimum_value();

    if ($depth > $info['depth']) $info['depth'] = $depth;
  }

  return !$error;
}

/**
 * Get Hierarchy Depth
 *
 * This function detects broken parents.
 * Cycles are detected (if a child is the parent of its parent).
 *
 * @param $nid
 *   The nid of the node which should be processed.
 * @param $nodes
 *   The nodes array. This can be both a nodes array or an input array.
 *   They share the same hierarchy properties.
 * @param $info
 *   The structured info array. Look at _draggableviews_info(..) to learn more.
 *
 * @return
 *   The hierarchy depth if no occured, else FALSE.
 */
function _draggableviews_get_hierarchy_depth($nid, $nodes, $info) {
  if (!isset($info['hierarchy'])) return 0;

  $depth     = 0;
  $error     = FALSE;
  $temp_nid  = $nid;
  $used_nids = array($temp_nid);

  while (!($error = !isset($nodes[$temp_nid])) && ($temp_nid = $nodes[$temp_nid]['parent']) > 0) {
    // In order to detect cycles we use an array,
    // where all used nids are saved in. If a node id
    // occurs twice -> return FALSE.
    $cycle_found = array_search($temp_nid, $used_nids);
    // The isset(..) is necessary because in PHP-Versions <= 4.2.0 array_search() returns NULL
    // instead of FALSE if $temp_nid was not found.
    if (isset($cycle_found) && $cycle_found !== FALSE) {
      drupal_set_message(t("Draggableviews: A cycle was found."));
      return FALSE;
    }

    $used_nids[] = $temp_nid;
    $depth++;
  }

  if ($error) {
    // If loop breaked caused by an error.
    return FALSE;
  }

  return $depth;
}

/**
 * Detect and Repair Order Values
 *
 * The nodes order value has to equal with the parents order value.
 *
 * @param $info
 *   The structured information array. Look at _draggableviews_info(..) to learn more.
 *
 */
function _draggableviews_check_order(&$info) {
  foreach ($info['nodes'] as $nid => $prop) {
    _draggableviews_check_node_order($nid, $info);

    if ($prop['depth'] > 0) {
      // Set the parent node order of the current level to 0 (B in the example). This should prevent
      // broken structures caused by "bad nodes" (A in the example) with the same order at the parents level.
      // B     (0 -1) The parent of C
      // A     (0 -1) A bad node
      // --C   (0  1) The child node of B
      // We take advantage of this when we sort the nodes.
      $parent = $prop['parent'];
      $depth  = $prop['depth'];
      $info['nodes'][$parent]['order'][$depth] = 0;
    }
  }
}

/**
 * Check Node Order Value
 *
 * We rely on the correctness of depth values and parent values.
 * The order values of each level have to equal with the values of the parent nodes.
 *
 * @param $nid
 *   The node id to check.
 * @param $info
 *   The structured information array. Look at _draggableviews_info(..) to learn more.
 *
 */
function _draggableviews_check_node_order($nid, &$info) {
  $error = FALSE;
  $nodes = &$info['nodes'];

  $temp_nid = $nodes[$nid]['parent'];

  for ($i = $nodes[$nid]['depth'] - 1; $i >= 0; $i--) {
    // We're operating top-down, so we determine the parents nid by the way.
    $nodes[$nid]['order'][$i] = $nodes[$temp_nid]['order'][$i];

    $temp_nid = $nodes[$temp_nid]['parent'];
  }
}

/**
 * Reload Info Array
 *
 * @param $info
 *   The structured info array. Look at _draggableviews_info(..) to learn more.
 * @param $items_per_page
 * @param $current_page
 */
function _draggableviews_reload_info(&$info, $items_per_page = NULL, $current_page = NULL, $offset = NULL) {
  // Re-execute the view because order values may have changed.
  _draggableviews_re_execute_view($info['view'], $items_per_page, $current_page, $offset);
  // Reload nodes of the info array.
  $info = _draggableviews_info($info['view'], $info);
}

/**
 * Re-execute View
 *
 * @param $view
 *   The view object.
 * @param $items_per_page
 * @param $current_page
 * @param $offset
 */
function _draggableviews_re_execute_view(&$view, $items_per_page = NULL, $current_page = NULL, $offset = NULL) {
  if (isset($items_per_page)) {
    $view->pager['items_per_page'] = $items_per_page;
  }
  if (isset($current_page)) {
    $view->pager['current_page'] = $current_page;
    // Views pager uses global variables where all already known information is dumped in.
    // We need to change the global variable $pager_page_array in order to set the page to 0 because
    // this variable would force the current page to another value. (see views/includes/view.inc:#717, function execute())
    global $pager_page_array;
    $pager_page_array[$view->pager['element']] = $current_page;
  }
  if (isset($offset)) $view->pager['offset'] = $offset;

  $view->executed = FALSE;
  $view->execute();
}

/**
 * Get Handler Instance
 *
 * @param $field
 *   The field options specified in the style plugin.
 * @param $view
 *   The view object.
 *
 * @return
 *   A handler instance.
 */
function _draggableviews_init_handler($field, &$view) {
  if (isset($field['handler'])) {
    $handler_info = draggableviews_discover_handlers($field['handler']);
    $file         = $handler_info['path'] .'/'. $handler_info['file'];
    if ($handler_info['path'] && $handler_info['file'] && file_exists($file)) {
      require_once($file);
      $handler = new $handler_info['handler'];
      $handler->init($field['field'], $view);
      return $handler;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Sort Nodes
 *
 * @param $nodes
 *   The nodes array to sort.
 */
function _draggableviews_sort_nodes(&$nodes) {
  uasort($nodes, '_draggableviews_compare_nodes');
}

/**
 * Compare Nodes (used by uasort)
 *
 *
 * @param $node1
 * @param $node2
 *
 * @return
 *   < or > or ==
 */
function _draggableviews_compare_nodes($node1, $node2) {
  // The first difference will be significant. If they equal in each level we
  // "survive" the loop and end up with return 0.
  for ($i = 0; $i < count($node1['order']); $i++) {
    // First compare the order values..
    if ($node1['order'][$i] < $node2['order'][$i]) return -1;
    if ($node1['order'][$i] > $node2['order'][$i]) return 1;
  }
  return 0;
}

