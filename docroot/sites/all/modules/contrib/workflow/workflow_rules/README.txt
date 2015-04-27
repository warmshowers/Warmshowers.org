WORKFLOW NODE
=============
When using the conventional 'Workflow Node API', Rules should be triggered upon
the workflow-specific 'transition post' event.

WORKFLOW FIELD
==============
As of Workflow 7.x-2.0, alternative 'Workflow Field API' is available. There
is no need to enable this module if you use Workflow Field.
You can add a Rule using:
- After saving a new workflow scheduled transition
- After updating an existing Entity
The 'transition post' hook is not called for Workflow Field, since at that
moment in the update process, the data is not saved yet.
