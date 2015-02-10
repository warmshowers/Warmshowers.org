<?php
/**
 * @file
 * Contains the basic 'order' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to the order adminstration page.
 */
class uc_views_handler_field_order_id extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_order'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to order adminstration page
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_order'] = array(
      '#title' => t('Link this field to the order view.'),
      '#description' => t('The link will go to the admin view if the user has "view all orders" permission, or the user view if the order belongs to the current user and they have "view own orders" permission. This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_order']),
    );
  }

  function render($values) {
    global $user;

    $this->options['alter']['make_link'] = FALSE;

    if (!empty($this->options['link_to_order']) && $values->{$this->field_alias}) {
      if (user_access('view all orders')) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "admin/store/orders/" . $values->{$this->field_alias};
      }
      elseif ($values->{$this->aliases['uid']} == $user->uid && user_access('view own orders')) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = 'user/' . $user->uid . '/order/' . $values->{$this->field_alias};
      }
    }

    return parent::render($values);
  }
}
