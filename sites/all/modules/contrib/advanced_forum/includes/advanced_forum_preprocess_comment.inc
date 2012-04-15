<?php

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_comment(&$variables) {
  /* Easy links to the comment and parent node */
  $comment = $variables['comment'];
  $node = node_load($comment->nid);
  $variables['first_post'] = $node;

  // This is a comment, not the node.
  $variables['top_post'] = FALSE;

  /* Determine the template file to use for the comment. */
  if (arg(1) == 'reply' || arg(1) == 'edit') {
    // Use the preview version
    advanced_forum_add_template_suggestions("post-preview", $variables['template_files']);
  }
  else {
    // Use our combined node/comment template file
    advanced_forum_add_template_suggestions("post", $variables['template_files']);
  }

  /* Assemble all the classes & id that go on the main div. */
  $classes = array();

  // Add our general classes.
  $classes[] = 'forum-post clear-block';

  // Add the poster's UID
  $classes[] = "posted-by-$comment->uid";

  // Add class if the poster is the viewer.
  global $user;
  if ($user->uid > 0 && $user->uid == $comment->uid) {
    $classes[] = "post-by-viewer";
  }

  // Add class if the poster is the topic starter.
  if ($node->uid > 0 && $node->uid == $comment->uid) {
    $classes[] = "post-by-starter";
  }

  // Add class if post is new. Comment is used here instead of post for
  // consistency with Node Comments.
  if (!empty($comment->new)) {
    $classes[] = "comment-new";
  }

  // Create the template variable.
  $variables['advanced_forum_classes'] = implode(' ', $classes);

  /* New marker */
  $variables['new_marker'] = '';
  static $new_anchor_shown = FALSE;

  if (!empty($comment->new)) {
    if ($new_anchor_shown) {
      $variables['new_marker'] = '<span class="new">' . t('(new)') . '</span>';
    }
    else {
      $new_anchor_shown = TRUE;
      $variables['new_marker'] = '<a id="new"></a><span class="new">' . t('(new)') . '</span>';
    }
  }

  /* Post ID */
  // Set the post ID for theming / targetting
  $variables['post_id'] = "post-$comment->cid";

  /* Linked post number */
  if (!isset($post_number)) {
    static $post_number = 1;
  }

  $posts_per_page = _comment_get_display_setting('comments_per_page', $node);

  $page_number = !empty($_GET['page']) ? $_GET['page'] : 0;
  if (!$page_number) {
    $page_number = 0;
  }

  $post_number++;

  $linktext = '#' . (($page_number * $posts_per_page) + $post_number);

  if (module_exists('permalink')) {
    // If Permalink is enabled, use that to get the comment # link.
    $permalink = permalink_link_render('comment', $comment);
    $variables['post_link'] = l($linktext, $permalink['href'], $permalink);
  }
  else {
    // Otherwise, link to the comment anchor on the appropriate page.
    $fragment = 'comment-' . $comment->cid;
    $query = ($page_number) ? 'page=' . $page_number : NULL;
    $linkpath = "node/$node->nid";

    $variables['post_link'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));
  }

  /* In reply to */
  $variables['in_reply_to'] = "";
  if ($comment->pid > 0) {
    // Find the display position of the parent post;.
    $post_position = advanced_forum_post_position($node->nid, $comment->pid);

    // This extra if test is a sanity check in case the comment being replied
    // to no longer exists.
    if ($post_position > 0) {
      // Find the page the post is on. We need to compensate for the topic node
      // being #1 which puts an extra post on the first page but not on the rest.
      $page_number = floor(($post_position - 2) / $posts_per_page);

      // Assemble the link.
      $fragment = 'comment-' . $comment->pid;
      $query = ($page_number) ? 'page=' . $page_number : NULL;
      $linktext = t("(Reply to #!post_position)", array('!post_position' => $post_position));
      $linkpath = "node/$node->nid";
      $variables['in_reply_to'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));
    }
  }

  /* Title */
  if (variable_get('comment_subject_field_' . $node->type, 1) == 0) {
    // if comment titles are disabled, don't display it.
    $variables['title'] = '';
  }
  else {
    // Assign the subject to the title variable for consistancy with nodes.
    $variables['title'] = check_plain($comment->subject);
  }

  /* User information / author pane */
  if ($comment->uid == 0) {
    // Anonymous user. Make a fake user object for theme_username
    $variables['account']->name = empty($comment->name) ? "" : $comment->name;
    $variables['account']->homepage = empty($comment->homepage) ? "" : $comment->homepage;
    $variables['account']->email = empty($comment->email) ? "" : $comment->email;
  }
  else {
    // Load up the real user object
    $variables['account'] = user_load(array('uid' => $comment->uid));
  }

  // Create the author pane
  if (module_exists('author_pane')) {
    $variables['author_pane'] = theme('author_pane', $variables['account'], 'advanced_forum', variable_get('advanced_forum_user_picture_preset', ''), $comment, TRUE);
  }
  else {
    $variables['author_pane'] = theme('advanced_forum_simple_author_pane', $comment);
  }

  /* Content */
  // Assign the comment to the content variable for consistancy with nodes.
  $variables['content'] = $comment->comment;

  /* Signatures */
  // Load the signature.
  if (module_exists('signature_forum')) {
    // If Signature For Forums is installed, use that
    $variables['signature'] = signature_forum_get_signature($comment);
  }
  elseif (variable_get('user_signatures', 0)) {
    if (!empty($variables['account']->signature)) {
      // Otherwise load Drupal's built in one, if enabled.
      $variables['signature'] = check_markup($variables['account']->signature, $variables['account']->signature_format, FALSE);
    }
  }

  /* Post edited */
  $variables['post_edited'] = (isset($variables['comment_edited'])) ? $variables['comment_edited'] : "";
}