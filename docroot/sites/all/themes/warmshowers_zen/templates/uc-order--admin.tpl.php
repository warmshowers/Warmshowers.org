<?php

/**
 * @file
 * Customized admin order notification template.
 */
?>

<?php $account = user_load($order->uid); ?>
<?php $account_link = l($account->fullname, 'user/' . $account->uid, array('attributes' => array('title' => t('View user profile.')))); ?>

<p>
<p>
  Comments:
  <?php echo $order_comments; ?>
</p>

Order Number: <?php echo $order_admin_link; ?><br />
Member: <?php print "{$account_link} ({$account->city}, {$account->province}, {$account->country})" ?><br/>
Member Join Date: <?php print date('Y-m-d', $account->created); ?><br/>
Member Language: <?php print $account->language . '(' . $account->languagesspoken . ')'; ?><br/>
<?php echo t('Email:'); ?> <?php echo $order_email; ?><br />
<?php echo t('Order total:'); ?> <?php echo $order_total; ?><br />
<?php echo t('Payment Method:' . $order_payment_method); ?><br/>
<?php echo t('Donortools search link:' . 'https://warmshowers.donortools.com/personas?search=' . urlencode($order_email) . '&go=true'); ?>

<p>
<?php print t('Products:'); ?><br />
<?php foreach ($products as $product): ?>
- <?php print $product->qty; ?> x <?php print $product->title; ?> - <?php print $product->total_price; ?><br />
&nbsp;&nbsp;<?php print t('SKU'); ?>: <?php print $product->model; ?><br />
    <?php if (!empty($product->data['attributes'])): ?>
    <?php foreach ($product->data['attributes'] as $attribute => $option): ?>
    &nbsp;&nbsp;<?php print t('@attribute: @options', array('@attribute' => $attribute, '@options' => implode(', ', (array)$option))); ?><br />
    <?php endforeach; ?>
    <?php endif; ?>
<br />
<?php endforeach; ?>
</p>

<p>
<?php print t('Order comments:'); ?><br />
<?php print $order_comments; ?>
</p>
