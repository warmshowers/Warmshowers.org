uc_recurring
~~~~~~~~~~~~

uc_recurring is a drupal module to provide recurring billing to the ubercart project.

INSTALL
~~~~~~~

See the getting started guild on installing drupal modules:
http://drupal.org/getting-started/install-contrib/modules

USAGE
~~~~~

This module allows you to add handle recurring payments in ubercart.

Step 1: Enable module on your drupal site.

Step 2: Setup Recurring Payments:
(This step can be skipped if you do not accept payments on site and you do not
have the uc_payment module enabled)
  * Requirement: Installed and setup payment gateways in ubercart.
  * Go to:
    "Store administration" -> "Configuration" -> "Payment Settings" -> "Edit" -> "Recurring payments"
  * Select payment methods that should be allows to process recurring payments,
    only the methods selected will be shown on the checkout page when a order
    includes a recurring product.

Step 3: Enable a module that triggers recurring payments on certain events.
  * Recurring Products (uc_recurring_product) - product specific recurring fees (e.g. subscriptions)
  * Recurring Order (uc_recurring_order) - entire order is recurring.

You site should be ready to accept orders with recurring payments.

TESTING
~~~~~~~
Ubercart includes a test payment gateway called test_gateway. This gateway
emulates a credit card payment gateway and uc_recurring supports this gateway.

If you are attempting to test if uc_recurring is setup correctly this is a good
gateway to initally test against before setting up your own live gateway.

If need to take snapshots of live databases with recurring fees setup ensure
that cron is not running on your test site or recurring payments may be
triggered from your test and live installs.

A simple way to ensure recurring payments are not triggered on cron runs is to
add the following php to your test sites settings.php

<?php
// disable ubercart recurring payments
$conf['uc_recurring_trigger_renewals'] = FALSE;
?>

DEVELOPERS
~~~~~~~~~~
This modules includes the file uc_recurring.api.php which is an attempt to
define all the drupal hooks this module exposes to developers.

To integrate with a new payment gateway you should first look at the
hook_recurring_info() function as this defines all the details uc_recurring
needs to work with a new gateway.

Credits
~~~~~~~
Ryan Szrama (http://www.commerceguys.com/)
Chris Hood (http://www.univate.com.au/)

LICENSE
~~~~~~~
No guarantee is provided with this software, no matter how critical your
information, module authors are not responsible for damage caused by this
software or obligated in any way to correct problems you may experience.

This software licensed under the GNU General Public License 2.0.
http://www.gnu.org/licenses/gpl-2.0.txt
