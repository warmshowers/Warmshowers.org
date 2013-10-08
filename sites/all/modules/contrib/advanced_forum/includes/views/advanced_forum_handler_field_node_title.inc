<?php

/**
 * @file
 * Field handler for node title, which overrides the render_as_link function
 * The purpose of doing so is to avoid character encoding errors caused by views.
 *  @see: http://drupal.org/node/1361526
 * If the problem is fixed in views, this can be removed.
 */

class advanced_forum_handler_field_node_title extends views_handler_field_node {

  /**
   * Render this field as a link, with the info from a fieldset set by
   * the user.
   */
  function render_as_link($alter, $text, $tokens) {
    $value = '';

    if (!empty($alter['prefix'])) {
      $value .= filter_xss_admin(strtr($alter['prefix'], $tokens));
    }

    $options = array(
      'html' => 'true',
    );

    // $path will be run through check_url() by l() so we do not need to
    // sanitize it ourselves.
    $path = $alter['path'];

    // html_entity_decode removes <front>, so check whether its different to front.
    if ($path != '<front>') {
      // Use strip tags as there should never be HTML in the path.
      // However, we need to preserve special characters like " that
      // were removed by check_plain().
      $path = strip_tags(html_entity_decode(strtr($path, $tokens)));
    }

    // If the path is empty do not build a link around the given text and return
    // it as is.
    if (empty($path)) {
      return $text;
    }

    // Parse the URL and move any query and fragment parameters out of the path.
    $url = parse_url($path);
    if (isset($url['query'])) {
      $path = strtr($path, array('?' . $url['query'] => ''));
      $options['query'] = $url['query'];
    }
    if (isset($url['fragment'])) {
      $path = strtr($path, array('#' . $url['fragment'] => ''));
      $options['fragment'] = $url['fragment'];
    }

    $alt = strtr($alter['alt'], $tokens);
    // Set the title attribute of the link only if it improves accessibility
    if ($alt && $alt != $text) {
      $options['attributes']['title'] = html_entity_decode($alt, ENT_QUOTES, 'UTF-8');
    }

    $class = strtr($alter['link_class'], $tokens);
    if ($class) {
      $options['attributes']['class'] = $class;
    }

    $target = check_plain(trim(strtr($alter['target'],$tokens)));
    if (!empty($target)) {
      $options['attributes']['target'] = $target;
    }

    // If the query and fragment were programatically assigned overwrite any
    // parsed values.
    if (isset($alter['query'])) {
      $options['query'] = strtr($alter['query'], $tokens);
    }
    if (isset($alter['alias'])) {
      // Alias is a boolean field, so no token.
      $options['alias'] = $alter['alias'];
    }
    if (isset($alter['fragment'])) {
      $options['fragment'] = strtr($alter['fragment'], $tokens);
    }
    if (isset($this->options['alter']['language'])) {
      $options['language'] = $this->options['alter']['language'];
    }

    $value .= l($text, $path, $options);

    if (!empty($alter['suffix'])) {
      $value .= filter_xss_admin(strtr($alter['suffix'], $tokens));
    }

    return $value;
  }

}

