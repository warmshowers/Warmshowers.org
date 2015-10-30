<?php

/**
 * @file
 * Default theme implementation to display a printable Ubercart packing slip.
 *
 * @see template_preprocess_uc_order_invoice_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print t('Packing slip'); ?></title>
  <style>
    .page-break {
      page-break-before: always;
    }
  </style>
</head>
<body>
  <?php print $content; ?>
</body>
</html>
