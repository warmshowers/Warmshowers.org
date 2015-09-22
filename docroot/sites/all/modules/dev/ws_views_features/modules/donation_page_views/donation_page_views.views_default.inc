<?php
/**
 * @file
 * donation_page_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function donation_page_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'donation_page';
  $view->description = 'The donation page for purchasing donation products.';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Your Warmshowers.org Annual Donation';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_amount_description_value' => 'field_amount_description_value',
    'list_price' => 'list_price',
    'addtocartlink' => 'addtocartlink',
    'title' => 'title',
    'body' => 'body',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_amount_description_value' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'list_price' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'addtocartlink' => array(
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'body' => array(
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = 'Your donation to the Warmshowers.org Foundation lets us improve and maintain the site, support the community, and continue to let hosts and guests develop lifelong friendships. Thanks! Questions about donations and how we use your generous donations? <a href="/faq/donations-and-membership-levels">Donation Level FAQs</a>. <b>All transactions are protected by secure encrypted https, and you can pay with debit/credit card or PayPal. Your card information is secure and does not travel through this site in either case.</b>

Signing up for any of these donation levels means you will not receive email appeals or on-site donation appeals for a year. ';
  $handler->display->display_options['header']['text']['format'] = '1';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['content'] = 'Donations are <a href="/faq#t58n84444">tax-deductible in the USA.</a> More questions? Read the <a href="/faq/donations-and-membership-levels">Donation/Membership FAQ</a>. 

If you would prefer to <strong>send us a check</strong> (US Dollars please), Warmshowers.org Foundation, 848 Montclair Drive, Palisade, CO 81526, USA. Thanks!

You\'re welcome to contact us any time via the <a href="/contact">Contact Form</a>, email (wsl@warmshowers.org).

(The Warmshowers.org Foundation is a certified nonprofit 501(c)(3) with the US government, US EIN is&nbsp;46-1370418<span style="font-size: 12px;">, phone +1 970.462.7450, view <a href="/node/72676">the IRS letter</a>, verify its status at the </span><a href="http://apps.irs.gov/app/eos/pub78Search.do?ein1=46-1370418&amp;names=&amp;city=&amp;state=All...&amp;country=US&amp;deductibility=all&amp;dispatchMethod=searchCharities&amp;submitName=Search" style="font-size: 12px;">IRS website</a>.)

Transparency information: All governance documents are in the <a href="/forums/site-administratorsdesigngovernancevolunteers/leadership-council">leadership forum</a>. <a href="/annual_report_2014">January 2015 Annual Report</a>, <a href="/node/82734">2015 budget</a>, <a href="/content/warmshowers-now-nonprofit-corporation">Articles of Incorporation and Bylaws</a>, <a href="/board">Board of Directors information</a>.';
  $handler->display->display_options['footer']['text']['format'] = '1';
  /* Field: Content: Amount Description */
  $handler->display->display_options['fields']['field_amount_description']['id'] = 'field_amount_description';
  $handler->display->display_options['fields']['field_amount_description']['table'] = 'field_data_field_amount_description';
  $handler->display->display_options['fields']['field_amount_description']['field'] = 'field_amount_description';
  $handler->display->display_options['fields']['field_amount_description']['label'] = '';
  $handler->display->display_options['fields']['field_amount_description']['element_label_colon'] = FALSE;
  /* Field: Product: List price */
  $handler->display->display_options['fields']['list_price']['id'] = 'list_price';
  $handler->display->display_options['fields']['list_price']['table'] = 'uc_products';
  $handler->display->display_options['fields']['list_price']['field'] = 'list_price';
  $handler->display->display_options['fields']['list_price']['label'] = 'Donation';
  $handler->display->display_options['fields']['list_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['list_price']['precision'] = '0';
  /* Field: Product: Add to cart form */
  $handler->display->display_options['fields']['addtocartlink']['id'] = 'addtocartlink';
  $handler->display->display_options['fields']['addtocartlink']['table'] = 'uc_products';
  $handler->display->display_options['fields']['addtocartlink']['field'] = 'addtocartlink';
  $handler->display->display_options['fields']['addtocartlink']['label'] = '';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body_1']['id'] = 'body_1';
  $handler->display->display_options['fields']['body_1']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body_1']['field'] = 'body';
  $handler->display->display_options['fields']['body_1']['label'] = '';
  $handler->display->display_options['fields']['body_1']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Weight (field_product_weight) */
  $handler->display->display_options['sorts']['field_product_weight_value']['id'] = 'field_product_weight_value';
  $handler->display->display_options['sorts']['field_product_weight_value']['table'] = 'field_data_field_product_weight';
  $handler->display->display_options['sorts']['field_product_weight_value']['field'] = 'field_product_weight_value';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'product' => 'product',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'donation_page');
  $handler->display->display_options['path'] = 'donate';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Donate!';
  $handler->display->display_options['menu']['weight'] = '-48';
  $handler->display->display_options['menu']['name'] = 'menu-footer-menu-en';
  $translatables['donation_page'] = array(
    t('Defaults'),
    t('Your Warmshowers.org Annual Donation'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Your donation to the Warmshowers.org Foundation lets us improve and maintain the site, support the community, and continue to let hosts and guests develop lifelong friendships. Thanks! Questions about donations and how we use your generous donations? <a href="/faq/donations-and-membership-levels">Donation Level FAQs</a>. <b>All transactions are protected by secure encrypted https, and you can pay with debit/credit card or PayPal. Your card information is secure and does not travel through this site in either case.</b>

Signing up for any of these donation levels means you will not receive email appeals or on-site donation appeals for a year. '),
    t('Donations are <a href="/faq#t58n84444">tax-deductible in the USA.</a> More questions? Read the <a href="/faq/donations-and-membership-levels">Donation/Membership FAQ</a>. 

If you would prefer to <strong>send us a check</strong> (US Dollars please), Warmshowers.org Foundation, 848 Montclair Drive, Palisade, CO 81526, USA. Thanks!

You\'re welcome to contact us any time via the <a href="/contact">Contact Form</a>, email (wsl@warmshowers.org).

(The Warmshowers.org Foundation is a certified nonprofit 501(c)(3) with the US government, US EIN is&nbsp;46-1370418<span style="font-size: 12px;">, phone +1 970.462.7450, view <a href="/node/72676">the IRS letter</a>, verify its status at the </span><a href="http://apps.irs.gov/app/eos/pub78Search.do?ein1=46-1370418&amp;names=&amp;city=&amp;state=All...&amp;country=US&amp;deductibility=all&amp;dispatchMethod=searchCharities&amp;submitName=Search" style="font-size: 12px;">IRS website</a>.)

Transparency information: All governance documents are in the <a href="/forums/site-administratorsdesigngovernancevolunteers/leadership-council">leadership forum</a>. <a href="/annual_report_2014">January 2015 Annual Report</a>, <a href="/node/82734">2015 budget</a>, <a href="/content/warmshowers-now-nonprofit-corporation">Articles of Incorporation and Bylaws</a>, <a href="/board">Board of Directors information</a>.'),
    t('Donation'),
    t('.'),
    t(','),
    t('Page'),
  );
  $export['donation_page'] = $view;

  return $export;
}
