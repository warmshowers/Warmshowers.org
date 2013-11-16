// $Id: thickbox_login.js,v 1.2.2.5 2009/07/30 09:13:53 frjo Exp $
// Contributed by user jmiccolis.
Drupal.behaviors.initThickboxLogin = function(context) {
  $("a[href*='/user/login']", context).addClass('thickbox').each(function() { this.href = this.href.replace(/user\/login(%3F|\?)?/,"user/login/thickbox?height=230&width=250&") });
  $("a[href*='?q=user/login']", context).addClass('thickbox').each(function() { this.href = this.href.replace(/user\/login/,"user/login/thickbox&height=230&width=250") });
}