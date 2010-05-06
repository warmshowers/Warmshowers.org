<?php
// $Id: views-view-xhtml.tpl.php,v 1.1.2.8 2009/09/08 16:43:07 allisterbeharry Exp $
/**
 * @file views-view-xhtml.tpl.php
 * View template to render views as XHTML microformats. Supports hCard and hCalendar format 
 *
 * - $view: The view in use.
 * - $rows: The raw result objects from the query, with all data it fetched.
 * - $options: The options for the style passed in from the UI.
 *
 * @ingroup views_templates
 * @see views_xhtml.views.inc
 */
  
if ($options['format'] == 'hcard') xhtml_hcard_render($view);
if ($options['format'] == 'hcalendar') xhtml_hcalendar_render($view);

function xhtml_hcard_render($view) {
  $xhtml .= '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'."\n";
  $xhtml .= '<html xmlns="http://www.w3.org/1999/xhtml" dir="ltr"'.">\r\n";
  $xhtml .= '<head>'."\r\n";
  $xhtml .= '  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />'."\r\n";
  $xhtml .= '  <meta name="KEYWORDS" content="hCards" />'."\r\n";
  $xhtml .= '  <title>hCards</title>'."\r\n";
  $xhtml .= '</head>'."\r\n";
  $xhtml .= '<body>'."\r\n";
  foreach ($view->result as $node) {
    $hcard = array('adr'=> array(
                           'type' => '', 
                           'post-office-box' => '',
                           'street-address' => array(),
                           'extended-address' => '',
                           'region' => '',
                           'locality' => '',
                           'postal-code' => '',
                           'country-name' => ''    
                           ),
                   'agent' => array(),
                   'bday' => '',
                   'class' => '',
                   'category' => array(),
                   'email' => array(),
                   'fn' => '',
                   'n' => array(
                          'honorific-prefix' => '',
                          'given-name' => '',
                          'additional-name' => '',
                          'family-name' => '',
                          'honorific-suffix' => ''    
                           ),
                   'nickname' => '',                
                   'org' => array (
                           'organization-name' => '',
                           'organization-unit' => array()
                           ),                        
                   'photo' => '',
                   'tel'=> array()         
                  );
    foreach($node as $field_label => $field_value) {
      $label = views_rdf_strip_illegal_chars($field_label);
      $value = views_xml_strip_illegal_chars(views_xml_is_date($field_value));
      $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
      if (is_null($value) || ($value === '')) continue;
      //$xhtml .= "$label:$value";
      if (stripos($label, 'address_type') !== FALSE) {
        $hcard['adr']['type'] = $value; 
      }
      if (stripos($label, 'post_office_box') !== FALSE) { 
        $hcard['adr']['post-office-box'] = $value;  
      }
      if (stripos($label, 'street_address') !== FALSE) {
        $hcard['adr']['street-address'][] = $value;  
      }
      if (stripos($label, 'extended_address') !== FALSE) {
        $hcard['adr']['extended-address'] = $value;  
      }
      if (stripos($label, 'region') !== FALSE) {
        $hcard['adr']['region'] = $value;  
      }
      if (stripos($label, 'locality') !== FALSE) {
        $hcard['adr']['locality'] = $value;  
      }
      if (stripos($label, 'postal_code') !== FALSE) {
        $hcard['adr']['postal-code'] = $value;  
      }
      if (stripos($label, 'country_name') !== FALSE) {
        $hcard['adr']['country-name'] = $value;  
      }
      if (stripos($label, 'agent') !== FALSE) {
        $hcard['agent'][] = $value;  
      }
      if (stripos($label, 'bday') !== FALSE) {
        if (preg_match('/\d/', $value)) {
          if (strtotime($value))
            $value = date(EXHIBIT_DATE_FORMAT, strtotime($value));
        }
        $hcard['bday'] = $value;  
      }
      if (stripos($label, 'class') !== FALSE) {
        $hcard['class'] = $value;  
      }
      if (stripos($label, 'category') !== FALSE) {
        $hcard['category'][] = $value;  
      }
      if (stripos($label, 'email') !== FALSE) {
        $hcard['email'][$label] = $value;  
      }
      if (stripos($label, 'honorific_prefix') !== FALSE) {
        $hcard['n']['honorific-prefix'] = $value;  
      }
      if (stripos($label, 'given_name') !== FALSE) {
        $hcard['n']['given-name'] = $value;  
      }
      if (stripos($label, 'additional_name') !== FALSE) {
        $hcard['n']['additional-name'] = $value;  
      }
      if (stripos($label, 'family-name') !== FALSE) {
        $hcard['n']['family-name'] = $value;  
      }
      if (stripos($label, 'honorific_suffix') !== FALSE) {
        $hcard['n']['honorific-suffix'] = $value;  
      }        
      if (stripos($label, 'fn') !== FALSE) {
        $hcard['fn'] = $value;  
      }
      if (stripos($label, 'nickname') !== FALSE) {
        $hcard['nickname'] = $value;  
      }
      if (stripos($label, 'organization_name') !== FALSE) {
        $hcard['org']['organization-name'] = $value;  
      }
      if (stripos($label, 'organization_unit') !== FALSE) {
        $hcard['org']['organization-unit'][] = $value;  
      }
      if (stripos($label, 'photo') !== FALSE) {
        $hcard['photo'] = $value;  
      }
      if (stripos($label, 'tel') === 0) {
        $hcard['tel'][$label] = $value;  
      }                                
    } 
    $xhtml .= '<div class = "vcard">'."\r\n";
    if ($hcard['photo'] != '')
      $xhtml .='  <img class="photo" alt="photo" title="photo" style="height:96px;width:96px" src="'.$hcard['photo'].'"/>'."<br/>\r\n";      
    if ($hcard['fn'])
      $xhtml .='  <span class="fn">'.$hcard['fn'].'</span>'."<br/>\r\n";
    if ($hcard['nickname'])
      $xhtml .='  <span class="nickname">'.$hcard['nickname'].'</span>'."<br/>\r\n";
    $name = $hcard['n'];
    if ($hcard['fn']) 
      $xhtml .= '  <span class = "n">'."\r\n";
    else
      $xhtml .= '  <span class = "fn n">'."\r\n";
    if ($name['honorific-prefix'] !== '')
      $xhtml .='    <span class="honorific-prefix">'.$name['honorific-prefix'].'</span>'."\r\n";
    if ($name['given-name'] !== '')
      $xhtml .='    <span class="given-name">'.$name['given-name'].'</span>'."\r\n";
    if ($name['additional-name'] !== '')
      $xhtml .='    <span class="additional-name">'.$name['additional-name'].'</span>'."\r\n";
    if ($name['family-name'] !== '')
      $xhtml .='    <span class="family-name">'.$name['family-name'].'</span>'."\r\n";
    if ($name['honorific-suffix'] !== '')
      $xhtml .='    <span class="honorific-suffix">'.$name['honorific-suffix'].'</span>'."\r\n";
    $xhtml .= '  </span><br/>'."\r\n";
    if ($hcard['nickname'] !== '')
      $xhtml .= '    <span class="nickname">'.$hcard['nickname'].'</span><br/>'."\r\n";                  
    $org = $hcard['org'];
    $xhtml .= '  <span class="org">'."\r\n";
    if ($org['organization-name'] !== '')
      $xhtml.= '    <span class="organization name">'.$org['organization-name'].'</span><br/>'."\r\n";
    $org_units = $org['organization-unit'];  
    foreach ($org_units as $org_unit) 
      $xhtml .='    <span class="organization-unit">'.$org_unit.'</span>'."<br/>\r\n";
    $xhtml .= '  </span>'."\r\n";  
    $address = $hcard['adr'];
    $xhtml .= '  <span class = "adr">'."\r\n";
    if ($address['type'] !== '')
      $xhtml .='    <span class="type">'.$address['type'].'</span>'."<br/>\r\n";
    if ($address['post-office-box'] !== '')
      $xhtml .='    <span class="post-office-box">'.$address['post-office-box'].'</span>'."<br/>\r\n";
    $street_addresses = $address['street-address'];  
    foreach ($street_addresses as $street_address) 
      $xhtml .='    <span class="street-address">'.$street_address.'</span>'."<br/>\r\n";
    if ($address['extended-address'] !== '')
      $xhtml .='    <span class="extended-address">'.$address['extended-address'].'</span>'."<br/>\r\n";
    if ($address['region'] !== '')
      $xhtml .='    <span class="region">'.$address['region'].'</span>'."<br/>\r\n";
    if ($address['locality'] !== '')
      $xhtml .='    <span class="locality">'.$address['locality'].'</span>'."<br/>\r\n";
    if ($address['postal-code'] !== '')
      $xhtml .='    <span class="postal-code">'.$address['postal-code'].'</span>'."<br/>\r\n";
    if ($address['country-name'] !== '')
      $xhtml .='    <span class="country-name">'.$address['country-name'].'</span>'."\r\n";
    $xhtml .= '  </span><br/>'."\r\n";       
    $agents = $hcard['agent'];
    foreach ($agents as $agent) 
      $xhtml .='  <span class="agent">'.$agent.'</span>'."<br/>\r\n";
    $birthday =  $hcard['bday'];
    if ($birthday !== '') 
      $xhtml .='  <span class="bday">'.$birthday.'</span>'."<br/>\r\n";      
    $class = $hcard['class'];
    if ($class !== '')
      $xhtml .='  <span class="class">'.$class.'</span>'."<br/>\r\n";
    $categories = $hcard['category'];  
    foreach ($categories as $category) 
      $xhtml .='  <span class="category">'.$category.'</span>'."<br/>\r\n";
    if ($hcard['email']) {
      $email_addrs = $hcard['email']; 
      foreach ($email_addrs as $email_type => $email_addr) 
        $xhtml .='  <span class="email">'."\r\n".
                  '    <span class="type">'.$email_type.': </span>'."\r\n".
                  '    <a class="value" href="mailto:'.$email_addr.'">'.$email_addr.'</a>'."\r\n".
                  '  </span>'."<br/>\r\n";    
    
    }
    if ($hcard['tel']) {
      $tel_nos = $hcard['tel'];
      foreach ($tel_nos as $tel_no_type => $tel_no) 
        $xhtml .='  <span class="tel">'.
                    '<span class="type">'.$tel_no_type.': </span>'.
                    '<span class="value">'.$tel_no.'</span>'.
                    '</span>'."<br/>\r\n";    
    }    
    $xhtml .= '</div>'."\r\n";
  }

  $xhtml.='</body>'."\r\n";
  $xhtml.='</html>'."\r\n";
  if ($view->override_path) //inside live preview 
    print htmlspecialchars($xhtml);
  else {  
   drupal_set_header('Content-Type: text/html');
   print $xhtml;
   //var_dump($view);
   module_invoke_all('exit');
   exit;
  }  
}

function xhtml_hcalendar_render ($view) {
  $xhtml .= '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'."\n";
  $xhtml .= '<html xmlns="http://www.w3.org/1999/xhtml" dir="ltr"'.">\r\n";
  $xhtml .= '<head>'."\r\n";
  $xhtml .= '  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />'."\r\n";
  $xhtml .= '  <meta name="KEYWORDS" content="hCalendars" />'."\r\n";
  $xhtml .= '  <title>hCards</title>'."\r\n";
  $xhtml .= '</head>'."\r\n";
  $xhtml .= '<body>'."\r\n";
  foreach ($view->result as $node) {
  	$hcalendar = array();
  	foreach($node as $field_label => $field_value) {
      $label = views_rdf_strip_illegal_chars($field_label);
      $value = views_xml_strip_illegal_chars(views_xhtml_is_date($field_value));
      $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
      if (is_null($value) || ($value === '')) continue;
  	      if (stripos($label, 'class') !== FALSE) {
        $hcalendar['class'] = $value;  
      }
      if (stripos($label, 'category') !== FALSE) {
        $hcalendar['category'][] = $value;  
      }
      if (stripos($label, 'description') !== FALSE) {
        $hcalendar['description'] = $value; 
      }
      if (stripos($label, 'summary') !== FALSE) {
        $hcalendar['summary'] = $value; 
      }
      if ((stripos($label, 'dtstart') !== FALSE) || (stripos($label, 'event_start') !== FALSE) || (stripos($label, 'eventstarttime') !== FALSE)) {
        if (preg_match('/\d/', $value)) {
          if (strtotime($value))
            $value = date(EXHIBIT_DATE_FORMAT, strtotime($value));
        }
        $hcalendar['dtstart'] = $value;        
      }
      if ((stripos($label, 'dtend') !== FALSE) || (stripos($label, 'event_end') !== FALSE) || (stripos($label, 'eventendtime') !== FALSE)) {
        if (preg_match('/\d/', $value)) {
          if (strtotime($value))
            $value = date(EXHIBIT_DATE_FORMAT, strtotime($value));
        }
        $hcalendar['dtend'] = $value; 
      }
      if (stripos($label, 'duration') !== FALSE) {
        $hcalendar['duration'] = $value; 
      }
      if (stripos($label, 'geo_latitude') !== FALSE) {
        $hcalendar['geo']['latitude'] = $value; 
      }
      if (stripos($label, 'geo_longitude') !== FALSE) {
        $hcalendar['geo']['longitude'] = $value; 
      }
      if (stripos($label, 'location') !== FALSE) {
        $hcalendar['location'] = $value; 
      }
      if (stripos($label, 'status') !== FALSE) {
        $hcalendar['status'] = $value; 
      }
      if (stripos($label, 'uid') !== FALSE) {
        $hcalendar['uid'] = $value; 
      }
      if (stripos($label, 'url') !== FALSE) {
        $hcalendar['url'] = $value; 
      }
      if (stripos($label, 'last_modified') !== FALSE) {
        if (preg_match('/\d/', $value)) {
          if (strtotime($value))
            $value = date(EXHIBIT_DATE_FORMAT, strtotime($value));
        }        
        $hcalendar['last-modified'] = $value; 
      }
      if (stripos($label, 'address_type') !== FALSE) {
        $hcalendar['adr']['type'] = $value; 
      }
      if (stripos($label, 'post_office_box') !== FALSE) {
        $hcalendar['adr']['post-office-box'] = $value;  
      }
      if (stripos($label, 'street_address') !== FALSE) {
        $hcalendar['adr']['street-address'][] = $value;  
      }
      if (stripos($label, 'extended_address') !== FALSE) {
        $hcalendar['adr']['extended-address'] = $value;  
      }
      if (stripos($label, 'region') !== FALSE) {
        $hcalendar['adr']['region'] = $value;  
      }
      if (stripos($label, 'locality') !== FALSE) {
        $hcalendar['adr']['locality'] = $value;  
      }
      if (stripos($label, 'postal_code') !== FALSE) {
        $hcalendar['adr']['postal-code'] = $value;  
      }
      if (stripos($label, 'country_name') !== FALSE) {
        $hcalendar['adr']['country-name'] = $value;  
      }
    }
    $xhtml .= '<div class = "vevent">'."\r\n";
    $class = $hcalendar['class'];
    if ($class) $xhtml .='  <span class="class">'.$class.'</span>'."<br/>\r\n";
    $categories = $hcalendar['category'];
    if ($categories)  
      foreach ($categories as $category) $xhtml .='  <span class="category">'.$category.'</span>'."<br/>\r\n";
    $dtstart = $hcalendar['dtstart'];
    if ($dtstart) 
    $xhtml .='  <span class="dtstart">'.$dtstart.'</span>'."<br/>\r\n";
    $summary = $hcalendar['summary'];
    if ($summary) $xhtml .='  <span class="summary">'.$summary.'</span>'."<br/>\r\n";
    $dtend = $hcalendar['dtend'];
    if ($dtend) $xhtml .='  <span class="dtend">'.$dtend.'</span>'."<br/>\r\n";
    $location = $hcalendar['location'];
    if ($location) $xhtml .='  <span class="location">'.$location.'</span>'."<br/>\r\n";
    $geo_latitude = $hcalendar['geo-latitude'];
    $geo_longitude = $hcalendar['geo-longitude'];
    if ($geo_latitude || $geo_longitude) {
      $xhtml .= "  <div class=\"geo\">\n";
      if ($location) $xhtml .= "    $location: ";
      if ($geo_latitude) $xhtml .= "    <span class=\"latitude\">$geo_latitude</span>; ";
      if ($geo_longitude) $xhtml .="    <span class=\"longitude\">$geo_longitude</span>";
      $xhtml .= "  </div>\n";
    }
    $status = $hcalendar['status'];
    if ($status) $xhtml .='  <span class="status">'.$status.'</span>'."<br/>\r\n";
    $duration = $hcalendar['duration'];
    if ($duration) $xhtml .='  <span class="duration">'.$duration.'</span>'."<br/>\r\n";
    $uid= $hcalendar['uid'];
    if ($uid) $xhtml .='  <span class="uid">'.$uid.'</span>'."<br/>\r\n";
    $url = $hcalendar['url'];
    if ($url) $xhtml .='  <span class="url">'.$url.'</span>'."<br/>\r\n";
    $last_modified = $hcalendar['last-modified'];
    if ($last_modified) $xhtml .='  <span class="last-modified">'.$last_modified.'</span>'."<br/>\r\n";
    $description = $hcalendar['description'];
    if ($description) $xhtml .='  <span class="description">'.$description.'</span>'."<br/>\r\n";
    $adr = $hcalendar['adr'];
    if ($adr) {
      $xhtml .= "  <div class=\"adr\">\n";
      $adr_type = $adr['address-type'];
      if ($adr_type) $xhtml .= '    <span class="address-type">'.$adr_type.'</span>'."<br/>\r\n"; 
      $xhtml .="  </div>";
    }
    $xhtml .= '</div>'."\r\n";    
  }
	$xhtml.='</body>'."\r\n";
  $xhtml.='</html>'."\r\n";
  drupal_set_header('Content-Type: text/html');
  //var_dump($view);
  print $xhtml;
  module_invoke_all('exit');
  exit;
}