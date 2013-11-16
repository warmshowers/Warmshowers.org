<?php

/**
 * Create the clone form.
 * 
 * @param form_state  reference to the state of the form
 * @param menu        the menu object received from hook_load()
 * @see               menu_clone_load()
 * 
 * TODO: See if we can improve the $form['edit_menu']['edit_tree'] field. Use
 * something else instead of 'markup'.
 */
function menu_clone_clone_form(&$form_state, $menu) {
  $form['menu_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu name'),
    '#default_value' => $menu['menu_name'],
    '#maxlength' => MENU_MAX_MENU_NAME_LENGTH_UI - strlen('menu-'),
    '#description' => t('The machine-readable name of this menu. This text will be used for constructing the URL of the <em>menu overview</em> page for this menu. This name must contain only lowercase letters, numbers, and hyphens, and must be unique.'),
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $menu['title'],
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $menu['description'],
  );
  if (!empty($menu['tree'])) {
    // Add language support if available.
    $form['edit_menu'] = array(
      '#type' => 'fieldset',
      '#title' => t('Customise menu'),
      '#description' => t('Before actually cloning the menu, you can customise it first.'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );
    if (module_exists('i18nmenu')) {
      $form['edit_menu']['menu_lang'] = array(
        '#type' => 'select',
        '#title' => t('Change language'),
        '#description' => t('You can globally change the language of <em>all</em> the available menu items.'),
        '#options' => array_merge(array('' => t('No change')), i18n_language_list()),
      );
    }
    // Append the tree form to the fieldset.
    $form['edit_menu']['edit_tree'] = array(
      '#type' => 'markup',
      '#value' => '<div class="form-item"><label>' .t('Edit menu') .':</label><div class="description">' . t('You can reorder the menu items and adjust basic settings here. Drag unwanted items below the <em>Ignore</em> row, or drag the <em>Ignore</em> row itself.') . '</div></div>',
    );
    $form['edit_menu']['tree'] = _menu_clone_tree_form($menu['tree']);
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new menu'),
  );
  return $form;
}
/**
 * Recursive helper function for menu_clone_clone_form(). Renders out the menu
 * tree as part of the main form.
 * This function is copied from the Drupal core Menu module and extended with an
 * ignore row that will allow exclusion of menu items in the cloning process.
 * 
 * @param tree the tree data
 * @see        menu_clone_clone_form()
 * @see        menu_clone_load()
 * @see        Drupal core menu.admin.inc _menu_overview_tree_form()
 */
function _menu_clone_tree_form($tree) {
  // Begin code taken from _menu_overview_tree_form().
  static $form = array('#tree' => TRUE);
  foreach ($tree as $data) {
    $title = '';
    $item = $data['link'];
    // Don't show callbacks; these have $item['hidden'] < 0.
    if ($item && $item['hidden'] >= 0) {
      $mlid = 'mlid:'. $item['mlid'];
      $form[$mlid]['#item'] = $item;
      $form[$mlid]['#attributes'] = $item['hidden'] ? array('class' => 'menu-disabled') : array('class' => 'menu-enabled');
      $form[$mlid]['title']['#value'] = l($item['title'], $item['href'], $item['localized_options']) . ($item['hidden'] ? ' ('. t('disabled') .')' : '');
      $form[$mlid]['hidden'] = array(
        '#type' => 'checkbox',
        '#default_value' => !$item['hidden'],
      );
      $form[$mlid]['expanded'] = array(
        '#type' => 'checkbox',
        '#default_value' => $item['expanded'],
      );
      $form[$mlid]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => isset($form_state[$mlid]['weight']) ? $form_state[$mlid]['weight'] : $item['weight'],
      );
      $form[$mlid]['mlid'] = array(
        '#type' => 'hidden',
        '#value' => $item['mlid'],
      );
      $form[$mlid]['plid'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($form_state[$mlid]['plid']) ? $form_state[$mlid]['plid'] : $item['plid'],
        '#size' => 6,
      );
    }

    if ($data['below']) {
      _menu_clone_tree_form($data['below']);
    }
  }
  // End code taken from _menu_overview_tree_form().
  
  // Add ignore row with fake mlid and plid fields to make the tabledrag work
  // properly.
  $form['ignore']['title'] = array(
    '#type' => 'markup',
    '#value' => '<strong>'.t("Ignore: the items below this row won't be cloned.").'</strong>',
  );
  $form['ignore']['check'] = array(
    '#type' => 'hidden',
    '#value' => 'ignore',
  );
  $form['ignore']['weight'] = array(
    '#type' => 'weight',
    '#delta' => 50,
    '#default_value' => 50,
  );
  $form['ignore']['mlid'] = array(
    '#type' => 'hidden',
    '#value' => 0,
  );
  $form['ignore']['plid'] = array(
    '#type' => 'textfield',
    '#default_value' => 0,
    '#size' => 6,
  );
  
  return $form;
}

/**
 * Validation function for menu_clone_cone_form().
 * 
 * @param form       the form object
 * @param form_state reference to the form state
 * @see              menu_clone_cone_form()
 */
function menu_clone_clone_form_validate($form, &$form_state) {
  $menu = $form_state['values'];
  if (preg_match('/[^a-z0-9-]/', $menu['menu_name'])) {
    form_set_error('menu_name', t('The menu name may only consist of lowercase letters, numbers, and hyphens.'));
  }
  $sql = "SELECT menu_name FROM {menu_custom} WHERE menu_name = '%s'";
  $result = db_result(db_query($sql, 'menu-' . $menu['menu_name']));
  if (!empty($result)) {
    form_set_error('menu_name', t("The machine-readable name '@menu_name' must be unique. A menu named '@menu_name' already exists.", array('@menu_name' => $menu['menu_name'])));
  }
}

/**
 * Submit function for menu_clone_cone_form(). Parts were taken from the Drupal
 * core Menu module.
 * 
 * @param form       the form object
 * @param form_state reference to the form state
 * @see              menu_clone_cone_form()
 * @see              Drupal core menu.admin.inc menu_overview_form_submit()
 * @see              Drupal core menu.admin.inc menu_edit_menu_submit()
 */
function menu_clone_clone_form_submit($form, &$form_state) {
  // Create new menu.
  // Begin code taken from _menu_overview_tree_form().
  $menu = $form_state['values'];
  $path = 'admin/build/menu-customize/';
  // Add 'menu-' to the menu name to help avoid name-space conflicts.
  $menu['menu_name'] = 'menu-'. $menu['menu_name'];
  $link['link_title'] = $menu['title'];
  $link['link_path'] = $path . $menu['menu_name'];
  $link['router_path'] = $path .'%';
  $link['module'] = 'menu';
  $link['plid'] = db_result(db_query("SELECT mlid FROM {menu_links} WHERE link_path = '%s' AND module = '%s'", 'admin/build/menu', 'system'));
  menu_link_save($link);
  db_query("INSERT INTO {menu_custom} (menu_name, title, description) VALUES ('%s', '%s', '%s')", $menu['menu_name'], $menu['title'], $menu['description']);
  // End code taken from _menu_overview_tree_form().

  // Add the menu items to the newly created menu.
  if (!empty($form['#post']['edit_menu']['tree'])) {
    // When dealing with saving menu items, the order in which these items are
    // saved is critical. If a changed child item is saved before its parent,
    // the child item could be saved with an invalid path past its immediate
    // parent. To prevent this, save items in the form in the same order they
    // are sent by $_POST, ensuring parents are saved first, then their children.
    // See http://drupal.org/node/181126#comment-632270
    $order = array_flip(array_keys($form['#post']['edit_menu']['tree'])); // Get the $_POST order.
    $form['edit_menu']['tree'] = array_merge($order, $form['edit_menu']['tree']); // Update our original form with the new order.
    $menu_items = array();

    $fields = array('expanded', 'weight', 'plid');
    foreach (element_children($form['edit_menu']['tree']) as $mlid) {
      if (isset($form['edit_menu']['tree'][$mlid]['#item'])) {
        $element = $form['edit_menu']['tree'][$mlid];
        // Update any fields that might have changed in this menu item.
        foreach ($fields as $field) {
          $element['#item'][$field] = $element[$field]['#value'];
        }
        if (isset($menu['edit_menu']['menu_lang']) && !empty($menu['edit_menu']['menu_lang'])) {
          $element['#item']['options']['langcode'] = $menu['edit_menu']['menu_lang'];
        }
        // Hidden is a special case, the value needs to be reversed.
        $element['#item']['hidden'] = !$element['hidden']['#value'];
        // Make sure we create a new item and not overwrite an existing one and
        // attach it to our new menu.
        $element['#item']['menu_name'] = $menu['menu_name'];
        $element['#item']['mlid'] = NULL;
        $element['#item']['module'] = 'menu';
        $menu_items[$mlid] = $element['#item'];
      }
      else if (isset($form['edit_menu']['tree'][$mlid]['check'])) {
        // Check if we have reached the ignore field so we can stop.
        if ($form['edit_menu']['tree'][$mlid]['check']['#value'] == 'ignore') {
          break;
        }
      }
    }

    // Save the items being cloned to the database.
    $depth_mlids = array(); // Keep track of the new mlids per depth and parent.
    foreach ($menu_items as $item) {
      if (intval($item['depth']) > 1 && !isset($depth_mlids[$item['depth']][$item['plid']])) {
        // Item with a parent, so keep track of the original parent and the
        // new mlid belonging to it.
        $item['plid'] = $depth_mlids[$item['depth']][$item['plid']] = $new_mlid;
      }
      else if (intval($item['depth']) > 1) {
        // Item with the same parent as we have aleady processed before.
        $item['plid'] = $depth_mlids[$item['depth']][$item['plid']];
      }
      $item['customized'] = 1;

      $new_mlid = menu_link_save($item);
    }
  }

  $form_state['redirect'] = $path . $menu['menu_name'];
}

/**
 * Theme the menu tree of the clone form into a table and display it inside its
 * proper fieldset.
 * 
 * @param form  The form array to be themed
 * @see         menu_clone_clone_form()
 * @ingroup     themeable
 */
function theme_menu_clone_clone_form($form) {
  // Theme only when necessary.
  if (!empty($form['edit_menu'])) {
    drupal_add_tabledrag('menu-clone', 'match', 'parent', 'menu-clone-plid', 'menu-clone-plid', 'menu-clone-mlid', TRUE, MENU_MAX_DEPTH - 1);
    drupal_add_tabledrag('menu-clone', 'order', 'sibling', 'menu-clone-weight');
    drupal_add_css(drupal_get_path('module', 'menu_clone').'/css/menu_clone.css');

    $header = array(
      t('Menu item'),
      array('data' => t('Enabled'), 'class' => 'checkbox'),
      array('data' => t('Expanded'), 'class' => 'checkbox'),
      t('Weight'),
    );
  
    $rows = array();
    foreach (element_children($form['edit_menu']['tree']) as $mlid) {
      if (isset($form['edit_menu']['tree'][$mlid]['hidden'])) {
        $element = &$form['edit_menu']['tree'][$mlid];
  
        // Add special classes to be used for tabledrag.js.
        $element['plid']['#attributes']['class'] = 'menu-clone-plid';
        $element['mlid']['#attributes']['class'] = 'menu-clone-mlid';
        $element['weight']['#attributes']['class'] = 'menu-clone-weight';
  
        // Change the parent field to a hidden. This allows any value but hides
        // the field.
        $element['plid']['#type'] = 'hidden';
  
        $row = array();
        $row[] = theme('indentation', $element['#item']['depth'] - 1) . drupal_render($element['title']);
        $row[] = array('data' => drupal_render($element['hidden']), 'class' => 'checkbox');
        $row[] = array('data' => drupal_render($element['expanded']), 'class' => 'checkbox');
        $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
  
        $row = array_merge(array('data' => $row), $element['#attributes']);
        $row['class'] = !empty($row['class']) ? $row['class'] .' draggable' : 'draggable';
        $rows[] = $row;
      }
    }
    // Add the Ignore row. The ignore row must be draggable as well to avoid
    // situations where the ignore row is at the top of the table and no items
    // can be dragged above it anymore.
    // A draggable ignore row does also add the ability of simply dragging the
    // ignore row to a spot in the menu, instead of having to dragg all the
    // unnecessary elements below it.
    $form['edit_menu']['tree']['ignore']['plid']['#attributes']['class'] = 'menu-clone-plid';
    $form['edit_menu']['tree']['ignore']['mlid']['#attributes']['class'] = 'menu-clone-mlid';
    $form['edit_menu']['tree']['ignore']['weight']['#attributes']['class'] = 'menu-clone-weight';
    $row = array();
    $row[] = array('data' => drupal_render($form['edit_menu']['tree']['ignore']['title']), 'colspan' => '3');
    $row[] = drupal_render($form['edit_menu']['tree']['ignore']['check']) . drupal_render($form['edit_menu']['tree']['ignore']['weight']) . drupal_render($form['edit_menu']['tree']['ignore']['mlid']) . drupal_render($form['edit_menu']['tree']['ignore']['plid']);
    $rows[] = array('data' => $row, 'class' => 'draggable ignore');
    
    // Make sure the draggable table is rendered inside the fieldset. Having
    // trouble with this? Check the possibilities here
    // http://drupal.org/node/82916 and here http://drupal.org/node/98065. The
    // latter worked for me, others did not.
    $form['edit_menu']['tree']['#children'] = theme('table', $header, $rows, array('id' => 'menu-clone'));
  }
  $output = drupal_render($form);

  return $output;
}
