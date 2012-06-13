<?php
/**
 * Argument handler for taxonomy terms with depth.
 *
 * This handler is actually part of the node table and has some restrictions,
 * because it uses a subquery to find nodes with
 */
class i18nviews_handler_argument_term_node_tid_depth extends views_handler_argument_term_node_tid_depth {
  function title() {
    $term = taxonomy_get_term($this->argument);
    if (!empty($term)) {
      // TODO check if translation is enabled for this vocab.
      return check_plain(tt('taxonomy:term:'. $term->tid .':name', $term->name));
    }
    // TODO review text
    return t('No name');
  }
}
