<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">
<head>
  <?php print $head ?>
  <title><?php print $head_title ?></title>
  <meta http-equiv="Content-Style-Type" content="text/css" />
</head>

<body <?php print theme("onload_attribute"); ?>>

<a href="<?php print url($_GET['q'], array('query' => NULL, 'fragment' => 'nav', 'absolute' => TRUE)); ?>"><?php print t('skip to navigation');?></a>
<?php if ($title != ""): ?>
<h2 class="content-title"><?php print $title ?></h2>
<?php endif; ?>  
<?php if ($help != ""): ?>
<p id="help"><?php print $help ?></p>
<?php endif; ?> 
<?php if ($messages != ""): ?>
<div id="message"><?php print $messages ?></div>
<?php endif; ?>
<?php print $content ?>
<?php if ($tabs != ""): ?>
<?php print $tabs ?>
<?php endif; ?>
<a name="nav"></a>
<?php print $left . $right; ?> 
<?php if ($footer_message) : ?>
<?php print $footer;?>
<?php endif; ?>
<?php print $closure;?>
</body>
</html>

