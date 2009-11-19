<?php
// $Id: views_handler_field.inc 530 2009-07-19 23:14:54Z rfay $
/**
 * @defgroup views_field_handlers Views' field handlers
 * @{
 * Handlers to tell Views how to build and display fields.
 *
 */

/**
 * Base field handler that has no options and renders an unformatted field.
 *
 * Definition terms:
 * - additional fields: An array of fields that should be added to the query
 *                      for some purpose. The array is in the form of:
 *                      array('identifier' => array('table' => tablename,
 *                      'field' => fieldname); as many fields as are necessary
 *                      may be in this array.
 * - click sortable: If TRUE, this field may be click sorted.
 */
class views_handler_field extends views_handler {
  var $field_alias = 'unknown';
  var $aliases = array();

  /**
   * Construct a new field handler.
   */
  function construct() {
    parent::construct();

    $this->additional_fields = array();
    if (!empty($this->definition['additional fields'])) {
      $this->additional_fields = $this->definition['additional fields'];
    }

    if (!isset($this->options['exclude'])) {
      $this->options['exclude'] = '';
    }
  }

  /**
   * Determine if this field can allow advanced rendering.
   *
   * Fields can set this to FALSE if they do not wish to allow
   * token based rewriting or link-making.
   */
  function allow_advanced_render() {
    return TRUE;
  }

  function init(&$view, $options) {
    parent::init($view, $options);

    $this->options += array(
      'exclude' => FALSE,
    );
  }

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $this->field_alias = $this->query->add_field($this->table_alias, $this->real_field);

    $this->add_additional_fields();
  }

  /**
   * Add 'additional' fields to the query.
   *
   * @param $fields
   * An array of fields. The key is an identifier used to later find the
   * field alias used. The value is either a string in which case it's
   * assumed to be a field on this handler's table; or it's an array in the
   * form of
   * @code array('table' => $tablename, 'field' => $fieldname) @endcode
   */
  function add_additional_fields($fields = NULL) {
    if (!isset($fields)) {
      // notice check
      if (empty($this->additional_fields)) {
        return;
      }
      $fields = $this->additional_fields;
    }
    if (!empty($fields) && is_array($fields)) {
      foreach ($fields as $identifier => $info) {
        if (is_array($info)) {
          if (isset($info['table'])) {
            $table_alias = $this->query->ensure_table($info['table'], $this->relationship);
          }
          else {
            $table_alias = $this->table_alias;
          }
          $this->aliases[$identifier] = $this->query->add_field($table_alias, $info['field']);
        }
        else {
          $this->aliases[$info] = $this->query->add_field($this->table_alias, $info);
        }
      }
    }
  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $this->query->add_orderby($this->table, $this->field, $order, $this->field_alias);
  }

  /**
   * Determine if this field is click sortable.
   */
  function click_sortable() {
    return !empty($this->definition['click sortable']);
  }

  /**
   * Get this field's label.
   */
  function label() {
    if (!isset($this->options['label'])) {
      return '';
    }
    return $this->options['label'];
  }

  /**
   * Return DIV or SPAN based upon the field's element type.
   */
  function element_type() {
    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }

    return 'span';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['label'] = array('default' => $this->definition['title'], 'translatable' => TRUE);
    $options['alter'] = array(
      'contains' => array(
        'alter_text' => array('default' => FALSE),
        'text' => array('default' => '', 'translatable' => TRUE),
        'make_link' => array('default' => FALSE),
        'path' => array('default' => '', 'translatable' => TRUE),
        'alt' => array('default' => '', 'translatable' => TRUE),
        'link_class' => array('default' => ''),
        'prefix' => array('default' => '', 'translatable' => TRUE),
        'suffix' => array('default' => '', 'translatable' => TRUE),
        'trim' => array('default' => FALSE),
        'max_length' => array('default' => ''),
        'word_boundary' => array('default' => TRUE),
        'ellipsis' => array('default' => TRUE),
        'strip_tags' => array('default' => FALSE),
        'html' => array('default' => FALSE),
      ),
    );

    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => isset($this->options['label']) ? $this->options['label'] : '',
      '#description' => t('The label for this field that will be displayed to end users if the style requires it.'),
    );
    $form['exclude'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude from display'),
      '#default_value' => $this->options['exclude'],
      '#description' => t('Check this box to not display this field, but still load it in the view.  Use this option to not show a grouping field in each record, or when doing advanced theming.'),
    );

    if ($this->allow_advanced_render()) {
      $form['alter']['#tree'] = TRUE;
      $form['alter']['alter_text'] = array(
        '#type' => 'checkbox',
        '#title' => t('Rewrite the output of this field'),
        '#description' => t('If checked, you can alter the output of this field by specifying a string of text with replacement tokens that can use any existing field output.'),
        '#default_value' => $this->options['alter']['alter_text'],
      );

      $form['alter']['text'] = array(
        '#title' => t('Text'),
        '#type' => 'textarea',
        '#default_value' => $this->options['alter']['text'],
        '#description' => t('The text to display for this field. You may include HTML. You may enter data from this view as per the "Replacement patterns" below.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-alter-text' => array(1)
        ),
      );

      $form['alter']['make_link'] = array(
        '#type' => 'checkbox',
        '#title' => t('Output this field as a link'),
        '#description' => t('If checked, this field will be made into a link. The destination must be given below.'),
        '#default_value' => $this->options['alter']['make_link'],
      );
      $form['alter']['path'] = array(
        '#title' => t('Link path'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['path'],
        '#description' => t('The Drupal path or absolute URL for this link. You may enter data from this view as per the "Replacement patterns" below.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1)
        ),
      );
      $form['alter']['link_class'] = array(
        '#title' => t('Link class'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['link_class'],
        '#description' => t('The CSS class to apply to the link.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1)
        ),
      );
      $form['alter']['alt'] = array(
        '#title' => t('Alt text'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['alt'],
        '#description' => t('Text to place as "alt" text which most browsers display as a tooltip when hovering over the link.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1)
        ),
      );
      $form['alter']['prefix'] = array(
        '#title' => t('Prefix text'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['prefix'],
        '#description' => t('Any text to display before this link. You may include HTML.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1)
        ),
      );
      $form['alter']['suffix'] = array(
        '#title' => t('Suffix text'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['suffix'],
        '#description' => t('Any text to display after this link. You may include HTML.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1)
        ),
      );

      // Get a list of the available fields and arguments for token replacement.
      $options = array();
      foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
        $options[t('Fields')]["[$field]"] = $handler->ui_name();
        // We only use fields up to (and including) this one.
        if ($field == $this->options['id']) {
          break;
        }
      }
      $count = 0; // This lets us prepare the key as we want it printed.
      foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
        $options[t('Arguments')]['%' . ++$count] = $handler->ui_name();
      }

      // Default text.
      $output = t('<p>You must add some additional fields to this display before using this field. These fields may be marked as <em>Exclude from display</em> if you prefer. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
      // We have some options, so make a list.
      if (!empty($options)) {
        $output = t('<p>The following substitution patterns are available for this display. Use the pattern shown on the left to display the value indicated on the right. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
        foreach (array_keys($options) as $type) {
          if (!empty($options[$type])) {
            $items = array();
            $title = t(ucwords($type));
            foreach ($options[$type] as $key => $value) {
              $items[] = $key .' == '. $value;
            }
            $output .= theme('item_list', $items, $title);
          }
        }
      }
      // This construct uses 'hidden' and not markup because process doesn't
      // run. It also has an extra div because the dependency wants to hide
      // the parent in situations like this, so we need a second div to
      // make this work.
      $form['alter']['help'] = array(
        '#type' => 'hidden',
        '#id' => 'views-tokens-help',
        '#prefix' => '<div><fieldset id="views-tokens-help"><legend>' . t('Replacement patterns') . '</legend>' . $output . '</fieldset></div>',
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
          'edit-options-alter-alter-text' => array(1),
        ),
      );

      $form['alter']['trim'] = array(
        '#type' => 'checkbox',
        '#title' => t('Trim this field to a maximum length'),
        '#description' => t('If checked, this field be trimmed to a maximum length in characters.'),
        '#default_value' => $this->options['alter']['trim'],
      );

      $form['alter']['max_length'] = array(
        '#title' => t('Maximum length'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['max_length'],
        '#description' => t('The maximum number of characters his field can be.'),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1)
        ),
      );

      $form['alter']['word_boundary'] = array(
        '#type' => 'checkbox',
        '#title' => t('Trim only on a word boundary'),
        '#description' => t('If checked, this field be trimmed only on a word boundary. This is guaranteed to be the maximum characters stated or less. If there are no word boundaries this could trim a field to nothing.'),
        '#default_value' => $this->options['alter']['word_boundary'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1)
        ),
      );

      $form['alter']['ellipsis'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add an ellipsis'),
        '#description' => t('If checked, a "..." will be added if a field was trimmed.'),
        '#default_value' => $this->options['alter']['ellipsis'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1)
        ),
      );

      $form['alter']['strip_tags'] = array(
        '#type' => 'checkbox',
        '#title' => t('Strip HTML tags'),
        '#description' => t('If checked, all HTML tags will be stripped.'),
        '#default_value' => $this->options['alter']['strip_tags'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1)
        ),
      );

      $form['alter']['html'] = array(
        '#type' => 'checkbox',
        '#title' => t('Field can contain HTML'),
        '#description' => t('If checked, HTML corrector will be run to ensure tags are properly closed after trimming.'),
        '#default_value' => $this->options['alter']['html'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1)
        ),
      );
    }
  }

  /**
   * Provide extra data to the administration form
   */
  function admin_summary() {
    return $this->label();
  }

  /**
   * Run before any fields are rendered.
   *
   * This gives the handlers some time to set up before any handler has
   * been rendered.
   *
   * @param $values
   *   An array of all objects returned from the query.
   */
  function pre_render($values) { }

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    $value = $values->{$this->field_alias};
    return check_plain($value);
  }

  /**
   * Render a field using advanced settings.
   *
   * This renders a field normally, then decides if render-as-link and
   * text-replacement rendering is necessary.
   */
  function advanced_render($values) {
    $this->last_render = $value = $this->render($values);
    $this->original_value = $value;

    if ($this->allow_advanced_render()) {
      $tokens = NULL;
      if (!empty($this->options['alter']['alter_text']) && !empty($this->options['alter']['text'])) {
        $tokens = $this->get_render_tokens();
        $value = $this->render_altered($tokens);
      }

      if (!empty($this->options['alter']['trim']) && !empty($this->options['alter']['max_length'])) {
        $value = $this->render_trim_text($value);
      }

      if (!empty($this->options['alter']['make_link']) && !empty($this->options['alter']['path'])) {
        if (!isset($tokens)) {
         $tokens = $this->get_render_tokens();
        }
        $value = $this->render_as_link($value, $tokens);
      }

      // This happens here so that render_as_link can get the unaltered value of
      // this field as a token rather than the altered value.
      $this->last_render = $value;
    }

    return $this->last_render;
  }

  /**
   * Render this field as altered text, from a fieldset set by the user.
   */
  function render_altered($tokens) {
    // Filter this right away as our substitutions are already sanitized.
    $value = filter_xss_admin($this->options['alter']['text']);
    $value = strtr($value, $tokens);

    return $value;
  }

  /**
   * Trim the field down to the specified length.
   */
  function render_trim_text($value) {
    if (!empty($this->options['alter']['strip_tags'])) {
      $value = strip_tags($value);
      // NOTE: It's possible that some external fields might override the
      // element type so if someone from, say, CCK runs into a bug here,
      // this may be why =)
      $this->definition['element type'] = 'span';
    }

    if (drupal_strlen($value) <= $this->options['alter']['max_length']) {
      return $value;
    }

    $value = drupal_substr($value, 0, $this->options['alter']['max_length']);

    if (!empty($this->options['alter']['word_boundary'])) {
      if (preg_match("/(.*)\b.+/us", $value, $matches)) {
        $value = $matches[1];
      }
    }
    // Remove scraps of HTML entities from the end of a strings
    $value = rtrim(preg_replace('/(?:<(?!.+>)|&(?!.+;)).*$/us', '', $value));

    if (!empty($this->options['alter']['ellipsis'])) {
      $value .= '...';
    }

    if (!empty($this->options['alter']['html'])) {
      $value = _filter_htmlcorrector($value);
    }

    return $value;
  }

  /**
   * Render this field as a link, with the info from a fieldset set by
   * the user.
   */
  function render_as_link($text, $tokens) {
    $value = '';

    if (!empty($this->options['alter']['prefix'])) {
      $value .= filter_xss_admin($this->options['alter']['prefix']);
    }

    $options = array(
      'html' => 'true',
    );

    // $path will be run through check_url() by l() so we do not need to
    // sanitize it ourselves.
    $path = $this->options['alter']['path'];

    // Use strip tags as there should never be HTML in the path.
    $path = strip_tags(strtr($path, $tokens));

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

    $alt = $this->options['alter']['alt'];
    $alt = strtr($alt, $tokens);
    if ($alt) {
      $options['attributes']['title'] = $alt;
      $options['attributes']['alt'] = $alt;
    }

    $class = $this->options['alter']['link_class'];
    if ($class) {
      $options['attributes']['class'] = $class;
    }

    // If the query and fragment were programatically assigned overwrite any
    // parsed values.
    if (isset($this->options['alter']['query'])) {
      $options['query'] = $this->options['alter']['query'];
    }
    if (isset($this->options['alter']['fragment'])) {
      $options['fragment'] = $this->options['alter']['fragment'];
    }

    $value .= l($text, $path, $options);

    if (!empty($this->options['alter']['suffix'])) {
      $value .= filter_xss_admin($this->options['alter']['suffix']);
    }

    return $value;
  }

  /**
   * Get the 'render' tokens to use for advanced rendering.
   *
   * This runs through all of the fields and arguments that
   * are available and gets their values. This will then be
   * used in one giant str_replace().
   */
  function get_render_tokens() {
    $tokens = array();
    if (!empty($this->view->build_info['substitutions'])) {
      $tokens = $this->view->build_info['substitutions'];
    }
    $count = 0;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $token = '%' . ++$count;
      if (!isset($tokens[$token])) {
        $tokens[$token] = '';
      }
    }

    // Now add replacements for our fields.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      if (isset($handler->last_render)) {
        $tokens["[$field]"] = $handler->last_render;
      }
      else {
        $tokens["[$field]"] = '';
      }
      // We only use fields up to (and including) this one.
      if ($field == $this->options['id']) {
        break;
      }
    }

    return $tokens;
  }

  /**
   * Call out to the theme() function, which probably just calls render() but
   * allows sites to override output fairly easily.
   */
  function theme($values) {
    return theme($this->theme_functions(), $this->view, $this, $values);
  }

  function theme_functions() {
    $themes = array();
    $hook = 'views_view_field';

    $display = $this->view->display[$this->view->current_display];

    if (!empty($display)) {
      $themes[] = $hook . '__' . $this->view->name  . '__' . $display->id . '__' . $this->options['id'];
      $themes[] = $hook . '__' . $this->view->name  . '__' . $display->id;
      $themes[] = $hook . '__' . $display->id . '__' . $this->options['id'];
      $themes[] = $hook . '__' . $display->id;
      if ($display->id != $display->display_plugin) {
        $themes[] = $hook . '__' . $this->view->name  . '__' . $display->display_plugin . '__' . $this->options['id'];
        $themes[] = $hook . '__' . $this->view->name  . '__' . $display->display_plugin;
        $themes[] = $hook . '__' . $display->display_plugin . '__' . $this->options['id'];
        $themes[] = $hook . '__' . $display->display_plugin;
      }
    }
    $themes[] = $hook . '__' . $this->view->name . '__' . $this->options['id'];
    $themes[] = $hook . '__' . $this->view->name;
    $themes[] = $hook . '__' . $this->options['id'];
    $themes[] = $hook;

    return $themes;
  }
}

/**
 * A special handler to take the place of missing or broken handlers.
 */
class views_handler_field_broken extends views_handler_field {
  function ui_name() {
    return t('Broken/missing handler');
  }

  function ensure_my_table() { /* No table to ensure! */ }
  function query() { /* No query to run */ }
  function options_form(&$form, &$form_state) {
    $form['markup'] = array(
      '#prefix' => '<div class="form-item description">',
      '#value' => t('The handler for this item is broken or missing and cannot be used. If a module provided the handler and was disabled, re-enabling the module may restore it. Otherwise, you should probably delete this item.'),
    );
  }

  /**
   * Determine if the handler is considered 'broken'
   */
  function broken() { return TRUE; }
}

/**
 * Render a numeric value as a size.
 */
class views_handler_field_file_size extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['file_size_display'] = array('default' => 'formatted');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['file_size_display'] = array(
      '#title' => t('File size display'),
      '#type' => 'select',
      '#options' => array(
        'formatted' => t('Formatted (in KB or MB)'),
        'bytes' => t('Raw bytes'),
      ),
    );
  }

  function render($values) {
    if ($values->{$this->field_alias}) {
      switch ($this->options['file_size_display']) {
        case 'bytes':
          return $values->{$this->field_alias};
        case 'formatted':
        default:
          return format_size($values->{$this->field_alias});
      }
    }
    else {
      return '';
    }
  }
}

/**
 * A handler to run a field through simple XSS filtering
 */
class views_handler_field_xss extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return filter_xss($value);
  }
}

/**
 * @}
 */

