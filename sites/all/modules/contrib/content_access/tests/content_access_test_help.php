<?php
// $Id: content_access_test_help.php,v 1.1.4.2 2009/01/02 15:01:01 fago Exp $

/**
 * @file
 * Helper class with auxiliary functions for content access module tests
 */

class ContentAccessTestCase extends DrupalWebTestCase {

  var $test_user;
  var $admin_user;
  var $content_type_name;
  var $url_content_type_name;
  var $node1;
  var $node2;
  
  /**
   * Preparation work that is done before each test.
   * Test users, content types, nodes etc. are created.
   */
  function setUp($module = '') {
    
    if (empty($module)) {
      // Enable content access module
      parent::setUp('content_access');
    }
    else {
      // Enable content access module plus another module
      parent::setUp('content_access', $module);
      // Stop setup when module could not be enabled
      if (!module_exists($module)) {
        return;
      }
    }
        
    // Create test user with seperate role
    $this->test_user = $this->drupalCreateUser();
    
    // Create admin user
    $this->admin_user = $this->drupalCreateUser(array('access content', 'administer content types', 'grant content access', 'grant own content access', 'administer nodes', 'access administration pages'));
    $this->drupalLogin($this->admin_user);
    
    // Rebuild content access permissions
    $this->drupalPost('admin/content/node-settings/rebuild', array(), t('Rebuild permissions'));
        
    // This would be nice to have - but it does not work in the current simpletest version
    // Create test content type
    //$content_type = $this->drupalCreateContentType();
    //$this->url_content_type_name = $content_type->type;
    //$this->url_content_type_name = str_replace('_', '-', $content_type->type);
    
    // Create test content type (the old way)
    $this->content_type_name = strtolower($this->randomName(5));
    $edit = array(
      'name' => $this->content_type_name,
      'type' => $this->content_type_name,
    );
    $this->drupalPost('admin/content/types/add', $edit, t('Save content type'));
    $this->assertRaw(t('The content type %type has been added.', array('%type' => $this->content_type_name)), 'Test content type was added successfully: '. $this->content_type_name);
    $this->url_content_type_name = str_replace('_', '-', $this->content_type_name);
  }
  
  /**
   * Change access permissions for a content type
   */
  function changeAccessContentType($access_settings) {
    $this->drupalPost('admin/content/node-type/'. $this->url_content_type_name .'/access', $access_settings, t('Submit'));
    $this->assertText(t('Your changes have been saved.'), 'access rules of content type were updated successfully');
  }
  
  /**
   * Change access permissions for a content type by a given keyword (view, update or delete)
   * for the role of the user
   */
  function changeAccessContentTypeKeyword($keyword, $access = TRUE, $user = NULL) {
    if ($user === NULL) {
      $user = $this->test_user;
    }
    $roles = $user->roles;
    end($roles);
    $access_settings = array(
      $keyword .'['. key($roles) .']' => $access,
    );
    $this->changeAccessContentType($access_settings);
  }
  
  /**
   * Change the per node access setting for a content type
   */
  function changeAccessPerNode($access = TRUE) {
    $access_permissions = array(
      'per_node' => $access,
    );
    $this->changeAccessContentType($access_permissions);
  }
  
  /**
   * Change access permissions for a node by a given keyword (view, update or delete)
   */
  function changeAccessNodeKeyword($node, $keyword, $access = TRUE) {
    $user = $this->test_user;
    $roles = $user->roles;
    end($roles);
    $access_settings = array(
      $keyword .'['. key($roles) .']' => $access,
    );
    $this->changeAccessNode($node, $access_settings);
  }
  
    /**
   * Change access permission for a node
   */
  function changeAccessNode($node, $access_settings) {
    $this->drupalPost('node/'. $node->nid .'/access', $access_settings, t('Submit'));
    $this->assertText(t('Your changes have been saved.'), 'access rules of node were updated successfully');
  }

}
