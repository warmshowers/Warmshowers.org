<?php

/**
 * @file
 * Views field handler.
 */

/**
 * Field handler: allows linking to a user from a cart id.
 *
 * TODO: Should we subclass views_handler_field_user rather than
 * views_handler_field?
 */
class uc_cart_handler_field_cart_user extends views_handler_field {

  /**
   * Overrides init function to provide generic option to link to user.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    if (!empty($this->options['link_to_user'])) {
      $this->additional_fields['cart_id'] = 'cart_id';
    }
  }

  /**
   * Overrides views_handler::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_user'] = array('default' => TRUE);
    $options['overwrite_anonymous'] = array('default' => TRUE);
    $options['anonymous_text'] = array('default' => 'Anonymous', 'translatable' => TRUE);
    return $options;
  }

  /**
   * Overrides views_handler::options_form().
   *
   * Provides link to node option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_user'] = array(
      '#title' => t('Link this field to its user'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['link_to_user'],
    );
    $form['overwrite_anonymous'] = array(
      '#title' => t('Overwrite the value to display for anonymous users'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['overwrite_anonymous']),
      '#description' => t('If selected, you will see a field to enter the text to use for anonymous users.'),
    );
    $form['anonymous_text'] = array(
      '#title' => t('Text to display for anonymous users'),
      '#type' => 'textfield',
      '#default_value' => $this->options['anonymous_text'],
      '#dependency' => array(
        'edit-options-overwrite-anonymous' => array(1),
      ),
    );
  }

  /**
   *
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_user']) &&
        user_access('access user profiles') &&
        $values->{$this->aliases['cart_id']} &&
        $data !== NULL &&
        $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "user/" . $values->{$this->aliases['cart_id']};
    }
    return $data;
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $result = db_query('SELECT name FROM {users} u WHERE u.uid = :uid', array(':uid' => $values->{$this->field_alias}))->fetchField();
    if ($result) {
      return $this->render_link($result, $values);
    }
    else {
      // If the cart belongs to an unauthenticated user
      if (!empty($this->options['overwrite_anonymous'])) {
        return check_plain($this->options['anonymous_text']);
      }
      else {
        return $values->{$this->field_alias};
      }
    }
  }

}
