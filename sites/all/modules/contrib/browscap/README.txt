The browscap module provides a replacement for PHP's get_browser() function.
get_browser() is difficult (or impossible) to configure for most
users in shared webhosting situations, and requires attention to keep the
underlying data (browscap.ini) up-to-date. This module avoids the
configuration issue by storing the data in a database table, and the
freshness issue by automatically retrieving the latest data on a 
weekly basis from http://www.garykeith.com/ (if cron.php is run regularly).

Also, statistics on browsers visiting the site may be captured by
enabling monitoring in the browscap settings.

Installation
------------

1. Place the browscap folder in the modules directory of your 
Drupal installation.

2. Enable the browscap module in the administration tools.

3. Go to http://www.example.com/cron.php to perform the initial import
of the browscap data or visit Administer > Site configuration > Browscap
and click the "Refresh now" link.

4. On Administer > Site configuration > Browscap you can enable logging of 
browser types for visitors to your site. 

5. The report itself is visible at Administer > Reports > Browscap and 
several other tabs in that area.

API
---

Modules wishing to make use of browscap data may call
browscap_get_browser() anywhere they would otherwise call get_browser()
(see http://us3.php.net/manual/en/function.get-browser.php). Note that
browser_name_regex is not returned - otherwise, the results should be
identical to calling get_browser().

Credits
-------
Mike Ryan (drupal@virtuoso-performance.com) is the author and maintainer 
of this module. 

Thanks to Gary Keith (http://www.garykeith.com/) for providing 
regular updates to the browscap data, and specifically for adding
a non-zipped CSV version of browscap to support this module.