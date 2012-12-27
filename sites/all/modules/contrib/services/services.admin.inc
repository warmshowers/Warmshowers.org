<?php

/**
 * @file
 * This file will define the administrative methods of services
 */
function theme_services_resource_table($variables) {
  $table = $variables;

  drupal_add_css(drupal_get_path('module', 'services') . '/css/services.admin.css');
  drupal_add_js(drupal_get_path('module', 'services') . '/js/services.admin.js');
  drupal_add_js('misc/tableselect.js');

  // Create header for resource selection table.
  $header = array(
    array('class' => 'select-all'),
    array('data' => t('Resource'), 'class' => 'resource_method'),
    array('data' => t('Settings'), 'class' => 'resource_description'),
    array('data' => t('Alias'), 'class' => 'resource_alias'),
  );

  // Define the images used to expand/collapse the method groups.
  $js = array(
    'images' => array(
      #theme('image', 'misc/menu-collapsed.png', 'Expand', 'Expand'),
      #theme('image', 'misc/menu-expanded.png', 'Collapsed', 'Collapsed'),
      # TODO: yuck.
      'collapsed' => theme('image', 'misc/menu-collapsed.png', t('Expand'),   t('Expand')) .   ' <a href="#" class="resource-collapse">(' . t('Expand') .  ')</a>',
      'expanded' =>  theme('image', 'misc/menu-expanded.png',  t('Collapse'), t('Collapse')) . ' <a href="#" class="resource-collapse">(' . t('Collapse') . ')</a>',
    ),
  );

  // Cycle through each method group and create a row.
  $rows = array();
  foreach (element_children($table) as $key) {
    $element = &$table[$key];
    $row = array();

    // Make the class name safe for output on the page by replacing all
    // non-word/decimal characters with a dash (-).
    $method_class = drupal_strtolower(trim(preg_replace("/[^\w\d]/", "-", $key)));

    // Select the right "expand"/"collapse" image, depending on whether the
    // category is expanded (at least one method selected) or not.
    $collapsed = !empty($element['#collapsed']);

    // Place-holder for checkboxes to select group of methods.
    $row[] = array('id' => $method_class, 'class' => 'resource-select-all');

    // Expand/collapse image and group title.
    $row[] = array(
      'data' => '<div class="resource-image" id="resource-method-group-' . $method_class . '" data-resource="' . $method_class . '"></div>' .
        '<label for="' . $method_class . '-select-all" class="resource-group-label">' . $key . '</label>',
      'class' => 'resource-group-label',
    );

    $row[] = array(
      'data' => '&nbsp;',
      'class' => 'resource-group-description',
    );
    $row[] = array(
      'data' => drupal_render($element['alias']),
      'class' => 'resource-group-alias',
    );
    $rows[] = array('data' => $row, 'class' => 'resource-group');

    // Add individual methods to group.
    $current_js = array(
      'collapsed' => $collapsed,
      'methodClass' => $method_class . '-method',
      'clickActive' => FALSE,
    );

    // Cycle through each method within the current group.
    foreach (element_children($element) as $class) {
      if (!strpos($class, 'alias') && $class != 'alias') {
        $class_element = $element[$class];

        // Add group (class) header row.
        $rows[] = array('data' => array(NULL, array(
          'data' => '<label>' . $class_element['#title'] . '</label>',
          'class' => 'resource-operation-class',
        ), NULL, NULL), 'class' => $method_class . '-method '. 'resource-operation-class');

        foreach (element_children($class_element) as $op_name) {
          $row = array();
          $method = $class_element[$op_name];

          // Store method title and description so that checkbox won't render them.
          $title = $method['#title'];
          $description = $method['#description'];

          /* from 7.x-3.x:
           * $method['#title_display'] = 'invisible';
           * $method['enabled']['#title_display'] = 'invisible';
           */
          $method['#title_display'] = 'invisible';
          $method['enabled']['#title_display'] = 'invisible';
          unset($method['#title']);
          unset($method['#description']);

          // Test name is used to determine what methods to run.
          $method['#name'] = $class;

          $row[] = array(
            'data' => drupal_render($method['enabled']),
            'class' => 'resource-method-select',
          );
          $row[] = array(
            'data' => '<label for="' . $method['enabled']['#id'] . '">' . $title . '</label>' . '<div class="description">' . $description . '</div>',
            'class' => 'resource-method-description',
          );
          $row[] = array(
            'data' => drupal_render($method['settings']),
            'class' => 'resource-method-settings',
          );
          $row[] = array(
            'data' => '<div class="alias">&nbsp;</div>',
            'class' => 'resource-method-alias',
          );
          $rows[] = array('data' => $row, 'class' => $method_class . '-method');
        }
      }
    }
    unset($table[$key]);
    $js['resources'][$method_class] = $current_js;
  }

  // Add js array of settings.
  drupal_add_js(array('services' => $js), 'setting');

  if (empty($rows)) {
    return '<strong>' . t('No resourcess to display.') . '</strong>';
  }
  else {
      watchdog('rows', print_r($rows, 1));
    return theme('table', $header, $rows, array('id' => 'resource-form-table'));
    //return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'resource-form-table')));
  }
}
