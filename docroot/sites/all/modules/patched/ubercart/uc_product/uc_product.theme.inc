<?php
/**
 * @file
 * Theme functions for uc_product module.
 */

/**
 * Formats a product's model number.
 *
 * @param $variables
 *   An associative array containing:
 *   - model: Product model number, also known as SKU.
 *   - attributes: (optional) Array of attributes to apply to enclosing DIV.
 *
 * @return string
 *   The HTML output.
 *
 * @ingroup themeable
 */
function theme_uc_product_model($variables) {
  $model = $variables['model'];
  $attributes = $variables['attributes'];
  $attributes['class'][] = "product-info";
  $attributes['class'][] = "model";

  $output = '<div ' . drupal_attributes($attributes) . '>';
  $output .= '<span class="product-info-label">' . t('SKU') . ':</span> ';
  $output .= '<span class="product-info-value">' . check_plain($model) . '</span>';
  $output .= '</div>';

  return $output;
}

/**
 * Wraps the "Add to Cart" form in a <div>.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the add-to-cart form.
 *
 * @ingroup themeable
 */
function theme_uc_product_add_to_cart($variables) {
  $form = $variables['form'];

  $output = '<div class="add-to-cart">';
  $output .= drupal_render($form);
  $output .= '</div>';

  return $output;
}

/**
 * Formats a product's price.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array render element containing:
 *     - #value: Price to be formatted.
 *     - #attributes: (optional) Array of attributes to apply to enclosing DIV.
 *     - #title: (optional) Title to be used as label.
 *
 * @ingroup themeable
 */
function theme_uc_product_price($variables) {
  $element = $variables['element'];
  $price = $element['#value'];
  $attributes = isset($element['#attributes']) ? $element['#attributes'] : array();
  $label = isset($element['#title']) ? $element['#title'] : '';

  if (isset($attributes['class'])) {
    array_unshift($attributes['class'], 'product-info');
  }
  else {
    $attributes['class'] = array('product-info');
  }

  $output = '<div ' . drupal_attributes($attributes) . '>';
  if ($label) {
    $output .= '<span class="uc-price-label">' . $label . '</span> ';
  }
  $vars = array('price' => $price);
  if (!empty($element['#suffixes'])) {
    $vars['suffixes'] = $element['#suffixes'];
  }
  $output .= theme('uc_price', $vars);
  $output .= drupal_render_children($element);
  $output .= '</div>';

  return $output;
}

/**
 * Formats a product's weight.
 *
 * @param $variables
 *   An associative array containing:
 *   - amount: A numerical weight value.
 *   - units: String abbreviation representing the units of measure.
 *   - attributes: (optional) Array of attributes to apply to enclosing DIV.
 *
 * @see uc_weight_format()
 * @ingroup themeable
 */
function theme_uc_product_weight($variables) {
  $amount = $variables['amount'];
  $units = $variables['units'];
  $attributes = $variables['attributes'];
  $attributes['class'][] = "product-info";
  $attributes['class'][] = "weight";

  $output = '';
  if ($amount) {
    $output = '<div ' . drupal_attributes($attributes) . '>';
    $output .= '<span class="product-info-label">' . t('Weight') . ':</span> ';
    $output .= '<span class="product-info-value">' . uc_weight_format($amount, $units) . '</span>';
    $output .= '</div>';
  }

  return $output;
}

/**
 * Formats a product's length, width, and height.
 *
 * @param $variables
 *   An associative array containing:
 *   - length: A numerical length value.
 *   - width: A numerical width value.
 *   - height: A numerical height value.
 *   - units: String abbreviation representing the units of measure.
 *   - attributes: (optional) Array of attributes to apply to enclosing DIV.
 *
 * @see uc_length_format()
 * @ingroup themeable
 */
function theme_uc_product_dimensions($variables) {
  $length = $variables['length'];
  $width = $variables['width'];
  $height = $variables['height'];
  $units = $variables['units'];
  $attributes = $variables['attributes'];
  $attributes['class'][] = "product-info";
  $attributes['class'][] = "dimensions";

  $output = '';
  if ($length || $width || $height) {
    $output = '<div ' . drupal_attributes($attributes) . '>';
    $output .= '<span class="product-info-label">' . t('Dimensions') . ':</span> ';
    $output .= '<span class="product-info-value">' ;
    $output .= uc_length_format($length, $units) . ' × ';
    $output .= uc_length_format($width, $units) . ' × ';
    $output .= uc_length_format($height, $units) . '</span>';
    $output .= '</div>';
  }

  return $output;
}

/**
 * Formats a product's images.
 *
 * @param $variables
 *   - images: An array of image render elements, each containing:
 *     - uri: URI of image.
 *     - alt: Alternate text to display for image.
 *     - title: Title for image.
 *
 * @see theme_image_style()
 * @ingroup themeable
 */
function theme_uc_product_image($variables) {
  static $rel_count = 0;
  $images = $variables['images'];

  // Get the current product image widget.
  $image_widget = uc_product_get_image_widget();

  $first = array_shift($images);

  $output = '<div class="product-image"><div class="main-product-image">';
  $output .= '<a href="' . image_style_url('uc_product_full', $first['uri']) . '" title="' . $first['title'] . '"';
  if ($image_widget) {
    $image_widget_func = $image_widget['callback'];
    $output .= $image_widget_func($rel_count);
  }
  $output .= '>';
  $output .= theme('image_style', array(
    'style_name' => 'uc_product',
    'path' => $first['uri'],
    'alt' => $first['alt'],
    'title' => $first['title'],
  ));
  $output .= '</a></div>';

  if (!empty($images)) {
    $output .= '<div class="more-product-images">';
    foreach ($images as $thumbnail) {
      // Node preview adds extra values to $images that aren't files.
      if (!is_array($thumbnail) || empty($thumbnail['uri'])) {
        continue;
      }
      $output .= '<a href="' . image_style_url('uc_product_full', $thumbnail['uri']) . '" title="' . $thumbnail['title'] . '"';
      if ($image_widget) {
        $output .= $image_widget_func($rel_count);
      }
      $output .= '>';
      $output .= theme('image_style', array(
        'style_name' => 'uc_thumbnail',
        'path' => $thumbnail['uri'],
        'alt' => $thumbnail['alt'],
        'title' => $thumbnail['title'],
      ));
      $output .= '</a>';
    }
    $output .= '</div>';
  }

  $output .= '</div>';
  $rel_count++;

  return $output;
}
