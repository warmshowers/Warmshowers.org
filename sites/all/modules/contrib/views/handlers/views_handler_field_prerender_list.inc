<?php
// $Id: views_handler_field_prerender_list.inc,v 1.1 2008/09/03 19:21:28 merlinofchaos Exp $

/**
 * Field handler to provide a list of items.
 *
 * The items are expected to be loaded by a child object during pre_render,
 * and 'my field' is expected to be the pointer to the items in the list.
 *
 * Items to render should be in a list in $this->items
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_prerender_list extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'separator');
    $options['separator'] = array('default' => ', ');
    $options['empty'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#options' => array(
        'ul' => t('Unordered list'),
        'ol' => t('Ordered list'),
        'separator' => t('Simple separator'),
      ),
      '#default_value' => $this->options['type'],
    );

    $form['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $this->options['separator'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[type]' => array('separator')),
    );

    $form['empty'] = array(
      '#type' => 'textfield',
      '#title' => t('Empty list text'),
      '#default_value' => $this->options['empty'],
      '#description' => t('If the list is empty, you may enter text here that will be displayed.'),
    );
  }

  function render($values) {
    $field = $values->{$this->field_alias};
    if (!empty($this->items[$field])) {
      if ($this->options['type'] == 'separator') {
        return implode(check_plain($this->options['separator']), $this->items[$field]);
      }
      else {
        return theme('item_list', $this->items[$field], NULL, $this->options['type']);
      }
    }
    else if (!empty($this->options['empty'])) {
      return $this->options['empty'];
    }
  }
}
