<?php 
// $Id: better_messages.inc,v 1.1.2.5 2010/03/29 09:56:01 doublethink Exp $

/*
Admin settings menu callback
*/
function better_messages_admin() {
	$settings = better_messages_get_settings();
	
	$form['position'] = array(
		'#type' => 'fieldset',
		'#title' => t('Messages positions and basic properties'),
		'#weight' => -5,
		'#collapsible' => TRUE,
		'#collapsed' => False,
	);
	
	$form['position']['pos'] = array(
		'#type' => 'radios',
		'#title' => t('Set position of Message'),
		'#default_value' => $settings['position'],
		'#description' => t('Position of message relative to screen'),
		'#options' => array(
			'center' => t('Center screen'), 
			'tl' => t('Top left'), 
			'tr' => t('Top right'),
			'bl' => t('Bottom left'), 
			'br' => t('Bottom right')
		),
	);
	
	$form['position']['fixed'] = array(
		'#type' => 'checkbox',
		'#default_value' => $settings['fixed'],
		'#title' => t('Keep fixed position of message as you scroll.'),
	);
	
	$form['position']['width'] = array(
		'#type' => 'textfield',
		'#title' => t('Custom width'),
		'#description' => t('Width in pixel. Example: 400px<br />Or percentage. Example: 100%'),
		'#default_value' => $settings['width'],
		'#size' => 20,
		'#maxlength' => 20,
		'#required' => TRUE,
	);
	
	$form['position']['horizontal'] = array(
		'#type' => 'textfield',
		'#title' => t('Left/Right spacing'),
		'#description' => t('In active when position is set to "center".<br />In pixel. Example: 10'),
		'#default_value' => $settings['horizontal'],
		'#size' => 20,
		'#maxlength' => 20,
		'#required' => TRUE,
	);
	
	$form['position']['vertical'] = array(
		'#type' => 'textfield',
		'#title' => t('Top/Down spacing'),
		'#description' => t('Inactive when position is set to "center".<br />In pixel. Example: 10'),
		'#default_value' => $settings['vertical'],
		'#size' => 20,
		'#maxlength' => 20,
		'#required' => TRUE,
	);
	
	$form['animation'] = array(
		'#type' => 'fieldset',
		'#title' => t('Messages animation settings'),
		'#weight' => -3,
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	
	$form['animation']['popin_effect'] = array(
		'#type' => 'select',
		'#title' => t('Pop-in (show) message box effect'),
		'#default_value' => $settings['popin']['effect'],
		'#options' => array( 
			'fadeIn' => t('Fade in'),
			'slideDown' => t('Slide down'),
		),
	);
	
	$form['animation']['popin_duration'] = array(
		'#type' => 'textfield',
		'#title' => t('Duration of (show) effect'),
		'#description' => t('A string representing one of the three predefined speeds ("slow", "normal", or "fast").<br />Or the number of milliseconds to run the animation (e.g. 1000).'),
		'#default_value' => $settings['popin']['duration'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['popout_effect'] = array(
		'#type' => 'select',
		'#title' => t('Pop-out (close) message box effect'),
		'#default_value' => $settings['popout']['effect'],
		'#options' => array( 
			'fadeIn' => t('Fade out'),
			'slideUp' => t('Slide Up'),
		),
	);
	
	$form['animation']['popout_duration'] = array(
		'#type' => 'textfield',
		'#title' => t('Duration of (close) effect'),
		'#description' => t('A string representing one of the three predefined speeds ("slow", "normal", or "fast").<br />Or the number of milliseconds to run the animation (e.g. 1000).'),
		'#default_value' => $settings['popout']['duration'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['autoclose'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of seconds to auto close after the page has loaded'),
		'#description' => t('0 for never. You can set it as 0.25 for quarter second'),
		'#default_value' => $settings['autoclose'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['disable_autoclose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable auto close if messages inculde an error message'),
    '#default_value' => $settings['disable_autoclose'],
  );
  
  $form['animation']['show_countdown'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show countdown timer'),
    '#default_value' => $settings['show_countdown'],
  );
  
  $form['animation']['hover_autoclose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Stop auto close timer when hover'),
    '#default_value' => $settings['hover_autoclose'],
  );
	
	$form['animation']['open_delay'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of seconds to delay message after the page has loaded'),
		'#description' => t('0 for never. You can set it as 0.25 for quarter second'),
		'#default_value' => $settings['opendelay'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['vis_settings'] = array(
		'#type' => 'fieldset',
		'#title' => t('Better Messages visibility'),
		'#weight' => 0,
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	
	$options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  $form['vis_settings']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show Better Messages on specific pages'),
    '#options' => $options,
    '#default_value' => $settings['extra']['visibility'],
  );
  
  $form['vis_settings']['pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => $settings['extra']['pages'],
    '#description' => $description,
  );
  
  $form['jquery_ui'] = array(
    '#type' => 'fieldset',
    '#title' => t('jQuery UI enhancements'),
    '#weight' => 10,
    '#description' => t('These settings require !jquery_ui module to be installed and enabled.', array('!jquery_ui' => l('jQuery UI', 'http://drupal.org/project/jquery_ui'))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  (module_exists('jquery_ui')) ?  $jquer_ui_disabled = false : $jquer_ui_disabled = true;
  
  $form['jquery_ui']['draggable'] = array(
    '#type' => 'checkbox',
    '#disabled' => $jquer_ui_disabled,
    '#title' => t('Make Better Messages draggable'),
    '#default_value' => $settings['jquery_ui']['draggable'],
  );
  
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );
  $form['#theme'] = 'system_settings_form';
  $form['#submit'][] = 'better_messages_admin_submit';
  
	return $form;
}

function better_messages_admin_submit($form, &$form_state) {
  $settings = array(
    'position' => $form_state['values']['pos'], 'vertical' => $form_state['values']['vertical'], 
    'horizontal' => $form_state['values']['horizontal'], 'fixed'  => $form_state['values']['fixed'],
    'width' => $form_state['values']['width'], 'autoclose' => $form_state['values']['autoclose'], 
    'opendelay' => $form_state['values']['open_delay'], 'disable_autoclose' => $form_state['values']['disable_autoclose'],
    'show_countdown' => $form_state['values']['show_countdown'], 'hover_autoclose' => $form_state['values']['hover_autoclose'],
    'popin' => array('effect' => $form_state['values']['popin_effect'], 'duration' => $form_state['values']['popin_duration']),
    'popout' => array('effect' => $form_state['values']['popout_effect'], 'duration' => $form_state['values']['popout_duration']),
    'jquery_ui' => array('draggable' => $form_state['values']['draggable'], 'resizable' => $form_state['values']['resizable']),
    'extra' => array(
      'pages' => $form_state['values']['pages'],
      'visibility' => $form_state['values']['visibility'],
    )
  );
  variable_set('better_messages', $settings);
  drupal_set_message(t('The configuration options have been saved.'));
  cache_clear_all();
}