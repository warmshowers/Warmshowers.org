
Welcome to Ubercart Views.
--------------------------

If you're having trouble installing this module, please ensure that your
tar program is not flattening the directory tree, truncating filenames
or losing files.


Installing Ubercart Views:
--------------------------

Place the content of this directory in sites/all/modules/uc_views

Navigate to administer >> build >> modules. Enable Ubercart Views.

Please note that most of the views included in this module assumes that images are turned on and
configured for products (See administer >> Store administration)

After installation you will have a number of default views which you can enable and modify by navigating to:
http://your-site/admin/build/views

The Ubercart Views Marketing submodule utilizes database views, so you should
make sure that your database user has CREATE VIEW permission. Some users have
reported that ALL PRIVILEGES oddly enough doesn't necessary include CREATE
VIEW.

To explicitly grant CREATE VIEW to your db user on MySQL via phpMyAdmin as root MySQL user,
execute these queries:
 GRANT CREATE VIEW ON yourdbname.* TO 'dbusername'@'localhost'
 (Replace your database and user / host names as appropriate.)
 FLUSH PRIVILEGES


Ubercart Views was developed with the help of Lenio A/S (www.lenio.dk)
