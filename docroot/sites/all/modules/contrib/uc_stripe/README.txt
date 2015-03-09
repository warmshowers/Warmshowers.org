This is an Ubercart payment gateway module for Stripe.

Installation and Setup
======================

a) Install and enable the module in the normal way for Drupal.

b) Visit your Ubercart Store Administration page, Configuration
section, and enable the gateway under the Payment Gateways.
(admin/store/settings/payment/edit/gateways)

c) On this page, provide the following settings:
   - Your Stripe API key, private

d) Download and install the latest version of the PHP Stripe
library (https://github.com/stripe/stripe-php. Put it in
sites/all/libraries/stripe such that the path to Stripe.php
is sites/all/libraries/stripe/lib/Stripe.php

e) If you are using recurring payments, install version 2.x
of the Ubercart Recurring module:
http://drupal.org/project/uc_recurring
and set up as described below


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

uc_stripe 6.x-2.x was based on Bitcookie's work (thanks!) which was posted at
http://bitcookie.com/blog/pci-compliant-ubercart-and-stripe-js
from discussion in the uc_stripe issue queue,
https://www.drupal.org/node/1467886
