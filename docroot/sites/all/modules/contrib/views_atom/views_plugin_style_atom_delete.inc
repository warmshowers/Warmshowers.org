<?php
// $Id: views_plugin_style_atom_delete.inc,v 1.1 2010/09/21 21:36:52 crell Exp $

/**
 * @file
 * Style plugin for an Atom feed using the Tombstone specification for entry deletion.
 *
 * http://tools.ietf.org/html/draft-snell-atompub-tombstones-06
 *
 * Note that as of this writing the Tombstone spec is still a draft.
 */

class views_plugin_style_atom_delete extends views_plugin_style {

  /**
   * Attach this view to another display as a feed.
   */
  function attach_to($display_id, $path, $title) {
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

}
