<?php

/**
 * @file
 * Template preprocessors for the views_watchdog module.
 */


/**
 * Display a view as a watchdog table style.
 */
function template_preprocess_views_view_watchdog_table(&$vars) {
  $view     = $vars['view'];

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  // Store rows so that they may be used by further preprocess functions.
  $result   = $vars['result'] = $vars['rows'];
  $vars['rows'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  parse_str(tablesort_get_querystring(), $query);
  if (isset($view->exposed_raw_input)) {
    $query += $view->exposed_raw_input;
  }
  $query = empty($query) ? '' : '&' . http_build_query($query, '', '&');

  $header = array();

  // Fields must be rendered in order as of Views 2.3, so we will pre-render
  // everything.
  $renders = $handler->render_fields($result);

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

        if (!isset($vars['rows'][$num][$column])) {
          $vars['rows'][$num][$column] = '';
        }

        // Don't bother with separators and stuff if the field does not show up.
        if ($field_output === '') {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if ($vars['rows'][$num][$column] !== '') {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }

  $count = 0;
  foreach ($vars['rows'] as $num => $row) {
    $vars['row_classes'][$num][] = ($count++ % 2 == 0) ? 'odd' : 'even';
  }


  // Add custom CSS classes.
  if (!empty($view->style_plugin->options['watchdog_table_type']) || !empty($view->style_plugin->options['watchdog_table_severity'])) {
    $type_class = trim($view->style_plugin->options['watchdog_table_type_class']);
    $severity_class = trim($view->style_plugin->options['watchdog_table_severity_class']);

    foreach ($view->result as $key => $value) {
      $watchdog = db_fetch_object(db_query('SELECT type, severity FROM {watchdog} WHERE wid = %d', $value->wid));

      if (!empty($view->style_plugin->options['watchdog_table_type']) && !empty($type_class) && $watchdog) {
        $type_value = preg_replace('/[^a-z]/i', '-', $watchdog->type);
        $vars['row_classes'][$key][] = check_plain(str_replace('[type]', $type_value, $type_class));
      }
      if (!empty($view->style_plugin->options['watchdog_table_severity']) && !empty($severity_class) && $watchdog) {
        $severity_value = _views_watchdog_get_severity($watchdog->severity, 'arg');
        $vars['row_classes'][$key][] = check_plain(str_replace('[severity]', $severity_value, $severity_class));
      }
    }
  }


  $vars['row_classes'][0][] = 'views-row-first';
  $vars['row_classes'][count($vars['row_classes']) - 1][] = 'views-row-last';

  $vars['class'] = 'views-watchdog-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }
  $vars['class'] .= ' cols-' . count($vars['header']);

  $vars['attributes'] = '';
  if (!empty($handler->options['summary'])) {
    $vars['attributes'] = drupal_attributes(array('summary' => $handler->options['summary']));
  }
}
