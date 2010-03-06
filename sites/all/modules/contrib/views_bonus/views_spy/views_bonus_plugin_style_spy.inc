<?php
// $Id: views_bonus_plugin_style_spy.inc,v 1.1 2009/02/16 15:20:38 manuelgarcia Exp $
/**
 * @file
 * Provide an spy style plugin for Views. This file is autoloaded by views.
 */

/**
  * Implementation of views_plugin_style().
  */
class views_bonus_plugin_style_spy extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['limit'] = array('default' => 4);
    $options['interval'] = array('default' => 6);
    $options['fadespeed'] = array('default' => 1);
    $options['slidespeed'] = array('default' => 1);
    $options['autopause'] = array('default' => 1);
    $options['include-style'] = array('default' => 1);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['limit'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#maxlength' => 5,
      '#required' => TRUE,
      '#title' => t('Items to show'),
      '#default_value' => $this->options['limit'],
      '#description' => t('How many items to show all the time. Must be lower than the items being shown by the view.'),
    );
    $form['interval'] = array(
      '#type' => 'textfield',
      '#title' => t('Transition time'),
      '#size' => 5,
      '#maxlength' => 5,
      '#default_value' => $this->options['interval'],
      '#field_suffix' => t('Seconds'),
      '#required' => TRUE,
      '#element_validate' => array('spy_speed_validate'),
      '#description' => t('The time in seconds to wait before switching the next item in. Note that this will be added to <em>Fading speed</em> and <em>Sliding speed</em> for the total value that will be used.'),
    );
    $form['fadespeed'] = array(
      '#type' => 'textfield',
      '#title' => t('Fading speed'),
      '#size' => 5,
      '#maxlength' => 5,
      '#default_value' => $this->options['fadespeed'],
      '#field_suffix' => t('Seconds'),
      '#required' => TRUE,
      '#element_validate' => array('spy_speed_validate'),
      '#description' => t('The time in seconds for the fading in and out of the elements.'),
    );
    $form['slidespeed'] = array(
      '#type' => 'textfield',
      '#title' => t('Sliding speed'),
      '#size' => 5,
      '#maxlength' => 5,
      '#default_value' => $this->options['slidespeed'],
      '#field_suffix' => t('Seconds'),
      '#required' => TRUE,
      '#element_validate' => array('spy_speed_validate'),
      '#description' => t('The time in seconds for the sliding down of the elements.'),
    );
    $form['autopause'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pause on mouse over'),
      '#default_value' => $this->options['autopause'],
      '#description' => t("Whether or not the Spy should pause when the user's mouse hovers the section."),
    );
    $form['include-style'] = array(
      '#type' => 'checkbox',
      '#title' => t("Use the module's default styling"),
      '#default_value' => $this->options['include-style'],
      '#description' => t("If you disable this, the file in the module's directory <em>views-spy.css</em> will not be loaded"),
    );
  }

  /**
   * Render the display in this style.
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Render each group separately and concatenate.  Plugins may override this
    // method if they wish some other way of handling grouping.
    $output = '';
    foreach ($sets as $title => $records) {
      if ($this->uses_row_plugin()) {
        $rows = array();
        foreach ($records as $label => $row) {
          $rows[] = $this->row_plugin->render($row);
        }
      }
      else {
        $rows = $records;
      }
      $output .= theme($this->theme_functions(), $this->view, $this->options, $rows, $title);
    }

    // Preparing the js variables and adding the js to our display
    // we do it here so we dont have it run once every group
    drupal_add_js(drupal_get_path('module', 'views_bonus_spy') .'/views-bonus-spy.js');
    $view_settings['limit'] = $this->options['limit'];
    $view_settings['interval'] = check_plain($this->options['interval']) * 1000;
    $view_settings['fadespeed'] = check_plain($this->options['fadespeed']) * 1000;
    $view_settings['slidespeed'] = check_plain($this->options['slidespeed']) * 1000;
    $view_settings['autopause'] = $this->options['autopause'];

    $view_id = 'views-spy-'. $this->view->name .'-'. $this->view->current_display;
    //dpm($view_id);
    drupal_add_js(array('views_spy' => array($view_id => $view_settings)), 'setting');

    return $output;
  }
}

/**
  * Validates the options entered
  */
function spy_speed_validate($element, &$form_state) {
  /* must be numeric value */
  if (!is_numeric($element['#value'])) {
    form_error($element, ('<em>'. ($element['#title'] . t('</em> must be a <em>numberic</em> value.'))));
  }

  /* dont allow commas */
  if (strstr($element['#value'], ',')) {
    form_error($element, t('Please use a "." instead of a "," in the <em>') . $element['#title'] . t('</em> field.'));
  }

  /* must be higher than 0 */
  if ($element['#value'] <= 0) {
    form_error($element, ('<em>'. ($element['#title'] . t('</em> must be a <em>number</em> higher than 0.<br />If you would like it to be instanteneous, enter 0.001'))));
  }
}
