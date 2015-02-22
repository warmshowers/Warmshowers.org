<?php

/**
 * @file
 * Customized admin order notification template.
 */
?>

<?php $account = user_load($order->uid); ?>

<p>
<p>
  Comments:
  <?php echo $order_comments; ?>
</p>



Order Number: <?php echo $order_admin_link; ?><br />
Member: <?php print theme('username', $account); ?>  (<?php print "{$account->city}, {$account->province}, {$account->country}"; ?>)<br/>
Member Join Date: <?php print date('Y-m-d', $account->created); ?><br/>
Member Language: <?php print $account->language . '(' . $account->languagesspoken . ')'; ?><br/>
<?php echo t('Email:'); ?> <?php echo $order_email; ?><br />
<?php echo t('Order total:'); ?> <?php echo $order_total; ?><br />
</p>

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

