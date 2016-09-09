#
# This file is only needed for Compass/Sass integration. If you are not using
# Compass, you may safely ignore or delete this file.
#
# If you'd like to learn more about Sass and Compass, see the sass/README.txt
# file for more information.
#


# Change this to :production when ready to deploy the CSS to the live server.
environment = :development
#environment = :production

# If in development (set above), we can turn on the sourcemap file generation.
# Requires sass 3.3+ and compass 1.0.1+
# Determine version from command line: sass --version && compass --version
sourcemap = false
#sourcemap = true

# Alternative development debugging methods
# If in development (above), we can enable line_comments for FireCompass plugin.
# Requires Firebug plugin and FireCompass plugin
firecompass = false
#firecompass = true

# If in development (above), we can enable debug_info for the FireSass plugin.
# Requires Firebug plugin and Firesass plugin
firesass = false
#firesass = true


# Location of the theme's resources.
css_dir         = "css"
sass_dir        = "sass"
extensions_dir  = "sass-extensions"
images_dir      = "images"
javascripts_dir = "js"


# Require any additional compass plugins installed on your system.
#require 'ninesixty'
#require 'zen-grids'

# Assuming this theme is in sites/*/themes/THEMENAME, you can add the partials
# included with a module by uncommenting and modifying one of the lines below:
#add_import_path "../../../default/modules/FOO"
#add_import_path "../../../all/modules/FOO"
#add_import_path "../../../../modules/FOO"


##
## You probably don't need to edit anything below this.
##

# You can select your preferred output style here (can be overridden via the command line):
# output_style = :expanded or :nested or :compact or :compressed
output_style = (environment == :development) ? :expanded : :compressed

# To enable relative paths to assets via compass helper functions. Since Drupal
# themes can be installed in multiple locations, we don't need to worry about
# the absolute path to the theme from the server root.
relative_assets = true

# To disable debugging comments that display the original location of your selectors. Uncomment:
# line_comments = false

# Pass options to sass. For development, we turn on the FireSass-compatible
# debug_info if the firesass config variable above is true.
sass_options = (environment == :development && firesass == true) ? {:debug_info => true} : {}

# Pass options to sass. For development, we turn on the FireCompass-compatible
# line_comments if the firecompass config variable above is true.
sass_options = (environment == :development && firecompass == true) ? {:line_comments => true} : sass_options

# Pass options to sass. For development and sourcemap variable is true (above),
# then pass the "--sourcemap" option flag to compass/sass.
sass_options = (environment == :development && sourcemap == true) ? {:sourcemap => true} : sass_options
