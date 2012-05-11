<?php

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_node(&$variables) {
  /* Shortcut variables */
  $node = $variables['node'];

  $node_parent_id = '';
  if (!empty($variables['node']->comment_target_nid)) {
    // This is a Node Comments reply node so grab the parent id and load the
    // parent node for use below.
    $node_parent_id = $variables['node']->comment_target_nid;
    $parent_node = node_load($node_parent_id);
  }

  /* Determine the template file to use for the node. */
  if (!$node_parent_id && isset($_GET['page']) && $_GET['page'] > 0) {
    // This is the repeated node on the top of subsequent pages.
    // We send this to a special .tpl so people can adjust it to their needs.
    advanced_forum_add_template_suggestions("post-repeated", $variables['template_files']);
  }
  elseif (arg(1) == 'reply' || (arg(0) == 'node' && arg(1) == 'add') || $variables['node']->build_mode == NODE_BUILD_PREVIEW) {
    // 'reply' deals with the first post being shown when adding a comment.
    // 'node/add' is when previewing a new forum post.
    advanced_forum_add_template_suggestions("post-preview", $variables['template_files']);
  }
  elseif (arg(0) != 'node' || arg(1) < 1) {
    // We aren't on the node page so use the teaser template.
    advanced_forum_add_template_suggestions("post-teaser", $variables['template_files']);
  }
  else {
    // Use our combined node/comment template file
    advanced_forum_add_template_suggestions("post", $variables['template_files']);
  }

  /* Topic header */
  // Check if this node is the first post, aka topic head. When using the
  // comment module, this will always be true because replies are not nodes
  // but nodecomment will use this same preprocess for all posts.
  $variables['top_post'] = '';
  if (!$node_parent_id) {
    $variables['top_post'] = TRUE;

    // Build the topic header
    $variables['topic_header'] = theme('advanced_forum_topic_header', $node, $variables['comment_count']);
  }

  /* Assemble all the classes & id that go on the main div. */
  $classes = array();

  // Add in our classes.
  $classes[] = 'forum-post clear-block';

  // Add the current language to the classes for image handling.
  global $language;
  if (!empty($language->language)) {
    $classes[] = $language->language;
  }

   // Add the poster's UID
  $classes[] = "posted-by-$node->uid";

  // Add class if the poster is the topic starter. This is only applicable
  // on Node Comment reply nodes as core comments are handled in the comment
  // template.
  if ($node_parent_id && $node->uid > 0 && $node->uid == $parent_node->uid) {
    $classes[] = "post-by-starter";
  }

  // Create the template variable.
  $variables['advanced_forum_classes'] = implode(' ', $classes);

  /* New marker */
  // This is dealt with in the caching section. Initialized here to avoid
  // notices on the top post.
  $variables['new_marker'] = '';

  /* Post ID */
  // Set the post ID for theming / targetting
  $variables['post_id'] = "post-$node->nid";

  /* Coment anchor */
  if ($node_parent_id) {
    // We need to add this because nodecomment puts this in the template
    // that we are overriding whereas the comment module adds it in code
    // outside of the template and it will be there twice if we always add it.
    // Must use 'comment' instead of 'post' or 'reply' to match NC & core.
    $variables['comment_anchor'] = '<a id="comment-' . $node->nid . '"></a>';
  }

  /* Linked post number */
  if (!isset($post_number)) {
    static $post_number = 1;
  }

  $page_number = !empty($_GET['page']) ? $_GET['page'] : 0;

  if (!$node_parent_id) {
    // If this is the topic starting node, we need to start off the numbering.
    $variables['post_link'] = l('#1', "node/$node->nid");
  }
  else {
    // This is a nodecomment node so we want to do the comment counting.
    $posts_per_page = _comment_get_display_setting('comments_per_page', $parent_node);
    $post_number++;

    $linktext = '#' . (($page_number * $posts_per_page) + $post_number);
    $linkpath = "node/$parent_node->nid";
    $query = ($page_number) ? 'page=' . $page_number : NULL;
    $fragment = 'comment-' . $node->nid;

    $variables['post_link'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));
  }

  /* In reply to */
  $variables['in_reply_to'] = "";
  if (!empty($node->comment_target_cid)) {
    // Find the display position of the parent post;.
    $post_position = advanced_forum_post_position($node_parent_id, $node->comment_target_cid);

    // This extra if test is a sanity check in case the comment being replied
    // to no longer exists.
    if ($post_position > 0) {
      // Find the page the post is on. We need to compensate for the topic node
      // being #1 which puts an extra post on the first page but not on the rest.
      $page_number = floor(($post_position - 2) / $posts_per_page);

      // Assemble the link.
      $fragment = 'comment-' . $node->comment_target_cid;
      $query = ($page_number) ? 'page=' . $page_number : NULL;
      $linktext = t("(Reply to #!post_position)", array('!post_position' => $post_position));
      $linkpath = "node/$parent_node->nid";
      $variables['in_reply_to'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));
    }
  }

  /* User information / author pane */
  $variables['account'] = user_load(array('uid' => $node->uid));

  if (!module_exists('author_pane')) {
    // If not using the Author Pane module, create a simple version with just
    // name and photo. If using AP, that's handled down in the
    // "uncached variables" section.
    $variables['author_pane'] = theme('advanced_forum_simple_author_pane', $node);
  }

  /* Reply link */
  if (!$node_parent_id) {
    // Build the reply link / button. This isn't used in the default template
    // but is provided as a convenience to people who want to customize theirs.
    $variables['reply_link'] = theme('advanced_forum_reply_link', $node);
  }

  /* Signatures */
  if (module_exists('signature_forum')) {
    // If Signatures for Forums is installed, use that.
    // Note: we cannot get around the signature being cached because we would
    // need to do a full node load to get all the info SfF needs to replace
    // the token and that would counteract the caching.
    $variables['signature'] = signature_forum_get_signature($node);
  }

  /* Uncached variables */
  // Node Comment caches the whole topic for performance reasons. This is
  // incompatable with any variables that change based on who's viewing the
  // topic. To get around this, we place a token here and replace it in post
  // rendering. Only do this if post rendering is supported and it's not a
  // preview of nodecomment.
  if (!$variables['top_post'] && method_exists('views_plugin_cache', 'post_render') && $variables['node']->build_mode != NODE_BUILD_PREVIEW) {
    /* Author Pane */
    if (module_exists('author_pane')) {
      $variables['author_pane'] = '<!--post:author-pane-' . $node->uid . '-->';
    }

    /* Revisions */
    if ($variables['created'] != $variables['revision_timestamp']) {
      $variables['post_edited'] = '<!--post:post-edited-' . $node->nid . '-->';
    }

    /* Signatures */
    if (!module_exists('signature_forum') && variable_get('user_signatures', 0)) {
      // Signature for Forums module not enabled; load Drupal's built in one,
      // if enabled.
      $variables['signature'] =  '<!--post:signature-core-' . $node->uid . '-->';
    }

    /* User specific node theming class */
    // Add class if the poster is the viewer.
    $variables['advanced_forum_classes'] .= ' <!--post:poster-id-' . $node->uid . '-->';

    /* New marker */
    // This is handled by Node Comments. Just make sure the tokens are printed.
    $variables['new_marker'] = $variables['first_new'] . ' ' . $variables['new_output'];
  }
  else {
    // Create variables normally.
    /* Author Pane */
    if (module_exists('author_pane')) {
      $variables['author_pane'] = theme('author_pane', $variables['account'], 'advanced_forum', variable_get('advanced_forum_user_picture_preset', ''), $node, TRUE);
    }

    /* Revisions */
    if (!empty($variables['revision_timestamp']) && user_access('view last edited notice') && $variables['created'] != $variables['revision_timestamp']) {
      $variables['post_edited'] = theme('advanced_forum_post_edited', $variables['revision_uid'], $variables['revision_timestamp'], $variables['log']);
    }

    /* Signatures */
    if (!module_exists('signature_forum') && variable_get('user_signatures', 0)) {
      if ($variables['account']->signature) {
        $variables['signature'] = check_markup($variables['account']->signature, $variables['account']->signature_format, FALSE);
      }
    }

    /* Viewer is poster class */
    global $user;
    if ($user->uid > 0 && $user->uid == $node->uid) {
      $variables['advanced_forum_classes'] .= " post-by-viewer";
    }
  }
}