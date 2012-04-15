<?php

/**
 * @file
 *
 * Theme implementation: Template the preview version of a post.
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
  <a id="forum-topic-top"></a>
<?php else: ?>
  <a id="forum-reply-preview"></a>
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
    </div>
  </div>

  <div class="forum-post-wrapper">
    <div class="forum-post-panel-sub">
      <?php print $author_pane; ?>
    </div>

    <div class="forum-post-panel-main clear-block">
      <?php if ($title): ?>
        <div class="post-title">
          <?php print $title ?>
        </div>
      <?php endif; ?>

      <div class="forum-post-content">
        <?php print $content ?>
      </div>

      <?php if ($signature): ?>
        <div class="author-signature">
          <?php print $signature ?>
        </div>
      <?php endif; ?>
    </div>
  </div>

  <div class="forum-post-footer clear-block">
    <?php // Purposely empty on preview just to keep the structure intact. ?>
  </div>
</div>