This is an Ubercart payment gateway module for Stripe.

Versions of the Stripe PHP Library and Stripe API that this module currently
supports are found in uc_stripe_libraries_info() in uc_stripe.module.

Installation and Setup
======================

a) Install and enable the module in the normal way for Drupal.

b) Visit your Ubercart Store Administration page, Configuration
section, and enable the gateway under the Payment Gateways.
(admin/store/settings/payment/edit/gateways)

c) On that page, provide the following settings:
   - Your Stripe API key, private

d) Download and install the Stripe PHP Library version 2.2.0 or 3.13.0
from https://github.com/stripe/stripe-php/releases. Put it in
sites/all/libraries/stripe such that the path to VERSION
is sites/all/libraries/stripe/lib/VERSION. YOU MUST CLEAR THE CACHE AFTER
CHANGING THE STRIPE PHP LIBRARY. The Libraries module caches its memory of
libraries like the Stripe Library.  (Version 2.2.0 support is maintained for
existing users; version 3.13.0 supports PHP 7 and will get ongoing support.)
(With the latest version of the libraries module you can use
drush ldl stripe
to download the appropriate library.)

e) If you are using recurring payments, install version 2.x
of the Ubercart Recurring module:
http://drupal.org/project/uc_recurring
and set up as described below.

f) Every site dealing with credit cards in any way should be using https. It's
your responsibility to make this happen. (Actually, almost every site should
be https everywhere at this time in the web's history.)

g) If you want Stripe to attempt to validate zip/postal codes, you must enable
that feature on your *Stripe* account settings. Click the checkbox for
"Decline Charges that fail zip code verification" on the "Account info" page.
(You must be collecting billing address information for this to work, of course.)

h) The uc_credit setting "Validate credit card numbers at checkout" must be
disabled on admin/store/settings/payment/method/credit - uc_credit never sees
the credit card number, so cannot properly validate it (and we don't want it to
ever know the credit card number.)

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
