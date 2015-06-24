<?php
/**
 * @file
 * Returns the HTML for the member welcome block.
 *
 * Available variables:
 *   - $menu: An array of profile items. Use render() to print them.
 */
?>
<div class='name'><?php print theme('username', array('account' => $account)); ?></div>
<?php print theme('linkbuttons', array('links' => $wsuser_member_welcome_block_menu_links)); ?>
