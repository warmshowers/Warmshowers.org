<?php
/**
 * @file
 * Generate user agent reports.
 */

/**
 * Menu callback; presents the user agents monitoring page.
 *
 * @param $view
 *   - "browsers": Only display "real" browsers
 *   - "crawlers": Only display search engine crawlers
 *   - "all": Display all user agents.
 */
function browscap_top_useragents($view = 'all') {
  if ($view == 'all') {
    $result = db_query('SELECT SUM(counter) FROM {browscap_statistics}');
    $total = db_result($result);
    if (!$total) $total = 1;
    $query = "SELECT parent,counter,(100*counter)/$total as percent,is_crawler FROM {browscap_statistics}";
    $query_cnt = 'SELECT COUNT(parent) FROM {browscap_statistics}';
    $title = t('Top user agents');
    $header = array(
      array('data' => t('User agent'), 'field' => 'parent'),
      array('data' => t('Count'), 'field' => 'counter', 'sort' => 'desc'),
      array('data' => t('Percent'), 'field' => 'percent'),
      array('data' => t('Crawler?'), 'field' => 'is_crawler')
    );
  }
  elseif ($view == 'browsers') {
    $result = db_query('SELECT SUM(counter) FROM {browscap_statistics} WHERE is_crawler=0');
    $total = db_result($result);
    if (!$total) $total = 1;
    $query = "SELECT parent,counter,(100*counter)/$total as percent FROM {browscap_statistics} WHERE is_crawler=0";
    $query_cnt = 'SELECT COUNT(parent) FROM {browscap_statistics} WHERE is_crawler=0';
    $title = t('Top browsers');
    $header = array(
      array('data' => t('Browser'), 'field' => 'parent'),
      array('data' => t('Count'), 'field' => 'counter', 'sort' => 'desc'),
      array('data' => t('Percent'), 'field' => 'percent')
    );
  }
  else {
    $result = db_query('SELECT SUM(counter) FROM {browscap_statistics} WHERE is_crawler=1');
    $total = db_result($result);
    if (!$total) $total = 1;
    $query = "SELECT parent,counter,(100*counter)/$total as percent FROM {browscap_statistics} WHERE is_crawler=1";
    $query_cnt = 'SELECT COUNT(parent) FROM {browscap_statistics} WHERE is_crawler=1';
    $title = t('Top crawlers');
    $header = array(
      array('data' => t('Crawler'), 'field' => 'parent'),
      array('data' => t('Count'), 'field' => 'counter', 'sort' => 'desc'),
      array('data' => t('Percent'), 'field' => 'percent')
    );
  }

  drupal_set_title($title);

  $query .= tablesort_sql($header);

  $result = pager_query($query, 50, 0, $query_cnt);
  $rows = array();

  while ($useragent = db_fetch_object($result)) {
    if (db_result(db_query_range("SELECT useragent FROM {browscap} WHERE useragent = '%s'", $useragent->parent, 0, 1))) {
      $parent = l($useragent->parent, 'admin/reports/browscap/useragent/'. urlencode($useragent->parent));
    }
    else {
      $parent = check_plain($useragent->parent);
    }
    if ($view == 'all') {
      if ($useragent->is_crawler) {
        $is_crawler = t('Yes');
      }
      else {
        $is_crawler = t('No');
      }
      $rows[] = array($parent, $useragent->counter, $useragent->percent, $is_crawler);
    }
    else {
      $rows[] = array($parent, $useragent->counter, $useragent->percent);
    }
  }
  if ($pager = theme('pager', NULL, 50, 0)) {
    $rows[] = array(array('data' => $pager, 'colspan' => 2));
  }

  $output = '';
  if (empty($rows)) {
    $output .= t('It appears that your site has not recorded any visits. If you want to record the visitors to your site you can enable "Monitor browsers" on the <a href="!settings_uri">Browscap settings screen</a>.', array('!settings_uri' => url('admin/settings/browscap')));
  }
  $output .= theme('table', $header, $rows);

  print theme('page', $output, $title);
}
