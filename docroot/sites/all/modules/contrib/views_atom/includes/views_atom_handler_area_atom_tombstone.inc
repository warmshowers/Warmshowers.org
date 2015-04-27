<?php

/**
* @file
* Views area text handler.
*/

/**
 * @ingroup views_area_handlers Views' area handlers
*/

class views_atom_handler_area_atom_tombstone extends views_handler_area_text {

  function option_definition() {
    $options = parent::option_definition();

    $options['feed_url'] = array('default' => NULL);
    $options['hub_url']  = array('default' => NULL);
    $options['item_url'] = array('default' => NULL);
    $options['use_push'] = array('default' => FALSE);

    // @todo, options could be added for updated, and email and name.
    return $options;
  }

  function options_form(&$form, &$form_state) {

    $form['use_push'] = array(
      '#type' => 'checkbox',
      // @todo better title?
      '#title' => t('Use Push Hub'),
      '#default_value' => $this->options['use_push'],
    );

    $form['hub_url'] = array(
      '#title' => 'Hub URL',
      '#type' => 'textfield',
      '#default_value' => $this->options['hub_url'],
      // @todo, better description.
      '#description' => t('Enter an absolute path of a pubsubhubbub URL. You can also enter a path relative to this site which will then be transformed into an absolute.'),
      '#dependency' => array(
        'edit-options-use-push' => array(1),
      ),
    );

    $form['feed_url'] = array(
      '#title' => 'Feed URL',
      '#type' => 'textfield',
      '#default_value' => $this->options['feed_url'],
      // @todo, better description.
      '#description' => t('Enter an absolute path of a feed URL. You can also enter a path relative to this site which will then be transformed into an absolute.'),
    );

    $form['item_url'] = array(
      '#title' => 'Item URL',
      '#type' => 'textfield',
      '#default_value' => $this->options['item_url'],
      // @todo, better description.
      '#description' => t('Enter an absolute path of the item to delete. You can also enter a path relative to this site which will then be transformed into an absolute.'),
    );

    parent::options_form($form, $form_state);
    unset($form['content']);
  }

  function options_submit(&$form, &$form_state) {
    // Redeclaring this blank method prevents the parent, views_handler_area_text,
    // from acting on the non-existent content variable.
  }

  // @todo, What is this parameter for?
  function render($empty = FALSE) {

    if (!$empty || !empty($this->options['empty'])) {

      $updated = date('c', REQUEST_TIME);
      // If we are using the REQUEST as the updated variable then we don't really
      // know when the entity was deleted and a human-readable comment will be set.
      $show_updated_comment = TRUE;

      $tombstone_array = array(
        // @todo, what should title be, a field?
        'title' => t('Atom Tombstone'),
        'use_push' => $this->options['use_push'],
        'hub_url' => $this->render_option_as_url('hub_url'),
        'feed_url' => $this->render_option_as_url('feed_url'),
        'updated' => $updated,
        'show_updated_comment' => $show_updated_comment,
        'items' => array(),
      );

      if (!empty($this->options['item_url'])) {
        // @todo, security on tokenize? What happens if something maliscious is used in "!1" ?
        $tombstone_array['items'][] = array(
          'guid' => $this->render_option_as_url('item_url'),
          'when' => $updated,
        );
      }

      return theme('views_atom_tombstone', $tombstone_array);
    }
    return '';
  }

  /**
   * Render a text area, using the proper format.
   */
  function render_option_as_url($option_name) {
    if (!empty($this->options[$option_name])) {
      if ($this->options['tokenize']) {
        $value = $this->view->style_plugin->tokenize_value($this->options[$option_name], 0);
      }
      else {
        $value = $this->options[$option_name];
      }
      return url($value, array('absolute' => TRUE));
    }
    else {
      return '';
    }
  }
}
