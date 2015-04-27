<?php

/**
 * @file
 * Utility classes for catalog module.
 */

/**
 * Data structure to mimic Drupal's menu system.
 */
class UcTreeNode {
  public $tid = 0;
  public $name = 'Catalog';
  public $children = array();
  public $depth = -1;
  public $sequence = 0;

  /**
   * Constructor.
   */
  function __construct($term = NULL) {
    if ($term) {
      $this->tid = $term->tid;
      $this->name = $term->name;
      $this->depth = $term->depth;
      $this->sequence = $term->sequence;
    }
  }

  /**
   * Determines if new child is an immediate descendant or not.
   *
   * This function is completely dependent on the structure of the array
   * returned by taxonomy_get_tree(). Each element in the array knows its
   * depth in the tree and the array is a preorder iteration of the logical
   * tree structure. Therefore, if the parameter is more than one level
   * deeper than $this, it should be passed to the last child of $this.
   */
  function add_child(&$child) {
    if ($child->depth - $this->depth == 1) {
      $this->children[] = $child;
    }
    else {
      $last_child =&$this->children[count($this->children)-1];
      $last_child->add_child($child);
    }
  }
}
