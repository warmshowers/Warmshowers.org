<?php
/**
 * @file
 * Variable Realm controller.
 */

/**
 * Controller for Language realms.
 */
class VariableMobileRealmController extends VariableRealmDefaultController {
  /**
   * Implementation of VariableRealmControllerInterface::getDefaultKey().
   */
  public function getDefaultKey() {
    return NULL;
  }
  /**
   * Implementation of VariableRealmControllerInterface::getRequestKey().
   */
  public function getRequestKey() {
    return variable_mobile_current_realm();
  }
  /**
   * Implementation of VariableRealmControllerInterface::getAllKeys().
   */
  public function getAllKeys() {
    return array(
      'mobile' => t('Mobile device'),
      'standard' => t('Standard device'),
    );
  }
}
