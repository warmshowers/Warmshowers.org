<?php
/**
 * Member actions block template
 *
 * Supported variables
 * - $account (not sanitized)
 * - $uid
 * - $username
 * - $fullname (User's full name from wsuser)
 * - $homephone
 * - $mobilephone
 * - $homephone
 * - $street
 * - $additional
 * - $city
 * - $province
 * - $country
 * - $postal_code
 * - $latitude
 * - $longitude
 * - $responsive_member: Count of times marked responsive
 * - $unresponsive_member : Count of times marked unresponsive
 * @see wsuser_preprocess_wsuser_member_contact_location()
 */

?>

<?php 

if (!$is_self):
	foreach($wsuser_member_actions_block_links as $link){
		if($link['type']=='flag_checkbox'){
			$f=flag_get_flag($link['data']['flag_name']);
			$html=flag_create_link($link['data']['flag_name'],$link['data']['user_id']);
			if (!$f->is_flagged($link['data']['user_id']) or $f->access($link['data']['user_id'], 'unflag')){
				//dirty code to add css classes to the link, Dont know how to do it properly with flag module but I guess it's not possible.
				print str_replace('linkbutton','linkbutton dark big rounded',$html);
				}
			else{
				print $html;
				}
			}
		else print theme($link['type'],$link['data']);
	}
endif;
?>
