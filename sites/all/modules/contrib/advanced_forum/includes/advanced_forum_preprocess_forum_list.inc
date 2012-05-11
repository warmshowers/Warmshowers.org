<?php

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forum_list(&$variables) {
  // Tell Drupal to use our forum list template file.
  advanced_forum_add_template_suggestions("forum-list", $variables['template_files']);

  // Add needed items for the collapsible containers.
  $variables['collapsible'] = variable_get('advanced_forum_collapsible_containers', 'slide');
  if ($variables['collapsible'] != 'none') {
    drupal_add_js(drupal_get_path('module', 'advanced_forum') . '/js/advanced_forum.js', 'module');
    drupal_add_js(array('advanced_forum' => array('effect' => $variables['collapsible'])), 'setting');
  }

  // The tid is the ID of the container or forum we are in. Assign it to
  // $parent_id for easy reference, assign it to the forum_id template variable
  // to give it a nice name for themers, then get rid of the original variable.
  $parent_id = isset($variables['tid']) ? $variables['tid'] : 0;
  $variables['forum_id'] = $parent_id;
  unset($variables['tid']);

  // Add a variable showning if we are using taxonomy image for easier theming.
  $variables['use_taxonomy_image'] = (function_exists('taxonomy_image_display') && variable_get('advanced_forum_use_taxonomy_image', TRUE));

  // Process the containers and forums and move them to a new variable.
  $items = array();
  $lowest_depth = 0;
  foreach ($variables['forums'] as $id => $forum) {
    if (empty($forum->container)) {
      $items[$id] = advanced_forum_process_forum($forum);
    }
    else {
      $items[$id] = advanced_forum_process_container($forum);
    }

    // Figure out how deep the hierarchy goes for the next step.
    if ($forum->depth > $lowest_depth) {
      $lowest_depth = $forum->depth;
    }
  }

  for ($depth = $lowest_depth; $depth > 0; $depth--)
    foreach ($items as $id => $item) {
      if ($item->depth == $depth) {
        $items[$item->parents['0']]->child_total_topics += $item->total_topics + $item->child_total_topics;
        $items[$item->parents['0']]->child_new_topics += $item->new_topics + $item->child_new_topics;
        $items[$item->parents['0']]->child_total_posts += $item->total_posts + $item->child_total_posts;
        $items[$item->parents['0']]->child_new_posts += $item->new_posts + $item->child_new_posts;
    }
  }

  // Loop through all the items and fill the $tables variable that will
  // hold all the tables with the containers and forums organized, processed,
  // and ready for the template.

  if ($parent_id) {
    // On a container page. Fake the main table.
    $table_counter = 1;
    $tables[$table_counter]['table_info'] = advanced_forum_process_container($variables['parents'][0]);
  }
  else {
    $table_counter = 0;
  }

  foreach ($items as $id => $item) {
    // Get a handle on the parent of this item.
    if ($parent_id && $item->depth == 0) {
      // If we are on a container page, the parent of root items is the main
      // container that is being used for the table info.
      $parent = $tables[$table_counter]['table_info'];
    }
    elseif (!empty($variables['forums'][$item->parents[0]])) {
      // For simplicity, we assume forums/containers have only one parent.
      $parent = $variables['forums'][$item->parents[0]];
    }
    // Intentional no "else" because only root items on the main page have
    // no parent and we don't use the variable there.

    // If we aren't on the main forum page, we need to bump up the depth.
    $item_depth = ($parent_id) ? $item->depth + 1 : $item->depth;

    if (!empty($item->container)) {
      // CONTAINERS.

      if ($item_depth == 0) {
        // Top level container always starts a new table.
        $table_counter++;
        $tables[$table_counter]['table_info'] = $item;
        $tables[$table_counter]['items'] = array();

        // Reset the striping.
        advanced_forum_stripe(TRUE);
      }
      elseif ($item_depth == 1) {
        // Subcontainer at top level is treated like a forum.

        // We set the forum icon here, rather than in the process_forum
        // function because we need to take into account new content
        // in children.
        if ($item->new_topics || $item->new_posts || $item->child_new_topics || $item->child_new_posts) {
          $item->icon_classes = "forum-list-icon forum-list-icon-new-posts";
          $item->icon_text = t("New posts");
        }
        else {
          $item->icon_classes = "forum-list-icon forum-list-icon-default";
          $item->icon_text = t("No new");
        }

        // Set the variable to control the row striping.
        $item->zebra = advanced_forum_stripe();

        // Add the container info to the table's item list.
        $tables[$table_counter]['items'][$id] = $item;
      }
      elseif ($item_depth == 2) {
        // A container elsewhere gets added to the parent's subcontainer list.
        $tables[$table_counter]['items'][$parent->tid]->subcontainer_list[$id] = $item;
      }
    }
    else {
      // FORUMS.

      // We set the forum icon here, rather than in the process_forum
      // function because we need to take into account new content
      // in children.
      if ($item->new_topics || $item->new_posts || $item->child_new_topics || $item->child_new_posts) {
        $item->icon_classes = "forum-list-icon forum-list-icon-new-posts";
        $item->icon_text = t("New posts");
      }
      else {
        $item->icon_classes = "forum-list-icon forum-list-icon-default";
        $item->icon_text = t("No new");
      }

      if ($item_depth == 0) {
        // This is a forum at the root. If it is the first or the first
        // since the last container, make a new table. Otherwise, put it in
        // the previous table.
        if (empty($table_counter) || !empty($tables[$table_counter]['table_info']->container)) {
          // This is the first root item or the current table belongs to a
          // container. Start a new generic one. We need a tid so just grab
          // the tid of the current forum for it.
          $table_counter++;
          $tables[$table_counter]['table_info']->name = t('Forums');
          $tables[$table_counter]['table_info']->description = '';
          $tables[$table_counter]['table_info']->tid = $item->tid;

          // Reset the striping.
          advanced_forum_stripe(TRUE);
        }

        // Set the variable to control the row striping.
        $item->zebra = advanced_forum_stripe();

        // Add the forum info to the table's item list.
        $tables[$table_counter]['items'][$id] = $item;
      }
      elseif ($item_depth == 1) {
        // Main forum. Add it to the item list.
        if (empty($item->container)) {
          $item->zebra = advanced_forum_stripe();
        }

        $tables[$table_counter]['items'][$id] = $item;
      }
      elseif ($item_depth == 2) {
        // Subforum.
        $tables[$table_counter]['items'][$parent->tid]->subforum_list[$id] = $item;
      }
    }
  }

  // Theme subcontainers, subforums and add post counts to parents.
  foreach ($tables as $table_id => $table) {
    foreach ($table['items'] as $item_id => $item) {
      if (!empty($item->subforum_list)) {
        $tables[$table_id]['items'][$item_id]->subforums = theme('advanced_forum_subforum_list', $item->subforum_list);
      }

      if (!empty($item->subcontainer_list)) {
        $tables[$table_id]['items'][$item_id]->subcontainers = theme('advanced_forum_subcontainer_list', $item->subcontainer_list);
      }
    }
  }

  $variables['tables'] = $tables;

  // Remove unneeded variables.
  unset($variables['zebra']);
  unset($variables['forums']);
}


function advanced_forum_stripe($reset = FALSE) {
  static $stripe = 'odd';

  if ($reset) {
    $stripe = 'odd';
  }
  else {
    $stripe = ($stripe == 'odd') ? 'even' : 'odd';
  }

  return $stripe;
}

/**
 * Prepare an individual container for display.
 */
function advanced_forum_process_container($container) {
  // Create the link to the container.
  $container->link = url("forum/$container->tid");

  // Sanitise the name and description so they can be safely printed.
  $container->name = check_plain($container->name);
  $container->description = !empty($container->description) ? filter_xss_admin($container->description) : '';

  // Create a variable to check if the item is a container in the template.
  $container->is_container = TRUE;

  // @TODO: Make the icon change if subforums have posts.
  $container->icon_classes = "forum-list-icon forum-list-icon-default";
  $container->icon_text = t("No new");

  // Add in the taxonomy image, if any.
  if (function_exists('taxonomy_image_display') && variable_get('advanced_forum_use_taxonomy_image', TRUE)) {
    $container->forum_image = taxonomy_image_display($container->tid);
  }

  // Initialize these variables to avoid notices later.
  $container->total_topics = 0;
  $container->new_topics = 0;
  $container->total_posts = 0;
  $container->new_posts = 0;
  $container->child_total_topics = 0;
  $container->child_new_topics = 0;
  $container->child_total_posts = 0;
  $container->child_new_posts = 0;

  return $container;
}

/**
 * Prepare an individual forum for display.
 */
function advanced_forum_process_forum($forum) {
  // Create a variable to check if the item is a container in the template.
  $forum->is_container = FALSE;

  // Create the link to the forum.
  $forum->link = url("forum/$forum->tid");

  // Sanitise the name and description so they can be safely printed.
  // We don't do this for subforum names because that is sent through l()
  // in the theme function which runs it through check_plain().
  $forum->name = empty($forum->parents[0]) ? check_plain($forum->name) : $forum->name;
  $forum->description = !empty($forum->description) ? filter_xss_admin($forum->description) : '';

  // Initialize these variables to avoid notices later since not all forums
  // have new content or even any content at all.
  $forum->total_topics = 0;
  $forum->child_total_topics = 0;

  $forum->new_topics = 0;
  $forum->new_topics_text = '';
  $forum->new_topics_link = '';
  $forum->child_new_topics = 0;

  $forum->total_posts = 0;
  $forum->child_total_posts = 0;

  $forum->new_posts = 0;
  $forum->new_posts_text = '';
  $forum->new_posts_link = '';
  $forum->child_new_posts = 0;

  // Rename these to make them more descriptive.
  if (isset($forum->num_topics)) {
    $forum->total_topics = $forum->num_topics;
    unset($forum->num_topics);
  }

  if (isset($forum->num_posts)) {
    $forum->total_posts = $forum->num_posts;
    unset($forum->num_posts);
  }

  // If the viewer is authenticated, check for new topics and posts.
  global $user;
  if ($user->uid) {
    // New topics.
    $forum->new_topics = _forum_topics_unread($forum->tid, $user->uid);
    if ($forum->new_topics) {
      $forum->new_topics_text = format_plural($forum->new_topics, '1 new', '@count new');
      $forum->new_topics_link = url("forum/$forum->tid", array('fragment' => 'new'));
    }

    // New posts are optional because the query is slow.
    if (variable_get('advanced_forum_get_new_comments', 0)) {
      $forum->new_posts = advanced_forum_unread_replies_in_forum($forum->tid, $user->uid) + $forum->new_topics;

      if ($forum->new_posts) {
        $forum->new_posts_text = format_plural($forum->new_posts, '1 new', '@count new');
        $forum->new_posts_link = url("forum/$forum->tid", array('fragment' => 'new'));
      }
    }
  }

  // Process the "last post" object into a printable string.
  // Trying to copy the string back into the variable directly caused odd bugs
  // so we move it to a temp variable then unset the original.
  $last_post = (empty($forum->last_post)) ? '' : $forum->last_post;
  unset($forum->last_post);
  $forum->last_post = theme('forum_submitted', $last_post);

  // Add in the taxonomy image, if any
  if (function_exists('taxonomy_image_display') && variable_get('advanced_forum_use_taxonomy_image', TRUE)) {
    $forum->forum_image = taxonomy_image_display($forum->tid);
  }

  return $forum;
}
