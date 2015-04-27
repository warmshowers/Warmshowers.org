<?php

/**
 * @file
 * Default theme implementation to display a printable Ubercart invoice.
 *
 * @see template_preprocess_uc_order_invoice_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <style type="text/css">
    .buttons {
      text-align: right;
      margin: 0 1em 1em 0;
    }
  </style>
  <style type="text/css" media="print">
    .buttons {
      display: none;
    }
  </style>
</head>
<body>
  <div class="buttons">
    <input type="button" value="<?php print t('Print invoice'); ?>" onclick="window.print();" />
  </div>

  <?php print $content; ?>
</body>
</html>
