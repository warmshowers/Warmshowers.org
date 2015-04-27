<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>

<feed xmlns="http://www.w3.org/2005/Atom">
  <title><?php print $view_title; ?></title>
  <link href="<?php print $link ?>" rel="self" />
  <id><?php print $id ?></id>
  <?php if ($use_push) : ?>
    <link rel="hub" href="<?php echo $hub_url; ?>" />
    <link rel="self" href="<?php echo $feed_url; /* @todo don't use both $feed_url and $link for the same thing. */?>" />
  <?php endif; ?>
  <updated><?php echo $updated; ?></updated>
  <generator uri="<?php echo $link; ?>">Drupal</generator>
  <?php print $content; ?>
</feed>
