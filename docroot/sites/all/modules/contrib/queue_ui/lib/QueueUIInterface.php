<?php

/**
 * @file
 * Declares the Queue UI interface for inspecting queue data.
 */

interface QueueUIInterface {

  /**
   * Starting working with a Queue class.
   */
  public function __construct();

  /**
   * Inspect the queue items in a specified queue.
   *
   * @param string $queue_name
   *  The name of the queue being inspected.
   *
   * @return
   *  FALSE if inspection is not implemented for this queue class. Otherwise returns the
   *  content to be rendered on the Queue inspection screen.
   */
  public function inspect($queue_name);

  /**
   * View item data for a specified queue item.
   *
   * @param integer $item_id
   *  The item id to be viewed.
   *
   * @return
   *  FALSE if viewing queue items is not implemented for this queue class. Otherwise returns
   *  the content to be renders on the Queue item details screen.
   */
  public function view($item_id);

  /**
   * Force the deletion of a specified queue item.
   *
   * @param integer $item_id
   *  The item id to be deleted.
   *
   * @return
   *  TRUE if deletion succeeds, FALSE if deletion fails.
   */
  public function delete($item_id);

  /**
   * Force the releasing of a specified queue item.
   *
   * @param integer $item_id
   *  The item id to be released.
   *
   * @return
   *  TRUE if releasing succeeds, FALSE if releasing fails.
   */
  public function release($item_id);

  /**
   * Retrieve the available operations for the implementing queue class.
   *
   * @return
   *  An array of the available operations for the implementing queue class.
   */
  public function getOperations();
}