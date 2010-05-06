<?php
// $Id: views-view-xml.tpl.php,v 1.1.2.10 2009/10/07 15:12:12 allisterbeharry Exp $
/**
 * @file views-view-xml.tpl.php
 * View template to render view fields as XML. Supports raw XML, OPML and Atoma schema.
 *
 * - $view: The view in use.
 * - $rows: The raw result objects from the query, with all data it fetched.
 * - $options: The options for the style passed in from the UI.
 *
 * @ingroup views_templates
 * @see views_xml.views.inc
 */

if ($options['schema'] == 'raw') xml_raw_render($view);
if ($options['schema'] == 'opml') xml_opml_render($view);
if ($options['schema'] == 'atom') xml_atom_render($view);

function xml_raw_render($view) {
	$xml .= '<?xml version="1.0" encoding="UTF-8" ?>'."\n";
  $xml .= '<!-- generator="Drupal Views_Datasource.Module" -->'."\n";
  $xml .='<nodes>'."\n";  
  
  foreach ($view->result as $node) {
    $xml .= '  <node>'."\n";   
    foreach($node as $label => $value) {
      $label = views_xml_strip_illegal_chars($label);
      $value = views_xml_strip_illegal_chars(views_xml_is_date($value));
      if (is_null($value) || ($value === '')) continue;
//      if (preg_match('/\d/', $value)) {
//        if (strtotime($value))
//          $value = date(DATE_ISO8601, strtotime($value));
//      }     
      $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
      
      $xml .= "    <$label><![CDATA[$value]]></$label>\n";
    }
  $xml .= '  </node>'."\n";
  }
  $xml .='</nodes>'."\n";
  if ($view->override_path) //inside live preview 
    print htmlspecialchars($xml);
  else {  
   drupal_set_header('Content-Type: text/xml');
   print $xml;
   module_invoke_all('exit');
   exit;
  }
}

function xml_opml_render($view) {
	//var_dump($view);
	//return;
  global $user;
  global $base_url;
	$xml .= '<?xml version="1.0" encoding="UTF-8" ?>'."\n";
  $xml .= '<!-- generator="Drupal Views_Datasource.Module" -->'."\n";
  $xml .='<opml version="2.0">'."\n";  
	$xml .='<head>'."\n";
	$xml .='  <title>'.variable_get('site_name', 'drupal').'-'.$view->name.'</title>'."\n";
	$xml .='  <ownerName>'.$user->name.'</ownerName>'."\n";
	$xml .='  <ownerEmail>'.$user->mail.'</ownerEmail>'."\n";
	$xml .='  <docs>'.$base_url.'</docs>';
	$xml .='  <dateCreated>'.date(DATE_ISO8601, time()).'</dateCreated>'."\n";
	$xml .='</head>'."\n";
	$xml .='<body>'."\n";
  foreach ($view->result as $node) {
    $xml .= '  <outline ';
    $fieldcount = 0;   
    foreach($node as $field_name=>$field_value) {
      $label = views_xml_strip_illegal_chars($field_name);
      $value = views_xml_strip_illegal_chars(views_xml_is_date($field_value));
      if (is_null($value) || ($value === '')) continue;
      $fieldcount++;      
      $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
      if (is_null($value) || ($value === '')) continue;
      if ((strtolower($label) == 'text') || (strtolower($label) == 'node_revisions_body'))
        $label = "text";
      if (is_null($value) || ($value === '') || ($value === 0)) continue;
      if ((strtolower($label) == 'type') || (strtolower($label) == 'node_type'))
        $label = "type";  
      if ((strtolower($label) == 'id') || (strtolower($label) == 'nid')) { //if a nid is given construct the url attribute
      	//$url = $base_url.'index.php?q=node/'.$value;
      	$url = url("node/".$value, array('absolute'=>true));  
      }
        
      if ((strtolower($label) == 'published') || (strtolower($label) == 'node_created')) {
      	$label = 'created';
        if (intval($value)) //timestamp
          $value =  date(DATE_RFC822, intval($value)) ;
        else if(getdate($value)) //string date
           $value = date(DATE_RFC822, strtotime($value)); 
      }  
      $xml .= $label. '="'.preg_replace('/[^A-Za-z0-9 :\/\-_\.\?\=]/','',$value).'" ';
      
      //$xml .= $label. '="'.$value.'" ';
    }
    if ($url) $xml.=' '.'url="'.$url.'" ';
  $xml .=  ' />'."\n";
  }
	$xml .='</body>'."\n";
  $xml .='</opml>'."\n";
	if ($view->override_path) //inside live preview 
	  print htmlspecialchars($xml);
	else {  
   drupal_set_header('Content-Type: text/xml');
   print $xml;
   //var_dump($view);
   module_invoke_all('exit');
   exit;
	}
}

function xml_atom_render($view) {
	global $base_url;
	
  $xml .= '<?xml version="1.0" encoding="UTF-8" ?>'."\n";
  $xml .= '<!-- generator="Drupal Views_Datasource.Module" -->'."\n";
  $xml .='<feed xmlns="http://www.w3.org/2005/Atom" xml:lang="en">'."\n";  
  $xml .='  <title>'.$view->name.'</title>'."\n";
  $xml .='  <link rel="alternate" type="text/html" href="'.$base_url.'"/>'."\n";
  $xml .='  <link rel ="self" type="application/atom+xml" href="'.$base_url.'/'.$view->display_handler->options['path'].'"/>'."\n";
  $xml .='  <id>'.$base_url.'/'.$view->display_handler->options['path'].'</id>'."\n";//use path as id
  $xml .='  <updated>###feed_updated###</updated>'."\n"; //will set later 
  $xml .='  <generator>Views Datasource module</generator>'."\n"; 
  $feed_last_updated = 0;
  foreach ($view->result as $node) {
  	$entry = array();   
    foreach($node as $field_name=>$field_value) {
      $label = views_xml_strip_illegal_chars($field_name);
      $value = views_xml_strip_illegal_chars(views_xml_is_date($field_value));
      if (is_null($value) || ($value === '')) continue;
//      if (preg_match('/\d/', $value)) {
//        if (strtotime($value))
//          $value = date(DATE_ISO8601, strtotime($value));
//      }
      $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
      if (strtolower($label) == 'nid') $entry['nid'] = $value;
      if ((strtolower($label) == 'updated') || (strtolower($label) == 'updated date') || (strtolower($label) == 'node_changed')) {
      	if (intval($value)) //timestamp
      	  $entry['updated'] =  intval($value) ;
      	else if(getdate($value)) { //string date
      		$entry['updated'] = strtotime($value);
      	}
      } 
      if ((strtolower($label) == 'title') || (strtolower($label) == 'node_title')) 
        $entry['title'] = $value;
      if (strtolower($label) == 'link') $entry['link'] = $value;
      if ((strtolower($label) == 'published') || (strtolower($label) == 'node_created')) {
      	if (intval($value)) //timestamp
          $entry['published'] =  intval($value) ;
        else if(getdate($value)) { //string date
           $entry['published'] = strtotime($value);
      	}
      }
      if ((strtolower($label) == 'author') || (strtolower($label) == 'users_name')) $entry['author'] = $value;
      if ((strtolower($label) == 'email') || (strtolower($label) == 'users_mail')) $entry['email'] = $value;
      if ((strtolower($label) == 'content') || (strtolower($label) == 'node_revisions_body')) $entry['content'] = $value;
      if ((strtolower($label) == 'summary') || (strtolower($label) == 'node_teaser') || (strtolower($label) == 'node_revisions_teaser')) $entry['summary'] = $value;
    }
    if (isset($entry['nid']) && (isset($entry['updated'])) && (isset($entry['link'])) && (isset($entry['title'])) && (isset($entry['published']))) {
   	  if (parse_url($entry['link'])) 
   	    $link = $entry['link'];	
   	  else {
        print ('<b style="color:red">The link URL is not valid.</b>');
        return;
   	  }
    }
    elseif (isset($entry['nid']) && (isset($entry['updated'])) && (isset($entry['title'])) && (isset($entry['published']))) { //make the entry path with base_url + nid {
   	  $entry['link'] = $base_url.'/index.php?q=node/'.$entry['nid'];
    }
    else {
      print ('<b style="color:red">The fields "nid", "title", "post date", and "updated date" must exist.');
      return;  
    }
    $link = $entry['link'];
    $link_url = parse_url($link);
    $nid = $entry['nid'];
    $updated = $entry['updated'];
    if ($updated > $feed_last_updated) $feed_last_updated = $updated; //Overall feed updated is the most recent node updated timestamp
    $title = $entry['title'];
    $published = $entry['published'];
    $author = $entry['author'];
    $email = $entry['email'];
    $content = $entry['content'];
    $summary = $entry['summary'];
    
    //Create an id for the entry using tag URIs
    $id = 'tag:'.$link_url['host'].','.date('Y-m-d', $updated).':'.$link_url['path'].'?'.$link_url['query'];
    $xml .= '  <entry>'."\n";
    $xml .= '    <id>'.$id.'</id>'."\n"; 
    $xml .= '    <updated>'.date(DATE_ATOM, $updated).'</updated>'."\n";
    $xml .= '    <title type="text">'.$title.'</title>'."\n";
    $xml .= '    <link rel="alternate" type="text/html" href="'.$link.'"/>'."\n";
    $xml .= '    <published>'.date(DATE_ATOM, $published).'</published>'."\n";
    if ($author) {
    	if ($email) {
    		$xml .= '    <author><name>'.$author.'</name><email>'.$email.'</email></author>'."\n";
    	}
      else $xml .= '    <author><name>'.$author.'</name></author>'."\n";
    }
    if ($content)$xml .= '    <content type="html" xml:base="'.$base_url.'"><![CDATA['.$content.']]></content>'."\n";
    if ($summary)$xml .= '    <summary type="html" xml:base="'.$base_url.'"><![CDATA['.$summary.']]></summary>'."\n";
    $xml .= '  </entry>'."\n";
  }
  $xml .='</feed>'."\n";
  $xml = str_replace('###feed_updated###', date(DATE_ATOM, $feed_last_updated), $xml);
  if ($view->override_path) //inside live preview 
    print htmlspecialchars($xml);
  else {  
   drupal_set_header('Content-Type: application/atom+xml');
   print $xml;
   //var_dump($label);   
   module_invoke_all('exit');
   exit;
  }
}
