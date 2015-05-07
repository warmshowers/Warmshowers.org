<?php
/**
 * @file
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * Use render($user_profile) to print all profile items, or print a subset
 * such as render($user_profile['user_picture']). Always call
 * render($user_profile) at the end in order to print all remaining items. If
 * the item is a category, it will contain all its profile items. By default,
 * $user_profile['summary'] is provided, which contains data on the user's
 * history. Other data can be included by modules. $user_profile['user_picture']
 * is available for showing the account picture.
 *
 * Available variables:
 *   - $user_profile: An array of profile items. Use render() to print them.
 *   - Field variables: for each field instance attached to the user a
 *     corresponding variable is defined; e.g., $account->field_example has a
 *     variable $field_example defined. When needing to access a field's raw
 *     values, developers/themers are strongly encouraged to use these
 *     variables. Otherwise they will have to explicitly specify the desired
 *     field language, e.g. $account->field_example['en'], thus overriding any
 *     language negotiation rule that was previously applied.
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-item.tpl.php
 *   Where the html is handled for each item in the group.
 * @see template_preprocess_user_profile()
 *
 * @ingroup themeable
 */
?>
<?php drupal_set_title($account->fullname); ?>
<div id="profile-container">
  <div id="profile-top">
    <div id="profile-image"><?php
      if (!empty($photo_scolding)) {
        print $photo_scolding;
      } else {
        print theme('user_picture', $account);
      } ?></div>
    <div id="name-title">
      <h3><?php print render($user_profile['fullname']); ?></h3>

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
    <div id="account-comments"><?php print check_markup($account->comments); ?></div>
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
          <?php print render($user_profile['services']); ?>
        </ul>
      <?php endif; ?>
    </div>
    <div id="recommendations">
      <h2><?php print t('Feedback'); ?></h2>
      <?php print views_embed_view('user_referrals_by_referee', 'block_1', $account->uid); ?>
    </div>
  </div>
</div>
