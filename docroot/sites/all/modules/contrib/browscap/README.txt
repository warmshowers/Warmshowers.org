Browscap provides an improved version of PHP's get_browser() function.

The get_browser() function can be used to tell what a visitor's browser is
capable of. Unfortunately, the version provided by PHP has a number of
limitations, namely:

* It can be difficult or impossible to configure for shared hosting
  environments.
* The data used to identify browsers and determine their capabilities requires
  consistent maintenance to keep up-to-date.

Browscap automates maintenance by storing browser data in a database and
automatically retrieving the latest data on a configurable schedule.

Requirements
------------

Browscap requires that your server be able to "phone out" (make a http request)
to retrieve and update its user agent database.

Note: Some hosting companies have this capability blocked.


Recommendations
---------------

If you are using MySQL, it is recommended to use InnoDB and not MyISAM as the
table storage engine. (InnoDB is the default for Drupal 7.) MyISAM does not
support transactions, and without transactions Browscap may not return correct
browser information while new Browscap data is being loaded.


Installation
------------

Browscap can be installed via the standard Drupal installation process.
https://drupal.org/node/895232


API
---

Modules can make use of browscap data by calling browscap_get_browser()
anywhere they would otherwise call the PHP get_browser()
(https://secure.php.net/manual/en/function.get-browser.php) function.

Note: browser_name_regex is not returned.


Drush integration
-----------------

To import the data via drush, use `drush browscap-import`.


Credits
-------

Development of Browscap is sponsored by Acquia (https://www.acquia.com) and the
Ontario Ministry of Northern Development and Mines (http://www.mndm.gov.on.ca).

A special thanks goes out to Gary Keith (http://www.garykeith.com) who provides
regular updates to the browscap user agent database, and specifically for
adding a non-zipped CSV version of browscap to support this module.
