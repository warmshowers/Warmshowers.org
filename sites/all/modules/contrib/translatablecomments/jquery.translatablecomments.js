//$Id: jquery.translatablecomments.js,v 1.1.4.2 2009/11/20 23:38:06 davetrainer Exp $

Drupal.behaviors.translatablecomments = function (context) {
  var link = $("<a />").attr("href", "#").attr("class","translator");
  var wrapper = $("<span></span>").attr("class","translator");
  var languages = ["it", "ja", "nl", "pt", "fr", "de", "es", "en"];
  var translate_classes = Drupal.settings.translatablecomments.translate_classes;
  var auto_translate = Drupal.settings.translatablecomments.auto_translate;
  var my_language = Drupal.settings.translatablecomments.my_language;

  $(translate_classes).each(function (i) {
    if (auto_translate) {
      // detect the language
      // Get the current language
      // translate/replace
      var text = $(this).html().substr(0,4999);
      var div = $(this);
      google.language.detect(text, function(result) {
        if (!result.error && result.language && my_language != result.language) {
          google.language.translate(text, result.language, my_language, function(result) {
            if (!result.error) {
              div.html(result.translation);
            } else {
              alert(result.error.message);
            }
          });
        }
      });
    }
    else {
      var w = $(wrapper).clone().insertBefore(this);
      jQuery.each(languages, function(x){
        w.append(link.clone().html(languages[x]).click(function () {
          var n = $(this).parent().next();
          google.language.translate(n.html(), "", languages[x], function(result) {
            if (!result.error) {
              n.html(result.translation);
            } else {
              alert(result.error.message);
            }
          });
          this.blur();
          return false;
        }));
      });
    }
  });
}

