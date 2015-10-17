<?php
/**
 * @file
 * Export-ui handler for the Ultimate Cron jobs.
 */

class ultimate_cron_job_ctools_export_ui extends ctools_export_ui {
  /**
   * Accumulator for jobs that are behind.
   * @var integer
   */
  protected $jobs_behind = 0;

  /**
   * Access handler for an operation on a specific item.
   *
   * @param string $op
   *   The operation in question.
   * @param UltimateCronJob $item
   *   The cron job.
   *
   * @return bool
   *   TRUE if access FALSE if not.
   */
  public function access($op, $item) {
    switch ($op) {
      case 'list':
        return user_access('administer ultimate cron') || user_access($this->plugin['access']);
    }

    // More fine-grained access control:
    $key = $op . ' access';
    if (!empty($this->plugin[$key])) {
      if (!user_access($this->plugin[$key])) {
        return FALSE;
      }
    }

    // If we need to do a token test, do it here.
    if (empty($this->notoken) && !empty($this->plugin['allowed operations'][$op]['token']) && (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], $op))) {
      return FALSE;
    }

    switch ($op) {
      case 'import':
        return user_access('use PHP for settings');

      case 'revert':
        return ($item->export_type & EXPORT_IN_DATABASE) && ($item->export_type & EXPORT_IN_CODE);

      case 'delete':
        return ($item->export_type & EXPORT_IN_DATABASE) && !($item->export_type & EXPORT_IN_CODE);

      case 'disable':
        return empty($item->disabled);

      case 'enable':
        return !empty($item->disabled);

      case 'configure':
        if (!empty($item->hook['configure'])) {
          $cache = cache_get($item->hook['configure'], 'cache_menu');
          if ($cache) {
            $router_item = menu_get_item($item->hook['configure'], $cache->data);
          }
          else {
            $router_item = menu_get_item($item->hook['configure']);
            cache_set($item->hook['configure'], $router_item, 'cache_menu');
          }
          return $router_item['access'];
        }
        return TRUE;

      default:
        return TRUE;
    }
  }

  /**
   * Ensure we cannot add, import, delete or clone.
   */
  public function hook_menu(&$items) {
    parent::hook_menu($items);

    unset($items['admin/config/system/cron/jobs/add']);
    // unset($items['admin/config/system/cron/jobs/import']);
    unset($items['admin/config/system/cron/jobs/list/%ctools_export_ui/delete']);
    unset($items['admin/config/system/cron/jobs/list/%ctools_export_ui/clone']);
  }

  /**
   * Ensure that we cannot clone from the operations link list.
   */
  public function build_operations($item) {
    $item->lock_id = isset($item->lock_id) ? $item->lock_id : $item->isLocked();
    $allowed_operations = parent::build_operations($item);
    unset($allowed_operations['clone']);
    if ($item->lock_id) {
      unset($allowed_operations['run']);
      $allowed_operations['unlock']['href'] .= '/' . $item->lock_id;
    }
    else {
      unset($allowed_operations['unlock']);
    }
    if (!empty($item->hook['configure'])) {
      $allowed_operations['configure'] = array(
        'title' => t('Configure'),
        'href' => $item->hook['configure'],
      );
    }

    if (!empty($item->hook['immutable'])) {
      unset($allowed_operations['edit']);
      unset($allowed_operations['disable']);
      unset($allowed_operations['enable']);
      unset($allowed_operations['export']);
    }

    if (variable_get('maintenance_mode', 0)) {
      unset($allowed_operations['run']);
    }

    $item->build_operations_alter($allowed_operations);

    $default_sort = array(
      'logs' => -10,
      'edit' => -2,
      'enable' => -1,
      'disable' => -1,
      'run' => 0,
      'unlock' => 0,
      'export' => 1,
      'configure' => 1,
    );

    $weight = 0;
    $this->notoken = TRUE;
    foreach ($allowed_operations as $name => &$operation) {
      if (!$this->access($name, $item)) {
        unset($allowed_operations[$name]);
        continue;
      }
      $operation += array(
        'sort' => array(
          isset($default_sort[$name]) ? $default_sort[$name] : 0,
        ),
        'alias' => TRUE,
      );
      $operation['sort'][] = $weight++;
    }
    unset($this->notoken);
    uasort($allowed_operations, '_ultimate_cron_multi_column_sort');
    return $allowed_operations;
  }

  /**
   * Custom action for plugins.
   */
  public function signal_page($js, $input, $item, $plugin_type, $plugin_name, $signal) {
    $item->signal($item, $plugin_type, $plugin_name, $signal);
    if (!$js) {
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }
    else {
      return $this->list_page($js, $input);
    }
  }

  /**
   * Run a job callback.
   */
  public function run_page($js, $input, $item) {
    $item->launch();
    if (!$js) {
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }
    else {
      return $this->list_page($js, $input);
    }
  }

  /**
   * Unlock a job callback.
   */
  public function unlock_page($js, $input, $item, $lock_id) {
    if ($item->unlock($lock_id, TRUE)) {
      $log_entry = $item->resumeLog($lock_id);
      global $user;
      $username = $user->uid ? $user->name : t('anonymous');
      watchdog('ultimate_cron', '@name manually unlocked by user @username (@uid)', array(
        '@name' => $item->name,
        '@username' => $username,
        '@uid' => $user->uid,
      ), WATCHDOG_NOTICE);
      $log_entry->finish();
    }

    if (!$js) {
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }
    else {
      return $this->list_page($js, $input);
    }
  }

  /**
   * Page with logs.
   */
  public function logs_page($js, $input, $item) {
    $log_entries = $item->getLogEntries();
    $output = '';
    $header = array(
      t('Started'),
      t('Duration'),
      t('Launched by'),
      t('Initial message'),
      t('Message'),
      t('Status'),
    );

    $item->lock_id = isset($item->lock_id) ? $item->lock_id : $item->isLocked();
    $rows = array();
    foreach ($log_entries as $log_entry) {
      $rows[$log_entry->lid]['data'] = array();
      $rows[$log_entry->lid]['data'][] = array('data' => $log_entry->formatStartTime(), 'class' => array('ctools-export-ui-start-time'));

      $progress = '';
      if ($log_entry->lid && $item->lock_id && $log_entry->lid === $item->lock_id) {
        $progress = ' ' . $item->formatProgress();
      }

      $rows[$log_entry->lid]['data'][] = array(
        'data' => $log_entry->formatDuration() . $progress,
        'class' => array('ctools-export-ui-duration'),
        'title' => strip_tags($log_entry->formatEndTime()),
      );

      $rows[$log_entry->lid]['data'][] = array('data' => $log_entry->formatUser(), 'class' => array('ctools-export-ui-user'));
      $rows[$log_entry->lid]['data'][] = array('data' => '<pre>' . $log_entry->init_message . '</pre>', 'class' => array('ctools-export-ui-init-message'));
      $rows[$log_entry->lid]['data'][] = array('data' => '<pre>' . $log_entry->message . '</pre>', 'class' => array('ctools-export-ui-message'));

      // Status.
      if ($item->lock_id && $log_entry->lid == $item->lock_id) {
        list($status, $title) = $item->getPlugin('launcher')->formatRunning($item);
      }
      elseif ($log_entry->start_time && !$log_entry->end_time) {
        list($status, $title) = $item->getPlugin('launcher')->formatUnfinished($item);
      }
      else {
        list($status, $title) = $log_entry->formatSeverity();
      }

      $rows[$log_entry->lid]['data'][] = array(
        'data' => $status,
        'class' => array('ctools-export-ui-status'),
        'title' => strip_tags($title),
      );

    }
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('No log entries exists for this job yet.'),
    ));
    $output .= theme('pager');
    return $output;
  }

  /**
   * Create the filter/sort form at the top of a list of exports.
   *
   * This handles the very default conditions, and most lists are expected
   * to override this and call through to parent::list_form() in order to
   * get the base form and then modify it as necessary to add search
   * gadgets for custom fields.
   */
  public function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);

    $class = _ultimate_cron_get_class('job');
    $lock_ids = $class::isLockedMultiple($this->items);
    $log_entries = $class::loadLatestLogEntries($this->items);
    $progresses = $class::getProgressMultiple($this->items);
    foreach ($this->items as $name => $item) {
      $item->log_entry = isset($item->log_entry) ? $item->log_entry : $log_entries[$name];
      $item->progress = isset($item->progress) ? $item->progress : $progresses[$name];
      $item->lock_id = isset($item->lock_id) ? $item->lock_id : $lock_ids[$name];
    }

    $form['#attached']['js'][] = drupal_get_path('module', 'ultimate_cron') . '/js/ultimate_cron.js';

    if (module_exists('nodejs')) {
      $settings = _ultimate_cron_plugin_load('settings', 'general')->getDefaultSettings();
      if (!empty($settings['nodejs'])) {
        nodejs_send_content_channel_token('ultimate_cron');
        $form['#attached']['js'][] = drupal_get_path('module', 'ultimate_cron') . '/js/ultimate_cron.nodejs.js';
      }
    }

    // There's no normal for Ultimate Cron!
    unset($form['top row']['storage']['#options'][t('Normal')]);

    $all = array('all' => t('- All -'));

    $options = $all + array(
      'running' => 'running',
      -1 => 'no info',
    ) + watchdog_severity_levels();
    $form['top row']['status'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => $options,
      '#default_value' => 'all',
      '#weight' => -2,
    );

    $jobs = ultimate_cron_get_hooks();
    $modules = array();
    foreach ($jobs as $job) {
      $info = system_get_info('module', $job['module']);
      $modules[$job['module']] = $info && !empty($info['name']) ? $info['name'] : $job['module'];
    }

    $form['top row']['module'] = array(
      '#type' => 'select',
      '#title' => t('Module'),
      '#options' => $all + $modules,
      '#default_value' => 'all',
      '#weight' => -1,
    );
    $form['bottom row']['reload'] = array(
      '#type' => 'submit',
      '#id' => 'ctools-export-ui-list-items-reload',
      '#value' => t('Reload'),
      '#attributes' => array('class' => array('use-ajax-submit')),
    );
  }

  /**
   * Determine if a row should be filtered out.
   *
   * This handles the default filters for the export UI list form. If you
   * added additional filters in list_form() then this is where you should
   * handle them.
   *
   * @return bool
   *   TRUE if the item should be excluded.
   */
  public function list_filter($form_state, $item) {
    $schema = ctools_export_get_schema($this->plugin['schema']);
    if ($form_state['values']['storage'] != 'all' && $form_state['values']['storage'] != $item->{$schema['export']['export type string']}) {
      return TRUE;
    }

    if ($form_state['values']['module'] != 'all' && $form_state['values']['module'] != $item->hook['module']) {
      return TRUE;
    }

    $item->log_entry = isset($item->log_entry) ? $item->log_entry : $item->loadLatestLogEntry();
    $item->lock_id = isset($item->lock_id) ? $item->lock_id : $item->isLocked();

    if ($form_state['values']['status'] == 'running') {
      if (!$item->lock_id) {
        return TRUE;
      }
    }
    elseif ($form_state['values']['status'] != 'all' && $form_state['values']['status'] != $item->log_entry->severity) {
      return TRUE;
    }

    if ($form_state['values']['disabled'] != 'all' && $form_state['values']['disabled'] != !empty($item->disabled)) {
      return TRUE;
    }

    if ($form_state['values']['search']) {
      $search = strtolower($form_state['values']['search']);
      foreach ($this->list_search_fields() as $field) {
        if (strpos(strtolower($item->$field), $search) !== FALSE) {
          $hit = TRUE;
          break;
        }
      }
      if (empty($hit)) {
        return TRUE;
      }
    }
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  public function list_table_header() {
    $header = array();
    $header[] = array('data' => t('Module'), 'class' => array('ctools-export-ui-module'));
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }

    $header[] = array('data' => t('Scheduled'), 'class' => array('ctools-export-ui-scheduled'));
    $header[] = array('data' => t('Started'), 'class' => array('ctools-export-ui-start-time'));
    $header[] = array('data' => t('Duration'), 'class' => array('ctools-export-ui-duration'));
    $header[] = array('data' => t('Status'), 'class' => array('ctools-export-ui-status'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

  /**
   * Provide a list of sort options.
   *
   * Override this if you wish to provide more or change how these work.
   * The actual handling of the sorting will happen in build_row().
   */
  public function list_sort_options() {
    if (!empty($this->plugin['export']['admin_title'])) {
      $options = array(
        'disabled' => t('Enabled, module, title'),
        $this->plugin['export']['admin_title'] => t('Title'),
      );
    }
    else {
      $options = array(
        'disabled' => t('Enabled, module, name'),
      );
    }

    $options += array(
      'name' => t('Name'),
      'start_time' => t('Started'),
      'duration' => t('Duration'),
      'storage' => t('Storage'),
    );

    return $options;
  }

  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  public function list_build_row($item, &$form_state, $operations) {
    // Set up sorting.
    $name = $item->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Started and duration.
    $item->lock_id = isset($item->lock_id) ? $item->lock_id : $item->isLocked();
    $item->log_entry = isset($item->log_entry) ? $item->log_entry : $item->loadLatestLogEntry();
    $item->progress = isset($item->progress) ? $item->progress : $item->getProgress();
    if ($item->log_entry->lid && $item->lock_id && $item->log_entry->lid !== $item->lock_id) {
      $item->log_entry = $item->loadLogEntry($item->lock_id);
    }

    // Note: $item->{$schema['export']['export type string']} should have
    // already been set up by export.inc so we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->rows[$name]['sort'] = array(
          (int) !empty($item->disabled),
          $item->getModuleName(),
          empty($this->plugin['export']['admin_title']) ? $name : $item->{$this->plugin['export']['admin_title']},
        );
        break;

      case 'title':
        $this->rows[$name]['sort'] = array($item->{$this->plugin['export']['admin_title']});
        break;

      case 'start_time':
        $this->rows[$name]['sort'] = array($item->log_entry->start_time);
        break;

      case 'duration':
        $this->rows[$name]['sort'] = array($item->log_entry->getDuration());
        break;

      case 'storage':
        $this->rows[$name]['sort'] = array($item->{$schema['export']['export type string']} . $name);
        break;
    }

    // Setup row.
    $this->rows[$name]['id'] = $name;
    $this->rows[$name]['data'] = array();

    // Enabled/disabled.
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // Module.
    $this->rows[$name]['data'][] = array(
      'data' => check_plain($item->getModuleName()),
      'class' => array('ctools-export-ui-module'),
      'title' => strip_tags($item->getModuleDescription()),
    );

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array(
        'data' => check_plain($item->{$this->plugin['export']['admin_title']}),
        'class' => array('ctools-export-ui-title'),
        'title' => strip_tags($item->name),
      );
    }

    // Schedule settings.
    $label = $item->getPlugin('scheduler')->formatLabel($item);
    $label = str_replace("\n", '<br/>', $label);
    if ($behind = $item->isBehindSchedule()) {
      $this->jobs_behind++;
      $label = "<em>$label</em><br/>" . format_interval($behind) . ' ' . t('behind schedule');
    }
    $this->rows[$name]['data'][] = array(
      'data' => $label,
      'class' => array('ctools-export-ui-scheduled'),
      'title' => strip_tags($item->getPlugin('scheduler')->formatLabelVerbose($item)),
    );

    $this->rows[$name]['data'][] = array(
      'data' => $item->log_entry->formatStartTime(),
      'class' => array('ctools-export-ui-start-time'),
      'title' => strip_tags($item->log_entry->formatInitMessage()),
    );

    $progress = $item->lock_id ? $item->formatProgress() : '';
    $this->rows[$name]['data'][] = array(
      'data' => '<span class="duration-time" data-src="' . $item->log_entry->getDuration() . '">' . $item->log_entry->formatDuration() . '</span> <span class="duration-progress">' . $progress . '</span>',
      'class' => array('ctools-export-ui-duration'),
      'title' => strip_tags($item->log_entry->formatEndTime()),
    );

    // Status.
    if ($item->lock_id && $item->log_entry->lid == $item->lock_id) {
      list($status, $title) = $item->getPlugin('launcher')->formatRunning($item);
    }
    elseif ($item->log_entry->start_time && !$item->log_entry->end_time) {
      list($status, $title) = $item->getPlugin('launcher')->formatUnfinished($item);
    }
    else {
      list($status, $title) = $item->log_entry->formatSeverity();
      $title = $item->log_entry->message ? $item->log_entry->message : $title;
    }
    $this->rows[$name]['data'][] = array(
      'data' => $status,
      'class' => array('ctools-export-ui-status'),
      'title' => strip_tags($title),
    );

    // Storage.
    $this->rows[$name]['data'][] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    // Operations.
    $ops = theme(
      'links__ctools_dropbutton',
      array(
        'links' => $operations,
        'attributes' => array('class' => array('links', 'inline')),
      )
    );

    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = strip_tags($item->{$this->plugin['export']['admin_description']});
    }
  }

  /**
   * Submit the filter/sort form.
   *
   * This submit handler is actually responsible for building up all of the
   * rows that will later be rendered, since it is doing the filtering and
   * sorting.
   *
   * For the most part, you should not need to override this method, as the
   * fiddly bits call through to other functions.
   */
  public function list_form_submit(&$form, &$form_state) {
    // Filter and re-sort the pages.
    $plugin = $this->plugin;

    $prefix = ctools_export_ui_plugin_base_path($plugin);

    $this->jobs_behind = 0;
    foreach ($this->items as $name => $item) {
      // Call through to the filter and see if we're going to render this
      // row. If it returns TRUE, then this row is filtered out.
      if ($this->list_filter($form_state, $item)) {
        continue;
      }

      $operations = $this->build_operations($item);

      $this->list_build_row($item, $form_state, $operations);
    }
    if ($this->jobs_behind) {
      drupal_set_message(format_plural(
        $this->jobs_behind,
        '@count job is behind schedule.',
        '@count jobs are behind schedule.'
      ), 'warning');
    }

    // Now actually sort.
    uasort($this->rows, '_ultimate_cron_multi_column_sort');

    if ($form_state['values']['sort'] == 'desc') {
      $this->rows = array_reverse($this->rows);
    }
    foreach ($this->rows as &$row) {
      unset($row['sort']);
    }
  }
}
