# Link CSS module for Drupal 7

Include CSS files using <link> element instead of @import. This is useful for
live refresh workflows such as CodeKit which do not support files loaded with
@import.

IMPORTANT: The reason Drupal does not behave this way by default is the
limitation in Internet Explorer <=7 which will not load more than 31 linked
stylesheets.

Only enable this module:
* For development and testing in other browsers.
* If you don't need to support these browser versions.
* If you're sure your site doesn't exceed this limit.

CSS aggregation should be enabled for production sites anyway, which negates
this issue.
