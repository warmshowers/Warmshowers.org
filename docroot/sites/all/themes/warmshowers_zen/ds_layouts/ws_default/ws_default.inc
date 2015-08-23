<?php
function ds_ws_default() {
  return array(
    'label' => t('WS - Simple stacked layout'),
    'regions' => array(
      'header' => t('Header'),
      'main' => t('Main'),
      'footer' => t('Footer'),
      'comments' => t('Comments'),
    ),
    'css' => FALSE,
    'image' => TRUE,
  );
}