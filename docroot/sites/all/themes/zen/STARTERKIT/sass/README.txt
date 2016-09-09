ABOUT SASS AND COMPASS
----------------------

This directory includes Sass versions of Zen's CSS files.

Sass is a language that is just normal CSS plus some extra features, like
variables, nested rules, math, mixins, etc. If your stylesheets are written in
Sass, helper applications can convert them to standard CSS so that you can
include the CSS in the normal ways with your theme.

To learn more about Sass, visit: http://sass-lang.com

Compass is a helper library for Sass. It includes libraries of shared mixins, a
package manager to add additional extension libraries, and an executable that
can easily convert Sass files into CSS.

To learn more about Compass, visit: http://compass-style.org


DEVELOPING WITH SASS AND COMPASS
--------------------------------

Zen 7.x-5.0 was developed with the latest version of Sass and Compass (at the
time!) Newer versions are not compatible with Zen's Sass files. To ensure you
are using the correct version of Sass and Compass, you will need to use the
"bundle" command which will read Zen's Gemfile to ensure the proper versions are
used when compiling your CSS. To install the correction versions of Sass and
Compass, go to the root directory of your sub-theme and type:

  bundle install

You will also need to prefix any compass commands with "bundle exec". For
example, type "bundle exec compass compile" instead of just "compass compile".

To automatically generate the CSS versions of the scss while you are doing theme
development, you'll need to tell Compass to "watch" the sass directory so that
any time a .scss file is changed it will automatically generate a CSS file in
your sub-theme's css directory:

  bundle exec compass watch <path to your sub-theme's directory>

  If you are already in the root of your sub-theme's directory, you can simply
  type:  bundle exec compass watch

While using generated CSS with Firebug, the line numbers it reports will not
match the .scss file, since it references the generated CSS file's lines, not
the line numbers of the "source" sass files. How then do we debug? Sourcemaps to
the rescue! To find the oringial, newer browsers have support for sourcemap
files (*.map). These files are used by the built-in development tools of newer
browsers to map the generated line to the SCSS source. When in development mode,
Zen can be set to generate sourcemap files. Edit config.rb, and uncomment:

  sourcemap=true


Enabling and using sourcemap files (*.map) in your browser

In short, Open Developer tools, go to settings, and enable an option to the
effect of: 'view original sources' or 'Enable CSS source maps'.

* Firefox: https://hacks.mozilla.org/2014/02/live-editing-sass-and-less-in-the-firefox-developer-tools/
* Chrome:  https://developer.chrome.com/devtools/docs/css-preprocessors#toc-enabling-css-source-maps
* IE: http://msdn.microsoft.com/en-US/library/ie/dn255007%28v=vs.85%29#source_maps


MOVING YOUR CSS TO PRODUCTION
-----------------------------

Once you have finished your sub-theme development and are ready to move your CSS
files to your production server, you'll need to tell sass to update all your CSS
files and to compress them (to improve performance). Note: the Compass command
will only generate CSS for .scss files that have recently changed; in order to
force it to regenerate all the CSS files, you can use the Compass' clean command
to delete all the generated CSS files.

- Delete all CSS files by running: bundle exec compass clean
- Edit the config.rb file in your theme's directory and uncomment this line by
  deleting the "#" from the beginning:
    #environment = :production
- Regenerate all the CSS files by running: bundle exec compass compile

And don't forget to turn on Drupal's CSS aggregation. :-)
