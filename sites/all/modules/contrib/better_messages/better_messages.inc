<?php 
// $Id: better_messages.inc,v 1.1.2.2 2009/10/07 12:22:21 doublethink Exp $

/*
Admin settings menu callback
*/
function better_messages_admin() {
	$settings = better_messages_get_settings();
	
	$form['position'] = array(
		'#type' => 'fieldset',
		'#title' => t('Messages positions and basic properties'),
		'#weight' => -5,
		'#prefix' => '<div style="clear:both;"></div>',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	
	$form['position']['better_messages_pos'] = array(
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
	
	$form['position']['better_messages_fixed'] = array(
		'#type' => 'checkbox',
		'#default_value' => $settings['fixed'],
		'#title' => t('Keep fixed position of message as you scroll.'),
	);
	
	$form['position']['better_messages_width'] = array(
		'#type' => 'textfield',
		'#title' => t('Custom width'),
		'#description' => t('Width in pixel. Example: 400px<br />Or percentage. Example: 100%'),
		'#default_value' => $settings['width'],
		'#size' => 20,
		'#maxlength' => 20,
		'#required' => TRUE,
	);
	
	$form['position']['better_messages_horizontal'] = array(
		'#type' => 'textfield',
		'#title' => t('Left/Right spacing'),
		'#description' => t('In active when position is set to "center".<br />In pixel. Example: 10'),
		'#default_value' => $settings['horizontal'],
		'#size' => 20,
		'#maxlength' => 20,
		'#required' => TRUE,
	);
	
	$form['position']['better_messages_vertical'] = array(
		'#type' => 'textfield',
		'#title' => t('Top/Down spacing'),
		'#description' => t('Inactive when position is set to "center".<br />In pixel. Example: 10'),
		'#default_value' => $settings['vertical'],
		'#size' => 20,
		'#maxlength' => 20,
		'#required' => TRUE,
	);
	
	$form['skins'] = array(
		'#type' => 'fieldset',
		'#title' => t('Messages appearance'),
		'#weight' => -4,
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	
	$form['skins']['better_messages_skin'] = array(
		'#type' => 'radios',
		'#title' => t('Choose a skin'),
		'#options' => $settings['extra']['skins'],
		'#default_value' => $settings['extra']['skin'],
		'#description' => t('Select the default skin for your popup messages.<br />You can create your own skin, put your files (CSS and images) in a folder named "skins" in your theme.'),
	);
	
	$form['animation'] = array(
		'#type' => 'fieldset',
		'#title' => t('Messages animation settings'),
		'#weight' => -3,
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	
	$form['animation']['better_messages_popin_effect'] = array(
		'#type' => 'select',
		'#title' => t('Pop-in (show) message box effect'),
		'#default_value' => $settings['popin']['effect'],
		'#options' => array( 
			'fadeIn' => t('Fade in'),
			'slideDown' => t('Slide down'),
		),
	);
	
	$form['animation']['better_messages_popin_duration'] = array(
		'#type' => 'textfield',
		'#title' => t('Duration of (show) effect'),
		'#description' => t('A string representing one of the three predefined speeds ("slow", "normal", or "fast").<br />Or the number of milliseconds to run the animation (e.g. 1000).'),
		'#default_value' => $settings['popin']['duration'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['better_messages_popout_effect'] = array(
		'#type' => 'select',
		'#title' => t('Pop-out (close) message box effect'),
		'#default_value' => $settings['popout']['effect'],
		'#options' => array( 
			'fadeIn' => t('Fade out'),
			'slideUp' => t('Slide Up'),
		),
	);
	
	$form['animation']['better_messages_popout_duration'] = array(
		'#type' => 'textfield',
		'#title' => t('Duration of (close) effect'),
		'#description' => t('A string representing one of the three predefined speeds ("slow", "normal", or "fast").<br />Or the number of milliseconds to run the animation (e.g. 1000).'),
		'#default_value' => $settings['popout']['duration'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['better_messages_autoclose'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of seconds to auto close after the page has loaded'),
		'#description' => t('0 for never. You can set it as 0.25 for quarter second'),
		'#default_value' => $settings['autoclose'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['better_messages_open_delay'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of seconds to delay message after the page has loaded'),
		'#description' => t('0 for never. You can set it as 0.25 for quarter second'),
		'#default_value' => $settings['opendelay'],
		'#size' => 20,
		'#maxlength' => 20,
	);
	
	$form['animation']['better_messages_disable_autoclose'] = array(
		'#type' => 'checkbox',
		'#title' => t('Disable auto close if messages inculde an error message'),
		'#default_value' => $settings['disable_autoclose'],
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

    $form['vis_settings']['better_messages_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show Better Messages on specific pages'),
      '#options' => $options,
      '#default_value' => $settings['extra']['visibility'],
    );
    $form['vis_settings']['better_messages_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $settings['extra']['pages'],
      '#description' => $description,
    );
	return system_settings_form($form);
}