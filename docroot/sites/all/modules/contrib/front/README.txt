
Introduction
================

This module is for people who want a custom front page to their Drupal sites.

For all bugs, feature requests or support requests please use the
Front Page issue queue at http://drupal.org/project/issues/front


Key Functionality
=====================

1. Allows you to specify a custom front page based on role type.

2. Allows 4 different override types:
    1. Themed - Allows you to add content that will display as a standard
          themed Drupal page.
    2. Full - Allows you to add content that will be displayed on the screen
          as is. This method is the same as declaring a whole HTML page.
    3. Redirect - Allows you to 301 redirect the user to another path.
    4. Alias - Allows you to specify a local path which will then display as
          the home page without redirecting the user.

4. Allow Themed and Full display types to be passed through Drupals input filters.

5. Override Home Links to go to another local path. This could be to stop users
    going back to a splash screen.



Installation
===============

1. Upload and install the Front Page module.

2. Go to Administer -> Config -> Front Page.

3. Expand any of the roles that you want to override the default front page and
    select the appropriate method of override as well as filling in the variables
    required for that method. Once the settings are correct save the form.

4. Go to Administer -> Config -> Front Page -> Arrange.

5. Arrange the roles in the order in which you want them to process. Roles at
    the top will process first and then work down the list. Once the order is
    correct save the form.

5. Go back to Administer -> Config -> Front Page.

6. Enable the 'Front Page Override' checkbox and then save the form. The front
    page module should now be working correctly.