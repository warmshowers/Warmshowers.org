<?php

/**
 * @file
 * Hooks provided by the Store module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows modules to alter the TAPIr table after the rows are populated.
 *
 * The example below adds a value for the custom 'designer' column to the table
 * rows. Each table row has a numeric key in $table and these keys can be
 * accessed using element_children() from the Form API.
 *
 * @param $table
 *   Table declaration containing header and populated rows.
 * @param $table_id
 *   Table ID. Also the function called to build the table declaration.
 */
function hook_tapir_table_alter(&$table, $table_id) {
  if ($table_id == 'uc_product_table') {
    foreach (element_children($table) as $key) {
      $node = node_load($table['#parameters'][1][$key]);

      $table[$key]['designer'] = array(
        '#markup' => l($node->designer, 'collections/' . $node->designer_tid),
        '#cell_attributes' => array('class' => array('designer')),
      );
    }
  }
}

/**
 * Allows modules to modify forms before Drupal invokes hook_form_alter().
 *
 * This hook will normally be used by core modules so any form modifications
 * they make can be further modified by contrib modules using a normal
 * hook_form_alter(). At this point, drupal_prepare_form() has not been called,
 * so none of the automatic form data (e.g.: #parameters, #build_id, etc.) has
 * been added yet.
 *
 * @see hook_form_alter()
 */
function hook_uc_form_alter(&$form, &$form_state, $form_id) {
  // If the node has a product list, add attributes to them
  if (isset($form['products']) && count(element_children($form['products']))) {
    foreach (element_children($form['products']) as $key) {
      $form['products'][$key]['attributes'] = _uc_attribute_alter_form(node_load($key));
      if (is_array($form['products'][$key]['attributes'])) {
        $form['products'][$key]['attributes']['#tree'] = TRUE;
        $form['products'][$key]['#type'] = 'fieldset';
      }
    }
  }
  // If not, add attributes to the node.
  else {
    $form['attributes'] = _uc_attribute_alter_form($node);

    if (is_array($form['attributes'])) {
      $form['attributes']['#tree'] = TRUE;
      $form['attributes']['#weight'] = -1;
    }
  }
}

/**
 * Convenience function to display large blocks of text in several places.
 *
 * There are many instances where Ubercart modules have configurable blocks of
 * text. These usually come with default messages, like e-mail templates for new
 * orders. Because of the way default values are normally set, you're then stuck
 * having to copy and paste a large chunk of text in at least two different
 * places in the module (when you're wanting to use the variable or to display
 * the settings form with the default value). To cut down code clutter, this
 * hook was introduced. It lets you put your messages in one place and use the
 * function uc_get_message() to retrieve the default value at any time (and from
 * any module).
 *
 * The function is very simple, expecting no arguments and returning a basic
 * associative array with keys being message IDs and their values being the
 * default message. When you call uc_get_message(), use the message ID you set
 * here to refer to the message you want.
 *
 * Note: When using t(), you must not pass it a concatenated string! So our
 * example has no line breaks in the message even though it is much wider than
 * 80 characters. Using concatenation breaks translation.
 *
 * @return
 *   An array of messages.
 */
function hook_uc_message() {
  $messages['configurable_message_example'] = t('This block of text represents a configurable message such as a set of instructions or an e-mail template.  Using hook_uc_message to handle the default values for these is so easy even your grandma can do it!');

  return $messages;
}

/**
 * Adds status messages to the "Store administration" page.
 *
 * This hook is used to add items to the store status table on the main store
 * administration screen. Each item gets a row in the table that consists of a
 * status icon, title, and description. These items should be used to give
 * special instructions, notifications, or indicators for components of the cart
 * enabled by the modules. At a glance, a store owner should be able to look
 * here and see if a critical component of your module is not functioning
 * properly.
 *
 * For example, if the catalog module is installed and it cannot find the
 * catalog taxonomy vocabulary, it will show an error message here to alert the
 * store administrator.
 *
 * @return
 *   An array of store status items which are arrays with the following keys:
 *   - status: "ok", "warning", or "error" depending on the message.
 *   - title: The title of the status message or module that defines it.
 *   - desc: The description; can be any message, including links to pages and
 *     forms that deal with the issue being reported.
 */
function hook_uc_store_status() {
  if ($key = uc_credit_encryption_key()) {
    $statuses[] = array(
      'status' => 'ok',
      'title' => t('Credit card encryption'),
      'desc' => t('Credit card data in the database is currently being encrypted.'),
    );
  }
  return $statuses;
}

/**
 * @} End of "addtogroup hooks".
 */
