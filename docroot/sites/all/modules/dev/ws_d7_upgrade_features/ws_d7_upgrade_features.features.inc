<?php
/**
 * @file
 * ws_d7_upgrade_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ws_d7_upgrade_features_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function ws_d7_upgrade_features_image_default_styles() {
  $styles = array();

  // Exported image style: 1200w.
  $styles['1200w'] = array(
    'label' => '1200w',
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1200,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: 400w.
  $styles['400w'] = array(
    'label' => '400w',
    'effects' => array(
      2 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 400,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: 50w.
  $styles['50w'] = array(
    'label' => '50w',
    'effects' => array(
      3 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 50,
          'height' => '100%',
          'upscale' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: 800w.
  $styles['800w'] = array(
    'label' => '800w',
    'effects' => array(
      4 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 800,
          'height' => '',
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: Center_Content.
  $styles['Center_Content'] = array(
    'label' => 'Center_Content',
    'effects' => array(
      5 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 440,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: FAQ_graphics.
  $styles['FAQ_graphics'] = array(
    'label' => 'FAQ_graphics',
    'effects' => array(
      7 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 600,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: contest_photo_thumbnail.
  $styles['contest_photo_thumbnail'] = array(
    'label' => 'contest_photo_thumbnail',
    'effects' => array(
      6 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 230,
          'height' => 230,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: fullsize.
  $styles['fullsize'] = array(
    'label' => 'fullsize',
    'effects' => array(
      8 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '100%',
          'height' => '100%',
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: map_infoWindow.
  $styles['map_infoWindow'] = array(
    'label' => 'map_infoWindow',
    'effects' => array(
      9 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 50,
          'height' => 50,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: mobile_photo_456.
  $styles['mobile_photo_456'] = array(
    'label' => 'mobile_photo_456',
    'effects' => array(
      17 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 456,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: mobile_photo_4x3.
  $styles['mobile_photo_4x3'] = array(
    'label' => 'mobile_photo_4x3',
    'effects' => array(
      10 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 400,
          'height' => 400,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: mobile_profile_photo_std.
  $styles['mobile_profile_photo_std'] = array(
    'label' => 'mobile_profile_photo_std',
    'effects' => array(
      11 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 400,
          'height' => 400,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: photo_album_block.
  $styles['photo_album_block'] = array(
    'label' => 'photo_album_block',
    'effects' => array(
      12 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 200,
          'height' => 200,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: profile_picture.
  $styles['profile_picture'] = array(
    'label' => 'profile_picture',
    'effects' => array(
      13 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 179,
          'height' => 200,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: profile_tiny.
  $styles['profile_tiny'] = array(
    'label' => 'profile_tiny',
    'effects' => array(
      14 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 30,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: random_member_photo.
  $styles['random_member_photo'] = array(
    'label' => 'random_member_photo',
    'effects' => array(
      15 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 200,
          'height' => 130,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: rightcolumn.
  $styles['rightcolumn'] = array(
    'label' => 'rightcolumn',
    'effects' => array(
      16 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 200,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 0,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function ws_d7_upgrade_features_node_info() {
  $items = array(
    'block' => array(
      'name' => t('Block'),
      'base' => 'node_content',
      'description' => t('Intended to be deployed as a block'),
      'has_title' => '1',
      'title_label' => t('Block Title'),
      'help' => '',
    ),
    'trust_referral' => array(
      'name' => t('Feedback'),
      'base' => 'node_content',
      'description' => t('This is a way for one member to leave feedback for another.  If you\'ve stayed with someone as host, or hosted someone who\'s a good guest, please put a reference for them here. It will show up on their member page.  And your story will be visible in the "all references area" too. '),
      'has_title' => '1',
      'title_label' => t('Your Experience'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_uc_product_default_classes().
 */
function ws_d7_upgrade_features_uc_product_default_classes() {
  $items = array(
    'product' => array(
      'name' => t('Product'),
      'base' => 'uc_product',
      'description' => t('Use <em>products</em> to represent items for sale on the website, including all the unique information that can be attributed to a specific model number.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
