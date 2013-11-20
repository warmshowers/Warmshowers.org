<?php
/**
 * Template for member welcome block.
  */
?>
<div id='welcome-text'><?php print t('Welcome'); ?></div>
<div class='name'><?php print theme('username', $user) ?></div>
<?php print theme('linkbuttons', $wsuser_member_welcome_block_menu_links); ?>
