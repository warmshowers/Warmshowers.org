<?php

/**
 * @file
 *   API documentation for Localize updater module.
 */

/**
 * Alter the list of project to be updated by l10n update.
 *
 * l10n_update uses the same list of projects as update module. Using this hook
 * the list can be altered.
 *
 * @param array $projects
 *   Array of projects.
 */
function hook_l10n_update_projects_alter(&$projects) {
  // The $projects array contains the project data produced by
  // update_get_projects(). A number of the array elements are described in
  // the documentation of hook_update_projects_alter().

  // In the .info file of a project a localization server can be specified.
  // Using this hook the localization server specification can be altered or
  // added. The 'l10n path' element is optional but can be specified to override
  // the translation download path specified in the 10n_server.xml file.
  $projects['existing_example_project'] = array(
    'info' => array(
      'l10n path' => 'http://example.com/files/translations/%core/%project/%project-%release.%language.po',
    ),
  );

  // With this hook it is also possible to add a new project wich does not
  // exist as a real module or theme project but is treated by the localization
  // update module as one. The below data is the minumum to be specified.
  // As in the previous example the 'l10n path' element is optional.
  $projects['new_example_project'] = array(
    'project_type'  => 'module',
    'name' => 'new_example_project',
    'info' => array(
      'name' => 'New example project',
      'version' => '7.x-1.5',
      'core' => '7.x',
      'l10n path' => 'http://example.com/files/translations/%core/%project/%project-%release.%language.po',
    ),
  );
}
