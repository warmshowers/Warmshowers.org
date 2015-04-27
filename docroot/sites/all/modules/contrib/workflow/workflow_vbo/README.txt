
CONTENTS OF THIS FILE
---------------------

 * Introduction

INTRODUCTION
------------

There is an important issue to keep in mind as you use action hooks and workflow!!

If the machine readable name of the content type on which you want to define actions
in the workflow exceeds 20 characters then the actions you define will not be visible
in the screen where you define the triggers nor will they execute.

The reason is that the length of the field "op" in the "trigger-assignments" table
is 32 characters. The name of this "op"-field is a concatenation of the string
"workflow-" with the machine readable name of the content type, another "-" and the
transition-id on which the action has to be performed. If the latter has a length of
1 then this leaves 32 - 9 - 1 - 1 = 21 characters for the machine readable name of
the content type.

So: KEEP YOUR CONTENT TYPE NAMES SHORT.

Unfortunately the code that handles this is in core, so not readily changeable. If
you have trouble seeing your actions check your name lengths.

See further discussion at:
  http://drupal.org/node/585726

See request put to core to make the change at:

  http://drupal.org/node/1062068

  Closed and told to have workflow make the table change ourselves. Given that changing
  name lengths hazardly would spread the bugs around even more this approach was
  not followed.
