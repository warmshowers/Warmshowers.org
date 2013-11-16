<?php

/**
 * @file
 *
 * Theme implementation: Template for each forum post whether node or comment.
 *
 * All variables available in node.tpl.php and comment.tpl.php for your theme
 * are available here. In addition, Advanced Forum makes available the following
 * variables:
 *
 * - $top_post: TRUE if we are formatting the main post (ie, not a comment)
 * - $reply_link: Text link / button to reply to topic.
 * - $total_posts: Number of posts in topic (not counting first post).
 * - $new_posts: Number of new posts in topic, and link to first new.
 * - $account: User object of the post author.
 * - $name: User name of post author.
 * - $author_pane: Entire contents of the Author Pane template.

 */
?>

<?php if ($top_post): ?>
  <?php print $topic_header ?>

<?php else: ?>
  <?php // If using nodecomment, add the anchor that comment normally provides ?>
  <?php if (!empty($comment_anchor)): ?>
    <?php print $comment_anchor; ?>
  <?php endif; ?>
<?php endif; ?>

<?php
// Gather other possible class list variables into ours. This must be done here
// rather than in the preprocess because themes run after the AF preprocess.
  $all_classes = "";
  if (!empty($advanced_forum_classes)) {
    $all_classes = $advanced_forum_classes;
  }

  if (!empty($classes)) {
    $all_classes .= ' ' . $classes;
  }

  if (!empty($node_classes)) {
    $all_classes .= ' ' . $node_classes;
  }

  if (!empty($comment_classes)) {
    $all_classes .= ' ' . $comment_classes;
  }
?>

<div id="<?php print $post_id; ?>" class="<?php print $all_classes; ?>">
  <div class="forum-post-info clear-block">
    <div class="forum-posted-on">
      <?php print $date ?>

      <?php print $new_marker ?>
    </div>

    <?php if (!empty($in_reply_to)): ?>
      <span class="forum-in-reply-to"><?php print $in_reply_to; ?></span>
    <?php endif; ?>

    <?php // Add a note when a post is unpublished so it doesn't rely on theming. ?>
    <?php if (!$node->status): ?>
      <span class="unpublished-post-note"><?php print t("Unpublished post") ?></span>
    <?php endif; ?>

    <span class="forum-post-number"><?php print $post_link; ?></span>
  </div> <?php // End of post info div ?>

  <div class="forum-post-wrapper">
    <div class="forum-post-panel-sub">
      <?php if (!empty($author_pane)): ?>
        <?php print $author_pane; ?>
      <?php endif; ?>
    </div>

    <div class="forum-post-panel-main clear-block">
      <?php if (!empty($title)): ?>
        <div class="forum-post-title">
          <?php print $title ?>
        </div>
      <?php endif; ?>

      <div class="forum-post-content">
        <?php print $content ?>
      </div>

      <?php if (!empty($post_edited)): ?>
        <div class="post-edited">
          <?php print $post_edited ?>
        </div>
      <?php endif; ?>

      <?php if (!empty($signature)): ?>
        <div class="author-signature">
          <?php print $signature ?>
        </div>
      <?php endif; ?>
    </div>
  </div> <?php // End of post wrapper div ?>

  <div class="forum-post-footer clear-block">
    <div class="forum-jump-links">
      <a href="#forum-topic-top" title="<?php print t('Jump to top of page'); ?>" class="af-button-small"><span><?php print t("Top"); ?></span></a>
    </div>

    <?php if (!empty($links)): ?>
      <div class="forum-post-links">
        <?php print $links ?>
      </div>
    <?php endif; ?>
  </div> <?php // End of footer div ?>
</div> <?php // End of main wrapping div ?>

<?php
// Print the taxonomy terms for this node. This will print all terms,
// including the term of the forum itself. If you don't use any other
// taxonomy on forum posts, you can safely delete this section.
?>
<?php if ($top_post): ?>
  <div class="forum-top-post-footer">
   <?php print t('Tags') ?>: <?php print $terms ?>
  </div>
<?php endif; ?>
