Welcome to Node Convert.

This module allows to convert one or many nodes between different node types.
It can convert the most popular field types: text, link, image, file, entity reference, node / term / user reference.

The module provides:
- an API for converting nodes
- hooks for executing additional behaviors on conversion
- integrates with hook_node_operations and Drupal's Action API
- integration with Rules

How to use:

I. Single node conversion:
1) Set 'administer conversion' and 'convert to x', 'convert from y' permissions.
2) Go to /node/x/convert and follow the provided steps to convert the node.

II. Multiple node conversion (using hook_node_operations)
1) Set appropriate permissions.
2) Go to admin/structure/node_convert_templates
3) Create a new template following the the provided steps.
4) Go to admin/content
5) Select the relevant nodes.
6) Choose "Convert template: x" (based on the template name created above) from the update options.
7) Click Update.

III. Multiple node conversion (using Actions API + Views Bulk Operations)
Note: This requires the contributed modules Views and Views Bulk Operations

1) Set appropriate permissions.
2) Go to admin/structure/node_convert_templates
3) Create a new template following the the provided steps (also check Create Action).
3) Create a new view with the options you require.
4) Add Views Bulk Operations under fields.
5) Configure all options of Bulk Operations as necessary.
6) Select "Convert a node" as an operation or an action that was created together with the conversion template.
Note that VBO Operations are gathered from two sources:
  - Drupal core actions (hook_action_info() and advanced actions added through the Actions UI)
  - Rules 2
7) Save the view. View it.
8) Select the necessary nodes and click the Convert x button.

Useful API calls:
node_convert_node_convert($nid, $dest_node_type, $source_fields, $dest_fields, $no_fields_flag, $hook_options = NULL);
node_convert_field_convert($nid, $source_field, $dest_field);
hook_node_convert_change($data, $op);

Modules can provide additional behavior to execute upon conversion by implementing hook_node_convert_change.
See node_convert.api.php for an example.