<?php

/**
 * @file
 * Style plugin for a standard Atom feed.
 */

class views_plugin_style_atom_fields extends views_plugin_style {

  /**
   * Initialize plugin.
   *
   * Set feed image for shared rendering later.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options = NULL);
    //$this->feed_image = drupal_get_path('module', 'views_kml') . '/images/kml.png';
  }

  /**
   * Attach this view to another display as a feed.
   *
   * Provide basic functionality for all export style views like attaching a
   * feed image link.
   */
  function attach_to($display_id, $path, $title) {
    $url_options = array('html' => true);
    $image = theme('image', array('path' => $this->feed_image));
    $this->view->feed_icon .= l($image, $path, $url_options);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['entry_type'] = array('default' => 'text');
    $options['entry_type_custom'] = array('default' => '');
    $options['description']['feed_description'] = array('default' => '');
    $options['fields'] = array('default' => array());
    $options['guid'] = array(
      // TODO determine what this variable is.
      'use_feed' => array('default' => array()),
    );

    return $options;
  }


  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = parent::option_definition();

    $handlers = $this->display->handler->get_handlers('field');

    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
    }
    else {

      $form['entry_type'] = array(
        '#type' => 'select',
        '#title' => t('Entry type'),
        '#options' => drupal_map_assoc(array('text', 'html', 'xhtml', 'xml')),
        '#default_value' => $this->options['entry_type'],
        '#description' => t('Select the media type of the content of each entry. If XML, enter the mimetype to use below.'),
      );

      $form['entry_type_custom'] = array(
        '#type' => 'textfield',
        '#title' => t('Custom entry type'),
        '#desctiption' => t('Enter the mimetype of the content of this feed.  It must be a valid XML mimetype.'),
        '#default_value' => $this->options['entry_type_custom'],
      );

      // Feed Description
      $form['description'] = array(
        '#type' => 'fieldset',
        '#title' => t('Feed Description'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['description']['feed_description'] = array(
        '#type' => 'textarea',
        '#default_value' => $this->options['description']['feed_description'],
        '#description' => t('Description for this feed.  If left blank, the default site mission will be used.'),
      );

      // Field Chooser
      $field_names = array('' => '--');
      foreach ($handlers as $field => $handler) {
        if ($label = $handler->label()) {
          $field_names[$field] = $label;
        }
        else {
          $field_names[$field] = $handler->ui_name();
        }
      }

      $field_options = $this->atom_mapping_fields();

      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field usage'),
        '#description' => t('Atom needs certain elements have values. Specify which fields should be used to populate each field. Note that all fields will still be available to the content of the record.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      foreach ($field_options as $k => $v) {
        $form['fields'][$k] = array(
          '#type' => 'select',
          '#title' => $v['label'],
          '#options' => $field_names,
          '#default_value' => $this->options['fields'][$k],
        );
      }
      // Feed Description
      $form['guid'] = array(
        '#type' => 'fieldset',
        '#title' => t('GUID settings'),
        '#description' => t('Controls how the GUID is created.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      if (module_exists('feeds')) {
        $form['guid']['use_existing_from_feed'] = array(
          '#title' => t('Use existing GUID when available'),
          '#type' => 'checkbox',
          '#default_value' => !empty($this->options['guid']['use_existing_from_feed']),
          '#description' => t('If the node was imported into the site from another feed, use the original GUID.'),
        );
      }
      $form['guid']['#access'] = count(element_children($form['guid'])) ? TRUE : FALSE;
    }
  }

  /**
   * Keep track of what fields are mapped for reuse.
   */
  function atom_mapping_fields() {
    return array(
      'atom_title' => array(
        'label' => t('Title'),
        'required' => TRUE,
      ),
      'atom_summary' => array(
        'label' => t('Summary'),
      ),
      'atom_url' => array(
        'label' => t('URL'),
        'required' => TRUE,
      ),
      'atom_updated' => array(
        'label' => t('Updated'),
        'required' => TRUE,
      ),
      'atom_published' => array(
        'label' => t('Published'),
      ),
      'atom_author' => array(
        'label' => t('Author Name'),
      ),
      'atom_author_email' => array(
        'label' => t('Author Email'),
      ),
      'atom_author_url' => array(
        'label' => t('Author URL'),
      ),
    );
  }

  /**
   * Validate that the feed has all needed fields for atom to function.
   */
  function validate() {
    $errors = array();
    $fields = $this->display->handler->get_handlers('field');
    $missing_fields = array();
    foreach ($this->atom_mapping_fields() as $field_key => $info) {
      if (!empty($info['required']) && (empty($this->options['fields'][$field_key]) || empty($fields[$this->options['fields'][$field_key]]))) {
        $missing_fields[] = $info['label'];
      }
    }
    if (!empty($missing_fields)) {
      $errors[] = t('Display "@display" is missing required atom fields: @fields.', array('@display' => $this->display->display_title, '@fields' => implode(', ', $missing_fields)));
    }
    return $errors;
  }

  function map_rows($rows) {
    // Fields must be rendered in order as of Views 2.3, so we will pre-render everything.
    $renders = array();
    $keys = array_keys($this->view->field);

    foreach ($rows as $count => $row) {
      foreach ($keys as $id) {
        $renders[$count][$id] = $this->view->field[$id]->theme($row);
      }
    }

    $items = array();
    foreach ($renders as $id => $row) {
      $item = array();
      foreach ($this->options['fields'] as $key => $val) {
        if (!isset($this->view->field[$val])) {
          continue;
        }
        $item[$key] = htmlentities($row[$val], ENT_QUOTES, '', FALSE);
      }
      $items[] = $item;
    }
    return $items;
  }

  /**
   * Override the parent render() method to check if this View is empty.
   *
   * @TODO, this override is useless without a new empty handler. The delete functionality
   * should be replicated there.
   */
  function render() {
   if (empty($this->view->result) && !$this->view->style_plugin->even_empty()) {
     return $this->view->display_handler->render_area('empty');
   } else {
     return parent::render();
   }
  }
}
