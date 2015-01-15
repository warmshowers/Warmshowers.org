<?php

/**
 * @file
 * Uc recurring subscription UI.
 */

/**
 * Create the subscription overiew page.
 */
function uc_recurring_subscription_overview() {

  // Warn user if the ubercart API patch not applied.
  if (!function_exists('uc_attribute_load_multiple')) {
    drupal_set_message(t('This module requires a patch to ubercart, read the <a href="@readme">README.txt</a>', array(
      '@readme' => url(drupal_get_path('module', 'uc_recurring_subscription') .'/README.txt')
    )), 'error');
    return '';
  }

  drupal_add_css(drupal_get_path('module', 'uc_recurring_subscription') .'/uc_recurring_subscription.css');
  $product_class = variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription');
  $result = db_query("SELECT n.nid, n.title
                      FROM {node} n
                        LEFT JOIN {uc_recurring_subscription} rs ON rs.nid=n.nid
                      WHERE n.type = '%s'
                      ORDER BY rs.weight", $product_class);

  $roles = user_roles(TRUE);

  $form['products']['#tree'] = TRUE;
  $form['products']['#theme'] = 'uc_recurring_subscription_products';
  while($node = db_fetch_object($result)) {
    $product = node_load($node->nid);
    $attribute = uc_attribute_load(variable_get('uc_recurring_subscription_attribute', ''));

    // roles
    $product_roles = array();
    if (!empty($product->subscription->access['subscribe_grant'])) {
      $product_roles = array_intersect_key($roles, $product->subscription->access['subscribe_grant']);
    }
    // payment options
    $products = _uc_recurring_subscription_get_product_features($node->nid);
    $intervals = array();
    foreach ($products as $id => $feature) {
      $interval = uc_price($feature->fee_amount, array()) .' - '. _uc_recurring_subscription_create_attribute_option($feature->regular_interval_value, $feature->regular_interval_unit);

      if ($product->attributes[$attribute->aid]->default_option == $feature->option->oid ) {
        $interval = '<span class="default-option">'. $interval .'</span>';
      }
      if (module_exists('uc_cart_links')) {
        $interval .= ' '. t('(<a href="@link">cart link</a>)', array('@link' => url('cart/add/e-p'. $node->nid .'_q1_a'. $attribute->aid .'o'. $feature->option->oid .'-isub', array('query' => 'destination=cart/checkout', 'absolute' => TRUE))));
      }

      if ($feature->regular_interval_value != $feature->initial_charge_value &&
          $feature->regular_interval_unit != $feature->initial_charge_unit) {
        $trial = uc_price($product->sell_price, array()) .' - '. $feature->initial_charge_value .' '. $feature->initial_charge_unit;
      }

      $intervals[] = $interval;
    }

    $form['products'][$node->nid]['title'] = array('#value' => l($product->title, 'node/'. $product->nid));
    $form['products'][$node->nid]['role'] = array('#value' => implode(', ', $product_roles));
    $form['products'][$node->nid]['trial'] = array('#value' => empty($trial) ? t('None') : $trial);
    $form['products'][$node->nid]['interval'] = array('#value' => implode('<br/>', $intervals));
    $form['products'][$node->nid]['operations'] = array(
      '#value' => l('edit', 'admin/store/subscriptions/'. $node->nid .'/edit') .' | '. l('delete', 'node/'. $node->nid .'/delete', array('query' => 'destination=admin/store/subscriptions'))
    );
    $form['products'][$node->nid]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $product->subscription->weight,
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save order'));

  return $form;
}

/**
 * Update the weights of the products.
 */
function uc_recurring_subscription_overview_submit($form, &$form_state) {
  foreach($form_state['values']['products'] as $nid => $product) {
    db_query("UPDATE {uc_recurring_subscription} SET weight = %d WHERE nid = %d", $product['weight'], $nid);
  }
}

/**
 * Form to add a new payment interval for the product.
 */
function uc_recurring_subscription_product_form($form_state, $product_id = NULL) {
  drupal_add_js(drupal_get_path('module', 'uc_recurring_subscription') .'/uc_recurring_subscription.js', 'module');
  drupal_add_css(drupal_get_path('module', 'uc_recurring_subscription') .'/uc_recurring_subscription.css');

  $form = array();
  $products = array();
  if (isset($product_id)) {
    $form['product_id'] = array(
      '#type' => 'hidden',
      '#value' => $product_id,
    );
    $node = node_load($product_id);
    $products = _uc_recurring_subscription_get_product_features($node->nid);
  }
  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => 'Step One: Product Details',
  );
  $form['product']['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Title',
    '#required' => TRUE,
    '#default_value' => $node->title,
  );
  $form['product']['body'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#default_value' => $node->body,
  );

  $form['recurring'] = array(
    '#type' => 'fieldset',
    '#title' => 'Step Two: Define Subscription Details',
    '#description' => t('By adding multiple payment options, users will be given the option to choose from one of these payment options, this allows you to specify different prices and payment plans for this subscription product'),
  );
  $form['recurring']['trial'] = array(
    '#type' => 'fieldset',
    '#title' => 'Add trial',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // We look at the default option for the trial information.
  $aid = variable_get('uc_recurring_subscription_attribute', '');
  if (!empty($aid)) {
    $default_option = $node->attributes[$aid]->default_option;
    $name = $node->attributes[$aid]->options[$default_option]->name;
    foreach($products as $product) {
      $num = $product->regular_interval_value;
      $unit = $product->regular_interval_unit;
      if ($name == _uc_recurring_subscription_create_attribute_option($num, $unit)) {
        // This is the default product, the price and initial interval should
        // be different if there is a trial.
        if ($node->sell_price != $product->fee_amount ||
            $product->regular_interval_value != $product->initial_charge_value ||
            $product->regular_interval_unit != $product->initial_charge_unit) {
          $trial_product = $product;
          $form['recurring']['trial']['#collapsed'] = FALSE;
        }
        break;
      }
    }
  }
  $form['recurring']['trial']['trial_amount'] = array(
    '#type' => 'textfield',
    '#title' => 'Trial Amount',
    '#default_value' => !empty($trial_product) ? $node->sell_price : '',
    '#size' => 20,
  );
  $form['recurring']['trial']['trial_interval_value'] = array(
    '#type' => 'textfield',
    '#default_value' => $trial_product->initial_charge_value,
    '#size' => 2,
    '#prefix' => '<div class="subscription-interval-value">',
    '#suffix' => '</div>',
  );
  $form['recurring']['trial']['trial_interval_unit'] = array(
    '#type' => 'select',
    '#options' => array(
      'days' => t('day(s)'),
      'weeks' => t('week(s)'),
      'months' => t('month(s)'),
      'years' => t('year(s)'),
    ),
    '#default_value' => $trial_product->initial_charge_unit,
    '#prefix' => '<div class="subscription-interval-period">',
    '#suffix' => '</div>',
  );

  $form['recurring']['add'] = array(
    '#type' => 'button',
    '#value' => 'Add a payment option',
    '#ahah' => array(
      'event' => 'click',
      'path' => 'subscriptions/ahah/add_interval',
      'wrapper' => 'recurring_intervals',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array(
        'type' => 'bar',
      ),
    ),
    '#prefix' => '<div style="clear:both"></div>',
  );

  $form['recurring']['recurring_intervals'] = array(
    '#value' => t(' '),
    '#prefix' => '<div id="recurring_intervals">',
    '#suffix' => '</div>',
  );

  if (isset($form_state['recurring_count'])) {
    $recurring_count = $form_state['recurring_count'];
  }
  else {
    $recurring_count = count($products);
  }
  $form['recurring']['recurring_intervals']['#theme'] = 'uc_recurring_subscription_item';
  if ($recurring_count > 0) {
    $delta = 0;
    foreach ($products as $product) {
      $form['recurring']['recurring_intervals'][$delta] = _uc_recurring_subscription_add_interval_form($delta, $product);
      $default_options[$delta] = '';
      if (isset($product) && $product->option->default_option) {
        $default_option = $delta;
      }
      $delta++;
    }
  }
  $form['recurring']['recurring_intervals']['default_payment'] = array(
    '#type' => 'radios',
    '#options' => $default_options,
    '#default_value' => $default_option,
  );

  $form['access'] = array(
    '#type' => 'fieldset',
    '#title' => 'Step Three: Access & Permissions',
    '#description' => t('In this section you can define the access that is granted or removed on subscription creation, renewal and expiration events.'),
  );

  // If uc_roles is enabled we will use the settings from this.
  $roles = array();
  if (module_exists('uc_roles')) {
    $roles = _uc_roles_get_choices();
  }
  else if (user_access('administer permissions')) {
    $roles = user_roles(TRUE);
    unset($roles[DRUPAL_AUTHENTICATED_RID]);
  }

  $form['access']['role'] = array(
    '#type' => 'fieldset',
    '#title' => 'Roles',
    '#description' => t('Select the role(s) which are assigned to members who subscribe to this subscription product and then removed when the subscription expires.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (empty($roles)) {
    $form['access']['role']['message'] = array(
      '#value' => t('You need to first add a user role <a href="@role_link">here</a>.', array('@role_link' => url('admin/user/roles'))),
    );
  }
  else {
    $form['access']['role']['#theme'] = 'uc_recurring_subscription_role_items';
    $form['access']['role']['subscribe_grant'] = array(
      '#type' => 'select',
      '#options' => $roles,
      '#multiple' => TRUE,
      '#default_value' => $node->subscription->access['subscribe_grant'],
      '#attributes' => array('class' => 'role-option'),
      '#parents' => array('access', 0, 'subscribe_grant'),
    );
    $form['access']['role']['expire_grant'] = array(
      '#type' => 'select',
      '#options' => $roles,
      '#multiple' => TRUE,
      '#default_value' => $node->subscription->access['expire_grant'],
      '#attributes' => array('class' => 'role-option'),
      '#parents' => array('access', 0, 'expire_grant'),
    );
    $form['access']['role']['expire_revoke'] = array(
      '#type' => 'select',
      '#options' => $roles,
      '#multiple' => TRUE,
      '#default_value' => $node->subscription->access['expire_revoke'],
      '#attributes' => array('class' => 'role-option'),
      '#parents' => array('access', 0, 'expire_revoke'),
    );
  }

  if (module_exists('og_actions')) {
    $groups = og_all_groups_options();
    if (count($groups)) {
      $form['access']['og'] = array(
        '#type' => 'fieldset',
        '#title' => 'Organic Groups',
        '#description' => t('Select which organic groups a user should be subscribed to on a new subscripion purchased or unsubscribed when the subscription expires.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#theme' => 'uc_recurring_subscription_og_items',
      );
      $form['access']['og']['subscribe_grant_og'] = array(
        '#type' => 'select',
        '#default_value' => $node->subscription->access['subscribe_grant_og'],
        '#options' => $groups,
        '#multiple' => TRUE,
        '#parents' => array('access', 0, 'subscribe_grant_og'),
      );
      $form['access']['og']['expire_grant_og'] = array(
        '#type' => 'select',
        '#default_value' => $node->subscription->access['expire_grant_og'],
        '#options' => $groups,
        '#multiple' => TRUE,
        '#parents' => array('access', 0, 'expire_grant_og'),
      );
      $form['access']['og']['expire_revoke_og'] = array(
        '#type' => 'select',
        '#default_value' => $node->subscription->access['expire_revoke_og'],
        '#options' => $groups,
        '#multiple' => TRUE,
        '#parents' => array('access', 0, 'expire_revoke_og'),
      );
    };
  }

/*
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => 'Step Four: Email/Notifications',
    '#description' => t('Setup emails to be sent on various events.'),
  );
  $form['email']['#theme'] = 'uc_recurring_subscription_email_options';
  $form['email']['notication_event'] = array(
    '#type' => 'select',
    '#title' => t('Add new email on event'),
    '#options' => array(),
  );
  $form['email']['add_notification'] = array('#type' => 'submit', '#value' => t('Add email'));
*/

  if (!empty($node->nid) && $node->type != variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription')) {
    return confirm_form($form, '', uc_referer_uri(), t('This product is not a subscription product, saving this form will convert this product to a subscription product, which could result in data being lost if you have specified custom fields.'), t('Save product and convert to a subscription'), t('Cancel'));
  }
  else {
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  }

  if (!isset($product_id)) {
    $form['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  }

  return $form;
}

/**
 * This is just a helper function for handling ahah callbacks.
 */
function uc_recurring_subscription_ahah($context) {
  $func = '_uc_recurring_subscription_ahah_'. $context;
  $form_state['values'] = $_POST;
  if (function_exists($func)) {
    $func($form_state);
  }
  exit();
}

/**
 * Creates the payment interval form elements.
 */
function _uc_recurring_subscription_add_interval_form($delta, $product = NULL) {
  $form = array(
    '#tree' => TRUE,
  );
  $form['pfid'] = array(
    '#type' => 'hidden',
    '#value' => $product->pfid,
    '#parents' => array('recurring_intervals', $delta, 'pfid'),
  );
  $form['interval_value'] = array(
    '#type' => 'textfield',
    '#default_value' => $product->regular_interval_value,
    '#size' => 2,
    '#prefix' => '<div class="subscription-interval-value">',
    '#suffix' => '</div>',
    '#parents' => array('recurring_intervals', $delta, 'interval_value'),
  );
  $form['interval_unit'] = array(
    '#type' => 'select',
    '#options' => array(
      'days' => t('day(s)'),
      'weeks' => t('week(s)'),
      'months' => t('month(s)'),
      'years' => t('year(s)'),
    ),
    '#default_value' => $product->regular_interval_unit,
    '#prefix' => '<div class="subscription-interval-period">',
    '#suffix' => '</div>',
    '#parents' => array('recurring_intervals', $delta, 'interval_unit'),
  );
  $form['amount'] = array(
    '#type' => 'textfield',
    '#default_value' => $product->fee_amount,
    '#size' => 10,
    '#prefix' => '<div class="subscription-amount">',
    '#suffix' => '</div>',
    '#parents' => array('recurring_intervals', $delta, 'amount'),
  );
  $form['number_intervals'] = array(
    '#type' => 'textfield',
    '#default_value' => $product->number_intervals < 0 ? '' : $product->number_intervals,
    '#attributes' => $product->number_intervals < 0 ? array('disabled' => 'disabled') : array(),
    '#size' => 10,
    '#prefix' => '<div class="subscription-num-intervals">',
    '#suffix' => '</div>',
    '#parents' => array('recurring_intervals', $delta, 'number_intervals'),
  );
  $attributes['class'] = 'unlimited-checkbox';
  if ($product->number_intervals < 0) {
    $attributes['checked'] = 'checked';
  }
  $form['unlimited'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unlimited'),
    '#attributes' => $attributes,
    '#prefix' => '<div class="subscription-unlimited-intervals">',
    '#suffix' => '</div>',
    '#parents' => array('recurring_intervals', $delta, 'unlimited'),
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#delta' => 50,
    '#default_value' => $product->option->ordering,
    '#parents' => array('recurring_intervals', $delta, 'weight'),
  );
  $form['operations'] = array(
    '#type' => 'button',
    '#value' => t('remove'),
    '#parents' => array('recurring_intervals', $delta, 'operations'),
    '#ahah' => array(
      'event' => 'click',
      'path' => 'subscriptions/ahah/remove_interval/'. $delta,
      'wrapper' => 'recurring_intervals',
      'method' => 'replace',
      'progress' => array(
        'type' => 'throbber',
      ),
    ),
  );
  return $form;
}

/**
 * Adds a payment intervals option.
 */
function _uc_recurring_subscription_ahah_add_interval() {
  $delta = sizeof($_POST['recurring_intervals']) ? max(array_keys($_POST['recurring_intervals']))+1 : 0;
  $fields = _uc_recurring_subscription_add_interval_form($delta);

  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);
  $form['recurring']['recurring_intervals'][$delta] = $fields;
  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $new_form = $form['recurring']['recurring_intervals'];
  $output = drupal_render($new_form);

  drupal_json(array('data' => $output, 'status' => true));
}

/**
 * Removes a payment interval option.
 */
function _uc_recurring_subscription_ahah_remove_interval() {
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];

  $delta = arg(3);
  $form = form_get_cache($form_build_id, $form_state);
  $pfid = $form['recurring']['recurring_intervals'][$delta]['pfid']['#value'];
  if (!empty($pfid)) {
    uc_product_feature_delete($pfid);
  }
  unset($form['recurring']['recurring_intervals'][$delta]);
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $new_form = $form['recurring']['recurring_intervals'];
  $output = drupal_render($new_form);

  print drupal_to_js(array('data' => $output, 'status' => true));
}

/**
 * Validates the subscription form submission.
 */
function uc_recurring_subscription_product_form_validate(&$form, &$form_state) {
  // Make the changes we want to the form state.
  if ($form_state['values']['recurring_intervals']) {
    $form_state['recurring_count'] = count($form_state['values']['recurring_intervals']);
  }

  if ($form_state['values']['op'] == 'submit') {
    if ($form_state['recurring_count'] <= 0) {
      form_set_error('recurring', t('You must specify at least one subscription/payment interval'));
    }
  }
}

/**
 *
 */
function uc_recurring_subscription_product_form_submit(&$form, &$form_state) {
  global $user;
  switch ($form_state['values']['op']) {
    case 'Delete':
      node_delete($form_state['values']['product_id']);
      break;
    default: // Save
      if (empty($form_state['values']['product_id'])) {
        $node = new stdClass();
        $node->type = variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription');
        $node->uid = $user->uid;
        $node->status = 1;
        $new = TRUE;
        node_save($node);
      }
      else {
        $node = node_load($form_state['values']['product_id']);
        if ($node->type != variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription')) {
          $node->type = variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription');
        }
        $new = FALSE;
      }

      if (!empty($node)) {
        $node->title = $form_state['values']['title'];
        $node->body = $form_state['values']['body'];
        $node->list_price = $node->sell_price = $form_state['values']['recurring'][0]['amount'];

        // Product attribute.
        $attribute = uc_attribute_load(variable_get('uc_recurring_subscription_attribute', ''), $node->nid, 'product');

        // We are going to delete and recreate the product option and adjustments.
        uc_attribute_subject_delete($attribute->aid, 'product', $node->nid);
        uc_attribute_adjustments_delete(array('nid' => $node->nid));

        // Create product attribute without the options.
        uc_attribute_subject_save($attribute, 'product', $node->nid);

        $subscription = uc_recurring_subscription_load($node->nid);
        if (empty($subscription)) {
          $subscription = new stdClass();
          $subscription->nid = $node->nid;
          $subscription->new = TRUE;
        }
        if (!empty($form_state['values']['access'][0])) {
          foreach ($form_state['values']['access'][0] as $access => $value) {
            $subscription->access[$access] = $value;
          }
        }
        $key = $subscription->new == TRUE ? NULL : 'nid';
        drupal_write_record('uc_recurring_subscription', $subscription, $key);

        // add/update the recurring features
        $features = array();
        foreach ($form_state['values']['recurring_intervals'] as $index => $value) {
          if (!is_numeric($index)) continue;

          $new_feature = FALSE;
          if (!empty($value['pfid'])) {
            $product = uc_recurring_product_fee_load($value['pfid']);
          }
          else {
            $product = new stdClass();
            $product->nid = $node->nid;
            uc_recurring_product_feature_save($product); // We need a new pfid
          }

          $product->fee_amount = $value['amount'];
          $product->initial_charge = $value['interval_value'] .' '. $value['interval_unit'];
          $product->regular_interval = $value['interval_value'] .' '. $value['interval_unit'];
          // If number intervals is negative, it means that it's unlimited intervals.
          $product->number_intervals = !empty($value['number_intervals']) ? $value['number_intervals'] : -1;

          // Create a model that hopefully will be unique.
          $model = 'sub-'. $node->nid .'-'. $product->pfid;

          // get option or create if it doesn't exist.
          $num = $value['interval_value'];
          $unit = $value['interval_unit'];
          $option_name = _uc_recurring_subscription_create_attribute_option($num, $unit);
          $option = _uc_recurring_subscription_attribute_option_by_name($attribute->aid, $option_name);

          // Set the product option.
          $option->nid = $node->nid;
          $option->oid = $option->oid;
          $option->ordering = $value['weight'];
          $option->price = $value['amount'];
          $attribute->options[$option->oid] = $option;

          // Set the default product options.
          if ($index == $form_state['values']['default_payment']) {
            $attribute->default_option = $option->oid;
            $node->model = $model;
            $node->sell_price = $product->fee_amount;
          }

          // Set the product adjustments.
          $adj = new stdClass();
          $adj->nid = $node->nid;
          $adj->combination = serialize(array($attribute->aid => $option->oid));
          $adj->model = $model;
          drupal_write_record('uc_product_adjustments', $adj);

          // If the model has changed we save the feature again.
          if ($model != $product->model) {
            $product->model = $model;
          }
          $features[$product->pfid] = $product;
        }

        // If there is only one option then don't make selection required.
        $attribute->required = (count($form_state['values']['recurring_intervals']) <= 1) ? 0 : 1;

        // Process trial information.
        $trial_amount = $form_state['values']['trial_amount'];
        $trial_value = intval($form_state['values']['trial_interval_value']);
        $trial_unit = $form_state['values']['trial_interval_unit'];
        if ($trial_value > 0) {
          $node->sell_price = $trial_amount;
        }

        // We need to calculate the option prices adjustments.
        foreach ($attribute->options as $index => $option) {
          if ($trial_value > 0) {
            $attribute->options[$index]->price = 0;
          }
          else {
            $attribute->options[$index]->price = $option->price - $node->sell_price;
          }
        }
        // Save the product attribute and options.
        uc_attribute_subject_save($attribute, 'product', $node->nid, TRUE);
        // Save the product features.
        foreach ($features as $feature) {
          if ($trial_value > 0) {
            $feature->initial_charge = $trial_value .' '. $trial_unit;
          }
          uc_recurring_product_feature_save($feature);
        }
        // Save the product/node.
        node_save($node);
      }
      break;
  }
  $form_state['redirect'] = 'admin/store/subscriptions';
}

/**
 *
 */
function uc_recurring_subscription_subscriber_list($form_state, $product_id = -1) {
  $form = array();

  $header = array(t('Product'), t('Subscribers'), '');

  $result = db_query("SELECT count(u.uid) AS num_users, n.title, n.nid
                      FROM {uc_recurring_users} u
                        LEFT JOIN {uc_order_products} p ON p.order_product_id = u.order_product_id
                        LEFT JOIN {uc_recurring_subscription} s ON s.nid = p.nid
                        LEFT JOIN {node} n ON n.nid = p.nid
                      WHERE u.status <> %d
                      GROUP BY n.nid", UC_RECURRING_FEE_STATUS_EXPIRED);

  while ($subscribers = db_fetch_object($result)) {
    if (!isset($subscribers->title)) {
      $subscribers->title = t('Unknown product');
    }
    $row = array(
      'title' => $subscribers->title,
      'count' => $subscribers->num_users,
      'ops' => isset($subscribers->nid) ? l('subscribers', 'admin/store/subscriptions/subscribers/'. $subscribers->nid) : '',
    );
    $rows[] = $row;
  }
  if (count($rows) <= 0) {
    $rows[] = array(
      array(
        'data' => t('No subscribers to any subscription products.'),
        'colspan' => count($header),
      ),
    );
  }
  $form['list'] = array(
    '#value' => theme('table', $header, $rows) . '<br />',
  );

  if (is_numeric(arg(4))) {
    $product = node_load(arg(4));
    $header = array(
      array('data' => t('Order')),
      array('data' => t('User'), 'field' => 'u.name'),
      array('data' => t('Subscription Started'), 'field' => 'ru.created'),
      array('data' => t('Next Renewal'), 'field' => 'ru.next_charge'),
      array('data' => t('Price')),
      array('data' => t('Status')),
      array('data' => t('Operations'))
    );
    $sql = "SELECT u.uid, u.name, ru.*, p.nid, p.title as product_title
            FROM {users} u
              LEFT JOIN {uc_recurring_users} ru ON ru.uid = u.uid
              LEFT JOIN {uc_order_products} p ON p.order_product_id = ru.order_product_id
              LEFT JOIN {uc_recurring_subscription} s ON s.nid = p.nid
              LEFT JOIN {node} n ON n.nid = p.nid
            WHERE ru.status <> ". UC_RECURRING_FEE_STATUS_EXPIRED ." AND n.nid = %d";

    $sql .= tablesort_sql($header);
    $result = pager_query($sql, 20, 0, NULL, $product->nid);

    while ($account = db_fetch_object($result)) {
      $user_rows[] = array(
        'order' => l($account->order_id, 'admin/store/orders/'. $account->order_id),
        'user' => l($account->name, 'user/'. $account->uid),
        'start' => format_date($account->created, 'small'),
        'next' => format_date($account->next_charge, 'small'),
        'price' => uc_price($account->fee_amount, array()),
        'status' => uc_recurring_fee_status_label($account->status),
        'ops' => implode(' | ', uc_recurring_get_fee_ops('user', $account))
      );
    }

    $form['users'] = array(
      '#value' => '<h2>'. t('Subscribers to <a href="@product_url">@product</a>', array('@product_url' => url('node/'. $product->nid), '@product' => $product->title)) .'</h2>'. 
        theme('table', $header, $user_rows) . theme('pager', NULL, 20, 0),
    );
  }

  return $form;
}

/**
 *
 */
function uc_recurring_subscription_settings_form($form_state) {
  $form = array();

  $options = array();
  $result = db_query("SELECT * FROM {uc_product_classes}");
  while ($class = db_fetch_array($result)) {
    $options[$class['pcid']] = $class['name'];
  }
  $form['uc_recurring_subscription_product_class'] = array(
    '#type' => 'select',
    '#title' => 'Product Class',
    '#description' => 'Only products from this class will be managed as subscription.',
    '#options' => $options,
    '#default_value' => variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription'),
  );

  $options = array();
  $attributes = uc_class_get_attributes(variable_get('uc_recurring_subscription_product_class', 'uc_recurring_subscription'));
  foreach($attributes as $id => $value) {
    $options[$id] = $value->name;
  }
  $form['uc_recurring_subscription_attribute'] = array(
    '#type' => 'select',
    '#title' => 'Payment Attribute',
    '#description' => 'The attribute used to provide the payment options to users.',
    '#options' => $options,
    '#default_value' => variable_get('uc_recurring_subscription_attribute', ''),
  );

  return system_settings_form($form);
}

/**
 *
 */
function theme_uc_recurring_subscription_item($form) {
  $header = array(t('Default'), t('Subscription Interval'), t('Amount'), t('Billing Intervals'), t('Order'), '');
  drupal_add_tabledrag('recurring-subscription', 'order', 'sibling', 'recurring-weight');

  foreach (element_children($form) as $key) {
    if ($key === 'default_payment') {
      continue;
    }
    // Add class to group weight fields for drag and drop.
    $form[$key]['weight']['#attributes']['class'] = 'recurring-weight';

    $row = array();
    $row[] = drupal_render($form['default_payment'][$key]);
    $row[] = drupal_render($form[$key]['interval_value']) .
             drupal_render($form[$key]['interval_unit']);
    $row[] = drupal_render($form[$key]['amount']);
    $row[] = drupal_render($form[$key]['number_intervals']) .
             drupal_render($form[$key]['unlimited']);
    $row[] = drupal_render($form[$key]['weight']);
    $row[] = drupal_render($form[$key]['operations']) . drupal_render($form[$key]['pfid']);
    $rows[] = array('data' => $row, 'class' => 'draggable tabledrag-leaf');
  }
  $output = theme('table', $header, $rows, array('id' => 'recurring-subscription'));
  //$output .= drupal_render($form);
  return $output;
}

/**
 * Add table drag effect to product commission table.
 */
function theme_uc_recurring_subscription_products($form) {
  $header = array(t('Name'), t('Assigned role(s)'), t('Trial'), t('Payment Options'), '');
  drupal_add_tabledrag('recurring-subscription-products', 'order', 'sibling', 'product-weight');

  foreach (element_children($form) as $key) {
    // Add class to group weight fields for drag and drop.
    $form[$key]['weight']['#attributes']['class'] = 'product-weight';

    $row = array();
    $row[] = drupal_render($form[$key]['title']);
    $row[] = drupal_render($form[$key]['role']);
    $row[] = drupal_render($form[$key]['trial']);
    $row[] = drupal_render($form[$key]['interval']);
    $row[] = drupal_render($form[$key]['weight']);
    $row[] = drupal_render($form[$key]['operations']);
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => l('Add new subscription', 'admin/store/subscriptions/create'),
        'colspan' => 5,
      ),
    );
  }

  $output = theme('table', $header, $rows, array('id' => 'recurring-subscription-products'));
  $output .= '<div><span class="default-option">'. t('default subscription option') .'</span></div>';
  $output .= drupal_render($form);
  return $output;
}

/**
 *
 */
function theme_uc_recurring_subscription_role_items($form) {
  $header = array(t('Granted on purchase'), t('Granted on expiration'), t('Revoked on expiration'));

  $row = array();
  $row[] = drupal_render($form['subscribe_grant']);
  $row[] = drupal_render($form['expire_grant']);
  $row[] = drupal_render($form['expire_revoke']);
  $rows[] = $row;

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 *
 */
function theme_uc_recurring_subscription_og_items($form) {
  $header = array(t('Subscribe on purchase'), t('Granted on expiration'), t('Revoked on expiration'));

  $row = array();
  $row[] = drupal_render($form['subscribe_grant_og']);
  $row[] = drupal_render($form['expire_grant_og']);
  $row[] = drupal_render($form['expire_revoke_og']);
  $rows[] = $row;

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}
