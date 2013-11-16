
-- SUMMARY --

Environment Indicator adds a coloured strip to the side of the site informing
the user which environment they are in (Development, Staging Production etc).

For a full description visit the project page:
  http://drupal.org/project/environment_indicator
  
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/environment_indicator


-- REQUIREMENTS --

* None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

You may configure the environment at /admin/settings/environment_indicator

You can also override settings in settings.php, allowing you to have different
settings for each of your environments. All configuration variables can be
overridden in settings.php, but the most common three are:

environment_indicator_text
    The text that will be displayed vertically down the indicator.
    e.g: $conf['environment_indicator_text'] = 'DEVELOPMENT SERVER';
    
environment_indicator_color
    A valid css color.
    e.g: $conf['environment_indicator_color'] = 'dark-red';
    
environment_indicator_enabled
    A boolean value indicating whether the Environment Indicator should be
    enabled or not. On your production environment, you should probably set
    this to FALSE.
    e.g: $conf['environment_indicator_enabled'] = FALSE;


-- CONTACT --

Author maintainers:
* Tom Kirkpatrick (mrfelton), www.kirkdesigns.co.uk


This project has been sponsored by:
* KIRKDESIGNS - Visit http://www.kirkdesigns.co.uk for more information.


