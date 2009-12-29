//$Id:  $
/**
 * Adapted from translatablecomments module by Dave Trainer.
 */
Drupal.behaviors.translatablecomments = function (context) {
  var link = $("<a />").attr("href", "#").attr("class","translator");
  var wrapper = $("<span></span>").attr("class","translator");
  var languages = ["it", "ja", "nl", "pt", "fr", "de", "es", "en"];
  var translate_classes = Drupal.settings.translatablecomments.translate_classes;
  var auto_translate = Drupal.settings.translatablecomments.auto_translate;
  var my_language_code = Drupal.settings.translatablecomments.my_language_code;
  var my_language_name = Drupal.settings.translatablecomments.my_language_name;
  var browser_language;
  if (navigator.userLanguage) // Explorer
    browser_language = navigator.userLanguage;
  else if (navigator.language) {
    browser_language = navigator.language;
  } 
  else {
    browser_language = "en";
  }
  browser_language = browser_language.substr(0,2);
  browser_language_name = "English";
  
  
  // If auto_translate, we'll just translate the block no matter what.
  if (auto_translate) {
    $(translate_classes).each(function (i) {
      $(this).translate(my_language_code);
      $.translate.getBranding().appendTo($(this)).prepend(Drupal.t("Translation") + " ");
    });
  } 
  // Otherwise, put a select at the top of the block offering translation.
  else {
    $.translate(function(){ //when the Google Language API is loaded
      var languages = $.translate.getLanguages(true);
      for (var key in languages) {
        if (languages[key] == browser_language) {
          // Capitalize so that it will match the name in the select.
          browser_language_name = key.charAt(0)+key.substring(1).toLowerCase();
          break;
        }
      }
      $(translate_classes).each(function (i) {
        var element = $(this);
        button = $("<input type='submit' value='" + Drupal.t('Translate To') + "' />")
        .click(function(){
          element.translate($(this).parent().children('select').val(), { 
            not: '.option, #demo, #source, pre, .jq-translate-ui', //exclude these elements
            fromOriginal:true //always translate from orig (even after the page has been translated)
          })
        });
        $.translate.ui('select', 'option')
        .change(function(){ //when selecting another language
          element.translate($(this).val(), { 
            not: '.option, #demo, #source, pre, .jq-translate-ui', //exclude these elements
            fromOriginal:true //always translate from orig (even after the page has been translated)
          })
        })
        .val(browser_language_name)
        .prependTo($(this))
        .parent()
        .prepend(button);
        
        $.translate.getBranding().appendTo($(this)).prepend(Drupal.t("Translation") + " ");
      });
    });
  }
}


/*
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
*/
