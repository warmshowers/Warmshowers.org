<?php

/**
 * @file queue_ui.pages.inc
 */

/**
 * Queue form handler.
 */
function queue_ui_page($form, &$form_state) {
  //queue_ui_test(); // @todo remove before prod
  // Initialize.
  if ($form_state['rebuild']) {
    $form_state['input'] = array();
  }
  if (empty($form_state['storage'])) {
    // First step, so start with our overview form.
    $form_state['storage'] = array(
      'step' => 'queue_ui_overview_form',
    );
  }
  // Return the form from the current step.
  $function = $form_state['storage']['step'];
  $form = $function($form, $form_state);
  return $form;
}

function queue_ui_page_submit($form, &$form_state) {
  $values = $form_state['values'];
  $queue_classes = _queue_ui_array_keys_contain($values, 'queues');

  $queues = array();

  foreach ($queue_classes as $class_name) {
    foreach ($values[$class_name] as $queue) {
      $queues[] = $queue;
    }
  }
  // Get submitted queues to act on.
  $queues = array_filter($queues);

  if (empty($queues)) {
    // Nothing to do.
    return;
  }
  if (isset($values['step_submit'])) {
    // Pass off to step submit handler.
    $function = $values['step_submit'];
    $function($form, $form_state, $queues);
  }
  return;
}

function queue_ui_overview_form() {
  $queues = $options = array();
  // @todo activation status
  $header = array(
    'name' => array('data' => t('Name')),
    'title' => array('data' => t('Title')),
    'items' => array('data' => t('Number of items')),
    'class' => array('data' => t('Class')),
    'inspect' => array('data' => t('Inspect')),
     //'operations' => array('data' => t('Operations')),
  );
  // Get queues defined via our hook.
  $defined_queues = queue_ui_defined_queues();
  // Get queues names.
  $queues = queue_ui_queues();

  foreach ($queues as $class => $classed_queues) {
    $options = array();

    $class_info = QueueUI::get('QueueUI' . $class);

    // Output information for each queue of the current class
    foreach($classed_queues as $name => $queue) {
      $title = '';
      $operations = '';
      $inspect = FALSE;

      if (isset($defined_queues[$name])) {
        $title = $defined_queues[$name]['title'];
      }
      if (isset($defined_queues[$name]['batch'])) {
        $operations = 'batch';
      }
      if (is_object($class_info) && $class_info->inspect) {
        $inspect = TRUE;
      }

      $options[$name] = array(
        'name' => array('data' => $name),
        'title' => array('data' => $title),
        'items' => array('data' => $queue['items']),
        'class' => array('data' => $class),
        //'operations' => array('data' => $operations),
      );

      // If queue inspection is enabled for this class, add to the options array.
      if ($inspect) {
        $options[$name]['inspect'] = array('data' => l(t('Inspect'), QUEUE_UI_BASE . "/inspect/$name"));
      }
      else {
        $options[$name]['inspect'] = '';
      }
    }

    $form[$class] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => $class,
     );

    $form[$class]['queues_' . $class] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No queues exist for @class.', array('@class' => $class)),
    );
  }

  // @todo deactivate options
  // Option to run batch.
  $form['batch'] = array(
    '#type' => 'submit',
    '#value' => t('Batch process'),
  );
  // Option to remove lease timestamps.
  $form['release'] = array(
    '#type' => 'submit',
    '#value' => t('Remove leases'),
  );
  // Option to run via cron.
  $form['cron'] = array(
    '#type' => 'submit',
    '#value' => t('Cron process'),
  );
  // Option to delete queue.
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete queues'),
  );
  // Specify our step submit callback.
  $form['step_submit'] = array('#type' => 'value', '#value' => 'queue_ui_overview_submit');
  return $form;
}

/**
 * Overview submit handler.
 */
function queue_ui_overview_submit($form, &$form_state, $queues) {
  $values = $form_state['values'];
  // Switch off submitted action.
  switch ($values['op']) {
    case $values['cron']:
      // Set variables for cron to TRUE.
      $defined_queues = queue_ui_defined_queues();
      $intersect = array_intersect(array_keys($defined_queues), $queues);
      foreach ($intersect as $name) {
        if (isset($defined_queues[$name]['cron'])) {
          variable_set('queue_ui_cron_' . $name, TRUE);
        }
      }
      break;
    case $values['batch']:
      // Process queue(s) with batch.
      // We can only run batch on queues using our hook_queue_info() that define batch.
      $defined_queues = queue_ui_defined_queues();
      $intersect = array_intersect(array_keys($defined_queues), $queues);
      foreach ($intersect as $name) {
        // Only if queue_info implementation defined batch can we set it here.
        if (isset($defined_queues[$name]['batch'])) {
          $batch = $defined_queues[$name]['batch'];
          // Add queue as argument to operations by resetting the operations array.
          $operations = array();
          $queue = DrupalQueue::get($name);
          foreach ($batch['operations'] as $operation) {
            // First element is the batch process callback, second is the argument.
            $operations[] = array($operation[0], array_merge(array($queue), $operation[1]));
          }
          $batch['operations'] = $operations;
          // Set.
          batch_set($batch);
        }
      }
      break;
    case $values['delete']:
      // Confirm before deleting. Go multistep!
      $form_state['rebuild'] = TRUE;
      $form_state['storage']['queues'] = $queues;
      $form_state['storage']['step'] = 'queue_ui_confirm_delete';
      break;
    case $values['release']:
      foreach ($queues as $name) {
        $num_updated = db_update('queue')
          ->fields(array(
            'expire' => 0,
          ))
          ->condition('name', $name, '=')
          ->execute();
        drupal_set_message(t('!count lease reset in queue !name', array('!count' => $num_updated, '!name' => $name)));
      }
      break;
  }
  return;
}

/**
 * Confirm form for deleting queues.
 */
function queue_ui_confirm_delete($form, &$form_state) {
  $form['queues'] = array('#type' => 'value', '#value' => $form_state['storage']['queues']);
  $description = t('All items in each queue will be deleted, regardless of if leases exist. This operation cannot be undone.');
  // Specify our step submit callback.
  $form['step_submit'] = array('#type' => 'value', '#value' => 'queue_ui_delete_submit');
  return confirm_form($form,
    format_plural(count($form_state['storage']['queues']), 'Are you sure you want to delete the queue?', 'Are you sure you want to delete @count queues?'),
    'admin/config/system/queue-ui',
    $description,
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting queues.
 */
function queue_ui_delete_submit($form, &$form_state, $queues) {
  $values = $form_state['values'];
  $defined_queues = queue_ui_defined_queues();
  foreach ($queues as $name) {
    $queue = DrupalQueue::get($name);
    if (isset($defined_queues[$name]['delete'])) {
      $function = $defined_queues[$name]['delete'];
      $function($queue);
    }
    $queue->deleteQueue();
  }
  drupal_set_message(format_plural(count($values['queues']), 'Queue deleted', '@count queues deleted'));
  return;
}

function _queue_ui_array_keys_contain($input, $search_value, $strict = FALSE) {
  $tmpkeys = array();

  $keys = array_keys($input);

  foreach ($keys as $k) {
    if ($strict && strpos($k, $search_value) !== FALSE) {
      $tmpkeys[] = $k;
    }
    elseif (!$strict && stripos($k, $search_value) !== FALSE) {
      $tmpkeys[] = $k;
    }
  }

  return $tmpkeys;
}