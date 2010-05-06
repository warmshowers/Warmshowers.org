<?php
// $Id: views-view-rdf.tpl.php,v 1.1.2.8 2009/05/22 01:25:41 allisterbeharry Exp $
/**
 * @file views-view-rdf.tpl.php
 * View template to render views as RDF. Supports FOAF and SIOC vocabulary.
 *
 * - $view: The view in use.
 * - $rows: The raw result objects from the query, with all data it fetched.
 * - $options: The options for the style passed in from the UI.
 *
 * @ingroup views_templates
 * @see views_rdf.views.inc
 */

if ($options['vocabulary'] == 'FOAF') rdf_foaf_xml_render($view);
if ($options['vocabulary'] == 'SIOC') rdf_sioc_xml_render($view);

/**
 * Render nodes as FOAF in XML
 *
 * @param array $nodes
 * @return none
 */
function rdf_foaf_xml_render($view) {
	global $base_url;
  $xml .= '<?xml version="1.0" encoding="UTF-8" ?>'."\n";
  $xml .= '<!-- generator="Drupal Views_Datasource.Module" -->'."\n";
  $xml .= '<rdf:RDF xmlns="http://xmlns.com/foaf/0.1"'."\n";
  $xml .= '  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"'."\n";
  $xml .= '  xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"'."\n";
  $xml .= '  xmlns:dc="http://purl.org/dc/elements/1.1/"'."\n";
  $xml .= '  xmlns:foaf="http://xmlns.com/foaf/0.1/">'."\n";
  foreach ($view->result as $node) {
    $xml.="<foaf:Person>";
    foreach($node as $field_label => $field_value) {
      $label = views_rdf_strip_illegal_chars($field_label);
      $value = views_xml_strip_illegal_chars(views_xml_is_date($field_value));
      if (is_null($value) || ($value === '')) continue;
//      if (strtotime($value))
//        $value = date(DATE_ISO8601, strtotime($value));
      if (stripos($label, 'firstname') !== false) {
        $xml.="  <foaf:firstName>$value</foaf:firstName>\n";
        continue;
      }      
      if (stripos($label, 'surname') !== false) {
        $xml.="  <foaf:surName>$value</foaf:surName>\n";
        continue;
      }      
      if ((stripos($label, 'name') == true) && ((stripos($label, 'surname') === false) && (stripos($label, 'firstname') === false))) {
        //if (stripos($xml, "<foaf:name>") === false)
          $xml.="  <foaf:name>$value</foaf:name>\n";
        continue;
      }
      if (stripos($label, 'title') !== false) {
        $xml.="  <foaf:title>$value</foaf:title>\n";
        continue;
      }
      if (stripos($label, 'nick') !== false) {
        $xml.="  <foaf:nick>$value</foaf:nick>\n";
        continue;
      }
      if ((stripos($label, 'mbox') !== false) && !(stripos($label, 'mbox_sha1sum') !== false)) {
        $xml.="  <foaf:mbox>$value</foaf:mbox>\n";
        continue;
      }
      if ((stripos($label, 'mail') == true) && (stripos($xml, '<foaf:mbox>') == false)) {
          $xml.="  <foaf:mbox>$value</foaf:mbox>\n";
          $xml.="  <foaf:mbox_sha1sum>".md5("mailto:".$value)."</foaf:mbox_sha1sum>\n";
        continue;
      }
      if (stripos($label, 'mbox_sha1sum') !== false) {
        $xml.="  <foaf:mbox_sha1sum>$value</foaf:mbox_sha1sum>\n";
        continue;
      }
      if (stripos($label, 'openid') !== false) {
        $xml.="  <foaf:openid>$value</foaf:openid>\n";
        continue;
      }
      if (strpos($label, 'workplaceHomepage') !== false) {
        $xml.='  <foaf:workplaceHomepage rdf:resource="'.$value.'"/>'."\n";
        continue;
      }
      if (strpos($label, 'homepage') !== false) {
        $xml.='  <foaf:homepage rdf:resource="'.$value.'"/>'."\n";
        continue;
      } 
      if (stripos($label, 'weblog') !== false) {
        $xml.='  <foaf:weblog rdf:resource="'.$value.'"/>'."\n";
        continue;
      }
      if (strpos($label, 'img') !== false) {
        $xml.='  <foaf:img rdf:resource="'.$value.'"/>'."\n";
        $xml.='  <foaf:depiction rdf:resource="'.$value.'"/>'."\n";
        continue;
      }
      if (stripos($label, 'member') !== false) {
        $xml.="  <foaf:member>$value</foaf:member>\n";
        continue;
      }      
      if (stripos($label, 'phone') !== false) {
        $xml.="  <foaf:phone>$value</foaf:phone>\n";
        continue;
      }
      if (stripos($label, 'jabberID') !== false) {
        $xml.="  <foaf:jabberID>$value</foaf:jabberID>\n";
        continue;
      }
      if (stripos($label, 'msnChatID') !== false) {
        $xml.="  <foaf:msnChatID>$value</foaf:msnChatID>\n";
        continue;
      }
      if (stripos($label, 'aimChatID') !== false) {
        $xml.="  <foaf:aimChatID>$value</foaf:aimChatID>\n";
        continue;
      }
      if (stripos($label, 'yahooChatID') !== false) {
        $xml.="  <foaf:yahooChatID>$value</foaf:yahooChatID>\n";
        continue;
      }            
    }
    $xml.="</foaf:Person>\n";
  }
  $xml.="</rdf:RDF>\n";
  if ($view->override_path) //inside live preview 
    print htmlspecialchars($xml);
  else {  
    drupal_set_header('Content-Type: application/rdf+xml');
    print $xml;
    module_invoke_all('exit');
    exit;
  }  
  
}

/**
 * Render users, blog and forum posts and comments, as SIOC in XML
 *
 * @param object $view
 * @return none
 */
function rdf_sioc_xml_render($view) {
	//var_dump($view);
	//module_invoke_all('exit');
  //exit;
	global $base_url;
	$xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	$xml .= '<!-- generator="Drupal Views_Datasource.Module" -->'."\n";
  $xml .= "<rdf:RDF\r\n";
  $xml .= "  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\r\n";
  $xml .= "  xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\r\n";
  $xml .= "  xmlns:sioc=\"http://rdfs.org/sioc/ns#\"\r\n";
  $xml .= "  xmlns:sioct=\"http://rdfs.org/sioc/terms#\"\r\n";
  $xml .= "  xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\r\n";
  $xml .= "  xmlns:dcterms=\"http://purl.org/dc/terms/\"\r\n";
  $xml .= "  xmlns:admin=\"http://webns.net/mvcb/\"\r\n";
  $xml .= "  xmlns:foaf=\"http://xmlns.com/foaf/0.1/\">\r\n";
  if ($view->base_table == 'users') {
  	$has_uid = false;
  	$has_name = false;
  	$has_email = false;
    foreach($view->field as $field) {
    	//if (($field->field_alias == 'uid') && ($field['options']['field'] ==  'uid'))
    	if ($field->options['field'] ==  'uid') 
    	  $has_uid = true;
    	if ($field->options['field'] ==  'name') 
        $has_name = true;
      if ($field->options['field'] ==  'mail') 
        $has_email = true;   
    }
  	if (!$has_uid) {
  		if ($view->override_path)
  		  print ('<b style="color:red">The uid field must be present.</b>');
      else
  	    drupal_set_message('The uid field must be present.', 'error');
  	  return;
  	}
    if (!$has_name) {
      if ($view->override_path)
        print ('<b style="color:red">The name field must be present.</b>');
      else
        drupal_set_message('The name field must be present.', 'error');
      return;
    }
    if (!$has_email) {
      if ($view->override_path)
        print ('<b style="color:red">The email field must be present.</b>');
      else
        drupal_set_message('The email field must be present.', 'error');
      return;
    } 
    $xml .= "<foaf:Document rdf:about=\"".url($view->name, array('absolute'=>true))."\">\n";
    $xml .= "  <dc:title>SIOC user profiles for: ".variable_get('site_name', 'drupal')."</dc:title>\n";
    $xml .= "  <dc:description>\n";
    $xml .= "    A User is an online account of a member of an online community. 
     It is connected to Items and Posts that a User creates or edits, 
     to Containers and Forums that it is subscribed to or moderates and 
     to Sites that it administers. Users can be grouped for purposes of 
     allowing access to certain Forums or enhanced community site features (weblogs, webmail, etc.).
     A foaf:Person will normally hold a registered User account on a Site 
     (through the property foaf:holdsAccount), and will use this account 
     to create content and interact with the community. sioc:User describes 
     properties of an online account, and is used in combination with a 
     foaf:Person (using the property sioc:account_of) which describes 
     information about the individual itself.\n";
    $xml .= "  </dc:description>\n";
    $xml .= "####foaf_topics####\n";
    $xml .= "  <admin:generatorAgent rdf:resource=\"http://drupal.org/project/views_datasource\"/>\n";
    $xml .= "</foaf:Document>\n";
    foreach($view->result as $node) $xml .= rdf_sioc_xml_user_render($node);     
  }
  if ($view->base_table == 'node') {
    $has_nid = false;
    $has_type = false;
    $has_created = false;
    $has_changed = false;
    $has_last_updated = false;
    $has_title = false;
    $has_body = false;
    $has_uid = false;
    foreach($view->field as $field) {
      //if (($field->field_alias == 'uid') && ($field['options']['field'] ==  'uid'))
      if ($field->options['field'] ==  'nid') 
        $has_nid = true;
      if ($field->options['field'] ==  'type') 
        $has_type = true;
      if ($field->options['field'] ==  'created') 
        $has_created = true;
      if ($field->options['field'] ==  'changed') 
        $has_changed = true;
      if ($field->options['field'] ==  'last_updated') 
        $has_last_updated = true;
      if ($field->options['field'] ==  'title') 
        $has_title = true;
      if ($field->options['field'] == 'body')
        $has_body = true;
      if ($field->options['field'] ==  'uid') 
        $has_uid = true;        
    }
    if (!$has_nid) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Nid field must be present.</b>');
      else
        drupal_set_message('The Node: Nid field must be present.', 'error');
      return;
    }
    if (!$has_type) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Type field must be present.</b>');
      else
        drupal_set_message('The Node: Type field must be present.', 'error');
      return;
    }
    if (!$has_created) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Post date field must be present.</b>');
      else
        drupal_set_message('The Node: Post date field must be present.', 'error');
      return;
    }
    if (!$has_changed) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Updated date field must be present.</b>');
      else
        drupal_set_message('The Node: Updated date field must be present.', 'error');
      return;
    }
    if (!$has_last_updated) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Updated/commented date field must be present.</b>');
      else
        drupal_set_message('The Node: Updated/commented date field must be present.', 'error');
      return;
    }
    if (!$has_title) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Title field must be present.</b>');
      else
        drupal_set_message('The Node: Title field must be present.', 'error');
      return;
    }
    if (!$has_body) {
      if ($view->override_path)
        print ('<b style="color:red">The Node: Body field must be present.</b>');
      else
        drupal_set_message('The Node: Body field must be present.', 'error');
      return;
    }    
    if (!$has_uid) {
      if ($view->override_path)
        print ('<b style="color:red">The User: Uid field must be present.</b>');
      else
        drupal_set_message('The User: Uid field must be present.', 'error');
      return;
    }
    $users = array();
    $nodes = array();
    $xml .= "<foaf:Document rdf:about=\"".url($view->name, array('absolute'=>true))."\">\n";
    $xml .= "  <dc:title>SIOC profile for: ".variable_get('site_name', 'drupal')."</dc:title>\n";
    $xml .= "  <dc:description>\n";
    $xml .= "    A SIOC profile describes the structure and contents of a weblog in a machine readable form. For more information please refer to http://sioc-project.org/.
    A Post is an article or message posted by a User to a Forum or Site. A series of Posts 
    may be threaded if they share a common subject and are connected by reply or 
    by date relationships. Posts will have content and may also have attached 
    files, which can be edited or deleted by the Moderator of the Forum or Site that 
    contains the Post.\n";
    $xml .= "  </dc:description>\n";
    //$xml .= "  <foaf:primaryTopic rdf:resource=\"$node_url\"/>\n";
    $xml .= "  <admin:generatorAgent rdf:resource=\"http://drupal.org/project/views_datasource\"/>\n";
    $xml .= "</foaf:Document>\n";
    foreach($view->result as $node) rdf_sioc_xml_node_render($node, &$users, &$nodes);
    foreach ($users as $user_xml) $xml .= $user_xml;
    foreach ($nodes as $node_xml) $xml .= $node_xml;
  }
  $xml.="</rdf:RDF>\n";
  if ($view->override_path) //inside live preview 
    print htmlspecialchars($xml);
  else {  
    drupal_set_header('Content-Type: application/rdf+xml');
    print $xml;
    module_invoke_all('exit');
    exit;
  }
}

function rdf_sioc_xml_user_render($node, $uid=null, $user_name=null, $user_email=null) {
  if (func_num_args() == 1) {
	  foreach($node as $field_label=>$field_value) {
      $label = views_rdf_strip_illegal_chars($field_label);
      $value = views_xml_strip_illegal_chars(views_xml_is_date($field_value));
      if (is_null($value) || ($value === '')) continue;
//      if (strtotime($value))
//        $value = date(DATE_ISO8601, strtotime($value));
      if ((strtolower($label) == 'id') || (strtolower($label) == 'uid')) {
        $uid = $value;      
      }
      if ((strtolower($label) == 'name') || (strtolower($label) == 'users_name')) {
        $user_name = $value;      
      }
      if ((strtolower($label) == 'email') || (strtolower($label) == 'users_mail')) {
        $user_email = $value;      
      }            
    }
    if (empty($user_name)) return;
  }
  $xml .="<foaf:Person rdf:about=\"".url('user/'.$uid, array('absolute'=>true))."\">\n";
  $xml .="  <foaf:name>$user_name</foaf:name>\n";
  $xml .="  <foaf:mbox_sha1sum>".md5('mailto:'.$user_email)."</foaf:mbox_sha1sum>\n";
  $xml .="  <foaf:holdsAccount>\n";
  $xml .="    <sioc:User rdf:nodeID=\"$uid\">\n";
  $xml .="      <sioc:name>$user_name</sioc:name>\n";
  $xml .="      <sioc:email rdf:resource=\"mailto:$user_email\"/>\n";
  $xml .="      <sioc:email_sha1>".md5('mailto:'.$user_email)."</sioc:email_sha1>\n";
  $xml .="      <sioc:link rdf:resource=\"".url('user/'.$uid, array('absolute'=>true))."\" rdfs:label=\"$user_name\"/>\n";
  $roles = array();
  $roles_query = db_query("SELECT r.name AS name, r.rid AS rid FROM {users_roles} ur, {role} r WHERE ur.uid = %d AND ur.rid = r.rid", $uid);
  while ($role = db_fetch_object($roles_query)) $roles[$role->rid] = $role->name;
  if (count($roles) > 0) {
    $xml .="      <sioc:has_function>\n";
    foreach($roles as $rid=>$name)
    $xml .="        <sioc:Role><rdfs:label><![CDATA[$name]]></rdfs:label></sioc:Role>\n";
    $xml .="      </sioc:has_function>\n";    
  }             
  $xml .="    </sioc:User>\n";
  $xml .="  </foaf:holdsAccount>\n";
  $xml .="</foaf:Person>\n";  
	return $xml;
}

function rdf_sioc_xml_node_render($node, &$users=null, &$nodes = null) {
	global $base_url;
  //i
  foreach($node as $field_label=>$field_value) {
    $label = views_rdf_strip_illegal_chars($field_label);
    $value = views_rdf_strip_illegal_chars(views_rdf_is_date($field_value));
    if (is_null($value) || ($value === '')) continue;
//    if (strtotime($value))
//      $value = date(DATE_ISO8601, strtotime($value));
    if ((strtolower($label) == 'id') || (strtolower($label) == 'nid')) {
      $nid = $value;      
    }
    if ((strtolower($label) == 'title') || (strtolower($label) == 'node_title')) {
      $title = $value;      
    }
    if ((strtolower($label) == 'body') || (strtolower($label) == 'node_revisions_body')) {
      $body = $value;      
    }
    if ((strtolower($label) == 'type') || (strtolower($label) == 'node_type')) {
      $type = $value;      
    }
    if ((strtolower($label) == 'created') || (strtolower($label) == 'node_created')) {
      $created = $value;
    }
    if ((strtolower($label) == 'changed') || (strtolower($label) == 'node_changed')) {
      $changed = $value;      
    }            
    if ((strtolower($label) == 'last_updated') || (strtolower($label) == 'node_comment_statistics_last_updated')) {
      $last_updated = $value;      
    }
    if ((strtolower($label) == 'uid') || (strtolower($label) == 'users_uid')) {
      $uid = $value;      
    }    
  }
  $user = user_load($uid);  
  if (!array_key_exists($uid, $users)) $users[$uid] = rdf_sioc_xml_user_render(null, $uid, $user->name, $user->mail);
  if (!array_key_exists($nid, $nodes)) {
    if (($type == 'page') || ($type == 'story') || ($type == 'forum') || ($type == 'blog')) $nodes[$nid] = rdf_sioc_xml_story_render($xml, $nid, $title, $type, $created, $changed, $last_updated, $uid, $body);  	
  }
  //$xml = '';
  //var_dump($nodes);
  //var_dump($users);
  //module_invoke_all('exit');
  return;
}

function rdf_sioc_xml_story_render($xml, $nid, $title, $type, $created, $changed, $last_updated, $uid, $body) {
	$node_url = url($nid, array('absolute'=>true));
  $xml .= "<sioc:Post rdf:about=\"$node_url\">\n";
  $xml .= "  <dc:title>$title</dc:title>\n";
  $xml .= "  <sioc:content>\n ";
  $xml .= "    <![CDATA[$body]]>\n";
  $xml .= "  </sioc:content>\n";
  $xml .= "  <dc:created>".date(DATE_ISO8601, $created)."</dc:created>\n";
  $xml .= "  <dc:modified>".date(DATE_ISO8601, $changed)."</dc:modified>\n";
  $xml .= "  <sioc:link rdf:resource=\"$node_url\" rdfs:label=\"$title\" />\n";
  $xml .= "  <sioc:has_creator rdf:nodeID=\"$uid\"/>\n";
  
  /*Add taxonomy terms as SIOC topics*/
  $query = db_query('SELECT tn.tid AS tid, td.name AS name FROM {term_node} tn, {term_data} td WHERE td.tid = tn.tid AND tn.nid = %d', $nid);
  while ($term = db_fetch_object($query)) {
    $taxonomy_terms = "  <sioc:topic rdfs:label=\"$term->name\" rdf:resource=\"".url("taxonomy/term/$term->tid", array('absolute' => TRUE))."\" />\n";
  }
  $xml .= $taxonomy_terms;
  
  /*Add comments as SIOC replies*/
  $query_count = 'SELECT COUNT(*) FROM {comments} WHERE nid = %d AND status = %d';
  $query = 'SELECT c.cid as cid, c.pid, c.nid, c.subject, c.comment, c.format, c.timestamp, c.name, c.mail, c.homepage, u.uid, u.name AS registered_name, u.signature, u.picture, u.data, c.thread, c.status FROM {comments} c INNER JOIN {users} u ON c.uid = u.uid WHERE c.nid = %d and c.status = %d ORDER BY SUBSTRING(c.thread, 1, (LENGTH(c.thread) - 1))';
  $query_args = array($nid, COMMENT_PUBLISHED);
  $query = db_rewrite_sql($query, 'c', 'cid');
  $comment_children = 0;
  $num_rows = FALSE;
  $comments = '';
  $result = db_query($query, $query_args);
  while ($comment = db_fetch_object($result)) {
    $comment = drupal_unpack($comment);
//    var_dump($comment);module_invoke_all('exit');return;
//    $comment->depth = count(explode('.', $comment->thread)) - 1;
//    if ($comment->depth > $comment_children) {
//      $comment_children++;
//      $comments .= "  <sioc:has_reply>\n";
//    }
//    else {
//      while ($comment->depth < $comment_children) {
//        $comment_children--;
//        $comments .= "  </sioc:has_reply>\n";
//      }
//    }        
//    $comments .="     <sioc:Post rdf:about=\"$node_url#comment-$comment->cid\">\n";    
//    while ($comment_children-- > 0) {
//      $num_rows = TRUE;
//      $comments .="       <sioc:content><![CDATA[$comment->comment]]></sioc:content>\n";
//      $comments .="     </sioc:Post>\n";
//      $comments .= "  </sioc:has_reply>\n";
//    }
//  }
    $comments .= "  <sioc:has_reply>\n";
    $comments .= "    <sioc:Post rdf:about=\"$node_url#comment-$comment->cid\">\n";
    if ($comment->subject) $comments .= "      <dc:title>$comment->subject</dc:title>\n";
    if ($comment->timestamp) $comments .= "      <dc:created>".date(DATE_ISO8601, $comment->timestamp)."</dc:created>\n";
    if ($comment->uid) {
    	$comments .= "    <sioc:has_creator>\n";
      $comments .= "      <sioc:User>\n";
      $comments .= "        <sioc:name>$comment->registered_name</sioc:name>\n";
      $comments .= "        <sioc:email rdf:resource=\"mailto:$comment->mail\"/>\n";
      $comments .="         <sioc:link rdf:resource=\"".url('user/'.$comment->uid, array('absolute'=>true))."\" rdfs:label=\"$comment->registered_name\"/>\n";
  	  $comments .= "      </sioc:User>\n";
  	  $comments .= "    </sioc:has_creator>\n";
    }    	    
    $comments .= "      <sioc:content><![CDATA[$comment->comment]]></sioc:content>\n";
    $comments .= "    </sioc:Post>\n";
    $comments .= "  </sioc:has_reply>\n";
  }
  $xml.= $comments;
  
  $xml .= "</sioc:Post>\n";
	return $xml;
}	 