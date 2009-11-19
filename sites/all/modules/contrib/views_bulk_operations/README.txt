********************************************************************
D R U P A L M O D U L E
********************************************************************
Name: Views Bulk Operations Module
Authors: Karim Ratib <karim dot ratib at open dash craft dot com>
Drupal: 6
********************************************************************
DESCRIPTION:

This module augments views by allowing bulk operations to be made on
the nodes displayed by a view. It does so by showing a checkbox in
front of each node, and adding a select box containing all available
actions and node operations found in the system. The chosen action
is applied on each selected node.

********************************************************************
USAGE:

**YOU SHOULD BE FAMILIAR WITH CREATING VIEWS IN DRUPAL 6**

After activating the views_bulk_operations module, create a new view 
of any type (VBO currently supports node, user and comment). In the
view admin page, modify the Style attribute of the Basic Settings to
"Bulk Operations". You then need to select the fields you want displayed.

