<?php
/**
 * @file
 * Contains logic to aid in attaching multiple ajax behaviors to form
 * elements on the checkout and order-edit forms.
 *
 * Both the checkout and the order edit forms are made up of multiple panes,
 * many supplied by contrib modules. Any pane may wish to update its own
 * display or that of another pane based on user input from input elements
 * anywhere on the form. The mechanism here described enables modules
 * to add ajax behaviors to the form in an orderly and efficient manner.
 *
 * Generally, an implementing pane should not add #ajax keys to existing form
 * elements directly. Rather, it should attach ajax behavior by adding
 * to the $form_state['uc_ajax'] array.
 *
 * $form_state['uc_ajax'] is an associative array keyed by the name of the
 * implementing module. Each implementing module should provide an array
 * of ajax callbacks, keyed by the name of the triggering element as it would
 * be specified when invoking form_set_error(). The entry for each element
 * may be either the name of a single ajax callback to be attached to that
 * element, or an array of ajax callbacks, optionally keyed by wrapper.
 * For example:
 *
 * @code
 *   $form_state['uc_ajax']['mymodule']['panes][quotes][quote_button'] = array(
 *      'quotes-pane' => 'uc_ajax_replace_checkout_pane',
 *   );
 * @endcode
 *
 * This will cause the contents of 'quotes-pane' to be replaced by the return
 * value of uc_ajax_replace_checkout_pane(). Note that if more than one module
 * assign a callback to the same wrapper key, the heavier module or pane will
 * take precedence.
 *
 * Implementors need not provide a wrapper key for each callback, in which case
 * the callback must return an array of ajax commands rather than a renderable
 * form element. For example:
 *
 * @code
 *   $form_state['uc_ajax']['mymodule']['panes][quotes][quote_button'] = array('my_ajax_callback');
 *   ...
 *   function my_ajax_callback($form, $form_state) {
 *     $commands[] = ajax_command_invoke('#my-input-element', 'val', 0);
 *     return array('#type' => 'ajax', '#commands' => $commands);
 *   }
 * @endcode
 *
 * However, using a wrapper key where appropriate will reduce redundant
 * replacements of the same element.
 *
 * NOTE: 'uc_ajax_replace_checkout_pane' is a convenience callback which will
 * replace the contents of an entire checkout pane. It is generally preferable
 * to use this when updating data on the checkout form, as this will
 * further reduce the likelihood of redundant replacements. You should use
 * your own callback only when behaviours other than replacement are
 * desired, or when replacing data that lie outside a checkout pane. Note
 * also that you may combine both formulations by mixing numeric and string keys.
 * For example:
 *
 * @code
 *   $form_state['uc_ajax']['mymodule']['panes][quotes][quote_button'] = array(
 *      'my_ajax_callback',
 *      'quotes-pane' => 'uc_ajax_replace_checkout_pane',
 *   );
 * @endcode
 */

/**
 * Form process callback to allow multiple Ajax callbacks on form elements.
 */
function uc_ajax_process_form($form, &$form_state) {
  // When processing the top level form, add any variable-defined pane wrappers.
  if (isset($form['#form_id'])) {
    switch ($form['#form_id']) {
      case 'uc_cart_checkout_form':
        $config = variable_get('uc_ajax_checkout', _uc_ajax_defaults('checkout'));
        foreach ($config as $key => $panes) {
          foreach (array_keys($panes) as $pane) {
            $config[$key][$pane] = 'uc_ajax_replace_checkout_pane';
          }
        }
        $form_state['uc_ajax']['uc_ajax'] = $config;
        break;
    }
  }

  if (!isset($form_state['uc_ajax'])) {
    return $form;
  }

  // We have to operate on the children rather than on the element itself, as
  // #process functions are called *after* form_handle_input_elements(),
  // which is where the triggering element is determined. If we haven't added
  // an '#ajax' key by that time, Drupal won't be able to determine which
  // callback to invoke.
  foreach (element_children($form) as $child) {
    $element =& $form[$child];

    // Add this process function recursively to the children.
    if (empty($element['#process']) && !empty($element['#type'])) {
      // We want to be sure the default process functions for the element type are called.
      $info = element_info($element['#type']);
      if (!empty($info['#process'])) {
        $element['#process'] = $info['#process'];
      }
    }
    $element['#process'][] = 'uc_ajax_process_form';

    // Multiplex any Ajax calls for this element.
    $parents = $form['#array_parents'];
    array_push($parents, $child);
    $key = implode('][', $parents);

    $callbacks = array();
    foreach ($form_state['uc_ajax'] as $module => $fields) {
      if (!empty($fields[$key])) {
        if (is_array($fields[$key])) {
          $callbacks = array_merge($callbacks, $fields[$key]);
        }
        else {
          $callbacks[] = $fields[$key];
        }
      }
    }

    if (!empty($callbacks)) {
      if (empty($element['#ajax'])) {
        $element['#ajax'] = array();
      }
      elseif (!empty($element['#ajax']['callback'])) {
        if (!empty($element['#ajax']['wrapper'])) {
          $callbacks[$element['#ajax']['wrapper']] = $element['#ajax']['callback'];
        }
        else {
          array_unshift($callbacks, $element['#ajax']['callback']);
        }
      }

      $element['#ajax'] = array_merge($element['#ajax'], array(
        'callback' => 'uc_ajax_multiplex',
        'list' => $callbacks,
      ));
    }
  }

  return $form;
}

/**
 * Ajax callback multiplexer.
 *
 * Processes a set of Ajax commands attached to the triggering element.
 */
function uc_ajax_multiplex($form, $form_state) {
  $element = $form_state['triggering_element'];
  if (!empty($element['#ajax']['list'])) {
    $commands = array();
    foreach ($element['#ajax']['list'] as $wrapper => $callback) {
      if (!empty($callback) && function_exists($callback) && $result = $callback($form, $form_state, $wrapper)) {
        if (is_array($result) && !empty($result['#type']) && $result['#type'] == 'ajax') {
          // If the callback returned an array of commands, simply add these to the list.
          $commands = array_merge($commands, $result['#commands']);
        }
        elseif (is_string($wrapper)) {
          // Otherwise, assume the callback returned a string or render-array, and insert it into the wrapper.
          $html = is_string($result) ? $result : drupal_render($result);
          $commands[] = ajax_command_replace('#' . $wrapper, trim($html));
          $commands[] = ajax_command_prepend('#' . $wrapper, theme('status_messages'));
        }
      }
    }
  }
  if (!empty($commands)) {
    return array('#type' => 'ajax', '#commands' => $commands);
  }
}

/**
 * Ajax callback to replace a whole checkout pane.
 *
 * @param $form
 *   The checkout form.
 * @param $form_state
 *   The current form state.
 * @param $wrapper
 *   Special third parameter passed for uc_ajax callbacks containing the ajax
 *   wrapper for this callback.  Here used to determine which pane to replace.
 *
 * @return
 *   The form element representing the pane, suitable for ajax rendering. If
 *   the pane does not exist, or if the wrapper does not refer to a checkout
 *   pane, returns nothing.
 */
function uc_ajax_replace_checkout_pane($form, $form_state, $wrapper = NULL) {
  if (empty($wrapper) && !empty($form_state['triggering_element']['#ajax']['wrapper'])) {
    // If $wrapper is absent, then we were not invoked by uc_ajax_multiplex,
    // so try to use the wrapper of the triggering element's #ajax array.
    $wrapper = $form_state['triggering_element']['#ajax']['wrapper'];
  }
  if (!empty($wrapper)) {
    list($pane, $verify) = explode('-', $wrapper);
    if ($verify === 'pane' && !empty($form['panes'][$pane])) {
      return $form['panes'][$pane];
    }
  }
}

/**
 * Retrieve the default ajax behaviors for a target form.
 *
 * @param $target_form
 *   The form whose default behaviors are to be retrieved.
 *
 * @return
 *   The array of default behaviors for the form.
 */
function _uc_ajax_defaults($target_form) {
  switch ($target_form) {
    case 'checkout':
      $quotes_defaults = drupal_map_assoc(array('payment-pane', 'quotes-pane'));
      return array(
        'panes][delivery][address][delivery_country' => $quotes_defaults,
        'panes][delivery][address][delivery_postal_code' => $quotes_defaults,
        'panes][delivery][select_address' => $quotes_defaults,
        'panes][billing][address][billing_country' => array('payment-pane' => 'payment-pane'),
      );
    default:
      return array();
  }
}
