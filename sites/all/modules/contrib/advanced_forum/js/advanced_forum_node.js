/**
 * JS added when rendering nodes
 */

/**
 * Quick reply - make comment form focused
 */ 
Drupal.behaviors.advancedForumQuickReplyFocus = function(context) {
  // only proceed if the comment form is on this page
  if ($('#edit-comment').length) { 
    $('.topic-reply-allowed a:not(.quick-reply-checked)', context).addClass('quick-reply-checked').click(function(e){
      if ($('#edit-subject').length) {
        $('#edit-subject').focus();
      }
      else {
        $('#edit-comment').focus();
      }
      e.preventDefault();
      return false;
    });
  }
  // version for nodecomment
  else if ($('#edit-body').length) { 
    $('.topic-reply-allowed a:not(.quick-reply-checked)', context).addClass('quick-reply-checked').click(function(e){
      if ($('#edit-title').length) {
        $('#edit-title').focus();
      }
      else {
        $('#edit-body').focus();
      }
      e.preventDefault();
      return false;
    });
  }
};