uc_recurring_product
~~~~~

uc_recurring_product is a drupal/ubercart module to add recurring fees to product purchases.

INSTALL
~~~~~~~

See the getting started guild on installing drupal modules:
http://drupal.org/getting-started/install-contrib/modules

USAGE
~~~~~

This module allows you to add recurring payments to an Product so that when a
order is placed with that product a payment schedule will be setup to charge
the user a set amount on a regular interval. 

Step 1: Enable module on your drupal site.

Step 2: Add recurring payments to a product:
  * Open product you want to create a recurring payment schedule on.
  * Click on "edit"
  * Click on "features"
  * Under "Add a new feature", select "Recurring fee"
  * Fill in the form as required
NOTE: you can add more then one recurring fee to a product and by selecting
different SKU's based on different attribute options setup different payment
options for the one product. e.g. weekly/monthly subscriptions.

You site should be ready to accept orders with recurring products fees.

TESTING
~~~~~~~
Ubercart includes a test payment gateway called test_gateway. This gateway
emulates a credit card payment gateway and uc_recurring supports this gateway.

If you are attempting to test if uc_recurring is setup correctly this is a good
gateway to initally test against before setup up your own live gateway.

Credits
*******
Chris Hood (http://www.univate.com.au/)

LICENSE
~~~~~~~

This software licensed under the GNU General Public License 2.0.
