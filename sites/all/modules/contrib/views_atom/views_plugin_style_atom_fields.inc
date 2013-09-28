<?php
// $Id: views_plugin_style_atom_fields.inc,v 1.1 2010/09/21 21:36:52 crell Exp $

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
    $image = theme('image', $this->feed_image);
    $this->view->feed_icon .= l($image, $path, $url_options);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['entry_type'] = array('default' => 'text');
    $options['entry_type_custom'] = array('default' => '');
    $options['description']['feed_description'] = array('default' => '');

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
      $field_names[$field] = array('' => '--');
      foreach ($handlers as $field => $handler) {
        if ($label = $handler->label()) {
          $field_names[$field] = $label;
        }
        else {
          $field_names[$field] = $handler->ui_name();
        }
      }

      $field_options = array(
        'atom_title' => t('Title'),
        'atom_summary' => t('Summary'),
        'atom_url' => t('URL'),
        'atom_updated' => t('Updated'),
        'atom_published' => t('Published'),
        'atom_author' => t('Author Name'),
        'atom_author_email' => t('Author Email'),
        'atom_author_url' => t('Author URL'),
      );

      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field usage'),
        '#description' => t('Atom requires certain elements have values. Specify which fields should be used to populate each required field. Note that all fields will still be available to the content of the record.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      foreach ($field_options as $k => $v) {
        $form['fields'][$k] = array(
          '#type' => 'select',
          '#title' => $v,
          '#options' => $field_names,
          '#default_value' => $this->options['fields'][$k],
        );
      }
    }
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
        if (!isset($this->view->field[$val])) continue;
        $item[$key] = htmlentities($row[$val], ENT_QUOTES, '', FALSE);
      }
      $items[] = $item;
    }
    return $items;
  }
}
