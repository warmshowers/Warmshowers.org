<?php
// $Id: wysiwyg.api.php,v 1.3 2009/06/09 00:18:11 sun Exp $

/**
 * hook_wysiwyg_plugin(). Return an array of editor plugins.
 *
 * @todo Completely outdated; rewrite necessary.
 *
 * Each wysiwyg editor as well as each contrib module implementing an editor
 * plugin has to return an associative array of available plugins. Each module
 * can add one or more plugins and editor buttons.
 *
 * Notes for TinyMCE:
 * A module is able to override almost all TinyMCE initialization settings.
 * However, modules should only make use of that if a plugin really needs to,
 * because customized configuration settings may clash with overrides by another
 * module. TinyMCE automatically assigns the baseURL of your plugin to the plugin
 * object. If you need to load or access additional files from your plugin
 * directory, retrieve the path via this.baseURL. tinyMCE.baseURL returns the
 * path of TinyMCE and not your module. For example:
 * @code
 * initInstance: function(inst) {
 *   tinyMCE.importCSS(inst.getDoc(), this.baseURL + '/myplugin.css');
 * },
 * @endcode
 *
 * @param string $editor
 *   An (lowercase) editor name to return plugins for.
 *
 * @return array
 *   An associative array having internal plugin names as keys, an array of
 *   plugin meta-information as values:
 *   - type: 'external' (optional); if omitted, wysiwyg editors will likely
 *     search for the plugin in their own plugins folder.
 *   - title: A human readable title of the plugin.
 *   - description: A (one-line) description of the plugin.
 *   - path: The patch to the javascript plugin.
 *   - callback: A Drupal menu callback returning the plugin UI. A plugin
 *     should return a callback *or* a path.
 *   - icon: An icon (usually 16x16 pixels) for the plugin button (optional).
 *   - ... Any other custom editor settings (optional).
 *
 * @todo Move this template into hooks.php.
 */
function hook_wysiwyg_plugin($editor) {
  switch ($editor) {
    case 'tinymce':
      return array(
        'myplugin' => array(
          'type' => 'external',
          'title' => t('My plugin title'),
          'description' => t('My plugin title'),
          // Regular callback URL for external TinyMCE plugins.
          'path' => drupal_get_path('module', 'mymodule') . '/myplugin',
          // Wysiwyg wrapper plugin AJAX callback.
          'callback' => url('myplugin/browse'),
          'icon' => drupal_get_path('module', 'mymodule') . '/myplugin/myplugin.png',
          'extended_valid_elements' => array('tag[attribute1|attribute2=default_value]'),
          // Might need to be set later on; after retrieving customized editor
          // layout.
          'theme_advanced_buttons1' => array(t('Button title (optional)') => 'myplugin'),
        ),
      );
  }
}

