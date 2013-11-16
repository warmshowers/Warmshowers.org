<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>

<feed xmlns="http://www.w3.org/2005/Atom" xmlns:at="http://purl.org/atompub/tombstones/1.0">
  <title><?php print $view_title; ?></title>
  <link href="<?php print $link ?>"/>
  <?php if ($use_push) : ?>
    <link rel="hub" href="<?php echo $hub_url; ?>" />
    <link rel="self" href="<?php echo $feed_url; ?>" />
  <?php endif; ?>
  <updated><?php echo $updated; ?></updated>
  <generator uri="<?php echo $link; ?>">Drupal</generator>
  <?php foreach ($items as $item) : ?>
    <at:deleted-entry ref="<?php echo $item['uri']; ?>" when="<?php echo $item['when']; ?>">
      <?php if ($item['by']) : ?>
          <at:by>
            <?php if ($item['by']['name']) :?>
              <name><?php echo $item['by']['name']; ?></name>
            <?php endif; ?>
            <?php if ($item['by']['email']) :?>
              <email><?php echo $item['by']['email']; ?></email>
            <?php endif; ?>
          </at:by>
      <?php endif; ?>
      <?php if ($item['comment']) : ?>
        <at:comment><?php echo $item['comment']; ?></at:comment>
      <?php endif; ?>
    </at:deleted-entry>
  <?php endforeach; ?>
</feed>
