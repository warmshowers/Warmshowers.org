<div id='friends'>
<?php

global $base_url;

print '<div class="friends_list">';

if ($is_self) {
  print '<h2>'.t('My friends list').'</h2>';
  print '<h3>'.t('Users you added as friends').'</h3>';
}
else {
  print '<h2>'.t($username.' friends list').'</h2>';
  print '<h3>'.t('Users '.$username.' added as friends').'</h3>';
}

if (sizeof($my_friends) == 0) {
	if ($is_self) {
   print '<p>'.t('You haven\'t added any user yet').'</p>';
  }
	else {
    print '<p>'.t($username.' hasn\'t added any user yet').'</p>';
  }
}
else {
	foreach ($my_friends as $friend) {
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
}

print '</div>';
print '<div class="friends_list clear">';

if ($is_self) {
  print '<h3>'.t('Users who added you as a friend').'</h3>';
}
else {
  print '<h3>'.t('Users who added '.$username.' as a friend').'</h3>';
}

if (sizeof($friend_of) == 0) {
  if ($is_self) {
   print '<p>'.t('Nobody has added you as a friend yet').'</p>';
  }
  else {
    print '<p>'.t('Nobody has added '.$username.' as a friend yet').'</p>';
  }
}
else {
  foreach ($friend_of as $friend) {
    print '<div class="friend">
      <div class="friend_picture">
      <a href="/user/'.$friend->uid.'"><img alt="'.$friend->name.'\'s picture" href="'.$base_url.'/'.$friend->picture.'"/></a>
      </div>
      <div class="friends_att">
      <div class="friend_name"><a href="/user/'.$friend->uid.'">'.$friend->name.' ('.$friend->fullname.')</a></div>
      <div class="friend_loc clear">'.t('Location :').' '.$friend->city.'</div>
      <div class="friend_date clear">'.t('Friend since : ').format_date($friend->added_as_friend,'small').'</div>';
    print "</div></div>";
  }
}
print '</div>';
?>
</div>

