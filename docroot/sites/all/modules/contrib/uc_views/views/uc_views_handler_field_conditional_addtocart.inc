<?php

/**
 * @file
 * Views handler: "Add to cart" form as a field if certain conditions are met.
 */

/**
 * Display the "Add to cart" form like the product page.
 */
class uc_views_handler_field_conditional_addtocart extends views_handler_field {
  /**
   * Provide conditions option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options;

    $form['price_conditions'] = array(
      '#title' => t('Price condition'),
      '#type' => 'radios',
      '#options' => array(
        'price_disabled' => t('Disabled'),
        'price_enabled' => t('Enabled'),
      ),
      '#default_value' => $options['price_conditions'] ? $options['price_conditions'] : 'price_disabled',
      '#weight' => 2,
    );
    $form['condition_price_operator'] = array(
      '#title' => t('Condition (show button if product sell price)'),
      '#type' => 'select',
      '#options' => array(
        '<' =>  t('Is less than'),
        '<=' =>  t('Is less than or equal to'),
        '==' => t('Is equal to'),
        '!=' => t('Is not equal to'),
        '>=' => t('Is greater than or equal to'),
        '>' => t('Is greater than'),
      ),
      '#default_value' => $options['condition_price_operator'] ? $options['condition_price_operator'] : '<',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[price_conditions]' => array('price_enabled')),
      '#weight' => 3,
     );
    $form['condition_price'] = array(
      '#title' => t('Price'),
      '#type' => 'textfield',
      '#default_value' => $options['condition_price'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[price_conditions]' => array('price_enabled')),
      '#weight' => 4,
     );
    $form['condition_price_default_text'] = array(
      '#title' => t('Default text'),
      '#description' => t('Text to show in stead of button if conditions are not met.'),
      '#type' => 'textfield',
      '#default_value' => $options['condition_price_default_text'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[price_conditions]' => array('price_enabled')),
      '#weight' => 5,
     );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (uc_product_is_product($values->{$this->aliases['type']})) {
      $type = node_get_types('type', $values->{$this->aliases['type']});
      $module = $type->module;
      $product = node_load($values->{$this->aliases['nid']});
      $result = '';
      if (function_exists('theme_'. $module .'_add_to_cart')) {
        $result = theme($module .'_add_to_cart', $product);
      }
      elseif (function_exists('theme_uc_product_add_to_cart')) {
        $result = theme('uc_product_add_to_cart', $product);
      }
      if($this->options['price_conditions'] == 'price_enabled') {
        if(eval('return ('.$product->sell_price.' '.$this->options['condition_price_operator'].' '.$this->options['condition_price'].');')) {
          return $result;
        } else {
          return $this->options['condition_price_default_text'];
        }
      } else {
        return $result;
      }
    }
  }
}
