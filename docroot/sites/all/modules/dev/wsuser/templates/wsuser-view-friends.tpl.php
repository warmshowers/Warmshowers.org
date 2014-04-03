<?php
/**
 * @file
 * Template for rendering friends.
 */
;?>

<?php global $base_url; ?>
<div id='friends'>
<div class="friends_list">

  <?php if ($is_self): ?>
    <h2><?php print t('My friends list'); ?></h2>
    <h3><?php print t('Users you added as friends');?></h3>
  <?php else: ?>
    <h2><?php print t("@name's friends list", array('@name' => $username));?></h2>
    <h3><?php print t('Users @name added as friends', array('@name' => $username));?></h3>
  <?php endif; ?>

  <?php if (count($my_friends) == 0):?>
    <?php if ($is_self): ?>
      <p><?php print t('You haven\'t marked any users as friends yet. To mark a user as a friend, go to their profile page and click "Add to your friendlist"');?></p>;
    <?php else: ?>
      <p><?php print t("@name hasn't marked any users as a friend yet", array('@name' => $username));?></p>
    <?php endif; ?>
  <?php else: ?>
    <?php foreach ($my_friends as $friend): ?>
      <div class="friend">
        <div class="friend_picture">
          <a href="/user/"><?php print $friend->uid;?>"><img alt="<?php print $friend->name;?>'s picture" href="'<?php print $base_url;?>'/'<?php print $friend->picture;?>'"/></a>
        </div>
	    <div class="friends_att">
        <div class="friend_name"><?php l(t('@name (@fullname)', array('@name' => $friend->name, '@fullname' => $friend->fullname)), '/user/' . $friend->uid);?></div>
        <div class="friend_loc clear"><?php print t('Location: @loc', array('@loc' => $friend->city));?></div>
        <div class="friend_date clear"><?php print t('Friend since: @date', array('@date' => format_date($friend->added_as_friend, 'small')));?></div>
      <?php if ($is_self): ?>
        <?php print flag_create_link('friends',$friend->uid);?>
      <?php endif; ?>
    <?php endforeach; ?>
    <?php endif; ?>
      </div>
      </div>
    </div>
    <div class="friends_list clear">

      <?php if ($is_self): ?>
        <h3><?php print t('Users who added you as a friend');?></h3>;
      <?php else: ?>
        <h3><?php print t('Users who added '.$username.' as a friend');?></h3>
      <?php endif;?>
      <?php if (sizeof($friend_of) == 0): ?>
        <?php if ($is_self): ?>
          <p><?php print t('Nobody has added you as a friend yet'); ?></p>
          <?php else: ?>
          <p><?php print t('Nobody has added '.$username.' as a friend yet');?></p>
        <?php endif;?>
      <?php else: ?>
        <?php foreach ($friend_of as $friend): ?>
          <div class="friend">
            <div class="friend_picture">
            <a href="/user/'.$friend->uid.'"><img alt="'.$friend->name.'\'s picture" href="'.$base_url.'/'.$friend->picture.'"/></a>
          </div>
          <div class="friends_att">
          <div class="friend_name"><a href="/user/'.$friend->uid.'">'.$friend->name.' ('.$friend->fullname.')</a></div>
          <div class="friend_loc clear">'.t('Location :').' '.$friend->city.'</div>
          <div class="friend_date clear">'.t('Friend since : ').format_date($friend->added_as_friend,'small').'</div>';
        </div></div>;
        <?php endforeach; ?>
      <?php endif; ?>
    </div>
</div>
