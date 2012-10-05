The role_delay module enables the configuration of one or more user roles that all new users
are automatically granted after a specified period of time from registration. A role delay of 
0 (zero), will automatically assign that role to every user at creation time.

A common use case for this functionality is to retrict the default "authenticated user" role
to a minimal set of permissions and progressively add more permissions via roles granted after
longer membership times. For example, authenticated users might only have the ability to view
content while another role, granted after a period of 30 days, is given to the ability to post
comments and still another role, given after a period of 6 months, is given the ability to post
comments without approval.


INSTALLATION
------------
This module is installed via the standard module installation process. For more information
see http://drupal.org/documentation/install/modules-themes/modules-5-6.


USAGE
-----
To enable role delays, navigate to admin/user/roles, and edit the individual
roles. There will be a field to add a specified "Delay". All users created 
after this delay has been configured, will receive the role at the designated
time.

Existing scheduled role delays can be managed on the standard core user admin
page at admin/user/user.


TROUBLESHOOTING
---------------
Roles are granted via cron, therefore if the module doesn't appear to be granting
roles as configured, check to be sure that 1) you have cron running and 2) that it's
completing successfully before creating an issue in the module's issue queue.
