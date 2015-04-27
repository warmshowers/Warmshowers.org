<?php

/**
 * @file
 * Contains the basic 'order' field handler.
 */

/**
 * Field handler: simple renderer that links to the order administration page.
 */
class uc_order_handler_field_order_id extends views_handler_field {

  /**
   * Override init function to provide generic option to link to user.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    if (!empty($this->options['link_to_order'])) {
      $this->additional_fields['order_id'] = array('table' => 'uc_orders', 'field' => 'order_id');
      $this->additional_fields['uid'] = array('table' => 'uc_orders', 'field' => 'uid');
    }
  }

  /**
   * Overrides views_handler::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_order'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Overrides views_handler::options_form().
   *
   * Provides link to order administration page.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_order'] = array(
      '#title' => t('Link this field to the order view page'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_order']),
    );
  }

  /**
   * Renders whatever the data is as a link to the order.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_order'])) {
      $this->options['alter']['make_link'] = FALSE;

      if (user_access('view all orders')) {
        $path = 'admin/store/orders/' . $this->get_value($values, 'order_id');
      }
      elseif (user_access('view own orders') && $this->get_value($values, 'uid') == $GLOBALS['user']->uid) {
        $path = 'user/' . $GLOBALS['user']->uid . '/orders/' . $this->get_value($values, 'order_id');
      }
      else {
        $path = FALSE;
      }

      if ($path && $data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $path;
      }
    }
    return $data;
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }

}
