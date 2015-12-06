<?php

/**
 * View all the items in a queue.
 */
function queue_ui_view_queue($queue_name) {
  if ($queue = _queue_ui_queueclass($queue_name)) {
    return $queue->inspect($queue_name);
  }
}


/**
 * Display the details of an individual queue item.
 */
function queue_ui_view_queue_item($queue_name, $queue_item) {
  if ($queue = _queue_ui_queueclass($queue_name)) {
    return $queue->view($queue_item);
  }
  // @TODO - if not implemented display an error.
}

/**
 * Releases an item from its queue by setting expiry to zero).
 *
 * @see queue_ui_release_item_submit()
 */
function queue_ui_release_item_form($form, $form_state, $queue_name, $queue_item) {
  if ($queue = _queue_ui_queueclass($queue_name)) {
    return confirm_form(
      array(
        'queue_item' => array(
          '#type' => 'value',
          '#value' => array('queue_name' => $queue_name, 'itemid' => $queue_item),
        ),
      ),
      t('Are you sure you want to release queue item %queue_item?', array('%queue_item' => $queue_item)),
      QUEUE_UI_BASE . "/inspect/$queue_name",
      t('This action cannot be undone and will force the release of the item even if it is currently being processed.'),
      t('Release item'),
      t('Cancel')
    );
  }
  // @TODO - if not implemented display an error.
}

/**
 * Form submission handler for queue_ui_release_item_form().
 */
function queue_ui_release_item_form_submit($form, &$form_state) {
  $queue_data = $form_state['values']['queue_item'];
  $queue_name = $queue_data['queue_name'];

  $queue = _queue_ui_queueclass($queue_name);
  $queue->release($queue_data['itemid']);

  $form_state['redirect'] = QUEUE_UI_BASE . "/inspect/$queue_name";
}

/**
 * Delete a specified queue item.
 */
function queue_ui_delete_item_form($form, $form_state, $queue_name, $queue_item) {
  if ($queue = _queue_ui_queueclass($queue_name)) {
    return confirm_form(
      array(
        'queue_item' => array(
          '#type' => 'value',
          '#value' => array('queue_name' => $queue_name, 'itemid' => $queue_item),
        ),
      ),
      t('Are you sure you want to delete queue item %queue_item?', array('%queue_item' => $queue_item)),
      QUEUE_UI_BASE . "/inspect/$queue_name",
      t('This action cannot be undone and will force the deletion of the item even if it is currently being processed.'),
      t('Release item'),
      t('Cancel')
    );
  }



  // @TODO - if not implemented display an error.
}

/**
 * Form submission handler for queue_ui_delete_item_form().
 */
function queue_ui_delete_item_form_submit($form, &$form_state) {
  $queue_data = $form_state['values']['queue_item'];
  $queue_name = $queue_data['queue_name'];

  $queue = _queue_ui_queueclass($queue_name);
  $queue->delete($queue_data['itemid']);

  $form_state['redirect'] = QUEUE_UI_BASE . "/inspect/$queue_name";
}