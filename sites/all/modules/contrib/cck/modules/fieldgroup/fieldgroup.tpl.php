<?php
// $Id: fieldgroup.tpl.php 505 2009-05-24 18:55:09Z rfay $

/**
 * @file fieldgroup.tpl.php
 * Default theme implementation to display the a 'simple-styled' fieldgroup.
 *
 * Available variables:
 * - $group_name - The group name
 * - $group_name_css - The css-compatible group name.
 * - $label - The group label
 * - $description - The group description
 * - $content - The group content
 *
 * @see template_preprocess_fieldgroup_simple()
 */
?>
<?php if ($content) : ?>
<div class="fieldgroup <?php print $group_name_css; ?>">

  <?php if ($label): ?>
    <h2><?php print $label; ?></h2>

    <?php if ($description): ?>
      <div class="description"><?php print $description; ?></div>
    <?php endif; ?>

  <?php endif; ?>

  <div class="content"><?php print $content; ?></div>

</div>
<?php endif; ?>
