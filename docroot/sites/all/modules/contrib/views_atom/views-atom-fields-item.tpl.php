<?php // @TODO, document this file ?>

<entry>
  <title><?php echo $atom_title; ?></title>
  <link href="<?php echo $atom_url; ?>" />
  <id><?php echo $atom_uuid; ?></id>
  <updated><?php echo $atom_updated; ?></updated>
  <published><?php echo $atom_published; ?></published>
  <author>
    <name><?php print $atom_author; ?></name>
    <email><?php print $atom_author_email; ?></email>
    <uri><?php print $atom_author_url; ?></uri>
  </author>
  <content type="<?php echo $entry_type; ?>" xml:lang="<?php echo $language; ?>">
    <?php print $content; ?>
  </content>
</entry>
