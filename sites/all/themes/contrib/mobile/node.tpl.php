<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>"><?php print $title ?></a></h2>
<?php endif; ?>

<?php print $content ?>
<?php if ($signature): ?>
  <div class="user-signature clear-block"><?php print $signature ?></div>
<?php endif; ?>
<?php print $submitted ?>

<?php if ($links): ?>
  <?php print $links ?>
<?php endif; ?>