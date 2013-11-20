
Introduction
================

This module is for people who want a custom front page to their Drupal sites.

The key functionality of this module is outlined below. 

If you are comfortable with theming using a mix of php/html, I recommend you check out the
handbook pages I have added to Drupal.org which explains how a more advanced method of theming
your front page. http://drupal.org/node/46017 

This module is intended for Drupal Version 6.x 
It will not work with earlier versions.


Key Functionality
=====================

1. Allows you to specify a custom front page based on role type.

e.g. For a musicians website, you could have a different front page for Drummers, Guitarists, Singers, Bass Players etc.

3. Allows you to have a FULL front page or SPLASH page - a completely different layout to your 
main drupal site - or alternatively as a THEMED full page, i.e. it loads with your default drupal site theme & layout.

4. Automatically REDIRECT users to a specific page or path.

5. Disable your site for everyone but Administrators and display a custom "under maintenance" message.

6. Display a custom "mission" style message for authenticated users who have not visited the site for a set period.

e.g. If you have had a significant update to the site, you might want to have a special notice for users who haven't 
visited for 1 month, or 1 week etc.

7. Override your HOME and Breadcrumb links on your site.

e.g. you might have a splash front page that you don't want visitors already on your site returning to when they
click on HOME.

8. You can include PHP/HTML or both in your frontpage



Installation
===============

1. Upload or copy the 'front' directory and its content to your MODULES folder.

2. Go to the ADMINISTER section of your drupal site.

3. Click on MODULES

4. Enable the front_page.module

5 Go to ADMINISTER - SITE CONGIFURATION - ADVANCED FRONT PAGE SETTINGS

6. Paste in the TEXT/HTML/PHP contents of your front pages. 

7. Once you are happy with your front_pages, set the DEFAULT FRONT PAGE setting to be front_page. 
    
8. Go to ADMINISTER - USER MANAGEMENT - PERMISSIONS and enable ACCESS FRONT PAGE for anonymous/authenticated users.  
   
############################################
IMPORTANT NOTE FOR THOSE USING PATH.MODULE (URL ALIAS):
Please ensure you have no other pages 
setup with the URL ALIAS 'front_page' when
installing the front_page.module which uses the
'front_page' URL Alias by default.
##############################################  


PROMOTED TO FRONT PAGE Example snippet
=======================================
The default front page when you install Drupal for the first time, is 'node' which displays a list of node teasers, where
the nodes have been tagged as pages that are 'Promoted to Front Page'. 

If you want to recreate that node listing after installing the front_page.module, simply paste the following snippet into 
the text area provided on the front_page settings page and select the PHP filter before saving your new configuration.  

<?php
   print node_page_default();
?>

   
   
Uninstall
=========

1. Go to ADMINISTER -> SITE CONFIGURATION -> ADVANCED FRONT PAGE SETTINGS and change the default front page to something other than 'front_page' (e.g. 'node').

2. Go to ADMINISTER -> MODULES and de-select the front_page.module from your list of modules.

3. Using FTP or other file manager, remove the front_page.module files and folder.

  
Hope you find it useful. 

This module has been developed by a few members of the drupal community and 
we're always looking for ideas for improving it. 

Email me or post a message on the drupal.org site if you have any ideas on 
how we can improve the module. 

Dublin Drupaller


dub@dublindrupaller.com
