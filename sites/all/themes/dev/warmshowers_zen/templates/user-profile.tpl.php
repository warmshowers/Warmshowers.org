


<div id="profile-container">
  <div id="profile-image"><?php print theme('user_picture', $account); ?></div>
  <div id="name-title">
    <h3><?php print $account->fullname; ?></h3>
    <br />
    1,045 <?php print t('Recommendations'); ?> -
    <?php print t('Member for') . ' ' . $account->content['summary']['member_for']['#value']; ?>
  </div>
  <div class="content">
    <h1><?php print t('About this Member'); ?></h1>
    <?php print check_markup($account->comments); ?>
    <div id="host-services">
      <h2><?php print t('This Host Offers'); ?></h2>
      <ul>
        <?php print $services; ?>
      </ul>
    </div>
    <div id="recommendations">
      <h2><?php print t('Recommendations'); ?></h2>
      <?php print views_embed_view('user_referrals_by_referee', 'block_1', $account->uid); ?>
    </div>
  </div>
  <div id="right-sidebar">
    <div id="block-1">
      Woot
    </div>
    <div id="block-2">
      Woot

    </div>
  </div>
</div>
  <?php
drupal_set_title(check_plain("$account->fullname"));
// Basic member info introduction
$mail = theme('email_link',$account);
$provincemap = _user_location_get_provincecode_to_province_map($account->country);
$countrylist = _user_location_supported_countries();
$countryname = $countrylist[$account->country];
$province = $provincemap[$account->province];
$output .= "
<div class='user-profile'>
<fieldset><legend><b>" . t("Introduction") . "</b></legend>";


$output .= "<b>" . t("Member") . ":</b>";
$output .= "<dl>";
$nameinfo = $account->fullname;

$output .= _output_with_tag("<dd><b>", $nameinfo, "</b></dd>");
if ($account->isunreachable) { $output .= "<dd><i>" . t("This member is considered unreachable. Email has bounced or telephone contact has been unsuccessful").".</i></dd>"; }
if ($account->isstale) { $output .= "<dd><i>" . t("This member is condered stale, as they have not logged into the system for a long time").".</i></dd>"; }
if (!$account->status) { $output .= "<dd><i>".t("This member is blocked").".</i></dd>"; }

$output .= "<dd>".t("Email").": $mail</dd>";
$output .= _output_with_tag("<dd>". t("Username").":", $account->name, "</dd>");
$output .= _output_with_tag("<dd>", $account->street, "</dd>");
$output .= _output_with_tag("<dd>", $account->additional, "</dd>");
$output .= _output_with_tag("<dd>","$account->city, $province " . (strtoupper($account->postal_code) != 'NONE' ? $account->postal_code : '') . " $countryname", "</dd>");
$output .= "</dl>";
if ($account->URL) {
  $output .= '<b>'.t("Website").':</b> <a href="' . check_url($account->URL) . '">' . check_url($account->URL) . "</a><br/>";
}
$output .= _output_with_tag_markup("<dl><dt><b>".t("About this member").":</b></dt><dd class='about-me'>", $account->comments, "</dd></dl><br/>");



$output .= "</fieldset>";


// Location section
if (user_access('administer user profiles') || user_is_current($account) || !$account->notcurrentlyavailable) {
  $output .= "<fieldset><legend><b>".t("Location Map")."</b></legend>";
  $output .= $account->content['#mapHTML'];
  $output .= "<div class='viewmembers' >".t("View members"). " <a href='/map/" . drupal_urlencode("uid=" . $account->uid) . "'>" . t("near this one"). "</a> ".t("on the full map")."</div>";


  $output .= "</fieldset>";
}
unset($fields['Location']);

// Add last login time
$account->content['summary']['last_login'] = array(
  '#printed' => TRUE,
  '#title' => t("Last login was"),
  '#value' => format_interval(time() - $account->login) . " " . t("ago"),
);
$output .= _item_output_html(t('History'), $account->content['summary']);

$output .= '<div class="profile">';




$output .= '<fieldset><legend><b>'. t('Member Information') .'</b></legend>';


if ($account->homephone || $account->mobilephone || $account->workphone || $account->fax) {
  $output .= "<b>".t("Phones") .": </b>" ;
  $output .= _output_with_tag(t("Home Phone Number"), $account->homephone);
  $output .= " ". _output_with_tag(t("Work"), $account->workphone);
  $output .= " ". _output_with_tag(t("Mobile"), $account->mobilephone);
  $output .= " ". _output_with_tag(t("Fax"), $account->fax_number);
  $output .= "<br/>\n";
}
if ($account->notcurrentlyavailable){
  $output .= "<b><i>".t("This user is set as unavailable, possibly on the road, so much of their information is not shown").".</i></b><br/>\n";

} else {
  $output .= "<div><a href='/map/" . drupal_urlencode("uid=" . $account->uid) . "'>".t("View members near this one on the map")."</a></div>";
  $output .= _output_with_tag("<b>".t("Preferred Notice")."</b>", $account->preferred_notice, "<br/>");
  $maxcyclists=$account->maxcyclists;
  if ($maxcyclists == '5') {  $maxcyclists = t("5 or more"); }
  $output .= _output_with_tag("<b>".t("Maximum Guests")."</b>", $maxcyclists, "<br/>");
  $output .= _output_with_tag("<b>".t("Nearest hotel/motel/accomodation").":</b>", $account->motel, "<br/>");
  $output .= _output_with_tag("<b>".t("Nearest Campground")."</b>", $account->campground, "<br/>");
  $output .= _output_with_tag("<b>".t("Nearest Bike Shop")."</b>", $account->bikeshop, "<br/>");
  $output .= _output_with_tag("<b>".t("Languages Spoken")."</b>", $account->languagesspoken, "<br/>");
}


$fieldlist = wsuser_fieldlist();
foreach (explode(" ", "bed food laundry lawnspace sag shower storage kitchenuse") as $item) {
  if ($account->$item) {
    $services .= "<li>".t($fieldlist[$item]['title'])."</li>";
  }
}
if (!$account->notcurrentlyavailable) {
  if ($services) {
    $output .= "<b>".t("Services this host may offer").":</b>";
    $output .= "<ul>";
    $output .= $services;
    $output .= "</ul>";
  }
}
$output .= "</fieldset>";

unset($fields['Member Information']);
$output .= '<fieldset><legend><b>'. t('Members I Recommend and Who Recommend Me') .'</b></legend>';


$output .= views_embed_view('user_referrals_by_referrer', 'default', $account->uid);
$output .= views_embed_view('user_referrals_by_referee', 'default', $account->uid);

// If the viewing user is not this user, offer to do a recommendation
if ($GLOBALS['user']->uid != $account->uid) {
  $url = url('node/add/trust-referral', array(
    'absolute' => TRUE,
    'query' => array(
      'edit[field_member_i_trust][0][uid][uid]' => $account->name,
    ),
  ));
  $link = l(t('Click here to add a recommendation'), $url);

  $output .= "<br/>".t("Do you recommend this member? ") . $link;
}

$output .= "</fieldset>";

$output .= '<fieldset><legend><b>' . t('Actions and Markings') . '</b></legend>' . $profile['flags'];
$flag = flag_get_flag('responsive_member') or die('no "responsive_member" flag');
$count = $flag->get_count($account->uid);
if ($count) {
  $output .= '<div class="num-responsive">' . t("%num members have marked %name as responsive", array('%num' => $count, '%name' => $account->name)) . "</div>";
}
$flag = flag_get_flag('unresponsive_member') or die('no "unresponsive_member" flag');
$count = $flag->get_count($account->uid);
if ($count) {
  $output .= '<div class="num-unresponsive">' . t("%num members have marked %name as unresponsive", array('%num' => $count, '%name' => $account->name)) . "</div>";
}
$output .= '</fieldset></div>';

$output .= "</div>";



//print $output;

function _output_with_tag($tag, $item, $closing="") {
  $output = "";
  if ($item) {
    $output .= "$tag ". check_plain($item);
    $output .= $closing;
  }
  return $output;
}
function _output_with_tag_markup($tag, $item, $closing="") {
  $output = "";
  if ($item) {
    $output .= "$tag ". check_markup($item,FILTER_FORMAT_DEFAULT,false);
    $output .= $closing;
  }
  return $output;
}

function _item_output_html($category, $items) {
  if (strlen($category) > 0) {
    $output .= "<fieldset><legend><b>$category</b></legend>";
  }
  if ($items) {
    foreach ($items as $item) {
      if ($item['#printed'] === TRUE) {
        if (isset($item['#title'])) {
          $output .= "<b>". $item['#title'] .':</b><br /> ' . $item['#value'] . "<br /><br />";
        }
      }
    }
  }
  $output .= "</fieldset>";
  return $output;
}
function _output_wsuser_data($title, $account, $fields) {

  $output .= '<h2 class="title">'. check_plain("Host Information") .'</h2>';


  $output .= _output_if_set($account->fullname) . "<br/>";
  $output .= _output_if_set($account->street) . "<br/>";

  return $output;


}

function _output_if_set($str) {
  if ($str) {
    return check_plain($str);
  }
}

function user_is_current($named_user) {
  global $user;
  return ($user->uid == $named_user->uid);
}
