<?php

/**
 * @file
 * Contains workflow_admin_ui\includes\Entity\EntityWorkflowUIController.
 */

class EntityWorkflowUIController extends EntityDefaultUIController {
  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Workflow is now an exportable entity. But the 'Import' menu item is
    // still broken. We show it, but it requires #1967794 before it works.
    // unset($items['admin/config/workflow/workflow/import']);

    // Set this on the object so classes that extend hook_menu() can use it.
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';
    $entityType = $this->entityInfo['entity class'];

    // @todo: Allow modules to insert their own action links to the 'workflow',
    // $workflow_operations = module_invoke_all('workflow_operations', 'workflow', NULL);

    $item = array(
      'file path' => isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']),
      'access arguments' => array('administer workflow'),
      'type' => MENU_LOCAL_TASK,
    );

    $items[$this->path . '/manage/' . $wildcard . '/states'] = $item + array(
      'file' => 'workflow_admin_ui/workflow_admin_ui.page.states.inc',
      'title' => 'States',
      'weight' => '1',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('workflow_admin_ui_states_form', $id_count + 1, $id_count + 2),
    );

    $items[$this->path . '/manage/' . $wildcard . '/transitions'] = $item + array(
      'file' => 'workflow_admin_ui/workflow_admin_ui.page.transitions.inc',
      'title' => 'Transitions',
      'weight' => '2',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('workflow_admin_ui_transitions_form', $id_count + 1, $id_count + 2),
    );

    $items[$this->path . '/manage/' . $wildcard . '/labels'] = $item + array(
      'file' => 'workflow_admin_ui/workflow_admin_ui.page.labels.inc',
      'title' => 'Labels',
      'weight' => '3',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('workflow_admin_ui_labels_form', $id_count + 1, $id_count + 2),
    );

    $items[$this->path . '/manage/' . $wildcard . '/permissions'] = $item + array(
      'file' => 'workflow_admin_ui/workflow_admin_ui.page.permissions.inc',
      'title' => 'Permission summary',
      'weight' => '4',
      'page callback' => 'workflow_admin_ui_view_permissions_form',
      'page arguments' => array($id_count + 1, $id_count + 2),
      // @todo: convert to drupal_get_form('workflow_admin_ui_view_permissions_form');
      // 'page callback' => 'drupal_get_form',
      // 'page arguments' => array('workflow_admin_ui_view_permissions_form', $id_count + 1, $id_count + 2),
    );

    return $items;
  }

  protected function operationCount() {
    // Add more then enough colspan.
    return parent::operationCount() + 8;
  }

/*
  public function operationForm($form, &$form_state, $entity, $op) {}
 */

  public function overviewForm($form, &$form_state) {
    // Add table and pager.
    $form = parent::overviewForm($form, $form_state);

    // Allow modules to insert their own action links to the 'table', like cleanup module.
    $top_actions = module_invoke_all('workflow_operations', 'top_actions', NULL);

    // Allow modules to insert their own workflow operations.
    foreach ($form['table']['#rows'] as &$row) {
      $url = $row[0]['data']['#url'];
      $workflow = $url['options']['entity'];
      foreach ($actions = module_invoke_all('workflow_operations', 'workflow', $workflow) as $action) {
        $action['attributes'] = isset($action['attributes']) ? $action['attributes'] : array();
        $row[] = l(strtolower($action['title']), $action['href'], $action['attributes']);
      }
    }

    // @todo: add these top actions next to the core 'Add workflow' action.
    $top_actions_args = array(
      'links' => $top_actions,
      'attributes' => array('class' => array('inline', 'action-links')),
    );

    $form['action-links'] = array(
      '#type' => 'markup',
      '#markup' => theme('links', $top_actions_args),
      '#weight' => -1,
    );

    if (module_exists('workflownode')) {
      // Append the type_map form, changing the form by reference.
      // The 'type_map' form is only valid for Workflow Node API.
      module_load_include('inc', 'workflow_admin_ui', 'workflow_admin_ui.page.type_map');
      workflow_admin_ui_type_map_form($form);
    }

    // Add a submit button. The submit functions are added in the sub-forms.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 100,
    );

    return $form;
  }
  /*
   * Avoids the 'Delete' action if the Workflow is used somewhere.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    // Avoid the 'delete' operation if the Workflow is used somewhere.
    $status = $entity->status;

    // @see parent::overviewTableRow() how to determine a deletable entity.
    if (!entity_has_status($this->entityType, $entity, ENTITY_IN_CODE) && !$entity->isDeletable())  {
      // Set to a state that does not allow deleting, but allows other actions.
      $entity->status = ENTITY_IN_CODE;
    }
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    // Just to be sure: reset status.
    $entity->status = $status;

    return $row;
  }

  /**
   * Overrides the 'revert' action, to not delete the workflows.
   *
   * @see https://www.drupal.org/node/2051079
   * @see https://www.drupal.org/node/1043634
   */
  public function applyOperation($op, $entity) {
    $label = entity_label($this->entityType, $entity);
    $vars = array('%entity' => $this->entityInfo['label'], '%label' => $label);
    $id = entity_id($this->entityType, $entity);
    $edit_link = l(t('edit'), $this->path . '/manage/' . $id . '/edit');

    switch ($op) {
      case 'revert':
        // Do not delete the workflow, but recreate features_get_default($entity_type, $module);
        // entity_delete($this->entityType, $id);
        $workflow = $entity;
        $entity_type = $this->entityType;
        $funcname = $workflow->module . '_default_' . $this->entityType;
        $defaults = $funcname();
        // No defaults, no processing.
        if (empty($defaults)) {
          return;
        }

        foreach ($defaults as $name => $entity) {
          $existing[$name] = workflow_load($name);
          // If we got an existing entity with the same name, we reuse its entity id.
          if (isset($existing[$name])) {
            // Set the original as later reference.
            $entity->original = $existing[$name];

            // As we got an ID, the entity is not new.
            $entity->wid = $entity->original->wid;
            unset($entity->is_new);

            // Update the status to be in code.
            // $entity->status |= ENTITY_IN_CODE;
            $entity->status = ENTITY_IN_CODE;

            // We mark it for being in revert mode.
            $entity->is_reverted = TRUE;
            entity_save($entity_type, $entity);
            unset($entity->is_reverted);
          }
          // The rest of the defaults is handled by default implementation.
          // @see entity_defaults_rebuild()
        }
        watchdog($this->entityType, 'Reverted %entity %label to the defaults.', $vars, WATCHDOG_NOTICE, $edit_link);
        return t('Reverted %entity %label to the defaults.', $vars);

      case 'delete':
      case 'import':
      default:
        return parent::applyOperation($op, $entity);
    }
  }
}
