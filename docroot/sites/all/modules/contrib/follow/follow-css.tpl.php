/**
 * <?php print $icon_style['label']; ?> icon style.
 */
<?php print $selector_prefix; ?>a.follow-link {
  display: block;
  height: 30px;
  line-height: 26px;
  background-position: 0 0;
  background-repeat: no-repeat;
  padding-left: 28px;
  padding-right: 3px;
}

<?php print $selector_prefix; ?>span.follow-link-wrapper a {
  float: left;
}

<?php print $selector_prefix; ?>a.follow-link-facebook {
  background-image: url(<?php print $icon_path; ?>/icon-facebook.png);
}

<?php print $selector_prefix; ?>a.follow-link-googleplus {
  background-image: url(<?php print $icon_path; ?>/icon-googleplus.png);
}

<?php print $selector_prefix; ?>a.follow-link-myspace {
  background-image: url(<?php print $icon_path; ?>/icon-myspace.png);
}

<?php print $selector_prefix; ?>a.follow-link-virb {
  background-image: url(<?php print $icon_path; ?>/icon-virb.png);
}

<?php print $selector_prefix; ?>a.follow-link-bliptv {
  background-image: url(<?php print $icon_path; ?>/icon-bliptv.png);
}

<?php print $selector_prefix; ?>a.follow-link-lastfm {
  background-image: url(<?php print $icon_path; ?>/icon-lastfm.png);
}

<?php print $selector_prefix; ?>a.follow-link-youtube {
  background-image: url(<?php print $icon_path; ?>/icon-youtube.png);
}

<?php print $selector_prefix; ?>a.follow-link-twitter {
  background-image: url(<?php print $icon_path; ?>/icon-twitter.png);
}

<?php print $selector_prefix; ?>a.follow-link-picasa {
  background-image: url(<?php print $icon_path; ?>/icon-picasa.png);
}

<?php print $selector_prefix; ?>a.follow-link-flickr {
  background-image: url(<?php print $icon_path; ?>/icon-flickr.png);
}

<?php print $selector_prefix; ?>a.follow-link-vimeo {
  background-image: url(<?php print $icon_path; ?>/icon-vimeo.png);
}

<?php print $selector_prefix; ?>a.follow-link-linkedin {
  background-image: url(<?php print $icon_path; ?>/icon-linkedin.png);
}

<?php print $selector_prefix; ?>a.follow-link-delicious {
  background-image: url(<?php print $icon_path; ?>/icon-delicious.png);
}

<?php print $selector_prefix; ?>a.follow-link-tumblr {
  background-image: url(<?php print $icon_path; ?>/icon-tumblr.png);
}

<?php print $selector_prefix; ?>a.follow-link-this-site {
  background-image: url(<?php print $icon_path; ?>/icon-feed.png);
}

<?php print $selector_prefix; ?>a.follow-link-viadeo {
  background-image: url(<?php print $icon_path; ?>/icon-viadeo.png);
}

<?php print $selector_prefix; ?>a.follow-link-xing {
  background-image: url(<?php print $icon_path; ?>/icon-xing.png);
}

<?php print $selector_prefix; ?>a.follow-link-spiceworks {
  background-image: url(<?php print $icon_path; ?>/icon-spiceworks.png);
}

<?php print $selector_prefix; ?>a.follow-link-newsletter {
  background-image: url(<?php print $icon_path; ?>/icon-newsletter.png);
}

<?php if (!empty($css_overrides)): ?>
/* Custom overrides for this style. */
<?php print $css_overrides; ?>
/* End custom overrides. */
<?php endif; ?>
