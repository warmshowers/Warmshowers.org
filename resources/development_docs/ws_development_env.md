## Warmshowers.org Development Environment Setup

To do any significant development on Warmshowers.org you're going to have to
have your own local copy of it. That means running Apache as a webserver and
Mysql as a database and using the Warmshowers code and a copy of the
configured database. This article will explain how to do those things.

This article won't describe the basics of setting up a local drupal environment,
because that would just make things too wordy for you, and of course there are
much better resources. You can set up a local Drupal environment on Windows,
Mac, or Linux, but it's really a bit easier on Linux or MacOS.
See [Drupal.org on setting up a development environment](https://drupal.org/setting-up-development-environment).
See especially [Local server setup](https://drupal.org/node/157602)

If you haven't ever set up a local Drupal environment before, make sure you can
do that before trying to set up a Warmshowers environment.

A few files will help you here, so do get them before you get far along. You'll
need:

* Partially sanitized Warmshowers.org database
* Settings.php

### Prerequisites: Apache and Mysql

You need Apache and Mysql as described above (or nginx and mariadb if you like).
You will need a mysql database named "warmshowers" preferably. Give the database
privileges for a user and password. I always use 'root' and no password on
development mysql databases.

### Git fork, checkout, and file configuration

* Using your free github account, fork the Warmshowers.org code by visiting
https://github.com/rfay/Warmshowers.org and clicking the "fork" button in the
upper right. You'll end up with a repository named you/Warmshowers.org
* Clone your forked repository onto your local machine. I always use the directory
workspace/warmshowers in my home directory for this:
```
cd
mkdir workspace
cd workspace
git clone https://github.com/<you>/Warmshowers.org.git warmshowers
```
* `mkdir -p ~/workspace/warmshowers/docroot/files` to create the directory for
user-created files like profile pictures. Later you may want to actually populate
this directory. (Note that since Warmshowers has been around since long before
Drupal moved the files directory to sites/default/files, the files are still in /files.)
* `cp settings.php ~/workspace/warmshowers/docroot/sites/default` - where settings.php comes
from the files provided to you as described above. Just ask.

### Loading the partially sanitized database

There are many ways to load the partially sanitized database; it just involves
gunzipping and loading via mysql. Please don't gunzip the database *within*
the Dropbox folder, as that will make that folder bigger for everybody.

```
gzip -dc ~/Dropbox/ws_private/warmshowers_sanitized_*.sql.gz | mysql warmshowers
```

will actually do the job. You could also copy it out to /tmp, gunzip it, and
load with the mysql tool.

### Editing the settings.php

If you don't use the same username/password in settings.php, you may need to
edit those. Edit the sites/default/settings.php as necessary.

### Optionally populate the files directory

The files directory has user pictures and some other random things. A tarball
of it is available, but it grows seriously over time. A [December 2012 tarball](https://dl.dropboxusercontent.com/u/7350603/Warmshowers/ws_files_dev.20121223.tgz) can be used; it's large.
The current is also available if you really want it. It's much larger.

If you had downloaded it into /tmp:

```
cd ~/workspace/warmshowers/docroot
tar -zxf /tmp/ws_files_dev.20121223.tgz
```

### Add hostnames into /etc/hosts

I use the hostname warmshowers.dev (and es.warmshowers.dev, fr.warmshowers.dev,
etc.) to access the site. You'll need to add those to your local hosts file.

My hosts file has this stanza:

```
127.0.0.1 warmshowers.dev
127.0.0.1 es.warmshowers.dev www.warmshowers.dev
127.0.0.1 fr.warmshowers.dev
127.0.0.1 pt.warmshowers.dev
127.0.0.1 de.warmshowers.dev
127.0.0.1 it.warmshowers.dev
```

### Add configuration for the site to Apache

You need an Apache vhost for warmshowers.dev, and can do this how you want it.
This is configured in various places depending on your apache, macos, or linux
distribution. For example the below might go into /etc/apache2/sites-available/warmshowers.conf
on Debian/Ubuntu.

I use something like this:

```
NameVirtualHost *
<VirtualHost *>
	ServerName warmshowers.dev
	ServerAlias *.warmshowers.dev
	ServerAdmin randy@randyfay.com

	DocumentRoot /Users/rfay/workspace/warmshowers/docroot
	<Directory /Users/rfay/workspace/warmshowers/docroot>
		Options FollowSymLinks
		AllowOverride All
		Order allow,deny
		allow from all
	</Directory>
</VirtualHost>
```

(You're welcome to use nginx for this of course... but do it your way. We actually
use nginx on the production site.)

Restart apache to make it read the configuration. On most systems this would be

`sudo apachectl restart` or `sudo service apache2 restart` or `sudo service httpd restart`

### Setup a Drush alias for the site (Optional)

To facilitate development using Drush, it's useful to have a Drush alias for
interacting with the site, so you can run `drush @warmshowers.dev status`
instead of having to `cd` into the warmshowers directory to use Drush.

1. Copy the file in `/assets/rebuild/warmshowers.aliases.drushrc.php` to `~/
.drush/warmshowers.aliases.drushrc.php`.

2. Edit the values in `~/.drush/warmshowers.aliases.drushrc.php` to match the paths
and values for your local environment. Important items to change are marked 'TODO'.

### Log into the site and configure it the way you want it

Now you need to log into the site, and of course your account may not have the
privileges you need to do what you need.

To get user 1 privileges you can use [drush](https://github.com/drush-ops/drush)
and do `drush uli` in the project root

or

```
cd <drupal_root_directory>
drush sql-cli   # or mysql -u<user> -p<pass> <drupal_db>
update users set name='admin', pass=md5('drupal') WHERE uid=1;
```

Now if you visit http://warmshowers.dev you should see the site and should be able
to log in as admin with the password 'drupal'.

### Rebuilding your local Warmshowers.org development environment

From time to time it's useful to revert your local environment to a clean state
using the supplied sanitized SQL dump. Often times, however, there are a number
of development tools and permissions that you need to enable (e.g. Devel, Devel
Node Access, Reroute Email, etc) for local development.

This is where [Drush Rebuild](https://drupal.org/project/rebuild) can help you.
Since you have already configured a Drush alias for `@warmshowers.dev`, you
can install Drush Rebuild with `drush dl rebuild-7.x-1.7` (or higher revision). Run `drush cc drush` then
`drush rebuild --version` to check that the extension was installed correctly.

Now type `drush @warmshowers.dev st` to verify that your alias works. If
that's good, then type `drush rebuild @warmshowers.dev` (add the `--verbose` flag
if you'd like more output). This will use the Drush script at
`assets/rebuild/import-db.php` to (1) drop the existing database, (2) create a new
database, (3) import the SQL dump from Dropbox, and then (4) enable modules, set
variables, define permissions, and finally log you in to your local environment.

You can easily customize the rebuild config by creating a `local.rebuild.yaml`
file in `assets/rebuild/local.rebuild.yaml` and defining config variables there.

You can run `drush rebuild @warmshowers.dev` any time you want to revert to
a clean state. The process takes anywhere from 5-15 minutes depending on your
hardware.

Windows users: use caution as the above has been tested only in Linux and Mac OS X.

### Automated tests

Check out `docroot/sites/default/behat-tests` for more information on running
tests for the Warmshowers dev environment.

### Notes

* Warmshowers.org developers have access to a free copy of the excellent
PHPStorm IDE and debugger. If you would like to use this (it runs on Windows,
Mac, and Linux) you can [download the standard phpstorm product](http://www.jetbrains.com/phpstorm/download/)
and just ask Randy for the license key.


