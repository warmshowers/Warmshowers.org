<?php
// Find all nodes that have comments but no record in the node_comment_statistics table.
$nids = db_query("SELECT DISTINCT c.nid FROM {comment} c LEFT JOIN {node_comment_statistics} ncs USING (nid) WHERE ncs.comment_count IS NULL")->fetchCol();
$nodes = node_load_multiple($nids);
foreach ($nodes as $node) {
  // Initialize an empty record for this node in node_comment_statistics
  comment_node_insert($node);
  // Update with the actual comment count.
  _comment_update_node_statistics($node->nid);
}
?>
