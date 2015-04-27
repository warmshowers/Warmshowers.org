<?php

/**
 * Create the clone form.
 * 
 * @param form        the form
 * @param form_state  reference to the state of the form
 * @param menu        the menu object received from hook_load()
 * @see               menu_clone_load()
 * 
 * TODO: See if we can improve the $form['edit_menu']['edit_tree'] field. Use
 * something else instead of 'markup'.
 */
function menu_clone_clone_form($form, &$form_state, $menu) {
  $form['menu_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu name'),
    '#default_value' => $menu['menu_name'],
    '#maxlength' => MENU_MAX_MENU_NAME_LENGTH_UI - drupal_strlen('menu-'),
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
    if (module_exists('i18n_menu')) {
      $form['edit_menu']['menu_lang'] = array(
        '#type' => 'select',
        '#title' => t('Change language'),
        '#description' => t('You can globally change the language of <em>all</em> the available menu items.'),
        '#options' => array_merge(array('' => t('No change')), i18n_language_list()),
      );
    }
    // Append the tree form to the fieldset.
    $form['edit_menu']['edit_tree'] = array(
      '#markup' => '<div class="form-item"><label>' . t('Edit menu') . ':</label><div class="description">' . t('You can reorder the menu items and adjust basic settings here. Drag unwanted items below the <em>Ignore</em> row, or drag the <em>Ignore</em> row itself.') . '</div></div>',
    );
    module_load_include('inc', 'menu', 'menu.admin');
    // In D6, the _menu_overview_tree_form() function also added the 'expanded'
    // field. In D7 it has been removed from that function. If we want it back
    // we will have to create a _menu_clone_overview_tree_form() and add an
    // additional checkbox after $form[$mlid]['hidden'].
    $form['edit_menu']['tree'] = _menu_overview_tree_form($menu['tree']);
    _menu_clone_add_ignore_row($form['edit_menu']['tree']);
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new menu'),
  );
  return $form;
}
/**
 * Add the ignore row to the menu tree.
 *
 * @param form        the form to which to add the ignore row.
 */
function _menu_clone_add_ignore_row(&$form) {
  // Add ignore row with fake mlid and plid fields to make the tabledrag work
  // properly.
  $form['ignore']['title'] = array(
    '#markup' => '<strong>' . t("Ignore: the items below this row won't be cloned.") . '</strong>',
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
  // SELECT menu_name FROM {menu_custom} WHERE menu_name = '%s'
  $result = db_select('menu_custom', 'm')
    ->fields('m', array('menu_name'))
    ->condition('menu_name', 'menu-' . $menu['menu_name'], '=')
    ->execute()
    ->fetchField();
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
  // Begin code taken from menu_edit_menu_submit().
  $menu = $form_state['values'];
  if (isset($menu['edit_menu']['menu_lang']) && !empty($menu['edit_menu']['menu_lang'])) {
    $menu['i18n_mode'] = I18N_MODE_MULTIPLE;
  }
  $path = 'admin/structure/menu/manage/';
  // Add 'menu-' to the menu name to help avoid name-space conflicts.
  $menu['menu_name'] = 'menu-' . $menu['menu_name'];
  $link['link_title'] = $menu['title'];
  $link['link_path'] = $path . $menu['menu_name'];
  $link['router_path'] = $path . '%';
  $link['module'] = 'menu';
  $link['plid'] = db_query("SELECT mlid FROM {menu_links} WHERE link_path = :link AND module = :module", array(
    ':link' => 'admin/structure/menu',
    ':module' => 'system'
  ))
  ->fetchField();

  menu_link_save($link);
  menu_save($menu);
  // End code taken from menu_edit_menu_submit().

  // Add the menu items to the newly created menu.
  if (!empty($form_state['input']['edit_menu']['tree'])) {
    // When dealing with saving menu items, the order in which these items are
    // saved is critical. If a changed child item is saved before its parent,
    // the child item could be saved with an invalid path past its immediate
    // parent. To prevent this, save items in the form in the same order they
    // are sent by $_POST, ensuring parents are saved first, then their children.
    // See http://drupal.org/node/181126#comment-632270
    $order = array_flip(array_keys($form_state['input']['edit_menu']['tree'])); // Get the $_POST order.
    $form['edit_menu']['tree'] = array_merge($order, $form['edit_menu']['tree']); // Update our original form with the new order.

    //$order = array_flip(array_keys($form['#post']['edit_menu']['tree'])); // Get the $_POST order.
    //$form['edit_menu']['tree'] = array_merge($order, $form['edit_menu']['tree']); // Update our original form with the new order.

    $menu_items = array();

    $fields = array('weight', 'plid');
    foreach (element_children($form['edit_menu']['tree']) as $mlid) {
      if (isset($form['edit_menu']['tree'][$mlid]['#item'])) {
        $element = $form['edit_menu']['tree'][$mlid];
        // Update any fields that might have changed in this menu item.
        foreach ($fields as $field) {
          $element['#item'][$field] = $element[$field]['#value'];
        }
        if (isset($menu['edit_menu']['menu_lang']) && !empty($menu['edit_menu']['menu_lang'])) {
          $element['#item']['language'] = $menu['edit_menu']['menu_lang'];
        }
        // Hidden is a special case, the value needs to be reversed.
        // Convert to integer rather than boolean due to PDO cast to string.
        $element['#item']['hidden'] = $element['hidden']['#value'] ? 0 : 1;
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
function theme_menu_clone_clone_form($variables) {
  $form = $variables['form'];

  // Theme only when necessary.
  if (!empty($form['edit_menu'])) {
    drupal_add_tabledrag('menu-clone', 'match', 'parent', 'menu-clone-plid', 'menu-clone-plid', 'menu-clone-mlid', TRUE, MENU_MAX_DEPTH - 1);
    drupal_add_tabledrag('menu-clone', 'order', 'sibling', 'menu-clone-weight');
    drupal_add_css(drupal_get_path('module', 'menu_clone') . '/css/menu_clone.css');

    $header = array(
      t('Menu item'),
      array('data' => t('Enabled'), 'class' => array('checkbox')),
      t('Weight'),
    );

    $rows = array();
    foreach (element_children($form['edit_menu']['tree']) as $mlid) {
      if (isset($form['edit_menu']['tree'][$mlid]['hidden'])) {
        $element = &$form['edit_menu']['tree'][$mlid];

        // Add special classes to be used for tabledrag.js.
        $element['plid']['#attributes']['class'] = array('menu-clone-plid');
        $element['mlid']['#attributes']['class'] = array('menu-clone-mlid');
        $element['weight']['#attributes']['class'] = array('menu-clone-weight');

        // Change the parent field to a hidden. This allows any value but hides
        // the field.
        $element['plid']['#type'] = 'hidden';

        $row = array();
        $row[] = theme('indentation', array('size' => $element['#item']['depth'] - 1)) . drupal_render($element['title']);
        $row[] = array('data' => drupal_render($element['hidden']), 'class' => array('checkbox'));
        $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);

        $row = array_merge(array('data' => $row), $element['#attributes']);
        $row['class'][] = 'draggable';
        $rows[] = $row;
      }
    }
    // Add the Ignore row. The ignore row must be draggable as well to avoid
    // situations where the ignore row is at the top of the table and no items
    // can be dragged above it anymore.
    // A draggable ignore row does also add the ability of simply dragging the
    // ignore row to a spot in the menu, instead of having to dragg all the
    // unnecessary elements below it.
    $form['edit_menu']['tree']['ignore']['plid']['#attributes']['class'] = array('menu-clone-plid');
    $form['edit_menu']['tree']['ignore']['mlid']['#attributes']['class'] = array('menu-clone-mlid');
    $form['edit_menu']['tree']['ignore']['weight']['#attributes']['class'] = array('menu-clone-weight');
    $row = array();
    $row[] = array('data' => drupal_render($form['edit_menu']['tree']['ignore']['title']), 'colspan' => '2');
    $row[] = drupal_render($form['edit_menu']['tree']['ignore']['check']) . drupal_render($form['edit_menu']['tree']['ignore']['weight']) . drupal_render($form['edit_menu']['tree']['ignore']['mlid']) . drupal_render($form['edit_menu']['tree']['ignore']['plid']);
    $rows[] = array('data' => $row, 'class' => array('draggable', 'ignore'));

    // Make sure the draggable table is rendered inside the fieldset. Having
    // trouble with this? Check the possibilities here
    // http://drupal.org/node/82916 and here http://drupal.org/node/98065. The
    // latter worked for me, others did not.
    $form['edit_menu']['tree']['#children'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'menu-clone')));
  }
  $output = drupal_render_children($form);

  return $output;
}
