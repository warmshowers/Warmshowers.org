<?php
/**
 * Member actions block template
 *
 * Supported variables
 * - $user_id
 */

?>

<?php if (!$is_self):
  print flag_create_link('i_hosted_this_member', $user_id);
endif;
?>
