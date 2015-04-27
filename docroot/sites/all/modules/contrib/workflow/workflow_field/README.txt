This is a re-implementation of the Workflow module, using the Field API instead of the Form API.

The field definition (and widget and formatters) is implemented as lazy-loading classes in the main workflow module.
The activation of the Field type is done in the submodule Workflow_field, 
as stated in https://drupal.org/node/1285540 "Field types should be defined by one module and implemented by a separate module."

ONLY USE THIS MODULE IF: 
- you are happy with the features the Workflow core API provides
  (not all persons may choose from all possible values at all moments.)
- you want to test and help developing this submodule.

The current version supports: 
- the default Workflow API. 
- Workflow Admin UI, which manages CRUD for Workflows, States and Transitions.
- Workflow Access, since this works via Workflow API.

The current version provides: 
- adding a Workflow Field on an Entity type (Node type), or a Node Comment;
- usage of the core formatter from the List module (just showing the description of the current value);
- usage of the core widgets from the Options module (select list, radio buttons);
- usage of the usual Workflow Form, which contains also a Comment text area and Scheduling options.
- changing the 'Workflow state' value on a Node Edit page.
- changing the 'Workflow state' value via a Node's Comment.

The current version DOES NOT provide: 
- support for other submodules from the Workflow module. (At least, this is not tested.)
