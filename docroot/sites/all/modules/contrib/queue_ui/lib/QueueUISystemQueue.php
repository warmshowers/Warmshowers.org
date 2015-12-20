<?php

class QueueUISystemQueue implements QueueUIInterface {

  public $inspect;

  public function __construct() {
    $this->inspect = TRUE;
  }

  /**
   * SystemQueue implements all default QueueUI methods.
   *
   * @return array
   *  An array of available QueueUI methods. Array key is system name of the
   *  operation, array key value is the display name.
   */
  public function getOperations() {
    return array(
      'view' => t('View'),
      'release' => t('Release'),
      'delete' => t('Delete'),
    );
  }

  /**
   * View the queue items in a queue and expose additional methods for inspection.
   *
   * @param string $queue_name
   * @return string
   */
  public function inspect($queue_name) {
    $query = db_select('queue', 'q')
      ->fields('q', array('item_id', 'expire', 'created'))
      ->condition('q.name', $queue_name)
      ->extend('PagerDefault')
      ->limit(25)
      ->execute();

    foreach ($query as $record) {
      $result[] = $record;
    }

    $header = array(
      t('Item ID'),
      t('Expires'),
      t('Created'),
      array('data' => t('Operations'), 'colspan' => '3'),
    );

    $rows = array();
    foreach ($result as $item) {
      $row = array();
      $row[] = $item->item_id;
      $row[] = ($item->expire ? date(DATE_RSS, $item->expire) : $item->expire);
      $row[] = date(DATE_RSS, $item->created);

      foreach ($this->getOperations() as $op => $title) {
        $row[] = l($title, QUEUE_UI_BASE . "/$queue_name/$op/$item->item_id");
      }

      $rows[] = array('data' => $row);
    }

    $content = theme('table', array('header' => $header, 'rows' => $rows));
    $content .= theme('pager');

    return $content;
  }

  /**
   * View the item data for a specified queue item.
   *
   * @param int $item_id
   * @return string
   */
  public function view($item_id) {
    $queue_item = $this->loadItem($item_id);

    $rows[] = array(
      'data' => array(
        'header' => t('Item ID'),
        'data' => $queue_item->item_id,
      ),
    );
    $rows[] = array(
      'data' => array(
        'header' => t('Queue name'),
        'data' => $queue_item->name,
      ),
    );
    $rows[] = array(
      'data' => array(
        'header' => t('Expire'),
        'data' => ($queue_item->expire ? date(DATE_RSS, $queue_item->expire) : $queue_item->expire),
      ),
    );
    $rows[] = array(
      'data' => array(
        'header' => t('Created'),
        'data' => date(DATE_RSS, $queue_item->created),
      ),
    );
    $rows[] = array(
      'data' => array(
        'header' => array('data' => t('Data'), 'style' => 'vertical-align:top'),
        'data' => '<pre>' . print_r(unserialize($queue_item->data), TRUE) . '</pre>',
        // @TODO - should probably do something nicer than print_r here...
      ),
    );

    return theme('table', array('rows' => $rows));
  }

  public function delete($item_id) {
    // @TODO - try... catch...
    drupal_set_message("Deleted queue item " . $item_id);

    db_delete('queue')
      ->condition('item_id', $item_id)
      ->execute();

    return TRUE;
  }

  public function release($item_id) {
    // @TODO - try... catch...
    drupal_set_message("Released queue item " . $item_id);

    db_update('queue')
      ->condition('item_id', $item_id)
      ->fields(array('expire' => 0))
      ->execute();

    return TRUE;
  }

  /**
   * Load a specified SystemQueue queue item from the database.
   *
   * @param $item_id
   *  The item id to load
   * @return
   *  Result of the database query loading the queue item.
   */
  private function loadItem($item_id) {
    // Load the specified queue item from the queue table.
    $query = db_select('queue', 'q')
      ->fields('q', array('item_id', 'name', 'data', 'expire', 'created'))
      ->condition('q.item_id', $item_id)
      ->range(0, 1) // item id should be unique
      ->execute();

    foreach ($query as $record) {
      $result[] = $record;
    }

    return $result[0];
  }
}