This is an Ubercart payment gateway module for Stripe.

Versions of the Stripe PHP Library and Stripe API that this module is currently
configured for are in the top of uc_stripe.module:
  define('UC_STRIPE_STRIPE_API_VERSION', '2015-06-15');
  define('UC_STRIPE_STRIPE_PHP_LIBRARY_VERSION', '2.2.0');


Installation and Setup
======================

a) Install and enable the module in the normal way for Drupal.

b) Visit your Ubercart Store Administration page, Configuration
section, and enable the gateway under the Payment Gateways.
(admin/store/settings/payment/edit/gateways)

c) On this page, provide the following settings:
   - Your Stripe API key, private

d) Download and install version 2.2.0 PHP Stripe
library (https://github.com/stripe/stripe-php. Put it in
sites/all/libraries/stripe such that the path to Stripe.php
is sites/all/libraries/stripe/lib/Stripe.php

e) If you are using recurring payments, install version 2.x
of the Ubercart Recurring module:
http://drupal.org/project/uc_recurring
and set up as described below.

f) Every site dealing with credit cards in any way should be using https. It's
your responsibility to make this happen. (Actually, almost every site should
be https everywhere at this time in the web's history.)

Upgrading from uc_stripe 6.x-1.x or 7.x-1.x
===========================================

7.x-2.x does not use Stripe subscriptions for recurring payments, but instead
uses the uc_recurring module. This means you have control of recurring
transactions without having to manage them on the Stripe dashboard. (Credit
card numbers and sensitive data are *not* stored on your site; only the Stripe
customer ID is stored.)

The upgrade hooks, however, must move the customer id stored in the obsolete
uc_recurring_stripe table into the user table. When this happens the old
record in the uc_recurring_stripe table will have its plan changed to
<old_plan>_obsolete. This just prevents an import from happening more than once
and gives you backout options if you wanted to downgrade.

If you were using 1.x of this module and want to cancel existing subscriptions
which were configured via the Stripe api (since they are now managed via
uc_recurring) a drush command is provided to cancel these. Use "drush help subcancel"
for more information.

Recurring Payments Setup
========================

You'll need the Ubercart Recurring module:
http://drupal.org/project/uc_recurring installed. It is not
listed as a dependency for this Stripe payment module because
this module could be used without recurring payments.
But it is a dependency to use the recurring payments piece of
this module. Note that this module does *not* use Stripe subscriptions.
Instead, recurring payments are managed by uc_recurring, which does not
retain any valid CC info, only the stripe customer id.

Recurring payments require automatically triggered renewals using
uc_recurring_trigger_renewals ("Enabled triggered renewals" must be enabled
on admin/store/settings/payment/edit/recurring)

If you were using Stripe subscriptions in v1 of this module, you may have to
disable those subscriptions in order to not double-charge your customers.

uc_stripe 6.x-2.x and 7.x-2.x were based on Bitcookie's work (thanks!) which was posted at
http://bitcookie.com/blog/pci-compliant-ubercart-and-stripe-js
from discussion in the uc_stripe issue queue,
https://www.drupal.org/node/1467886
