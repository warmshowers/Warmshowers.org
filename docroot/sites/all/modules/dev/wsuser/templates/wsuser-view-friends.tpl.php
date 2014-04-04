<?php
/**
 * @file
 * Template for rendering friends.
 */
;?>

<?php global $base_url; ?>
<div id='friends'>
<?php if ($is_self):
  $title1 = t('My friends');
  $title2 = t('My pending friends requests');
  $title3 = t('Friends waiting for your approval');
  $title4 = t('My friends list');
  else:
  $title1 = t("@name's friends", array('@name' => $username));
  $title2 = t("@name's pending friends", array('@name' => $username));
  $title3 = t("Friends waiting for @name's approval", array('@name' => $username));
  $title4 = t("@name's friends list", array('@name' => $username));
  endif;
?>
  
<h2><?php print $title1;?></h2>

<?php if (count($my_pending_friends)!=0):?>
<h3><?php print $title2;?></h3>
<div class="friends_list clear">
<?php foreach ($my_pending_friends as $friend): ?>
  <div class="friend">
    <div class="friend_picture">
      <a href="/user/<?php print $friend->uid;?>">
        <img alt="<?php print t("@name's picture", array('@name' => $friend->name));?>" href="<?php print $base_url;?>/<?php print $friend->picture;?>"/>
      </a>
    </div>
    <div class="friends_att">
      <div class="friend_name"><?php print l(t('@name (@fullname)', array('@name' => $friend->name, '@fullname' => $friend->fullname)), '/user/' . $friend->uid);?></div>
      <div class="friend_loc clear"><?php print t('Location: @loc', array('@loc' => $friend->city));?></div>
      <div class="friend_date clear"><?php print t('Friend since: @date', array('@date' => format_date($friend->added_as_friend, 'small')));?></div>
      <?php if ($is_self): ?>
        <?php print flag_create_link('friends',$friend->uid);?>
      <?php endif; ?>
    </div>
  </div>
<?php endforeach; ?>
</div>
<?php endif;?>

<?php if (count($my_waiting_friends)!=0):?>
<h3 class="clear"><?php print $title3;?></h3>
<div class="friends_list clear">
<?php foreach ($my_waiting_friends as $friend): ?>
    <div class="friend">
      <div class="friend_picture">
        <a href="/user/<?php print $friend->uid;?>">
          <img alt="<?php print t("@name's picture", array('@name' => $friend->name));?>" href="<?php print $base_url;?>/<?php print $friend->picture;?>"/>
        </a>
      </div>
      <div class="friends_att">
        <div class="friend_name"><?php print l(t('@name (@fullname)', array('@name' => $friend->name, '@fullname' => $friend->fullname)), '/user/' . $friend->uid);?></div>
        <div class="friend_loc clear"><?php print t('Location: @loc', array('@loc' => $friend->city));?></div>
        <div class="friend_date clear"><?php print t('Friend since: @date', array('@date' => format_date($friend->added_as_friend, 'small')));?></div>
        <?php if ($is_self): ?>
          <?php print flag_create_link('friends',$friend->uid);?>
        <?php endif; ?>
      </div>
    </div>
  <?php endforeach; ?>
</div>
<?php endif;?>

<h3 class="clear"><?php print $title4;?></h3>
<?php if (count($my_friends)!=0):?>
<div class="friends_list clear">
<?php foreach ($my_friends as $friend): ?>
  <div class="friend">
    <div class="friend_picture">
      <a href="/user/<?php print $friend->uid;?>">
        <img alt="<?php print t("@name's picture", array('@name' => $friend->name));?>" href="<?php print $base_url;?>/<?php print $friend->picture;?>"/>
      </a>
    </div>
    <div class="friends_att">
      <div class="friend_name"><?php print l(t('@name (@fullname)', array('@name' => $friend->name, '@fullname' => $friend->fullname)), '/user/' . $friend->uid);?></div>
      <div class="friend_loc clear"><?php print t('Location: @loc', array('@loc' => $friend->city));?></div>
      <div class="friend_date clear"><?php print t('Friend since: @date', array('@date' => format_date($friend->added_as_friend, 'small')));?></div>
      <?php if ($is_self): ?>
        <?php print flag_create_link('friends',$friend->uid);?>
      <?php endif; ?>
    </div>
  </div>
  <?php endforeach; ?>
</div>
<?php else:?>
  <?php if ($is_self): ?>
    <p><?php print t('You haven\'t marked any users as friends yet. To mark a user as a friend, go to their profile page and click "Add to your friendlist"');?></p>
  <?php else: ?>
    <p><?php print t("@name hasn't marked any users as a friend yet", array('@name' => $username));?></p>
  <?php endif; ?>
<?php endif;?>
</div>
