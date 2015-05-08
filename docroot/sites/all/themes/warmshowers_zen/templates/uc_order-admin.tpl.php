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
<?php echo t('Products:'); ?><br />
<?php
$context = array(
  'revision' => 'themed',
  'type' => 'order_product',
  'subject' => array(
    'order' => $order,
  ),
);
foreach ($products as $product) {
  $price_info = array(
    'price' => $product->price,
    'qty' => $product->qty,
  );
  $context['subject']['order_product'] = $product;
?>
- <?php echo $product->qty; ?> x <?php echo $product->title .' - '. uc_price($price_info, $context); ?><br />
&nbsp;&nbsp;<?php echo t('SKU: ') . $product->model; ?><br />
    <?php if (isset($product->data['attributes']) && is_array($product->data['attributes']) && count($product->data['attributes']) > 0) {?>
    <?php foreach ($product->data['attributes'] as $attribute => $option) {
      echo '&nbsp;&nbsp;'. t('@attribute: @options', array('@attribute' => $attribute, '@options' => implode(', ', (array)$option))) .'<br />';
    } ?>
    <?php } ?>
<br />
<?php } ?>
</p>

