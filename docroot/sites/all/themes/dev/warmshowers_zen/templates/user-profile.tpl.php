<?php
/**
 * @file user-profile.tpl.php
 *
 * warmshowers_zen version of user profile theming.
 *
 * Available variables
 * - $uid
 * - $username
 * - $fullname
 * - $homephone
 * - $mobilephone
 * - $workphone
 * - $street
 * - $additional
 * - $city
 * - $province
 * - $country
 * - $postal_code
 * - $latitude
 * - $longitude
 * - $reference_count
 * - $last_login
 * - $responsive_member
 * - $member_hosted_me
 * - $services
 * - $URL
 * - $motel, $bikeshop, $maxcyclists, $campground, $languagesspoken
 * - $global_stats - array()
 * - $personal_stats - array()
  */
?>
<?php drupal_set_title(check_plain($account->fullname)); ?>
<div id="profile-container">
  <div id="profile-top">
    <div id="profile-image"><?php
      if (!empty($photo_scolding)) {
        print $photo_scolding;
      } else {
        print theme('user_picture', $account);
      } ?></div>
    <div id="name-title">
      <h3><?php print $fullname; ?></h3>

      <ul id="user_stats"><?php $i=0; foreach ($global_stats as $classname=>$stat){
        $i++;
        $classname .= " number";
            if ($i == 0) { $classname .= " leaf-first"; }
        ?><li class="<?php print $classname; ?>"><?php print $stat; ?></li><?php
      }
        $i=0;
        if ($personal_stats) {
          foreach ($personal_stats as $classname=>$stat) {
        $i++;
        $classname .= " personal";
            if ($i == count($personal_stats)) {
              $classname .= " leaf-last";
            }
            print '<li class="' . $classname . '">' . $stat . '</li>';
          }
        }
    ?></ul>
    </div>
    <div id="profile-tabs">
      <?php print $menu_primary_local_tasks; ?>
    </div>
  </div>
  <div class="content">
    <h1><?php print t('About this Member'); ?></h1>
    <?php print check_markup($account->comments); ?>
    <div id="guest-info">
      <h2><?php print t('Traveler information'); ?></h2>
      <div class="member-info"></div>
        <?php
          // TODO: Needs to be refactored into the profile preprocess. Too much logic here.
          $result_count = 0;
          $view = warmshowers_site_embed_view('traveler_trip', 'block_1', $result_count, $account->uid);
          if ($result_count) {
            print '<h3>' . t('Current/Upcoming tour'). '</h3>';
            print $view;
          }
        ?>
      <div class="member-info"></div>
      <?php
        $result_count = 0;
        $view = warmshowers_site_embed_view('traveler_trip', 'default', $result_count, $account->uid);
        if ($result_count) {
          print '<h3>' . t('All tours'). '</h3>';
          print $view;
        }

      ?>

    </div>
    <div id="host-services">
      <h2><?php print t('Hosting information'); ?></h2>
      <?php if ($notcurrentlyavailable) : ?>
        <?php print t('This member has marked themselves as not currently available for hosting, so their hosting information is not displayed. <br/>Expected return @return.', array('@return' => $return_date)); ?>
      <?php else: ?>
        <?php foreach (array('preferred_notice', 'maxcyclists', 'bikeshop', 'campground', 'motel') as $item) : ?>
           <?php if (!empty($$item)): ?>
             <div class="member-info-<?php print $item; ?>"><span class="item-title"><?php print $fieldlist[$item]['title'];?></span>: <span class="item-value"><?php print $$item; ?></span></div>
           <?php endif; ?>
        <?php endforeach; ?>
        <h4><?php print t('This host may offer'); ?></h4>
        <ul>
          <?php print $services; ?>
        </ul>
      <?php endif; ?>
    </div>
    <div id="recommendations">
      <h2><?php print t('Feedback'); ?></h2>
      <?php print views_embed_view('user_referrals_by_referee', 'block_1', $account->uid); ?>
    </div>
  </div>
</div>
