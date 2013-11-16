
/**
 * Code taken from user.js
 */
Drupal.behaviors.commentformsettings = function (context) {
  // Options to hide the annonymous alters
  $('input#edit-comment-anonymous-0').click(function() {
    if ($("input:checked")) {
      $("#anonymous-choices").hide();
    }
  });
  $('input#edit-comment-anonymous-1').click(function() {
    if ($("input:checked")) {
      $("#anonymous-choices").show();
    }
  });
  $('input#edit-comment-anonymous-2').click(function() {
    if ($("input:checked")) {
      $("#anonymous-choices").hide();
    }
  });
};