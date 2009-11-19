<?php
// $Id: draggableviews_theme.inc,v 1.6.2.19 2009/03/26 22:18:19 sevi Exp $

/**
 * @file
 * Theme functions.
 */

function template_preprocess_draggableviews_view_draggabletable(&$vars) {
  $view     = $vars['view'];

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  $result   = $vars['rows'];
  $vars['rows'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_querystring();
  if ($query) {
    $query = '&' . $query;
  }

  // Fields must be rendered in order as of Views 2.3, so we will pre-render
  // everything.
  $renders = array();
  foreach ($result as $count => $row) {
    foreach ($view->field as $id => $field) {
      $renders[$count][$id] = $view->field[$id]->theme($row);
    }
  }

  foreach ($columns as $field => $column) {
    // render the header labels
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable']) || !$fields[$field]->click_sortable()) {
        $vars['header'][$field] = $label;
      }
      else {
        // @todo -- make this a setting
        $initial = 'asc';

        if ($active == $field && $order == 'asc') {
          $initial = 'desc';
        }

        $title = t('sort by @s', array('@s' => $label));
        if ($active == $field) {
          $label .= theme('tablesort_indicator', $initial);
        }
        $link_options = array(
          'html' => true,
          'attributes' => array('title' => $title),
          'query' => 'order=' . urlencode($field) . '&sort=' . $initial . $query,
        );
        $vars['header'][$field] = l($label, $_GET['q'], $link_options);
      }
    }

    // Create a second variable so we can easily find what fields we have and what the
    // CSS classes should be.
    $vars['fields'][$field] = views_css_safe($field);
    if ($active == $field) {
      $vars['fields'][$field] .= ' active';
    }

    // Render each field into its appropriate column.
    foreach ($result as $num => $row) {
      if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {
        $field_output = $renders[$num][$field];

        // Don't bother with separators and stuff if the field does not show up.
        if (!isset($field_output) && isset($vars['rows'][$num][$column])) {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if (isset($vars['rows'][$num][$column])) {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }
        else {
          $vars['rows'][$num][$column] = '';
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }

  foreach ($vars['rows'] as $num => $row) {
    $vars['row_classes'][$num][] = ($num % 2 == 0) ? 'odd' : 'even';
  }

  $vars['class'] = 'views-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }


  // Draggableviews output begin

  $info = $view->draggableviews_info;

  if (!isset($info['order'])) return;

  // loop through all rows the view returned
  foreach ($vars['rows'] as $i => $row) {

    $nid = $result[$i]->nid;

    // build indentation (as tabledrag requires)
    $indentation = theme('indentation', $info['nodes'][$nid]['depth']);

    // get node object
    $node = node_load(array('nid' => $nid));

    if (isset($info['types'][$node->type])) {
      // set node type specification, if available (e.g. tabledrag-root)
      $vars['tabledrag_type'][$i] = 'tabledrag-'. $info['types'][$node->type];
    }

    // Tabledrag needs all concerned fields to be input elements.
    // The values of the input elements will be changed by tabledrag while
    // dragging the rows. As we want to use these values we need to
    // give them names.
    //
    // Concerned fields are
    //  *) the first order field
    //  *) the parent field (if hierarchy used)
    //
    // The remaining fields are not used by tabledrag. But - depending
    // on the depth - one of them will simulate the first order field.
    // (This behaviour is based on the fact that tabledrag handles each depth as it was the only one.)

    // this for loop should prevent copy->paste'ing code
    for ($modus = 'hierarchy'; $modus !== FALSE ; $modus = ($modus == 'hierarchy' ? 'order' : FALSE)) {

      if (isset($info[$modus])) {
        if ($modus == 'hierarchy') {
          $value = $info['nodes'][$nid]['parent'];

          // get the field that should be rendered
          $field = &$info['hierarchy']['field'];
        }
        else {
          // get depth of current field
          $depth = $info['nodes'][$node->nid]['depth'];

          $value = $info['nodes'][$nid]['order'][$depth];

          // get the field that should be rendered
          $field = &$info['order']['field'];
        }

        // get the form element
        $form_element = $field['handler']->get_form_element($value, array('field_name' => $field['field_name'] .'_'. $nid, 'class' => $field['field_name']));

        // render new form element
        $vars['rows'][$i][$field['field_name']] = drupal_render($form_element);

        if (!user_access('Allow Reordering') || $info['locked']) {
          // The tabledrag js would hide the input fields (depending on the settings). But the
          // tabledrag js will not be added (permissions or locked), so we have to hide the input fields on our own.
          if (!$info[$modus]['visible']) {
            $vars['style'][$field['field_name']] = 'display: none;';
          }
        }
      }
    }

    if (isset($info['hierarchy'])) {
      // Put indentation in front of first field and append rendered hidden node id
      // that we need for tabledrag and expand/collapse.
      $hidden_nid = array(
        '#type' => 'hidden',
        '#name' => 'hidden_nid',
        '#value' => $nid,
        '#attributes' => array('class' => 'hidden_nid'),
      );
      $key = key($vars['rows'][$i]);
      $vars['rows'][$i][$key] = $indentation . $vars['rows'][$i][$key] . drupal_render($hidden_nid);
    }

    // Mark extended nodes.
    if ($info['nodes'][$nid]['extension'] == TRUE) {
      $vars['draggableviews_extended'][$i] = TRUE;
    }

    // Let extension modules alter the output.
    foreach (module_implements('draggableviews_row_alter') as $module) {
      $function = $module .'_draggableviews_row_alter';
      $function($vars['rows'][$i], $nid, $info, $view);
    }
  }

  // Output data.
  $vars['tabledrag_table_id'] = 'draggableview_'. $view->name .'_'. $vars['id'];

  if (user_access('Allow Reordering') && !$info['locked']) {
    // Prepare tabledrag settings for output.
    $vars['tabledrag'] = array();
    if (isset($info['order'])) {
      $vars['tabledrag'][] = array(
        'action'       => 'order',
        'relationship' => 'sibling',
        'source'       => $info['order']['field']['field_name'],
        'group'        => $info['order']['field']['field_name'],
        'subgroup'     => $info['order']['field']['field_name'],
        'hidden'       => !$info['order']['visible'],
        'limit'        => 0,
      );
    }
    if (isset($info['hierarchy'])) {
      $vars['tabledrag'][] = array(
        'action'       => 'match',
        'relationship' => 'parent',
        'source'       => 'hidden_nid',
        'group'        => $info['hierarchy']['field']['field_name'],
        'subgroup'     => $info['hierarchy']['field']['field_name'],
        'hidden'       => !$info['hierarchy']['visible'],
        'limit'        => $info['depth_limit'],
      );
    }
  }

  if (isset($info['hierarchy'])) {
    // Fetch expand information from database.
    $uid = $info['expand_links']['by_uid'] ? $user->uid : 0;
    $result = db_query("SELECT parent_nid, collapsed FROM {draggableviews_collapsed} WHERE uid=%d AND vid=%d", $uid, $view->vid);
    $states = array();
    while ($state = db_fetch_object($result)) {
      $states[$state->parent_nid] = $state->collapsed;
    }

    // Check if "expand" links should be shown.
    if ($info['expand_links']['show']) {
      drupal_add_js(drupal_get_path('module', 'draggableviews') .'/draggableviews.js');

      // Let javascript know about the parent field.
      drupal_add_js(array(
                      'draggableviews' => array(
                        $vars['tabledrag_table_id'] => array(
                          'parent' => $info['hierarchy']['field']['field_name'],
                        ),
                      ),
                    ),
                    'setting');

      if (empty($states)) {
        // let js know whether child nodes should be expanded or not
        drupal_add_js(array(
                        'draggableviews' => array(
                           $vars['tabledrag_table_id'] => array(
                            'expand_default' =>  $options['tabledrag_expand']['collapsed'] ? 1 : 0,
                           ),
                        ),
                      ),
                      'setting');
      }
      else {
        drupal_add_js(array(
                        'draggableviews' => array(
                          $vars['tabledrag_table_id'] => array(
                            'states' => $states,
                          ),
                        ),
                      ),
                      'setting');
      }
    }
  }

  drupal_add_css(drupal_get_path('module', 'draggableviews') .'/styles.css');
}

function template_preprocess_draggableviews_view_draggabletable_form($vars) {
  global $user;

  // get view object
  $view_obj = $vars['form']['#parameters'][2];
  // get structured info array
  $info     = $view_obj->view->draggableviews_info;

  if ($view_obj->uses_row_plugin() && empty($view_obj->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
  }
  else {
    // Group the rows according to the grouping field, if specified.
    $sets = $view_obj->render_grouping($view_obj->view->result, $view_obj->options['grouping']);

    // Render each group separately and concatenate.  Plugins may override this
    // method if they wish some other way of handling grouping.
    $output = '';
    foreach ($sets as $title => $records) {
/*      if ($view_obj->uses_row_plugin()) {
        $rows = array();
        foreach ($records as $label => $row) {
          $rows[] = $view_obj->row_plugin->render($row);
        }
      }
      else {
        $rows = $records;
      }*/

      $output .= theme($view_obj->theme_functions(), $view_obj->view, $view_obj->options, $records, $title);
    }
  }
  $vars['view'] = $output;

  // Render submit form.
  // Don't render if user has no access or view hasn't been saved yet or structure is locked.
  if (is_numeric($view_obj->view->vid) && user_access('Allow Reordering') && !$info['locked']) {
    $vars['submit_form'] =  drupal_render($vars['form']);
  }
  else {
    $vars['submit_form'] = '';
  }
}

/**
 * Theme the form for the table style plugin
 */
function theme_draggableviews_ui_style_plugin_draggabletable($form) {
  // the following lines are copied from the table style plugin
  /* COPY BEGIN */
  $output = drupal_render($form['description_markup']);

  $header = array(
    t('Field'),
    t('Column'),
    t('Separator'),
    array(
      'data' => t('Sortable'),
      'align' => 'center',
    ),
    array(
      'data' => t('Default sort'),
      'align' => 'center',
    ),
  );
  $rows = array();
  foreach (element_children($form['columns']) as $id) {
    $row = array();
    $row[] = drupal_render($form['info'][$id]['name']);
    $row[] = drupal_render($form['columns'][$id]);
    $row[] = drupal_render($form['info'][$id]['separator']);
    if (!empty($form['info'][$id]['sortable'])) {
      $row[] = array(
        'data' => drupal_render($form['info'][$id]['sortable']),
        'align' => 'center',
      );
      $row[] = array(
        'data' => drupal_render($form['default'][$id]),
        'align' => 'center',
      );
    }
    else {
      $row[] = '';
      $row[] = '';
    }
    $rows[] = $row;
  }

  // Add the special 'None' row.
  $rows[] = array(t('None'), '', '', '', array('align' => 'center', 'data' => drupal_render($form['default'][-1])));

  $output .= theme('table', $header, $rows);
  /* COPY END */


  // Render Draggable view settings
  // The following lines add the new output from draggableviews

  // Build order table.
  $tabledrag_order_columns = array();
  $tabledrag_order_columns[] = 'Order Field:';
  foreach (element_children($form['tabledrag_order']) AS $col) {
    $tabledrag_order_columns[] = drupal_render($form['tabledrag_order'][$col]);
  }
  $tabledrag_order_visible = drupal_render($form['tabledrag_order_visible']);
  $description_order = drupal_render($form['tabledrag_description_order']);

  // Build hierarchy table.
  $tabledrag_hierarchy_columns = array();
  $tabledrag_hierarchy_columns[] = 'Parent:';
  foreach (element_children($form['tabledrag_hierarchy']) AS $col) {
    $tabledrag_hierarchy_columns[] = drupal_render($form['tabledrag_hierarchy'][$col]);
  }
  $tabledrag_hierarchy_visible = drupal_render($form['tabledrag_hierarchy_visible']);
  $description_hierarchy = drupal_render($form['tabledrag_description_hierarchy']);

  $draggableviews_depth_limit = drupal_render($form['draggableviews_depth_limit']);

  // build types table
  $tabledrag_types_rows = array();
  foreach (element_children($form['tabledrag_types']) AS $id) {
    $columns = array();
    foreach (element_children($form['tabledrag_types'][$id]) AS $col) {
      $columns[] = drupal_render($form['tabledrag_types'][$id][$col]);
    }
    $tabledrag_types_rows[] = $columns;
  }
  $tabledrag_types_add = drupal_render($form['tabledrag_types_add']);
  $description_types   = drupal_render($form['tabledrag_description_types']);

  $tabledrag_expand = drupal_render($form['tabledrag_expand']);

  $draggableviews_extensions = drupal_render($form['draggableviews_extensions']);
  $description_extensions    = drupal_render($form['draggableviews_description_extensions']);

  $tabledrag_lock = drupal_render($form['tabledrag_lock']);
  $description_tabledrag_lock = drupal_render($form['description_tabledrag_lock']);

  $draggableviews_default_on_top = drupal_render($form['draggableviews_default_on_top']);

  // Let extension modules append to the output
  $extensions = array();
  foreach (module_implements('draggableviews_style_plugin_render') as $module) {
    $function = $module .'_draggableviews_style_plugin_render';
    $extensions[] = $function($form);
  }

  // append form data left to output
  // all items that have not been rendered until this point
  // will be rendered now. So we can seperate the settings we
  // inherited from plugin_table from plugin_draggableviews settings.
  $output .= drupal_render($form);

  // Append order settings.
  $output .= $tabledrag_header;
  $output .= $description_tabledrag;
  $output .= $description_order;
  $output .= theme('table', array('&nbsp;', t('Field'), t('Handler')), array($tabledrag_order_columns));
  $output .= $tabledrag_order_add;
  $output .= $tabledrag_order_visible;
  // Append hierarchy settings.
  $output .= theme('table', array('&nbsp;', t('Field'), t('Handler')), array($tabledrag_hierarchy_columns));
  $output .= $description_hierarchy;
  $output .= $tabledrag_hierarchy_visible;
  // Append depth limit textfield to output.
  $output .= $draggableviews_depth_limit;
  // Append tabledrag types settings.
  $output .= $description_types;
  $output .= theme('table', array(t('Node type'), t('Type'), '&nbsp;'), $tabledrag_types_rows);
  $output .= $tabledrag_types_add;
  // Append expand yes/no checkbox to output.
  $output .= $tabledrag_expand;
  $output .= $tabledrag_expand_default;
  // Append page extensions output.
  $output .= $description_extensions;
  $output .= $draggableviews_extensions;
  // Append lock output.
  $output .= $description_tabledrag_lock;
  $output .= $tabledrag_lock;
  // Append default behaviour radios.
  $output .= $draggableviews_default_on_top;
  // append extension modules output
  $output .= implode('', $extensions);

  // return output
  return $output;
}

/*
 * filter handlers by type
 *
 * @param $type
 *   the field type
 * @param $fields
 *   the fields array
 * return
 *   the available fields
 */
function _draggableviews_filter_fields($types = array(''), $handlers) {
  $available_fields = array();

  foreach ($handlers as $field => $handler) {
    if ($label = $handler->label()) {
      $available_fields[$field] = $label;
    }
    else {
      $available_fields[$field] = $handler->ui_name();
    }
  }
  return $available_fields;
}