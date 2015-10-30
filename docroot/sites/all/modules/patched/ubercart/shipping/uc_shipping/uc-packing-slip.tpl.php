<?php

/**
 * @file
 * The shipment packing slip template.
 */
?>

<table width="95%" border="0" cellspacing="0" cellpadding="1" align="center" bgcolor="#006699" style="font-family: verdana, arial, helvetica; font-size: small;">
  <tr>
    <td>
      <table width="100%" border="0" cellspacing="0" cellpadding="5" align="center" bgcolor="#FFFFFF" style="font-family: verdana, arial, helvetica; font-size: small;">
        <tr valign="top">
          <td>
            <table width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td>
                  <?php print $site_logo; ?>
                </td>
                <td width="20%" nowrap="nowrap">
                  <?php print $store_address; ?>
                  <br />
                  <?php print $store_phone; ?>
                </td>
              </tr>
            </table>
          </td>
        </tr>

        <tr valign="top">
          <td>

            <table cellpadding="4" cellspacing="0" border="0" width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td colspan="2" bgcolor="#006699" style="color: white;">
                  <b><?php echo t('Purchasing Information:'); ?></b>
                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('E-mail Address:'); ?></b>
                </td>
                <td width="98%">
                  <?php print $order_email; ?>
                </td>
              </tr>
              <tr>
                <td colspan="2">

                  <table width="100%" cellspacing="0" cellpadding="0" style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td valign="top" width="50%">
                        <b><?php echo t('Billing Address:'); ?></b><br />
                        <?php print $billing_address; ?><br />
                        <br />
                        <b><?php echo t('Billing Phone:'); ?></b><br />
                        <?php print $billing_phone; ?><br />
                      </td>
                      <td valign="top" width="50%">
                        <b><?php echo t('Shipping Address:'); ?></b><br />
                        <?php print $shipping_address; ?><br />
                        <br />
                        <b><?php echo t('Shipping Phone:'); ?></b><br />
                        <?php print $shipping_phone; ?><br />
                      </td>
                    </tr>
                  </table>

                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('Payment Method:'); ?></b>
                </td>
                <td width="98%">
                  <?php print $payment_method; ?>
                </td>
              </tr>

              <tr>
                <td colspan="2" bgcolor="#006699" style="color: white;">
                  <b><?php echo t('Order Summary:'); ?></b>
                </td>
              </tr>

              <tr>
                <td colspan="2" bgcolor="#EEEEEE">
                  <font color="#CC6600"><b><?php echo t('Shipping Details:'); ?></b></font>
                </td>
              </tr>

              <tr>
                <td colspan="2">

                  <table border="0" cellpadding="1" cellspacing="0" width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Order #:'); ?></b>
                        <?php print $order_link; ?>
                      </td>
                    </tr>

                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Carrier:'); ?></b>
                        <?php print $carrier; ?>
                      </td>
                    </tr>
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Tracking #:'); ?></b>
                        <?php print $tracking_number; ?>
                      </td>
                    </tr>

                    <tr>
                      <td colspan="2">
                        <br /><br /><b><?php echo t('Products on order:'); ?>&nbsp;</b>

                        <table width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">

                          <?php if (is_array($packages)) {
                            foreach ($packages as $package) {
                              foreach ($package->products as $product) { ?>
                          <tr>
                            <td valign="top" nowrap="nowrap">
                              <b><?php print $product->qty; ?> x </b>
                            </td>
                            <td width="98%">
                              <b><?php print $product->title; ?></b>
                              <br />
                                <?php echo t('SKU: ') . $product->model; ?><br />
                                <?php if (isset($product->data['attributes']) && is_array($product->data['attributes']) && count($product->data['attributes']) > 0) {
                                  foreach ($product->data['attributes'] as $attribute => $option) {
                                    echo '<li>' . t('@attribute: @options', array('@attribute' => $attribute, '@options' => implode(', ', (array)$option))) . '</li>';
                                  }
                                } ?>
                              <br />
                            </td>
                          </tr>
                              <?php }
                            }
                          } ?>
                        </table>

                      </td>
                    </tr>
                  </table>

                </td>
              </tr>

            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
