<?php

/**
 * Filter by term id.
 */
class i18nviews_handler_filter_term_node_tid extends views_handler_filter_term_node_tid {

  function value_form(&$form, &$form_state) {
    $vocabulary = taxonomy_vocabulary_load($this->options['vid']);
    if (empty($vocabulary) && $this->options['limit']) {
      $form['markup'] = array(
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#value' => t('An invalid vocabulary is selected. Please change it in the options.'),
      );
      return;
    }

    if ($this->options['type'] == 'textfield') {
      $default = '';
      if ($this->value) {
        $result = db_query("SELECT * FROM {term_data} td WHERE td.tid IN (" . implode(', ', $this->value) . ')');
        while ($term = db_fetch_object($result)) {
          if ($default) {
            $default .= ', ';
          }
          $default .= tt('taxonomy:term:'. $term->tid .':name', $term->name);
        }
      }

      $form['value'] = array(
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
        '#type' => 'textfield',
        '#default_value' => $default,
      );

      if ($this->options['limit']) {
        $form['value']['#autocomplete_path'] = 'taxonomy/autocomplete/' . $vocabulary->vid;
      }
    }
    else {
      if (!empty($this->options['hierarchy']) && $this->options['limit']) {
        $tree = taxonomy_get_tree($vocabulary->vid);
        $options = array();

        if ($tree) {
          foreach ($tree as $term) {
            $choice = new stdClass();
            $choice->option = array($term->tid => str_repeat('-', $term->depth) . tt('taxonomy:term:'. $term->tid .':name', $term->name));
            $options[] = $choice;
          }
        }
      }
      else {
        $options = array();
        if ($this->options['limit']) {
          $result = db_query("SELECT * FROM {term_data} WHERE vid = %d ORDER BY weight, name", $vocabulary->vid);
        }
        else {
          $result = db_query("SELECT td.* FROM {term_data} td INNER JOIN {vocabulary} v ON td.vid = v.vid ORDER BY v.weight, v.name, td.weight, td.name");
        }
        while ($term = db_fetch_object($result)) {
          $options[$term->tid] = tt('taxonomy:term:'. $term->tid .':name', $term->name);
        }
      }

      $default_value = (array) $this->value;

      if (!empty($form_state['exposed'])) {
        $identifier = $this->options['expose']['identifier'];

        if (!empty($this->options['expose']['reduce'])) {
          $options = $this->reduce_value_options($options);

          if (empty($this->options['expose']['single']) && !empty($this->options['expose']['optional'])) {
            $default_value = array();
          }
        }

        if (!empty($this->options['expose']['single'])) {
          if (!empty($this->options['expose']['optional']) && (empty($default_value) || !empty($this->options['expose']['reduce']))) {
            $default_value = 'All';
          }
          else if (empty($default_value)) {
            $keys = array_keys($options);
            $default_value = array_shift($keys);
          }
          else {
            $copy = $default_value;
            $default_value = array_shift($copy);
          }
        }
      }
      $form['value'] = array(
        '#type' => 'select',
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
        '#multiple' => TRUE,
        '#options' => $options,
        '#size' => min(9, count($options)),
        '#default_value' => $default_value,
      );

      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $default_value;
      }
    }


    if (empty($form_state['exposed'])) {
      // Retain the helper option
      $this->helper->options_form($form, $form_state);
    }
  }

  /**
   * Validate the user string. Since this can come from either the form
   * or the exposed filter, this is abstracted out a bit so it can
   * handle the multiple input sources.
   */
  function validate_term_strings(&$form, $values) {
    if (empty($values)) {
      return array();
    }

    $tids = array();
    $placeholders = array();
    $args = array();
    $results = array();
    foreach ($values as $value) {
      $missing[strtolower($value)] = TRUE;
      $args[] = $value;
      $placeholders[] = "'%s'";
    }

    if (!$args) {
      return;
    }

    // add the taxonomy vid to the argument list.
    $args[] = $this->options['vid'];

    $result = db_query("SELECT * FROM {term_data} WHERE name IN (" . implode(', ', $placeholders) . ") AND vid = %d", $args);
    while ($term = db_fetch_object($result)) {
      unset($missing[strtolower(tt('taxonomy:term:'. $term->tid .':name', $term->name))]);
      $tids[] = $term->tid;
    }

    if ($missing) {
      form_error($form, format_plural(count($missing), 'Unable to find term: @terms', 'Unable to find terms: @terms', array('@terms' => implode(', ', array_keys($missing)))));
    }

    return $tids;
  }

  function admin_summary() {
    // set up $this->value_options for the parent summary
    $this->value_options = array();

    if ($this->value) {
      $result = db_query("SELECT * FROM {term_data} td WHERE td.tid IN ("  . implode(', ', $this->value) . ")");

      while ($term = db_fetch_object($result)) {
        $this->value_options[$term->tid] = tt('taxonomy:term:'. $term->tid .':name', $term->name);
      }
    }
    return parent::admin_summary();
  }
}
