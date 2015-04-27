<?php

/**
 * @file
 * Views row plugin for the views_watchdog module.
 */


/**
 * Formats watchdog events as RSS items.
 *
 * @ingroup views_row_plugins
 */
class views_plugin_row_watchdog_rss extends views_plugin_row {

  var $base_table = 'watchdog';
  var $base_field = 'wid';


  function option_definition() {

    $options = parent::option_definition();

    $options['watchdog_rss_type'] = array('default' => 'default');
    $options['watchdog_rss_title'] = array('default' => '[severity] - [type]');

    return $options;
  }


  function options_form(&$form, &$form_state) {

    $form['watchdog_rss_type'] = array(
      '#type' => 'select',
      '#title' => t('Display type'),
      '#description' => t('The title includes the severity level and system event type.'),
      '#default_value' => !empty($this->options['watchdog_rss_type']) ? $this->options['watchdog_rss_type'] : 'default',
      '#options' => array(
        'fulltext' => t('Title plus message'),
        'title' => t('Title only'),
        'default' => t('Use default RSS settings'),
      ),
    );
    $form['watchdog_rss_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Alter the output of this field by specifying a string of text with replacement tokens. Replacement patterns: [type], [severity], [hostname]'),
      '#default_value' => !empty($this->options['watchdog_rss_title']) ? $this->options['watchdog_rss_title'] : '[severity] - [type]',
    );
  }


  function render($row) {

    global $base_url;

    $wid = $row->{$this->field_alias};
    if (!is_numeric($wid)) {
      return;
    }

    $rss_type = $this->options['watchdog_rss_type'];
    if ($rss_type == 'default') {
      $rss_type = variable_get('feed_item_length', 'teaser');
    }

    $watchdog = db_query('SELECT w.wid, w.type, w.message, w.variables, w.severity, w.hostname, w.timestamp, u.name FROM {watchdog} w LEFT JOIN {users} u ON w.uid = u.uid WHERE w.wid = :wid', array(':wid' => $wid))
      ->fetchObject();

    $title_tokens = array('[type]', '[severity]', '[hostname]');
    $title_values = array($watchdog->type, _views_watchdog_get_severity($watchdog->severity, 'title'), $watchdog->hostname);

    $watchdog->rss_elements = array(
      array('key' => 'pubDate', 'value' => gmdate('r', $watchdog->timestamp)),
      array('key' => 'dc:creator', 'value' => $watchdog->name),
      array('key' => 'guid', 'value' => $watchdog->wid . ' at ' . $base_url, 'attributes' => array('isPermaLink' => 'false'))
    );

    $watchdog->rss_namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');
    $this->view->style_plugin->namespaces = array_merge($this->view->style_plugin->namespaces, $watchdog->rss_namespaces);

    $item = new stdClass();
    $item->title = check_plain(str_replace($title_tokens, $title_values, $this->options['watchdog_rss_title']));
    $item->description = ($rss_type == 'fulltext' || $rss_type == 'teaser') ? _views_watchdog_format_message($watchdog->message, $watchdog->variables) : '';
    $item->link = url('admin/reports/event/'. $watchdog->wid, array('absolute' => TRUE));
    $item->wid = $watchdog->wid;
    $item->elements = $watchdog->rss_elements;

    return theme($this->theme_functions(), array('view' => $this->view, 'options' => $this->options, 'row' => $item));
  }
}
