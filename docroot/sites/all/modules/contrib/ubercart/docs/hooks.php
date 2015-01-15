<?php

/**
 * @file
 * These are the hooks that are invoked by the Ubercart core.
 *
 * Core hooks are typically called in all modules at once using
 * module_invoke_all().
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Performs extra processing when an item is added to the shopping cart.
 *
 * Some modules need to be able to hook into the process of adding items to a
 * cart. For example, an inventory system may need to check stock levels and
 * prevent an out of stock item from being added to a customer's cart. This hook
 * lets developers squeeze right in at the end of the process after the product
 * information is all loaded and the product is about to be added to the cart.
 * In the event that a product should not be added to the cart, you simply have
 * to return a failure message described below. This hook may also be used
 * simply to perform some routine action when products are added to the cart.
 *
 * @param $nid
 *   The node ID of the product.
 * @param $qty
 *   The quantity being added.
 * @param $data
 *   The data array, including attributes and model number adjustments.
 *
 * @return
 *   The function can use this data to whatever purpose to see if the item can
 *   be added to the cart or not. The function should return an array containing
 *   the result array. (This is due to the nature of Drupal's
 *   module_invoke_all() function. You must return an array within an array or
 *   other module data will end up getting ignored.) At this moment, there are
 *   only three keys:
 *   - success: TRUE or FALSE for whether the specified quantity of the item
 *     may be added to the cart or not; defaults to TRUE.
 *   - message: The fail message to display in the event of a failure; if
 *     omitted, Ubercart will display a default fail message.
 *   - silent: Return TRUE to suppress the display of any messages; useful when
 *     a module simply needs to do some other processing during an add to cart
 *     or fail silently.
 */
function hook_add_to_cart($nid, $qty, $data) {
  if ($qty > 1) {
    $result[] = array(
      'success' => FALSE,
      'message' => t('Sorry, you can only add one of those at a time.'),
    );
  }
  return $result;
}

/**
 * Adds extra information to a cart item's "data" array.
 *
 * This is effectively the submit handler of any alterations to the Add-to-cart
 * form. It provides a standard way to store the extra information so that it
 * can be used by hook_add_to_cart().
 *
 * @param $form_values
 *   The values submitted to the Add-to-cart form.
 *
 * @return
 *   An array of data to be merged into the item added to the cart.
 */
function hook_add_to_cart_data($form_values) {
  $node = node_load($form_values['nid']);
  return array('module' => 'uc_product', 'shippable' => $node->shippable);
}

/**
 * Calculates tax line items for an order.
 *
 * @param $order
 *   An order object or an order id.
 *
 * @return
 *   An array of tax line item objects keyed by a module-specific id.
 */
function hook_calculate_tax($order) {
  global $user;
  if (is_numeric($order)) {
    $order = uc_order_load($order);
    $account = user_load(array('uid' => $order->uid));
  }
  elseif ((int)$order->uid) {
    $account = user_load(array('uid' => intval($order->uid)));
  }
  else {
    $account = $user;
  }
  if (!is_object($order)) {
    return array();
  }
  if (empty($order->delivery_postal_code)) {
    $order->delivery_postal_code = $order->billing_postal_code;
  }
  if (empty($order->delivery_zone)) {
    $order->delivery_zone = $order->billing_zone;
  }
  if (empty($order->delivery_country)) {
    $order->delivery_country = $order->billing_country;
  }

  $order->taxes = array();

  if (isset($order->order_status)) {
    $state = uc_order_status_data($order->order_status, 'state');
    $use_same_rates = in_array($state, array('payment_received', 'completed'));
  }
  else {
    $use_same_rates = FALSE;
  }

  $arguments = array(
    'order' => array(
      '#entity' => 'uc_order',
      '#title' => t('Order'),
      '#data' => $order,
    ),
    'tax' => array(
      '#entity' => 'tax',
      '#title' => t('Tax rule'),
      // #data => each $tax in the following foreach() loop;
    ),
    'account' => array(
      '#entity' => 'user',
      '#title' => t('User'),
      '#data' => $account,
    ),
  );

  $predicates = ca_load_trigger_predicates('calculate_taxes');
  foreach (uc_taxes_rate_load() as $tax) {
    if ($use_same_rates) {
      foreach ((array)$order->line_items as $old_line) {
        if ($old_line['type'] == 'tax' && $old_line['data']['tax_id'] == $tax->id) {
          $tax->rate = $old_line['data']['tax_rate'];
          break;
        }
      }
    }

    $arguments['tax']['#data'] = $tax;
    if (ca_evaluate_conditions($predicates['uc_taxes_'. $tax->id], $arguments)) {
      $line_item = uc_taxes_action_apply_tax($order, $tax);
      if ($line_item) {
        $order->taxes[$line_item->id] = $line_item;
      }
    }
  }

  return $order->taxes;
}

/**
 * Controls the display of an item in the cart.
 *
 * Product type modules allow the creation of nodes that can be added to the
 * cart. The cart determines how they are displayed through this hook. This is
 * especially important for product kits, because it may be displayed as a
 * single unit in the cart even though it is represented as several items.
 *
 * This hook is only called for the module that owns the cart item in
 * question, as set in $item->module.
 *
 * @param $item
 *   The item in the cart to display.
 *
 * @return
 *   A form array containing the following elements:
 *   - "nid"
 *     - #type: value
 *     - #value: The node id of the $item.
 *   - "module"
 *     - #type: value
 *     - #value: The module implementing this hook and the node represented by
 *       $item.
 *   - "remove"
 *     - #type: submit
 *     - #value: t('Remove'); when clicked, will remove $item from the cart.
 *   - "description"
 *     - #type: markup
 *     - #value: Themed markup (usually an unordered list) displaying extra
 *       information.
 *   - "title"
 *     - #type: markup
 *     - #value: The displayed title of the $item.
 *   - "#total"
 *     - "type": float
 *     - "value": Numeric price of $item. Notice the '#' signifying that this is
 *       not a form element but just a value stored in the form array.
 *   - "data"
 *     - #type: hidden
 *     - #value: The serialized $item->data.
 *   - "qty"
 *     - #type: textfield
 *     - #value: The quantity of $item in the cart. When "Update cart" is
 *       clicked, the customer's input is saved to the cart.
 */
function hook_cart_display($item) {
  $node = node_load($item->nid);
  $element = array();
  $element['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $element['module'] = array('#type' => 'value', '#value' => 'uc_product');
  $element['remove'] = array('#type' => 'checkbox');

  $element['title'] = array(
    '#value' => node_access('view', $node) ? l($item->title, 'node/'. $node->nid) : check_plain($item->title),
  );

  $context = array(
    'revision' => 'altered',
    'type' => 'cart_item',
    'subject' => array(
      'cart_item' => $item,
      'node' => $node,
    ),
  );
  $price_info = array(
    'price' => $item->price,
    'qty' => $item->qty,
  );

  $element['#total'] = uc_price($price_info, $context);
  $element['data'] = array('#type' => 'hidden', '#value' => serialize($item->data));
  $element['qty'] = array(
    '#type' => 'textfield',
    '#default_value' => $item->qty,
    '#size' => 5,
    '#maxlength' => 6
  );

  if ($description = uc_product_get_description($item)) {
    $element['description'] = array('#value' => $description);
  }

  return $element;
}

/**
 * Adds extra data about an item in the cart.
 *
 * Products that are added to a customer's cart are referred as items until the
 * sale is completed. Just think of a grocery store having a bunch of products
 * on the shelves but putting a sign over the express lane saying "15 Items or
 * Less." hook_cart_item() is in charge of acting on items at various times like
 * when they are being added to a cart, saved, loaded, and checked out.
 *
 * Here's the rationale for this hook: Products may change on a live site during
 * a price increase or change to attribute adjustments. If a user has previously
 * added an item to their cart, when they go to checkout or view their cart
 * screen we want the latest pricing and model numbers to show. So, the
 * essential product information is stored in the cart, but when the items in a
 * cart are loaded, modules are given a chance to adjust the data against the
 * latest settings.
 *
 * @param $op
 *   The action that is occurring. Possible values:
 *   - load: Passed for each item when a cart is being loaded in the function
 *     uc_cart_get_contents(). This gives modules the chance to tweak
 *     information for items when the cart is being loaded prior to being view
 *     or added to an order. No return value is expected.
 *   - can_ship: Passed when a cart is being scanned for items that are not
 *     shippable items. Ubercart will bypass cart and checkout operations
 *     specifically related to tangible products if nothing in the cart is
 *     shippable. hook_cart_item() functions that check for this op are expected
 *     to return TRUE or FALSE based on whether a product is shippable or not.
 *   - remove: Passed when an item is removed from the cart.
 *   - checkout: Passed for each item when the cart is being emptied for
 *     checkout.
 *
 * @return
 *   No return value for load. TRUE or FALSE for can_ship.
 */
function hook_cart_item($op, &$item) {
  switch ($op) {
    case 'load':
      $term = array_shift(taxonomy_node_get_terms_by_vocabulary($item->nid, variable_get('uc_manufacturer_vid', 0)));
      $arg1->manufacturer = $term->name;
      break;
  }
}

/**
 * Registers callbacks for a cart pane.
 *
 * The default cart view page displays a table of the cart contents and a few
 * simple form features to manage the cart contents. For a module to add
 * information to this page, it must use hook_cart_pane() to define extra panes
 * that may be ordered to appear above or below the default information.
 *
 * @param $items
 *   The current contents of the shopping cart.
 *
 * @return
 *   The function is expected to return an array of pane arrays with the
 *   following keys:
 *   - "id"
 *     - type: string
 *     - value: The internal ID of the pane, using a-z, 0-9, and - or _.
 *   - "title"
 *     - type: string
 *     - value: The name of the cart pane displayed to the user.  Use t().
 *   - "enabled"
 *     - type: boolean
 *     - value: Whether the pane is enabled by default or not. (Defaults to TRUE.)
 *   - "weight"
 *     - type: integer
 *     - value: The weight of the pane to determine its display order. (Defaults
 *         to 0.)
 *   - "body"
 *     - type: string
 *     - value: The body of the pane when rendered on the cart view screen.
 *
 * The body gets printed to the screen if it is on the cart view page.  For the
 * settings page, the body field is ignored.  You may want your function to
 * check for a NULL argument before processing any queries or foreach() loops.
 */
function hook_cart_pane($items) {
  $panes[] = array(
    'id' => 'cart_form',
    'title' => t('Default cart form'),
    'enabled' => TRUE,
    'weight' => 0,
    'body' => !is_null($items) ? drupal_get_form('uc_cart_view_form', $items) : '',
  );

  return $panes;
}

/**
 * Alters cart pane definitions.
 *
 * @param $panes
 *   The array of pane information in the format defined in hook_cart_pane(),
 *   passed by reference.
 *
 * @param $items
 *   The array of item information.
 */
function hook_cart_pane_alter(&$panes, $items) {
  foreach ($panes as &$pane) {
    if ($pane['id'] == 'cart') {
      $pane['body'] = drupal_get_form('my_custom_pane_form_builder', $items);
    }
  }
}

/**
 * Registers callbacks for a checkout pane.
 *
 * The checkout screen for Ubercart is a compilation of enabled checkout panes.
 * A checkout pane can be used to display order information, collect data from
 * the customer, or interact with other panes. Panes are defined in enabled
 * modules with hook_checkout_pane() and displayed and processed through
 * specified callback functions. Some of the settings for each pane are
 * configurable from the checkout settings page with defaults being specified
 * in the hooks.
 *
 * The default panes are defined in uc_cart.module in the function
 * uc_cart_checkout_pane(). These include panes to display the contents of the
 * shopping cart and to collect essential site user information, a shipping
 * address, a payment address, and order comments. Other included modules offer
 * panes for shipping and payment purposes as well.
 *
 * @return
 *   An array of checkout pane arrays using the following keys:
 *   - "id"
 *     - type: string
 *     - value: The internal ID of the checkout pane, using a-z, 0-9, and - or _.
 *   - "title"
 *     - type: string
 *     - value: The name of the pane as it appears on the checkout form.
 *   - "desc"
 *     - type: string
 *     - value: A short description of the pane for the admin pages.
 *   - "callback"
 *     - type: string
 *     - value: The name of the callback function for this pane.  View
 *       @link http://www.ubercart.org/docs/developer/245/checkout this page @endlink
 *       for more documentation and examples of checkout pane callbacks.
 *   - "weight"
 *     - type: integer
 *     - value: Default weight of the pane, defining its order on the checkout form.
 *   - "enabled"
 *     - type: boolean
 *     - value: Optional. Whether or not the pane is enabled by default. Defaults
 *       to TRUE.
 *   - "process"
 *     - type: boolean
 *     - value: Optional. Whether or not this pane needs to be processed when
 *       the checkout form is submitted. Defaults to TRUE.
 *   - "collapsible"
 *     - type: boolean
 *     - value: Optional. Whether or not this pane is displayed as a collapsible
 *       fieldset. Defaults to TRUE.
 */
function hook_checkout_pane() {
  $panes[] = array(
    'id' => 'cart',
    'callback' => 'uc_checkout_pane_cart',
    'title' => t('Cart contents'),
    'desc' => t("Display the contents of a customer's shopping cart."),
    'weight' => 1,
    'process' => FALSE,
    'collapsible' => FALSE,
  );
  return $panes;
}

/**
 * Alters checkout pane definitions.
 *
 * @param $panes
 *   Array with the panes information as defined in hook_checkout_pane(), passed
 *   by reference.
 */
function hook_checkout_pane_alter(&$panes) {
  foreach ($panes as &$pane) {
    if ($pane['id'] == 'cart') {
      $pane['callback'] = 'my_custom_module_callback';
    }
  }
}

/**
 * Gives clearance to a user to download a file.
 *
 * By default the uc_file module can implement 3 restrictions on downloads: by
 * number of IP addresses downloaded from, by number of downloads, and by a set
 * expiration date. Developers wishing to add further restrictions can do so by
 * implementing this hook. After the 3 aforementioned restrictions are checked,
 * the uc_file module will check for implementations of this hook.
 *
 * @param $user
 *   The drupal user object that has requested the download.
 * @param $file_download
 *   The file download object as defined as a row from the uc_file_users table
 *   that grants the user the download.
 *
 * @return
 *   TRUE or FALSE depending on whether the user is to be permitted download of
 *   the requested files. When a implementation returns FALSE it should set an
 *   error message in Drupal using drupal_set_message() to inform customers of
 *   what is going on.
 */
function hook_download_authorize($user, $file_download) {
  if (!$user->status) {
    drupal_set_message(t("This account has been banned and can't download files anymore. "), 'error');
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Performs actions on file products.
 *
 * The uc_file module comes with a file manager (found at Administer » Store
 * administration » Products » View file downloads) that provides some basic
 * functionality: deletion of multiple files and directories, and upload of
 * single files (those looking to upload multiple files should just directly
 * upload them to their file download directory then visit the file manager
 * which automatically updates new files found in its directory). Developers
 * that need to create more advanced actions with this file manager can do so
 * by using this hook.
 *
 * @param $op
 *   The operation being taken by the hook, possible ops defined below.
 *   - info: Called before the uc_file module builds its list of possible file
 *     actions. This op is used to define new actions that will be placed in
 *     the file action select box.
 *   - insert: Called after uc_file discovers a new file in the file download
 *     directory.
 *   - form: When any defined file action is selected and submitted to the form
 *     this function is called to render the next form. Because this is called
 *     whenever a module-defined file action is selected, the variable
 *     $args['action'] can be used to define a new form or append to an existing
 *     form.
 *   - upload: After a file has been uploaded, via the file manager's built in
 *     file upload function, and moved to the file download directory this op
 *     can perform any remaining operations it needs to perform on the file
 *     before its placed into the uc_files table.
 *   - upload_validate: This op is called to validate the uploaded file that
 *     was uploaded via the file manager's built in file upload function. At
 *     this point, the file has been uploaded to PHP's temporary directory.
 *     Files passing this upload validate function will be moved into the file
 *     downloads directory.
 *   - validate: This op is called to validate the file action form.
 *   - submit: This op is called to submit the file action form.
 * @param $args
 *   A keyed array of values that varies depending on the op being performed,
 *   possible values defined below.
 *   - info: None
 *   - insert:
 *     - file_object: The file object of the newly discovered file.
 *   - form:
 *     - action: The file action being performed as defined by the key in the
 *       array sent by hook_file_action($op = 'info').
 *     - file_ids: The file ids (as defined in the uc_files table) of the
 *       selected files to perform the action on.
 *   - upload:
 *     - file_object: The file object of the file moved into file downloads
 *       directory.
 *     - form_id: The form_id variable of the form_submit function.
 *     - form_values: The form_values variable of the form_submit function.
 *   - upload_validate:
 *     - file_object: The file object of the file that has been uploaded into
 *       PHP's temporary upload directory.
 *     - form_id: The form_id variable of the form_validate function.
 *     - form_values: The form_values variable of the form_validate function.
 *   - validate:
 *     - form_id: The form_id variable of the form_validate function.
 *     - form_values: The form_values variable of the form_validate function.
 *   - submit:
 *     - form_id: The form_id variable of the form_submit function.
 *     - form_values: The form_values variable of the form_submit function.
 *
 * @return
 *   The return value of hook depends on the op being performed, possible return
 *   values defined below.
 *   - info: The associative array of possible actions to perform. The keys are
 *     unique strings that defines the actions to perform. The values are the
 *     text to be displayed in the file action select box.
 *   - insert: None
 *   - form: This op should return an array of drupal form elements as defined
 *     by the drupal form API.
 *   - upload: None
 *   - upload_validate: None
 *   - validate: None
 *   - submit: None
 */
function hook_file_action($op, $args) {
  switch ($op) {
    case 'info':
      return array('uc_image_watermark_add_mark' => 'Add Watermark');
    case 'insert':
      // Automatically adds watermarks to any new files that are uploaded to
      // the file download directory
      _add_watermark($args['file_object']->filepath);
    break;
    case 'form':
      if ($args['action'] == 'uc_image_watermark_add_mark') {
        $form['watermark_text'] = array(
          '#type' => 'textfield',
          '#title' => t('Watermark text'),
        );
        $form['submit_watermark'] = array(
          '#type' => 'submit',
          '#value' => t('Add watermark'),
        );
      }
    return $form;
    case 'upload':
      _add_watermark($args['file_object']->filepath);
      break;
    case 'upload_validate':
      // Given a file path, function checks if file is valid JPEG
      if (!_check_image($args['file_object']->filepath)) {
        form_set_error('upload', t('Uploaded file is not a valid JPEG'));
      }
    break;
    case 'validate':
      if ($args['form_values']['action'] == 'uc_image_watermark_add_mark') {
        if (empty($args['form_values']['watermark_text'])) {
          form_set_error('watermar_text', t('Must fill in text'));
        }
      }
    break;
    case 'submit':
      if ($args['form_values']['action'] == 'uc_image_watermark_add_mark') {
        foreach ($args['form_values']['file_ids'] as $file_id) {
          $filename = db_result(db_query("SELECT filename FROM {uc_files} WHERE fid = %d", $file_id));
          // Function adds watermark to image
          _add_watermark($filename);
        }
      }
    break;
  }
}

/**
 * Makes changes to a file before it is downloaded by the customer.
 *
 * Stores, either for customization, copy protection or other reasons, might
 * want to send customized downloads to customers. This hook will allow this to
 * happen. Before a file is opened to be transferred to a customer, this hook
 * will be called to make any alterations to the file that will be used to
 * transfer the download to the customer. This, in effect, will allow a
 * developer to create a new, personalized, file that will get transferred to a
 * customer.
 *
 * @param $file_user
 *   The file_user object (i.e. an object containing a row from the
 *   uc_file_users table) that corresponds with the user download being
 *   accessed.
 * @param $ip
 *   The IP address from which the customer is downloading the file.
 * @param $fid
 *   The file id of the file being transferred.
 * @param $file
 *   The file path of the file to be transferred.
 *
 * @return
 *   The path of the new file to transfer to customer.
 */
function hook_file_transfer_alter($file_user, $ip, $fid, $file) {
  $file_data = file_get_contents($file) ." [insert personalized data]"; // For large files this might be too memory intensive
  $new_file = tempnam(file_directory_temp(), 'tmp');
  file_put_contents($new_file, $file_data);
  return $new_file;
}

/**
 * Defines line items that are attached to orders.
 *
 * A line item is a representation of charges, fees, and totals for an order.
 * Default line items include the subtotal and total line items, the tax line
 * item, and the shipping line item. There is also a generic line item that
 * store admins can use to add extra fees and discounts to manually created
 * orders.
 *
 * Module developers will use this hook to define new types of line items for
 * their stores. An example use would be for a module that allows customers to
 * use coupons and wants to represent an entered coupon as a line item.
 *
 * Once a line item has been defined in hook_line_item(), Ubercart will begin
 * interacting with it in various parts of the code. One of the primary ways
 * this is done is through the callback function you specify for the line item.
 *
 * @return
 *   Your hook should return an array of associative arrays. Each item in the
 *   array represents a single line item and should use the following keys:
 *   - "id"
 *     - type: string
 *     - value: The internal ID of the line item.
 *   - "title"
 *     - type: string
 *     - value: The title of the line item shown to the user in various
 *       interfaces. Use t().
 *   - "callback"
 *     - type: string
 *     - value: Name of the line item's callback function, called for various
 *       operations.
 *   - "weight"
 *     - type: integer
 *     - value: Display order of the line item in lists; "lighter" items are
 *       displayed first.
 *   - "stored"
 *     - type: boolean
 *     - value: Whether or not the line item will be stored in the database.
 *       Should be TRUE for any line item that is modifiable from the order
 *       edit screen.
 *   - "add_list"
 *     - type: boolean
 *     - value: Whether or not a line item should be included in the "Add a Line
 *       Item" select box on the order edit screen.
 *   - "calculated"
 *     - type: boolean
 *     - value: Whether or not the value of this line item should be added to
 *       the order total. (Ex: would be TRUE for a shipping charge line item but
 *       FALSE for the subtotal line item since the product prices are already
 *       taken into account.)
 *   - "display_only"
 *     - type: boolean
 *     - value: Whether or not this line item is simply a display of information
 *       but not calculated anywhere. (Ex: the total line item uses display to
 *       simply show the total of the order at the bottom of the list of line
 *       items.)
 */
function hook_line_item() {
  $items[] = array(
    'id' => 'generic',
    'title' => t('Empty line'),
    'weight' => 2,
    'default' => FALSE,
    'stored' => TRUE,
    'add_list' => TRUE,
    'calculated' => TRUE,
    'callback' => 'uc_line_item_generic',
  );

  return $items;
}

/**
 * Alters a line item on an order when the order is loaded.
 *
 * @param &$item
 *   The line item array.
 * @param $order
 *   The order object containing the line item.
 */
function hook_line_item_alter(&$item, $order) {
  $account = user_load($order->uid);
  ca_pull_trigger('calculate_line_item_discounts', $item, $account);
}

/**
 * Alters the line item definitions declared in hook_line_item().
 *
 * @param &$items
 *   The combined return value of hook_line_item().
 */
function hook_line_item_data_alter(&$items) {
  foreach ($items as &$item) {
    // Tax amounts are added in to other line items, so the actual tax line
    // items should not be added to the order total.
    if ($item['id'] == 'tax') {
      $item['calculated'] = FALSE;
    }
    // Taxes are included already, so the subtotal without taxes doesn't
    // make sense.
    elseif ($item['id'] == 'tax_subtotal') {
      $item['callback'] = NULL;
    }
  }
}


/**
 * Performs actions on orders.
 *
 * An order in Ubercart represents a single transaction. Orders are created
 * during the checkout process where they sit in the database with a status of
 * "In Checkout". When a customer completes checkout, the order's status gets
 * updated to show that the sale has gone through. Once an order is created,
 * and even during its creation, it may be acted on by any module to connect
 * extra information to an order. Every time an action occurs to an order,
 * hook_order() gets invoked to let your modules know what's happening and
 * make stuff happen.
 *
 * @param $op
 *   The action being performed.
 * @param &$arg1
 *   This is the order object.
 * @param $arg2
 *   This is variable and is based on the value of $op:
 *   - new: Called when an order is created. $arg1 is a reference to the new
 *     order object, so modules may add to or modify the order at creation.
 *   - presave: Before an order object is saved, the hook gets invoked with this
 *     op to let other modules alter order data before it is written to the
 *     database. $order is a reference to the order object.
 *   - save: When an order object is being saved, the hook gets invoked with
 *     this op to let other modules do any necessary saving. $arg1 is a
 *     reference to the order object.
 *   - load: Called when an order is loaded after the order and product data has
 *     been loaded from the database. Passes $arg1 as the reference to the
 *     order object, so modules may add to or modify the order object when it's
 *     loaded.
 *   - submit: When a sale is being completed and the customer has clicked the
 *     Submit order button from the checkout screen, the hook is invoked with
 *     this op. This gives modules a chance to determine whether or not the
 *     order should be allowed. An example use of this is the credit module
 *     attempting to process payments when an order is submitted and returning
 *     a failure message if the payment failed.
 *     To prevent an order from passing through, you must return an array
 *     resembling the following one with the failure message:
 *     @code
 *       return array(array('pass' => FALSE, 'message' => t('We were unable to process your credit card.')));
 *     @endcode
 *   - can_update: Called before an order's status is changed to make sure the
 *     order can be updated. $arg1 is the order object with the old order
 *     status ID ($arg1->order_status), and $arg2 is simply the new order
 *     status ID. Return FALSE to stop the update for some reason.
 *   - update: Called when an order's status is changed. $arg1 is the order
 *     object with the old order status ID ($arg1->order_status), and $arg2 is
 *     the new order status ID.
 *   - can_delete: Called before an order is deleted to verify that the order
 *     may be deleted. Returning FALSE will prevent a delete from happening.
 *     (For example, the payment module returns FALSE by default when an order
 *     has already received payments.)
 *   - delete: Called when an order is deleted and before the rest of the order
 *     information is removed from the database. Passes $arg1 as the order
 *     object to let your module clean up it's tables.
 *   - total: Called when the total for an order is being calculated after the
 *     total of the products has been added. Passes $arg1 as the order object.
 *     Expects in return a value (positive or negative) by which to modify the
 *     order total.
 */
function hook_order($op, $arg1, $arg2) {
  switch ($op) {
    case 'save':
      // Do something to save payment info!
      break;
  }
}

/**
 * Adds links to local tasks for orders on the admin's list of orders.
 *
 * @param $order
 *   An order object.
 *
 * @return
 *   An array of specialized link arrays. Each link has the following keys:
 *   - name: The title of page being linked.
 *   - url: The link path. Do not use url(), but do use the $order's order_id.
 *   - icon: HTML of an image.
 *   - title: Title attribute text (mouseover tool-tip).
 */
function hook_order_actions($order) {
  $actions = array();
  $module_path = base_path() . drupal_get_path('module', 'uc_shipping');
  if (user_access('fulfill orders')) {
    $result = db_query("SELECT nid FROM {uc_order_products} WHERE order_id = %d AND data LIKE '%%s:9:\"shippable\";s:1:\"1\";%%'", $order->order_id);
    if (db_num_rows($result)) {
      $title = t('Package order !order_id products.', array('!order_id' => $order->order_id));
      $actions[] = array(
        'name' => t('Package'),
        'url' => 'admin/store/orders/'. $order->order_id .'/packages',
        'icon' => '<img src="'. $module_path .'/images/package.gif" alt="'. $title .'" />',
        'title' => $title,
      );
      $result = db_query("SELECT package_id FROM {uc_packages} WHERE order_id = %d", $order->order_id);
      if (db_num_rows($result)) {
        $title = t('Ship order !order_id packages.', array('!order_id' => $order->order_id));
        $actions[] = array(
          'name' => t('Ship'),
          'url' => 'admin/store/orders/'. $order->order_id .'/shipments',
          'icon' => '<img src="'. $module_path .'/images/ship.gif" alt="'. $title .'" />',
          'title' => $title,
        );
      }
    }
  }
  return $actions;
}

/**
 * Registers callbacks for an order pane.
 *
 * This hook is used to add panes to the order viewing and administration
 * screens. The default panes include areas to display and edit addresses,
 * products, comments, etc. Developers should use this hook when they need to
 * display or modify any custom data pertaining to an order. For example, a
 * store that uses a custom checkout pane to find out a customer's desired
 * delivery date would then create a corresponding order pane to show the data
 * on the order screens.
 *
 * hook_order_pane() works by defining new order panes and providing a little
 * bit of information about them. View the return value section below for
 * information about what parts of an order pane are defined by the hook.
 *
 * The real meat of an order pane is its callback function (which is specified
 * in the hook). The callback function handles what gets displayed on which
 * screen and what data can be manipulated. That is all somewhat out of the
 * scope of this API page, so you'll have to click here to read more about what
 * a callback function should contain.
 */
function hook_order_pane() {
  $panes[] = array(
    'id' => 'payment',
    'callback' => 'uc_order_pane_payment',
    'title' => t('Payment'),
    'desc' => t('Specify and collect payment for an order.'),
    'class' => 'pos-left',
    'weight' => 4,
    'show' => array('view', 'edit', 'customer'),
  );
  return $panes;
}

/**
 * Alters order pane definitions.
 *
 * @param $panes
 *   Array with the panes information as defined in hook_order_pane(), passed
 *   by reference.
 */
function hook_order_pane_alter(&$panes) {
  foreach ($panes as &$pane) {
    if ($pane['id'] == 'payment') {
      $pane['callback'] = 'my_custom_module_callback';
    }
  }
}

/**
 * Allows modules to alter ordered products when they're loaded with an order.
 *
 * @param &$product
 *   The product object as found in the $order object.
 * @param $order
 *   The order object to which the product belongs.
 *
 * @return
 *   Nothing should be returned. Hook implementations should receive the
 *   $product object by reference and alter it directly.
 */
function hook_order_product_alter(&$product, $order) {
  drupal_set_message('hook_order_product_alter(&$product, $order):');
  drupal_set_message('&$product: <pre>'. print_r($product, TRUE) .'</pre>');
  drupal_set_message('$order: <pre>'. print_r($order, TRUE) .'</pre>');
}

/**
 * Registers static order states.
 *
 * Order states are module-defined categories for order statuses. Each state
 * will have a default status that is used when modules need to move orders to
 * new state, but don't know which status to use.
 *
 * @return
 *   An array of order state definitions. Each definition is an array with the
 *   following keys:
 *   - id: The machine-readable name of the order state.
 *   - title: The human-readable, translated name.
 *   - weight: The list position of the state.
 *   - scope: Either "specific" or "general".
 */
function hook_order_state() {
  $states[] = array(
    'id' => 'canceled',
    'title' => t('Canceled'),
    'weight' => -20,
    'scope' => 'specific',
  );
  $states[] = array(
    'id' => 'in_checkout',
    'title' => t('In checkout'),
    'weight' => -10,
    'scope' => 'specific',
  );
  $states[] = array(
    'id' => 'post_checkout',
    'title' => t('Post checkout'),
    'weight' => 0,
    'scope' => 'general',
  );
  $states[] = array(
    'id' => 'completed',
    'title' => t('Completed'),
    'weight' => 20,
    'scope' => 'general',
  );

  return $states;
}

/**
 * Registers payment gateway callbacks.
 *
 * Payment gateways handle payments directly, without needing to redirect
 * off-site. The implementation allows for payment methods other than uc_credit
 * to use gateways, but in practice, they are only used for credit card
 * payments.
 *
 * @see http://www.ubercart.org/docs/api/hook_payment_gateway
 * @see hook_payment_gateway_charge()
 *
 * @return
 *   Returns an array of payment gateways, each with the following members:
 *   - "id": the machine-readable name of the payment gateway.
 *   - "title": the human-readable name of the payment gateway.
 *   - "description": a human-readable description of the payment gateway.
 *   - "settings": A callback function that returns the gateway settings form.
 *   - "credit": A callback function that processes the credit card. See
 *     hook_payment_gateway_charge() for details.
 */
function hook_payment_gateway() {
  $gateways[] = array(
    'id' => 'test_gateway',
    'title' => t('Test Gateway'),
    'description' => t('Process credit card payments through the Test Gateway.'),
    'credit' => 'test_gateway_charge',
  );
  return $gateways;
}

/**
 * Credit card charge callback.
 *
 * Called when a credit card should be processed. Credit card details supplied
 * by the user are available in $order->payment_details[].
 *
 * @see hook_payment_gateway()
 * @see uc_authorizenet_charge()
 * @see test_gateway_charge()
 *
 * @param $order_id
 *   The order ID that the payment relates to.
 * @param $amount
 *   The amount that should be charged.
 * @param $data
 *   An array of data related to the charge. By default, includes a 'txn_type'
 *   key which defines the transaction type, usually UC_CREDIT_AUTH_ONLY
 *   or UC_CREDIT_AUTH_CAPTURE.
 *
 * @return
 *   Returns an associative array with the following members:
 *   - "success": TRUE if the transaction succeeded, FALSE otherwise.
 *   - "message": a human-readable message describing the result of the
 *     transaction.
 *   - "log_payment": TRUE if the transaction should be regarded as a
 *     successful payment.
 */
function hook_payment_gateway_charge($order_id, $amount, $data) {
}

/**
 * Alters payment gateways.
 *
 * @param $gateways
 *   Payment gateways passed by reference.
 */
function hook_payment_gateway_alter(&$gateways) {
  // Change the title of all gateways.
  foreach ($gateways as &$gateway) {
    // $gateway was passed by reference.
    $gateway['title'] = t('Altered gateway @original', array('@original' => $gateway['title']));
  }
}

/**
 * Registers callbacks for payment methods.
 *
 * Payment methods are different ways to collect payment. By default, Ubercart
 * comes with support for check, credit card, and generic payments. Payment
 * methods show up at checkout or on the order administration screens, and they
 * collect different sorts of information from the user that is used to process
 * or track the payment.
 *
 * @see hook_payment_method_callback()
 *
 * @return
 *   An array of payment methods. The array contains a sub-array for each
 *   payment method. Required attributes:
 *   - "id": the machine-readable name of the payment method.
 *   - "name": the human-readable name of the payment method.
 *   - "title": the human-readable title of the payment method, displayed
 *     during checkout.
 *   - "desc": a human-readable description of the payment method.
 *   - "callback": a callback function that handles operations that the method
 *     may need to perform. See hook_uc_payment_method_callback()
 *   - "weight": the default weight of the payment method.
 *   - "checkout": if TRUE, the payment method will be enabled by default.
 *   - "no_gateway": should be set to TRUE, except for uc_credit which uses
 *     payment gateways.
 */
function hook_payment_method() {
  $methods[] = array(
    'id' => 'check',
    'name' => t('Check'),
    'title' => t('Check or Money Order'),
    'desc' => t('Pay by mailing a check or money order.'),
    'callback' => 'hook_payment_method_callback',
    'weight' => 1,
    'checkout' => TRUE,
  );
  return $methods;
}

/**
 * Callback function to perform various operations for a payment method.
 *
 * Possible operations are as follows:
 * - "cart-details": The payment method has been selected at checkout. Return
 *   HTML to be displayed in the payment method pane.
 * - "cart-process": Called when the user submits the checkout form with this
 *   payment method selected, used to process any form elements output by the
 *   'cart-details' op. Return FALSE to abort the checkout process, or NULL or
 *   TRUE to continue with checkout.
 * - "cart-review": Called when the checkout review page is being displayed.
 *   Return an array of data to be displayed below the payment method title on
 *   the checkout review page.
 * - "customer-view": Called when the order is being displayed to a customer.
 *   Return HTML to be displayed to customers.
 * - "order-delete": Called when an order is being deleted. Payment methods
 *   should clean up any extra data they stored related to the order.
 * - "order-details": Called when an order is being edited by an administrator.
 *   Return a string or a form array to be displayed to the administator.
 * - "order-load": Called from hook_uc_order('load') when this payment method
 *   is selected for the order.
 * - "order-process": Called when an order has been edited by an administrator.
 *   Process any form elements returned by the "order-details" op.
 * - "order-save": Called from hook_uc_order('save') when this payment method
 *   is selected for the order.
 * - "order-submit": Called from hook_uc_order('submit') when this payment
 *   method is selected for the order.
 * - "order-view": Called when the order is being displayed on the order admin
 *   pages. Return HTML to be displayed to administrators.
 * - "settings": Called when the payment methods page is being displayed.
 *   Return a system settings form array to configure the payment method.
 *
 * @see hook_payment_method()
 *
 * @param $op
 *   The operation being performed.
 * @param &$arg1
 *   The order object that relates to this operation.
 * @param $silent
 *   If TRUE, the callback should not call drupal_set_message().
 *
 * @return
 *   Dependent on $op.
 */
function hook_payment_method_callback($op, &$arg1, $silent = FALSE) {
  switch ($op) {
    case 'cart-details':
      return array('#markup' => t('Continue with checkout to complete payment.'));

    case 'settings':
      $form['uc_payment_method_account_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Payment gateway account number'),
        '#default_value' => variable_get('uc_payment_method_account_number', ''),
      );
      return $form;
  }
}

/**
 * Alter payment methods.
 *
 * @param $methods
 *   Payment methods passed by reference.
 */
function hook_payment_method_alter(&$methods) {
  // Change the title of all methods.
  foreach ($methods as &$method) {
    // $method was passed by reference.
    $method['title'] = t('Altered method @original', array('@original' => $method['title']));
  }
}

/**
 * Performs actions on product classes.
 *
 * @param $type
 *   The node type of the product class.
 * @param $op
 *   The action being performed on the product class:
 *   - insert: A new node type is created, or an existing node type is being
 *     converted into a product type.
 *   - update: A product class has been updated.
 *   - delete: A product class has been deleted. Modules that have attached
 *     additional information to the node type because it is a product type
 *     should delete this information.
 */
function hook_product_class($type, $op) {
  switch ($op) {
    case 'delete':
      db_query("DELETE FROM {uc_class_attributes} WHERE pcid = '%s'", $type);
      db_query("DELETE FROM {uc_class_attribute_options} WHERE pcid = '%s'", $type);
    break;
  }
}

/**
 * Returns a structured array representing the given product's description.
 *
 * Modules that add data to cart items when they are selected should display it
 * with this hook. The return values from each implementation will be
 * sent through to hook_product_description_alter() implementations and then
 * all descriptions are rendered using drupal_render().
 *
 * @param $product
 *   Product. Usually one of the values of the array returned by
 *   uc_cart_get_contents().
 *
 * @return
 *   A structured array that can be fed into drupal_render().
 */
function hook_product_description($product) {
  $description = array(
    'attributes' => array(
      '#product' => array(
        '#type' => 'value',
        '#value' => $product,
      ),
      '#theme' => 'uc_product_attributes',
      '#weight' => 1,
    ),
  );

  $desc =& $description['attributes'];

  // Cart version of the product has numeric attribute => option values so we
  // need to retrieve the right ones
  $weight = 0;
  if (empty($product->order_id)) {
    foreach (_uc_cart_product_get_options($product) as $option) {
      if (!isset($desc[$option['aid']])) {
        $desc[$option['aid']]['#attribute_name'] = $option['attribute'];
        $desc[$option['aid']]['#options'] = array($option['name']);
      }
      else {
        $desc[$option['aid']]['#options'][] = $option['name'];
      }
      $desc[$option['aid']]['#weight'] = $weight++;
    }
  }
  else {
    foreach ((array)$product->data['attributes'] as $attribute => $option) {
      $desc[] = array(
        '#attribute_name' => $attribute,
        '#options' => $option,
        '#weight' => $weight++,
      );
    }
  }

  return $description;
}

/**
 * Alters the given product description.
 *
 * @param $description
 *   Description array reference.
 * @param $product
 *   The product being described.
 */
function hook_product_description_alter(&$description, $product) {
  $description['attributes']['#weight'] = 2;
}

/**
 * Lists node types which should be considered products.
 *
 * Trusts the duck philosophy of object identification: if it walks like a duck,
 * quacks like a duck, and has feathers like a duck, it's probably a duck.
 * Products are nodes with prices, SKUs, and everything else Ubercart expects
 * them to have.
 *
 * @return
 *   Array of node type ids.
 */
function hook_product_types() {
  return array('product_kit');
}

/**
 * Handles additional data for shipments.
 *
 * @param $op
 *   The action being taken on the shipment. One of the following values:
 *   - load: The shipment and its packages are loaded from the database.
 *   - save: Changes to the shipment have been written.
 *   - delete: The shipment has been deleted and the packages are available
 *     for reshipment.
 * @param &$shipment
 *   The shipment object.
 *
 * @return
 *   Only given when $op is "load". An array of extra data to be added to the
 *   shipment object.
 */
function hook_shipment($op, &$shipment) {
  switch ($op) {
    case 'save':
      $google_order_number = uc_google_checkout_get_google_number($shipment->order_id);
      if ($google_order_number && $shipment->is_new) {
        $xml_data = '';
        foreach ($shipment->packages as $package) {
          if ($package->tracking_number) {
            $tracking_number = $package->tracking_number;
          }
          elseif ($shipment->tracking_number) {
            $tracking_number = $shipment->tracking_number;
          }
          if ($tracking_number) {
            foreach ($package->products as $product) {
              $xml_data .= '<item-shipping-information>';
              $xml_data .= '<item-id>';
              $xml_data .= '<merchant-item-id>'. check_plain($product->nid .'|'. $product->model) .'</merchant-item-id>';
              $xml_data .= '</item-id>';
              $xml_data .= '<tracking-data-list>';
              $xml_data .= '<tracking-data>';
              $xml_data .= '<carrier>'. check_plain($shipment->carrier) .'</carrier>';
              $xml_data .= '<tracking-number>'. check_plain($tracking_number) .'</tracking-number>';
              $xml_data .= '</tracking-data>';
              $xml_data .= '</tracking-data-list>';
              $xml_data .= '</item-shipping-information>';
            }
          }
        }
        if ($xml_data) {
          $request = '<?xml version="1.0" encoding="UTF-8"?>'. "\n";
          $request .= '<ship-items xmlns="http://checkout.google.com/schema/2" google-order-number="'. $google_order_number .'">';
          $request .= '<item-shipping-information-list>';
          $request .= $xml_data;
          $request .= '</item-shipping-information-list>';
          $request .= '<send-email>true</send-email>';
          $request .= '</ship-items>';
          $response = uc_google_checkout_send_request('request', $request);
        }
      }
    break;
    case 'delete':
      $google_order_number = uc_google_checkout_get_google_number($shipment->order_id);
      if ($google_order_number) {
        foreach ($shipment->packages as $package) {
          foreach ($package->products as $product) {
            $reset_ids[] = check_plain($product->nid .'|'. $product->model);
          }
        }
        $request = '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
        $request .= '<reset-items-shipping-information xmlns="http://checkout.google.com/schema/2" google-order-number="'. $google_order_number .'">';
        $request .= '<item-ids>';
        foreach (array_unique($reset_ids) as $item_id) {
          $request .= '<item-id>';
          $request .= '<merchant-item-id>'. $item_id .'</merchant-item-id>';
          $request .= '</item-id>';
        }
        $request .= '</item-ids>';
        $request .= '<send-email>false</send-email>';
        $request .= '</reset-items-shipping-information>';
      }
      $response = uc_google_checkout_send_request('request', $request);
    break;
  }
}

/**
 * Defines callbacks and service options for shipping methods.
 *
 * The shipping quote controller module, uc_quote, expects a very specific
 * structured array of methods from the implementations of this hook.
 *
 * The weights and enabled flags for shipping methods and types are set at the
 * Shipping Quote Settings page under Store Configuration. They keys of the
 * variables are the ids of the shipping methods. The "quote" and "ship" arrays of
 * the method are both optional.
 *
 * @return
 *   An array of shipping methods which have the following keys.
 *   - type: The quote and shipping types are ids of the product shipping type
 *     that these methods apply to. type may also be 'order' which indicates
 *     that the quote applies to the entire order, regardless of the shipping
 *     types of its products. This is used by quote methods that are base on
 *     the location of the customer rather than their purchase.
 *   - callback: The function that is called by uc_quote when a shipping quote
 *     is requested. Its arguments are the array of products and an array of
 *     order details (the shipping address). The return value is an array
 *     representing the rates quoted and errors returned (if any) for each
 *     option in the accessorials array.
 *   - accessorials: This array represents the different options the customer
 *     may choose for their shipment. The callback function should generate a
 *     quote for each option in accessorials and return them via an array.
 *     drupal_to_js() is very useful for this.
 *     @code
 *     return array(
 *       '03' => array('rate' => 15.75, 'format' => uc_price(15.75, $context) 'option_label' => t('UPS Ground'),
 *       'error' => 'Additional handling charge automatically applied.'),
 *       '14' => array('error' => 'Invalid package type.'),
 *       '59' => array('rate' => 26.03, 'format' => uc_price(26.03, $context), 'option_label' => t('UPS 2nd Day Air A.M.'))
 *     );
 *     @endcode
 *   - pkg_types: The list of package types that the shipping method can handle.
 *     This should be an associative array that can be used as the #options of
 *     a select form element. It is recommended that a function be written to
 *     output this array so the method doesn't need to be found just for the
 *     package types.
 */
function hook_shipping_method() {
  $methods = array();

  $enabled = variable_get('uc_quote_enabled', array('ups' => TRUE));
  $weight = variable_get('uc_quote_method_weight', array('ups' => 0));
  $methods['ups'] = array(
    'id' => 'ups',
    'title' => t('UPS'),
    'enabled' => $enabled['ups'],
    'weight' => $weight['ups'],
    'quote' => array(
      'type' => 'small package',
      'callback' => 'uc_ups_quote',
      'accessorials' => array(
        '03' => t('UPS Ground'),
        '11' => t('UPS Standard'),
        '01' => t('UPS Next Day Air'),
        '13' => t('UPS Next Day Air Saver'),
        '14' => t('UPS Next Day Early A.M.'),
        '02' => t('UPS 2nd Day Air'),
        '59' => t('UPS 2nd Day Air A.M.'),
        '12' => t('UPS 3-Day Select'),
      ),
    ),
    'ship' => array(
      'type' => 'small package',
      'callback' => 'uc_ups_fulfill_order',
      'pkg_types' => array(
        '01' => t('UPS Letter'),
        '02' => t('Customer Supplied Package'),
        '03' => t('Tube'),
        '04' => t('PAK'),
        '21' => t('UPS Express Box'),
        '24' => t('UPS 25KG Box'),
        '25' => t('UPS 10KG Box'),
        '30' => t('Pallet'),
      ),
    ),
  );

  return $methods;
}

/**
 * Defines shipping types for shipping methods.
 *
 * This hook defines a shipping type that this module is designed to handle.
 * These types are specified by a machine- and human-readable name called 'id',
 * and 'title' respectively. Shipping types may be set for individual products,
 * manufacturers, and for the entire store catalog. Shipping modules should be
 * careful to use the same shipping type ids as other similar shipping modules
 * (i.e., FedEx and UPS both operate on "small package" shipments). Modules that
 * do not fulfill orders may not need to implement this hook.
 *
 * @return
 *   An array of shipping types keyed by a machine-readable name.
 */
function hook_shipping_type() {
  $weight = variable_get('uc_quote_type_weight', array('small_package' => 0));

  $types = array();
  $types['small_package'] = array(
    'id' => 'small_package',
    'title' => t('Small packages'),
    'weight' => $weight['small_package'],
  );

  return $types;
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
 *   - "status": "ok", "warning", or "error" depending on the message.
 *   - "title" The title of the status message or module that defines it.
 *   - "desc": The description; can be any message, including links to pages and
 *     forms that deal with the issue being reported.
 */
function hook_store_status() {
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
        '#value' => l($node->designer, 'collections/'. $node->designer_tid),
        '#cell_attributes' => array('class' => 'designer'),
      );
    }
  }
}

/**
 * Allows modules to alter TAPIr table headers.
 *
 * This is most often done when a developer wants to add a sortable field to
 * the table. A sortable field is one where the header can be clicked to sort
 * the table results. This cannot be done using hook_tapir_table_alter() as
 * once that is called the query has already executed.
 *
 * The example below adds a 'designer' column to the catalog product table. The
 * example module would also have added joins to the query using
 * hook_db_rewrite_sql() in order for table 'td2' to be valid. The 'name' field
 * is displayed in the table and the header has the title 'Designer'.
 *
 * Also shown are changes made to the header titles for list_price and price
 * fields.
 *
 * @see hook_db_rewrite_sql()
 *
 * @param $header
 *   Reference to the array header declaration (i.e $table['#header']).
 * @param $table_id
 *   Table ID. Also the function called to build the table declaration.
 */
function hook_tapir_table_header_alter(&$header, $table_id) {
  if ($table_id == 'uc_product_table') {
    $header['designer'] = array(
      'weight' => 2,
      'cell' => array(
        'data' => t('Designer'),
        'field' => 'td2.name',
      ),
    );

    $header['list_price']['cell']['data'] = t('RRP');
    $header['price']['cell']['data'] = t('Sale');
    $header['add_to_cart']['cell']['data'] = '';
  }
}

/**
 * Takes action when checkout is completed.
 *
 * @param $order
 *   The resulting order object from the completed checkout.
 * @param $account
 *   The customer that completed checkout, either the current user, or the
 *   account created for an anonymous customer.
 */
function hook_uc_checkout_complete($order, $account) {
  // Get previous records of customer purchases.
  $nids = array();
  $result = db_query("SELECT uid, nid, qty FROM {uc_customer_purchases} WHERE uid = %d", $account->uid);
  while ($record = db_fetch_object($result)) {
    $nids[$record->nid] = $record->qty;
  }

  // Update records with new data.
  $record = array('uid' => $account->uid);
  foreach ($order->products as $product) {
    $record['nid'] = $product->nid;
    if (isset($nids[$product->nid])) {
      $record['qty'] = $nids[$product->nid] + $product->qty;
      db_write_record($record, 'uc_customer_purchases', array('uid', 'nid'));
    }
    else {
      $record['qty'] = $product->qty;
      db_write_record($record, 'uc_customer_purchases');
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
 * For a description of the hook parameters:
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
 * Adds invoice templates to the list of suggested template files.
 *
 * Allows modules to declare new "types" of invoice templates (other than the
 * default 'admin' and 'customer').
 *
 * @return
 *   Array of template names that are available choices when mailing an invoice.
 */
function hook_uc_invoice_templates() {
  return array('admin', 'customer');
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
 * Takes action when a payment is entered for an order.
 *
 * @param $order
 *   The order object.
 * @param $method
 *   The name of the payment method used.
 * @param $amount
 *   The value of the payment.
 * @param $account
 *   The user account that entered the order. When the payment is entered
 *   during checkout, this is probably the order's user. Otherwise, it is
 *   likely a store administrator.
 * @param $data
 *   Extra data associated with the transaction.
 * @param $comment
 *   Any comments from the user about the transaction.
 */
function hook_uc_payment_entered($order, $method, $amount, $account, $data, $comment) {
  drupal_set_message(t('User @uid entered a @method payment of @amount for order @order_id.',
    array(
      '@uid' => $account->uid,
      '@method' => $method,
      '@amount' => uc_price($amount, array('location' => 'hook-payment', 'revision' => 'formatted-original')),
      '@order_id' => $order->order_id,
    ))
  );
}

/**
 * Defines price handlers for your module.
 *
 * You may define one price alterer and one price formatter. You may also
 * define options that are merged into the options array in order of each price
 * alterer's weight.
 */
function hook_uc_price_handler() {
  return array(
    'alter' => array(
      'title' => t('My price handler'),
      'description' => t('Handles my price alteration needs.'),
      'callback' => 'my_price_handler_alter',
    ),
    'format' => array(
      'title' => t('My price handler'),
      'description' => t('Handles my price formatting needs.'),
      'callback' => 'my_price_handler_format',
    ),
    'options' => array(
      'sign' => variable_get('uc_currency_sign', '*'),
      'sign_after' => TRUE,
      'prec' => 4,
      'dec' => ',',
      'thou' => '.',
      'label' => FALSE,
      'my_option_that_my_formatter_recognizes' => 1337,
    )
  );
}

/**
 * Defines default product classes.
 *
 * The results of this hook are eventually passed through hook_node_info(), so
 * you may include any keys that hook_node_info() uses. Defaults will be
 * provided where keys are not set. This hook can also be used to override the
 * default "product" product class name and description.
 */
function hook_uc_product_default_classes() {
  return array(
    'my_class' => array(
      'name' => t('My product class'),
      'description' => t('Content type description for my product class.'),
    ),
  );
}

/**
 * Notifies core of any SKUs your module adds to a given node.
 *
 * NOTE: DO NOT map the array keys, as the possibility for numeric SKUs exists,
 * and this will conflict with the behavior of module_invoke_all(), specifically
 * array_merge_recursive().
 *
 * Code lifted from uc_attribute.module.
 */
function hook_uc_product_models($nid) {
  $models = array();

  // Get all the SKUs for all the attributes on this node.
  $adjustments = db_query("SELECT DISTINCT model FROM {uc_product_adjustments} WHERE nid = %d", $nid);
  while ($adjustment = db_result($adjustments)) {
    $models[] = $adjustment;
  }

  return $models;
}

/**
 * Allows modules to take action when a stock level is changed.
 *
 * @param $sku
 *   The SKU whose stock level is being changed.
 * @param $stock
 *   The stock level before the adjustment.
 * @param $qty
 *   The amount by which the stock level was changed.
 */
function hook_uc_stock_adjusted($sku, $stock, $qty) {
  $params = array(
    'sku' => $sku,
    'stock' => $stock,
    'qty' => $qty,
  );

  drupal_mail('uc_stock_notify', 'stock-adjusted', uc_store_email_from(), language_default(), $params);
}

/**
 * Determines whether or not UC Google Analytics should add e-commerce tracking
 * code to the bottom of the page.
 *
 * The Google Analytics module takes care of adding the necessary .js file from
 * Google for tracking general statistics.  The UC Google Analytics module works
 * in conjunction with this code to add e-commerce specific code.  However, the
 * e-commerce code should only be added on appropriate pages.  Generally, the
 * correct page will be the checkout completion page at cart/checkout/complete.
 * However, because modules can change the checkout flow as necessary, it must
 * be possible for alternate pages to be used.
 *
 * This hook allows other modules to tell the UC Google Analytics module that
 * it should go ahead and add the e-commerce tracking code to the current page.
 * A module simply needs to implement this hook and return TRUE on the proper
 * order completion page to let UC Google Analytics know it should add the
 * e-commerce tracking code to the current page.
 *
 * The implementation below comes from the 2Checkout.com module which uses an
 * alternate checkout completion page.
 *
 * @return
 *   TRUE if e-commerce tracking code should be added to the current page.
 */
function hook_ucga_display() {
  // Tell UC Google Analytics to display the e-commerce JS on the custom
  // order completion page for this module.
  if (arg(0) == 'cart' && arg(1) == '2checkout' && arg(2) == 'complete') {
    return TRUE;
  }
}

/**
 * Allows modules to alter items before they're added to the UC Google Analytics
 * e-commerce tracking code.
 *
 * The UC Google Analytics module constructs function calls that work through
 * the Google Analytics JS API to report purchased items for e-commerce tracking
 * purposes.  The module builds the argument list for each product on an order
 * and uses this hook to give other modules a chance to alter what gets reported
 * to Google Analytics.  Additional arguments passed to implementations of this
 * hook are provided for context.
 *
 * @param $item
 *   An array of arguments being passed to Google Analytics representing an item
 *   on the order, including order_id, sku, name, category, price, and qty.
 * @param $product
 *   The product object as found in the $order object.
 * @param $trans
 *   The array of arguments that were passed to Google Analytics to represent
 *   the transaction.
 * @param $order
 *   The order object being reported to Google Analytics.
 *
 * @return
 *   Nothing should be returned. Hook implementations should receive the $item
 *   array by reference and alter it directly.
 */
function hook_ucga_item_alter(&$item, $product, $trans, $order) {
  // Example implementation: always set the category to "UBERCART".
  $item['category'] = 'UBERCART';
}

/**
 * Allows modules to alter transaction info before it's added to the UC Google
 * Analytics e-commerce tracking code.
 *
 * The UC Google Analytics module constructs function calls that work through
 * the Google Analytics JS API to report order information for e-commerce
 * tracking purposes.  The module builds the argument list for the transaction
 * and uses this hook to give other modules a chance to alter what gets reported
 * to Google Analytics.
 *
 * @param $trans
 *   An array of arguments being passed to Google Analytics representing the
 *   transaction, including order_id, store, total, tax, shipping, city, state,
 *   and country.
 * @param $order
 *   The order object being reported to Google Analytics.
 *
 * @return
 *   Nothing should be returned. Hook implementations should receive the $trans
 *   array by reference and alter it directly.
 */
function hook_ucga_trans_alter(&$trans, $order) {
  // Example implementation: prefix all orders with "UC-".
  $trans['order_id'] = 'UC-'. $trans['order_id'];
}

/**
 * Handles requests to update a cart item.
 *
 * @param $nid
 *   Node id of the cart item.
 * @param $data
 *   Array of extra information about the item.
 * @param $qty
 *   The quantity of this item in the cart.
 * @param $cid
 *   The cart id. Defaults to NULL, which indicates that the current user's cart
 *   should be retrieved with uc_cart_get_id().
 */
function hook_update_cart_item($nid, $data = array(), $qty, $cid = NULL) {
  if (!$nid) return NULL;
  $cid = !(is_null($cid) || empty($cid)) ? $cid : uc_cart_get_id();
  if ($qty < 1) {
    uc_cart_remove_item($nid, $cid, $data);
  }
  else {
    db_query("UPDATE {uc_cart_products} SET qty = %d, changed = %d WHERE nid = %d AND cart_id = '%s' AND data = '%s'", $qty, time(), $nid, $cid, serialize($data));
  }

  // Rebuild the items hash
  uc_cart_get_contents(NULL, 'rebuild');
  if (!strpos(request_uri(), 'cart', -4)) {
    drupal_set_message(t('Your item(s) have been updated.'));
  }
}

/**
 * Allows modules to react to the removal of an expiring role.
 *
 * @param $account
 *   The Drupal user object.
 * @param $rid
 *   The Drupal role ID.
 */
function hook_uc_roles_delete($account, $rid) {
  // Example: set the expiration date CCK field on a content profile node
  // to midnight of the current date when an expiring role is deleted
  $node = content_profile_load('profile', $account->uid, '', true);

  if ($node) {
    $node->field_expiration_date['0']['value'] = date('Y-m-dT00:00:00');
    node_save($node);
  }
}

/**
 * Allows modules to react to the addition of an expiring role.
 *
 * @param $account
 *   The Drupal user object.
 * @param $rid
 *   The Drupal role ID.
 * @param $timestamp
 *   The UNIX timestamp of the role expiration.
 */
function hook_uc_roles_grant($account, $rid, $timestamp) {
  // Example: update the expiration date CCK field on a content profile node
  // when an expiring role is granted
  $node = content_profile_load('profile', $account->uid, '', true);

  if ($node) {
    $node->field_expiration_date['0']['value'] = date('c', $timestamp);
    node_save($node);
  }
}

/**
 * Allows modules to react to the renewal of an expiring role.
 *
 * @param $account
 *   The Drupal user object.
 * @param $rid
 *   The Drupal role ID.
 * @param $timestamp
 *   The UNIX timestamp of the role expiration.
 */
function hook_uc_roles_renew($account, $rid, $timestamp) {
  // Example: update the expiration date CCK field on a content profile node
  // when an expiring role is renewed
  $node = content_profile_load('profile', $account->uid, '', true);

  if ($node) {
    $node->field_expiration_date['0']['value'] = date('c', $timestamp);
    node_save($node);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
