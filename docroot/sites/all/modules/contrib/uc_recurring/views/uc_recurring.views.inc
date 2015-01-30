<?php
// $Id

/**
 * @file
 * Views 2 hooks and callback registries.
 */

/**
 * Implementation of hook_views_data().
 */
function uc_recurring_views_data() {
  $data = array(
    // uc_recurring_users table
    'uc_recurring_users' => array(
      // Define the table information.
      'table' => array(
        // Put all of these in a group in the UI.
        'group' => 'UC Recurring Fees',
        // Declare this as a base table.
        'base' => array(
          // The primary key.
          'field' => 'rfid',
          // UI title to choose from.
          'title' => t('UC Recurring Fees'),
          'help' => t('Ubercart recurring fees and subscription information.'),
        ),
        // Define how this links up with other tables.
        'join' => array(
          // The core Users table is handy.
          'users' => array(
            'left_field' => 'uid',
            'field' => 'uid',
          ),
        ),
      ),
      ///// Define the fields. /////
      'uid' => array(
        // The title that appears in the UI.
        'title' => t('User ID'),
        // The help that appears in the UI.
        'help' => t('User ID for a user with a recurring fee subscription.'),
        // Information for displaying the field.
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'users',
          'base field' => 'uid',
        ),
      ),
      'fee_handler' => array(
        // The title that appears in the UI.
        'title' => t('Fee Handler'),
        // The help that appears in the UI.
        'help' => t('The payment processor for the recurring fee.'),
        // Information for displaying the field.
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
      ),
      'next_charge' => array(
        'title' => t('Next Charge Date'),
        'help' => t('The timestamp when the next charge should be performed.'),
        'field' => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'fee_amount' => array(
        'title' => t('Fee Amount'),
        'help' => t('The amount of the recurring fee.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'regular_interval' => array(
        'title' => t('Regular Interval'),
        'help' => t('The amount of time between charges.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'remaining_intervals' => array(
        'title' => t('Remaining Intervals'),
        'help' => t('The remaining number of times the fee should be charged.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'charged_intervals' => array(
        'title' => t('Charged Intervals'),
        'help' => t('Counter for how many times the fee has been charged.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'order_id' => array(
        'title' => t('Order ID'),
        'help' => t('ID for the initial recurring fee order.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'created' => array(
        'title' => t('Creation Date'),
        'help' => t('Timestamp for when the fee was first associated with the user.'),
        'field' => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'attempts' => array(
        'title' => t('Payment Attempts'),
        'help' => t('Number of times we have attempted to process this payment.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'pfid' => array(
        'title' => t('Product Fee ID'),
        'help' => t('The recurring fee product this fee was created from.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'status' => array(
        'title' => t('Status'),
        'help' => t('The current status for the recurring fee.'),
        'field' => array(
          // Custom handler to turn the number into a label.
          'handler' => 'uc_recurring_status_views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'uc_recurring_status_views_handler_filter',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
        ),
      ),
    ), // End of uc_recurring_users table.
  );

  return $data;
}

/**
 * Implement hook_views_handlers().
 */
function uc_recurring_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_recurring') . '/views',
    ),
    'handlers' => array(
      /**** Fields ****/
      'uc_recurring_status_views_handler_field' => array(
        'parent' => 'views_handler_field',
      ),
      /**** Filters ****/
      'uc_recurring_status_views_handler_filter' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
