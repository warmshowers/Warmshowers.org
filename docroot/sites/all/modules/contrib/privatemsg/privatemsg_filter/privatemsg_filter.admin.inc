<?php

/**
 * @file
 * Admin menu callbacks for privatemsg_filter module.
 */

/**
 * Display a list of tags to administer them.
 */
function privatemsg_tags_admin() {
  drupal_set_title(t('Tags'));

  $header = array(
    'tag' => array(
      'data'    => t('Tag'),
      'field'   => 'tag',
      'class'   => 'privatemsg-filter-header-tag',
      'sort'    => 'asc',
      '#weight' => -40,
    ),
    'usage' => array(
      'data'    => t('Usage'),
      'field'   => 'count',
      'class'   => 'privatemsg-filter-header-usage',
      '#weight' => -30,
    ),
    'public' => array(
      'data'    => t('Public'),
      'field'   => 'public',
      'class'   => 'privatemsg-filter-header-public',
      '#weight' => -20,
    ),
    'operations' => array(
      'data'    => t('Operations'),
      'class'   => 'privatemsg-filter-header-operations',
      '#weight' => -10,
    ),
  );
  $query = _privatemsg_assemble_query(array('tags', 'privatemsg_filter'));
  $result = pager_query($query['query'] . tablesort_sql($header), 100);

  $rows = array();
  while ($tag = db_fetch_array($result)) {
    $row = array();
    $row['tag'] = check_plain($tag['tag']);
    $row['count'] = $tag['count'];
    $row['public'] = $tag['public'] ? t('Yes') : '-';
    $row['operations'] = l(t('edit'), 'admin/settings/messages/tags/edit/' . $tag['tag_id']);
    $row['operations'] .= ' ' . l(t('delete'), 'admin/settings/messages/tags/delete/' . $tag['tag_id']);

    $rows[] = $row;
  }

  if (empty($rows)) {
    // Display a message if now tags are available.
    $rows[] = array(array('data' => t('No tags available.'), 'colspan' => count($header)));
  }

  return theme('table', $header, $rows) . theme('pager');
}

/**
 * A form to edit existing or create new tags.
 *
 * @param $form_state
 *   Form state array
 * @param $tag_id
 *   Tag Id if an edit form should be displayed, NULL for a create
 *   new tag form.
 */
function privatemsg_tags_form(&$form_state, $tag_id = NULL) {
  $form = array();

  if ($tag_id) {
    $tag = db_fetch_array(db_query('SELECT * from {pm_tags} pmt WHERE pmt.tag_id = %d', $tag_id));
    $form['tag_id'] = array(
      '#value' => $tag_id,
      '#type' => 'value',
    );
    drupal_set_title(t('Edit @tag', array('@tag' => $tag['tag'])));
  }

  $form['tag'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => empty($tag_id) ? NULL : $tag['tag'],
  );

  $form['public'] = array(
    '#title' => t('Public'),
    '#type' => 'checkbox',
    '#default_value' => empty($tag_id) ? TRUE : $tag['public'],
    '#description' => t('Public tags are visible for all users, private tags are only visible if they use them.'),
  );

  $form['submit'] = array(
    '#value' => empty($tag_id) ? t('Create tag') : t('Save tag'),
    '#type'  => 'submit',
  );

  return $form;
}

/**
 * Form validate function for tags admin page.
 *
 * Make sure that a tag name is unique.
 */
function privatemsg_tags_form_validate($form, &$form_state) {
  $tag_id = isset($form_state['values']['tag_id']) ? $form_state['values']['tag_id'] : 0;
  if ($tag_id) {
    // We are editing an existing tag, exclude the current tag from the search.
    $exists = db_result(db_query("SELECT 1 FROM {pm_tags} WHERE tag = '%s' AND tag_id <> %d", $form_state['values']['tag'], $tag_id));
  }
  else {
    $exists = db_result(db_query("SELECT 1 FROM {pm_tags} WHERE tag = '%s'", $form_state['values']['tag']));
  }
  if ($exists) {
    form_set_error('tag', t('Tag already exists, choose a different name.'));
  }
}

/**
 * Form submit handler for add/edit forms.
 */
function privatemsg_tags_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['tag_id'])) {
    // Tag already exists, update the existing tag.
    db_query("UPDATE {pm_tags} SET tag = '%s', public = %d WHERE tag_id = %d", $form_state['values']['tag'], $form_state['values']['public'], $form_state['values']['tag_id']);
    drupal_set_message(t('Tag updated.'));
    $form_state['redirect'] = 'admin/settings/messages/tags';
  }
  else {
    // Tag does not yet exist, create a new one.
    db_query("INSERT INTO {pm_tags} (tag, public) VALUES ('%s', %d)", $form_state['values']['tag'], $form_state['values']['public']);
    drupal_set_message(t('Tag created.'));
  }
}

/**
 * Confirmation form for deleting tags.
 *
 * @param $tag_id
 *   Id of the tag that should be deleted.
 */
function privatemsg_filter_tags_delete(&$form_state, $tag_id) {
  $form['tag_id'] = array(
    '#type' => 'value',
    '#value' => $tag_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete?'),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/settings/messages/tags',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for the confirm delete form.
 */
function privatemsg_filter_tags_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_query('DELETE FROM {pm_tags} WHERE tag_id = %d', $form_state['values']['tag_id']);
    db_query('DELETE FROM {pm_tags_index} WHERE tag_id = %d', $form_state['values']['tag_id']);

    drupal_set_message(t('Tag has been deleted'));
  }
  $form_state['redirect'] = 'admin/settings/messages/tags';
}


function privatemsg_filter_admin() {
  $form = array();

  $form['privatemsg_filter_searchbody'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Search message body'),
    '#description'   => t('WARNING: turning on this feature will slow down search performance by a large factor. Gets worse as your messages database increases.'),
    '#default_value' => variable_get('privatemsg_filter_searchbody', FALSE),
  );

    $form['privatemsg_filter_tagfield_weight'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Position of the tagging textfield'),
    '#description'   => t('Use higher values to push the form lower down the page, lower or negative values to raise it higher.'),
    '#size'          => 4,
    '#default_value' => variable_get('privatemsg_filter_tagfield_weight', 10),
  );

  return system_settings_form($form);
}

/**
 * Display confirmation message before rebuilding inbox tag.
 */
function privatemsg_filter_inbox_rebuid_form() {
  $status = '<p>'. t('Newly sent messages are automatically tagged with a hidden system tag. To also have existing messages show up in the inbox, you need to process these by starting the rebuild process. Rebuilding may take some time if there are many messages. After rebuilding has completed messages will automatically show up in the inbox.') .'</p>';

  $form['status'] = array('#value' => $status);
  $form['rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild inbox'),
  );

  return $form;
}

/**
 * Submit callback for inbox rebuild form.
 */
function privatemsg_filter_inbox_rebuid_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Rebuilding inbox'),
    'operations' => array(
      array('privatemsg_filter_inbox_rebuild_process', array()),
    ),
    'finished' => 'privatemsg_filter_inbox_rebuild_finished',
    'file' => drupal_get_path('module', 'privatemsg_filter') . '/privatemsg_filter.admin.inc',
  );
  batch_set($batch);
}

/**
 * Batch processing function for rebuilding the index.
 */
function privatemsg_filter_inbox_rebuild_process(&$context) {
  // First run, initialize sandbox.
  if (!isset($context['sandbox']['current_thread_id'])) {
    $context['sandbox']['current_thread_id'] = 0;
    // Assume that the thread ids are distributed more or less equally over the
    // whole data set. This allows us to calculate the approximate progress.
    $context['sandbox']['max'] = db_result(db_query('SELECT MAX(thread_id) FROM {pm_index}'));
    $context['results']['count'] = 0;
  }

  // Fetch the 10 next thread_ids.
  $result = db_query_range('SELECT DISTINCT thread_id FROM {pm_index} WHERE thread_id > %d ORDER BY thread_id ASC', $context['sandbox']['current_thread_id'], 0, 20);
  $threads = array();
  while ($row = db_fetch_object($result)) {
    $threads[] = $row->thread_id;
  }

  if (!empty($threads)) {
    // By limiting this slow query (having condition with 2 depending subqueries)
    // on a specific set of threads, this allows us to process the slow having
    // part on a relatively small subset of pm_index that can be selected based on
    // the thread_id index.
    $sql = 'SELECT pmi.thread_id, pmi.recipient, pmi.type FROM {pm_index} pmi WHERE thread_id IN (' . db_placeholders($threads) . ')  GROUP BY pmi.thread_id, pmi.recipient HAVING ((SELECT pmf.author FROM {pm_message} pmf WHERE pmf.mid = pmi.thread_id) = pmi.recipient AND pmi.type IN ("user", "hidden") AND COUNT(pmi.thread_id) > 1) OR (SELECT COUNT(*) FROM {pm_message} pmf INNER JOIN {pm_index} pmif ON (pmf.mid = pmif.mid) WHERE pmif.thread_id = pmi.thread_id AND pmf.author <> pmi.recipient AND pmi.type IN ("user", "hidden")) > 0';
    $result = db_query($sql, $threads);
    while ($row = db_fetch_object($result)) {
      // $row is an object with uid property, so we pass it to the function as a
      // pseudo user object.
      $row->uid = $row->recipient;
      privatemsg_filter_add_tags(array($row->thread_id), variable_get('privatemsg_filter_inbox_tag', ''), $row);
      $context['results']['count']++;
    }
    $context['sandbox']['current_thread_id'] = max($threads);
  }
  // Set #finished based on sandbox.
  $context['finished'] = empty($context['sandbox']['max']) ? 1 : ($context['sandbox']['current_thread_id'] / $context['sandbox']['max']);
}

/**
 * Finished batch callback for index rebuild.
 */
function privatemsg_filter_inbox_rebuild_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural($results['count'], 'One thread tagged.', '@count threads tagged.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}