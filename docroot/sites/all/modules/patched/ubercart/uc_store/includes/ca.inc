<?php

/**
 * @file
 * Helper functions for upgrade from Ubercart 2.x to Ubercart 3.x.
 */

/**
 * Base helper function to convert CA predicates to Rules configurations.
 */
function ca_convert_predicate($predicate) {
  // Convert event names to corresponding triggers.
  if ($predicate->ca_trigger == 'calculate_taxes') {
    return ca_extract_conditions($predicate, $predicate->pid);
  }
  elseif (strpos($predicate->ca_trigger, 'get_quote_from_') === 0) {
    return ca_extract_conditions($predicate, $predicate->ca_trigger);
  }

  $rule = rules_reaction_rule();
  if (is_numeric($predicate->pid)) {
    $rule->name = $predicate->ca_trigger . '_' . $predicate->pid;
  }
  else {
    $rule->name = $predicate->pid;
  }
  $rule->label = $predicate->title;
  $rule->active = (bool) $predicate->status;
  $rule->event($predicate->ca_trigger);

  ca_add_conditions($rule, $predicate->conditions['#conditions']);
  ca_add_actions($rule, $predicate->actions);

  $rule->save();
}

/**
 * Saves the conditions of the predicate as a separate component.
 *
 * @param $predicate
 *   An object row from {ca_predicates}.
 * @param $name
 *   The name to give the Rules component.
 */
function ca_extract_conditions($predicate, $name) {
  $component = rules_and(array('order' => array('uc_order', 'label' => t('Order'))));
  $component->name = $name;
  $component->label = t('@title conditions', array('@title' => $predicate->title));

  // CA predicates always have an AND at the root level.
  ca_add_conditions($component, $predicate->conditions['#conditions']);

  $component->save();
}

/**
 * Reads a predicate's conditions array and add them to a component.
 *
 * @param &$component
 *   A RulesConditionContainer or a Rule to receive conditions.
 * @param $conditions
 *   A predicate's array of conditions.
 */
function ca_add_conditions(&$component, $conditions) {
  foreach ($conditions as $condition) {
    // Handle condition containers.
    if (isset($condition['#conditions'])) {
      switch ($condition['#operator']) {
        case 'AND':
          $sub_tree = rules_and();
          break;
        case 'OR':
          $sub_tree = rules_or();
          break;
      }

      // Recurse.
      ca_add_conditions($sub_tree, $condition['#conditions']);

      if ($sub_tree->getIterator()->hasChildren()) {
        $component->condition($sub_tree);
      }
    }
    // Handle individual conditions.
    else {
      // Handle certain conditions as a generic "data_is" condition.
      $map = ca_data_map();
      if (isset($map[$condition['#name']])) {
        $name = 'data_is';
      }
      else {
        $name = $condition['#name'];
      }

      $settings = array();
      // The argument maps are like data selectors pointing to event variables.
      foreach ($condition['#argument_map'] as $key => $value) {
        if ($name == 'data_is') {
          $settings['data:select'] = $map[$condition['#name']]['data'];
        }
        // Special case: parameter changed, but doesn't use 'data_is'.
        elseif ($name == 'uc_attribute_ordered_product_option' && $key == 'order') {
          $settings['product:select'] = $value;
        }
        else {
          $key .= ':select';
          $settings[$key] = $value;
        }
      }

      $negate = FALSE;
      foreach ($condition['#settings'] as $key => $value) {
        // Save negation for later.
        if ($key == 'negate') {
          $negate = TRUE;
          continue;
        }

        if ($condition['#name'] == 'ca_condition_date' && $key == 'date') {
          $settings['date'] = $settings['date']['year'] . '/' . $settings['date']['month'] . '/' . $settings['date']['day'];
        }
        else {
          $settings[$key] = $value;
        }
      }

      if ($name == 'data_is') {
        $settings['#info'] = $map[$condition['#name']]['#info'];

        // data_is doesn't handle multiple values. Use a condition container.
        if (count($settings[$map[$condition['#name']]['value']]) > 1) {
          if (isset($map[$condition['#name']]['op']) && $settings[$map[$condition['#name']]['op']] == 'AND') {
            $rules_condition = rules_and();
          }
          else {
            $rules_condition = rules_or();
          }

          foreach ($settings[$map[$condition['#name']]['value']] as $value) {
            $rules_condition->condition('data_is', array('data:select' => $settings['data:select'], 'op' => 'contains', 'value' => $value, '#info' => $settings['#info']));
          }
        }
        elseif (is_array($settings[$map[$condition['#name']]['value']])) {
          $rules_condition = rules_condition('data_is', array('data:select' => $settings['data:select'], 'op' => 'contains', 'value' => $settings[$map[$condition['#name']]['value']], '#info' => $settings['#info']));
        }
        else {
          // Translate operator settings.
          if (isset($map[$condition['#name']]['op'])) {
            switch ($settings[$map[$condition['#name']]['op']]) {
              case 'before':
              case 'less':
                $ops = '<';
                break;
              case 'less_equal':
                $ops = array('<', '==');
                break;
              case 'only':
              case 'equal':
                $ops = '==';
                break;
              case 'not':
                $negate = !$negate;
                $settings['operator'] = '==';
                break;
              case 'after':
              case 'greater':
                $ops = '>';
                break;
              case 'greater_equal':
                $ops = array('>', '==');
                break;
              case 'begins':
              case 'contains':
              case 'ends':
                $settings['operator'] = 'contains';
                break;
              case 'yes':
                $settings['operator'] = '==';
                $settings['value'] = TRUE;
                break;
              case 'no':
                $settings['operator'] = '==';
                $settings['value'] = FALSE;
                break;
            }

            if ($condition['#name'] == 'node_field_comparison') {
              switch ($settings['field']) {
                case 'nid':
                case 'vid':
                case 'uid':
                  $type = 'integer';
                  break;
                case 'type':
                case 'title':
                  $type = 'text';
                  break;
                case 'status':
                case 'promote':
                case 'sticky':
                  $type = 'boolean';
                  break;
              }

              $settings = array(
                'data:select' => array_shift($settings) . ':' . $settings['field'],
                'op' => $settings['operator'],
                'value' => $settings['value'],
                '#info' => array(
                  'parameter' => array(
                    'value' => array(
                      'type' => $type,
                    ),
                  ),
                ),
              );
            }

            // data_is only provides <, =, and > for numeric data. Use two
            // separate conditions for <= and >= cases.
            if (is_array($ops)) {
              $data_condition = data_or();
              foreach ($ops as $op) {
                $data_condition->condition('data_is', array('data:select' => $settings['data:select'], 'op' => $op, 'value' => $settings[$map[$condition['#name']]['value']], '#info' => $settings['#info']));
              }

              $rules_condition = rules_condition($data_condition);
            }
            else {
              $rules_condition = rules_condition('data_is', array('data:select' => $settings['data:select'], 'op' => $ops, 'value' => $settings[$map[$condition['#name']]['value']], '#info' => $settings['#info']));
            }
          }
          // Standard data_is check that 'data' is equal to 'value'.
          else {
            $rules_condition = rules_condition('data_is', array('data:select' => $settings['data:select'], 'value' => $settings[$map[$condition['#name']]['value']], '#info' => $settings['#info']));
          }
        }
      }
      else {
        if ($name == 'ca_condition_custom_php') {
          $name = 'php_eval';
          $settings = array('code' => $settings['php']);
        }

        $rules_condition = rules_condition($name, $settings);
      }

      if ($negate) {
        $rules_condition->negate();
      }

      $component->condition($rules_condition);
    }
  }
}

/**
 * Reads a predicate's actions array and add them to a Rule.
 *
 * @param Rule &$rule
 *   The configuration to receive actions.
 * @param $actions
 *   The predicate's actions array.
 */
function ca_add_actions(&$rule, $actions) {
  foreach ($actions as $action) {
    $settings = array();
    // The argument maps are like data selectors pointing to event variables.
    foreach ($action['#argument_map'] as $key => $value) {
      $key .= ':select';
      $settings[$key] = $value;
    }

    foreach ($action['#settings'] as $key => $value) {
      $settings[$key] = $value;
    }

    switch ($action['#name']) {
      case 'ca_drupal_set_message':
        $name = 'drupal_message';
        $settings = array(
          'message' => $settings['message_text'],
          'error' => $settings['message_type'] == 'error',
        );
        break;
      case 'ca_action_custom_php':
        $name = 'php_eval';
        $settings = array('code' => $settings['php']);
        break;
      default:
        $name = $action['#name'];
        break;
    }
    $rule->action($name, $settings);
  }
}

/**
 * Maps obsolete conditions to correct settings for 'data_is'.
 */
function ca_data_map() {
  return array(
    'ca_condition_date' => array(
      'data' => 'system:date', // @todo: use the correct selector
      'op' => 'operator',
      'value' => 'date',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'date',
          ),
        ),
      ),
    ),
    'ca_condition_user_roles' => array(
      'data' => 'user:roles',
      'value' => 'roles',
      'op' => 'operator',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<integer>',
            'options list' => 'entity_metadata_user_roles',
          ),
        ),
      ),
    ),
    'node_field_comparison' => array(
      'data' => '*',
      'op' => 'operator',
      'value' => 'value',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => '*',
          ),
        ),
      ),
    ),
    'uc_cart_condition_product_class' => array(
      'data' => 'product:type',
      'value' => 'class',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<text>',
            'options list' => 'node_type_get_names',
          ),
        ),
      ),
    ),
    'uc_quote_condition_product_shipping_type' => array(
      'data' => 'product:shipping-type',
      'value' => 'type',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
            'options list' => 'uc_quote_shipping_type_options',
          ),
        ),
      ),
    ),
    'uc_order_condition_payment_method' => array(
      'data' => 'order:payment-method',
      'value' => 'payment_method',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
          ),
        ),
      ),
    ),
    'uc_order_status_condition' => array(
      'data' => 'order:order-status',
      'value' => 'order_status',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
            'options list' => 'uc_order_status_list',
          ),
        ),
      ),
    ),
    'uc_order_condition_total' => array(
      'data' => 'order:order-total',
      'value' => 'order_total_value',
      'op' => 'order_total_comparison',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'decimal',
          ),
        ),
      ),
    ),
    'uc_order_condition_delivery_postal_code' => array(
      'data' => 'order:delivery-address:postal-code',
      'value' => 'pattern',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
          ),
        ),
      ),
    ),
    'uc_order_condition_delivery_zone' => array(
      'data' => 'order:delivery-address:zone',
      'value' => 'zones',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<integer>',
            'options list' => 'uc_zone_option_list',
          ),
        ),
      ),
    ),
    'uc_order_condition_delivery_country' => array(
      'data' => 'order:delivery-address:country',
      'value' => 'countries',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<integer>',
            'options list' => 'uc_country_option_list',
          ),
        ),
      ),
    ),
    'uc_order_condition_billing_postal_code' => array(
      'data' => 'order:billing-address:postal-code',
      'value' => 'pattern',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
          ),
        ),
      ),
    ),
    'uc_order_condition_billing_zone' => array(
      'data' => 'order:billing-address:zone',
      'value' => 'zones',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<integer>',
            'options list' => 'uc_zone_option_list',
          ),
        ),
      ),
    ),
    'uc_order_condition_billing_country' => array(
      'data' => 'order:billing-address:country',
      'value' => 'countries',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<integer>',
            'options list' => 'uc_country_option_list',
          ),
        ),
      ),
    ),
    'uc_order_condition_user_name' => array(
      'data' => 'user:name',
      'value' => 'name',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
          ),
        ),
      ),
    ),
    'uc_order_condition_user_email' => array(
      'data' => 'user:mail',
      'value' => 'mail',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
          ),
        ),
      ),
    ),
    'uc_order_condition_user_created' => array(
      'data' => 'user:created',
      'value' => 'created',
      'op' => 'operator',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'date',
          ),
        ),
      ),
    ),
    'uc_order_condition_user_login' => array(
      'data' => 'user:login',
      'value' => 'login',
      'op' => 'operator',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'date',
          ),
        ),
      ),
    ),
    'uc_order_condition_user_language' => array(
      'data' => 'user:language',
      'value' => 'language',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'text',
          ),
        ),
      ),
    ),
    'uc_order_condition_user_roles' => array(
      'data' => 'user:roles',
      'value' => 'roles',
      'op' => 'operator',
      '#info' => array(
        'parameter' => array(
          'value' => array(
            'type' => 'list<integer>',
            'options list' => 'entity_metadata_user_roles',
          ),
        ),
      ),
    ),
  );
}
