<?php
// $Id: views_handler_field_upload_fid.inc,v 1.6 2009/04/07 22:48:03 merlinofchaos Exp $
/**
 * Field handler to provide a list of roles.
 */
class views_handler_field_upload_fid extends views_handler_field_prerender_list {
  function construct() {
    parent::construct();
  }

  function allow_advanced_render() {
    return FALSE;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_file'] = array('default' => FALSE);
    $options['only_listed'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_file'] = array(
      '#title' => t('Link this field to download the file'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_file']),
    );

    $form['only_listed'] = array(
      '#title' => t('Only show "listed" file attachments'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['only_listed']),
    );
  }

  function pre_render($values) {
    $vids = array();
    $this->items = array();

    foreach ($values as $result) {
      $vids[] = $result->{$this->field_alias};
    }

    if ($vids) {
      // Support "only listed files" option.
      $where = '';
      if (!empty($this->options['only_listed'])) {
        $where = " AND u.list <> 0";
      }
      $result = db_query("SELECT u.vid, u.fid, f.filepath, u.description FROM {upload} u LEFT JOIN {files} f ON f.fid = u.fid WHERE u.vid IN (" . implode(', ', $vids) . ")$where ORDER BY u.weight");
      while ($file = db_fetch_object($result)) {
        $this->items[$file->vid][$file->fid] = $this->render_link(check_plain($file->description), $file);
      }
    }
  }

  /**
   * Render whatever the data is as a link to the file.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_file']) && $data !== NULL && $data !== '') {
      return l($data, file_create_url($values->filepath), array('html' => TRUE));
    }
    return $data;
  }
}
