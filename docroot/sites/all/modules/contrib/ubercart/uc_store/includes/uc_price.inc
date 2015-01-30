<?php

/**
 * @file
 * Price handling functions and hooks.
 */

/**
 * The heart of the price modification/displaying system,
 * this function handles price alteration, formatting, theming, and caching.
 *
 * @param $price_info
 *   Either a simple price to alter/format or an associative array containing
 *     the following keys:
 *     - 'price' => the price per item
 *     - 'qty' => a quantity
 *     - 'placeholder' => a placeholder string to use as the formatted price
 *         value in lieu of actually formatting the altered price value
 * @param $context
 *   An associative array containing information about where the function call
 *     came from.  This function will at least add an 'account' key to the array
 *     if it is not already set with the value being the current user object.
 *     A revision may also be specified, accepted values follow:
 *
 *     'revision':
 *     Default: 'themed'
 *     - 'original' => Original price value,
 *     - 'altered' => Original price passed through the alterer(s),
 *     - 'formatted-original' => Original price passed through the formatter,
 *     - 'formatted' => Altered price passed through the formatter,
 *     - 'themed-original' => Formatted original price passed through the
 *                              theme layer,
 *     - 'themed' => Formatted altered price passed through the theme layer.
 *
 * @param $options
 *   An associative array containing options that will be passed through to
 *     any alteration/formatting/theming functions implemented. A list of accepted
 *     options follows.
 *
 *     'sign':
 *     Default: variable_get('uc_currency_sign', '$')
 *     The sign to use when formatting this price.
 *
 *     'sign_after':
 *     Default: variable_get('uc_sign_after_amount', FALSE)
 *     If set to TRUE, the sign will come after the price.
 *
 *     'prec':
 *     Default: variable_get('uc_currency_prec', 2)
 *     Precision to round the price to.
 *
 *     'dec':
 *     Default: variable_get('uc_currency_dec', '.')
 *     Decimal separator.
 *
 *     'thou':
 *     Default: variable_get('uc_currency_thou', ',')
 *     Thousand separator.
 *
 *     'label':
 *     Default: TRUE
 *     If set to TRUE, themed prices will include any prefixes and suffixes.
 */
function uc_price($price_info, $context = array(), $options = array()) {
  global $user;

  // If we're passed just a number for price, we'll set the quantity to 1.
  if (is_numeric($price_info)) {
    $price_info = array(
      'price' => $price_info,
      'qty' => 1,
    );
  }
  elseif (!is_array($price_info)) {
    $price_info = array(
      'price' => 0,
      'qty' => 1,
    );
  }

  // Initialize the context.
  $context += array(
    'revision' => 'themed',
    'type' => 'amount',
  );

  // Clamp to allowed revisions.
  $revisions = array('original', 'altered', 'formatted-original', 'formatted', 'themed-original', 'themed');
  if (!in_array($context['revision'], $revisions)) {
    $context['revision'] = 'themed';
  }

  // Calculate the original price.
  $original = $price_info['price'] * $price_info['qty'];

  // Exit early if the original price was requested.
  if ($context['revision'] == 'original') {
    return $original;
  }

  // Get all the active handlers.
  $handlers = _uc_price_get_handlers($options);
  $formatter = $handlers['formatter'];

  // Use the global user if none was passed in.
  if (!isset($context['account'])) {
    $context['account'] = $user;
  }

  // Merge any incoming options, giving them precedence.
  $options += $handlers['options'];

  // Exit early if no alterations are needed.
  switch ($context['revision']) {
    case 'formatted-original':
      return $formatter($original, $options);

    case 'themed-original':
      return theme('uc_price', $formatter($original, $options), $context, $options);
  }

  // Alter the price, context, and options.
  foreach ($handlers['alterers'] as $alterer) {
    $alterer($price_info, $context, $options);
  }
  $altered = $price_info['price'] * $price_info['qty'];

  if ($context['revision'] == 'altered') {
    return $altered;
  }

  // Use a price placeholder if specified.
  if (isset($price_info['placeholder'])) {
    $formatted = $price_info['placeholder'];
  }
  else {
    $formatted = $formatter($altered, $options);
  }

  // Return the requested revision.
  switch ($context['revision']) {
    case 'formatted':
      return $formatted;

    case 'themed':
      return theme('uc_price', $formatted, $context, $options);
  }
}

/**
 * Return an array of price handler data.
 *
 * @param $options
 *   An associative array of options used when building the array with keys:
 *     - 'rebuild_handlers' => TRUE or FALSE indicating whether we should nuke
 *         the cache and rebuild the handler data.
 *     - 'all_handlers' => TRUE or FALSE indicating whether or not to return all
 *         defined price handlers' alterers or just enabled ones.
 * @return
 *   A structured array of price handler data.
 */
function _uc_price_get_handlers($options = array()) {
  static $handlers = array();

  // Set default options.
  $options += array(
    'rebuild_handlers' => FALSE,
    'all_handlers' => FALSE,
  );

  // Get handlers only if we haven't already, or if this is a rebuild.
  if (empty($handlers) || $options['rebuild_handlers']) {
    // Get the handlers and sort them by weight.
    $config = variable_get('uc_price_handler_config', array());

    foreach (module_implements('uc_price_handler') as $module) {
      // Create a price handler hook data array and merge in sensible defaults.
      $hooks[$module] = module_invoke($module, 'uc_price_handler') + array(
        'weight' => 0,
        'enabled' => TRUE,
      );

      // Merge any configuration state in.
      if (isset($config[$module])) {
        $hooks[$module] = $config[$module] + $hooks[$module];
      }

      // Unset disabled hooks if we're not building the selection form.
      if (!$options['all_handlers'] && !$hooks[$module]['enabled']) {
        unset($hooks[$module]);
      }
    }

    // Sort the hook data by weight.
    uasort($hooks, 'uc_weight_sort');

    // Store the raw data for selection form building.
    $handlers['hook_data'] = $hooks;

    // Store the selected formatter, defaulting to uc_store's implementation.
    $formatter = variable_get('uc_price_format_callback', 'uc_store_price_handler_format');

    if (function_exists($formatter)) {
      $handlers['formatter'] = $formatter;
    }
    else {
      $handlers['formatter'] = 'uc_store_price_handler_format';
    }

    // Grab all the alter/format callbacks, as well as merging the options.
    // This happens in order by weight, so we're kosher.
    $handlers['alterers'] = array();

    // We set some default options here. We could set them in the uc_store price handler,
    // but that means if that handler is disabled, we won't get them merged in.
    $handlers['options'] = array(
      'sign' => variable_get('uc_currency_sign', '$'),
      'sign_after' => variable_get('uc_sign_after_amount', FALSE),
      'prec' => variable_get('uc_currency_prec', 2),
      'dec' => variable_get('uc_currency_dec', '.'),
      'thou' => variable_get('uc_currency_thou', ','),
      'label' => TRUE,
    );

    foreach ($hooks as $hook) {
      if (isset($hook['alter']['callback']) && function_exists($hook['alter']['callback'])) {
        $handlers['alterers'][] = $hook['alter']['callback'];
      }
      if (isset($hook['format']['callback']) && function_exists($hook['format']['callback'])) {
        $handlers['formatters'][] = $hook['format']['callback'];
      }

      if (isset($hook['options']) && is_array($hook['options'])) {
        $handlers['options'] = $hook['options'] + $handlers['options'];
      }
    }
  }

  return $handlers;
}

/**
 * Implements hook_uc_price_handler().
 */
function uc_store_uc_price_handler() {
  return array(
    'alter' => array(
      'title' => t('Default price handler'),
      'description' => t('The default handler alterer is simply responsible for prefixing various product prices for display.'),
      'callback' => 'uc_store_price_handler_alter',
    ),
    'format' => array(
      'title' => t('Default price handler'),
      'description' => t('The default handler formatter passes prices through a single currency formatter based on the store currency display settings.'),
      'callback' => 'uc_store_price_handler_format',
    ),
  );
}

/**
 * Default price handler alterer; adds the default prefixes to the various
 *   product prices when viewing a product node.
 */
function uc_store_price_handler_alter(&$price, &$context, &$options) {
  // If a class was specified in the price's context array...
  if (isset($context['class']) && is_array($context['class'])) {
    // Look for a product price type in the class array and adjust the price
    // prefix accordingly.
    if (in_array('list', $context['class'])) {
      $options['prefixes'][] = t('List Price: ');
    }
    elseif (in_array('cost', $context['class'])) {
      $options['prefixes'][] = t('Cost: ');
    }
    elseif (in_array('sell', $context['class'])) {
      $options['prefixes'][] = t('Price: ');
    }
  }
}

/**
 * Default price handler formatter; formats the price using the store currency
 *   display settings.
 */
function uc_store_price_handler_format($price, $options) {
  $output = '';

  // If the value is less than the minimum precision, zero it.
  if ($options['prec'] > 0 && abs($price) < 1 / pow(10, $options['prec'])) {
    $price = 0;
  }

  // Force the price to a positive value and add a negative sign if necessary.
  if ($price < 0) {
    $price = abs($price);
    $output .= '-';
  }

  // Add the currency sign first if specified.
  if ($options['sign'] && !$options['sign_after']) {
    $output .= $options['sign'];
  }

  // Format the number, like 1234.567 => 1,234.57
  $output .= number_format($price, $options['prec'], $options['dec'], $options['thou']);

  // Add the currency sign last if specified.
  if ($options['sign'] && $options['sign_after']) {
    $output .= $options['sign'];
  }

  return $output;
}
