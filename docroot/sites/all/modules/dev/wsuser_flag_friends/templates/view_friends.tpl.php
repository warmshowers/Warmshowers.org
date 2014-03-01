<div id='friends'>
<?php

global $base_url;

if ($is_self) {
  print '<h2>'.t('My friends list').'</h2>';
}
else {
  print '<h2>'.t($username.' friends list').'</h2>';
}

if (sizeof($friend_list) == 0) {
	if ($is_self) {
   print '<p>'.t('You don\'t have any friends yet').'</p>';
  }
	else {
    print '<p>'.t($username.' doesn\'t have any friends yet').'</p>';
  }
}
else {
	print '<div class="friends_list">';
	foreach ($friend_list as $friend) {
	  print '<div class="friend">
	    <div class="friend_picture">
		  <a href="/user/'.$friend->uid.'"><img alt="'.$friend->name.'\'s picture" href="'.$base_url.'/'.$friend->picture.'"/></a>
	    </div>
	    <div class="friends_att">
		  <div class="friend_name"><a href="/user/'.$friend->uid.'">'.$friend->name.' ('.$friend->fullname.')</a></div>
		  <div class="friend_loc clear">'.t('Location :').' '.$friend->city.'</div>
		  <div class="friend_date clear">'.t('Friend since : ').format_date($friend->added_as_friend,'small').'</div>';
		if ($is_self) {
			print flag_create_link('friends',$friend->uid);
			}
	  print "</div></div>";
	}
	print '</div>';
}
?>
</div>

