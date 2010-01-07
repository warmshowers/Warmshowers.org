//$Id:  $
/**
 * @file
 * Javascript to support translatable regions on the page.
 * 
 * Primarily uses the jquery.translate plugin.
 * Adapted from translatableregions module by Dave Trainer.
 */
Drupal.behaviors.translatableregions = function (context) {
  var link = $("<a />").attr("href", "#").attr("class","translator");
  var wrapper = $("<span></span>").attr("class","translator");
  var languages = ["it", "ja", "nl", "pt", "fr", "de", "es", "en"];
  var translate_selectors = Drupal.settings.translatableregions.translate_selectors;
  var auto_translate = Drupal.settings.translatableregions.auto_translate;
  var my_language_code = Drupal.settings.translatableregions.my_language_code;
  var my_language_name = Drupal.settings.translatableregions.my_language_name;
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
    $(translate_selectors).each(function (i) {
      // TODO: Handle the mapping of browser language to Google's languages. 
      // They aren't all the same. Portuguese is the biggest example.
      $(this).translate(browser_language);
      $.translate.getBranding().appendTo($(this)).prepend(Drupal.t("Translation") + " ");
    });
  } 
  // Otherwise, put a select at the top of the block offering translation.
  else {
    $.translate(function(){ //when the Google Language API is loaded
      var languages = $.translate.getLanguages(true);
      for (var key in languages) {
        if (languages[key].substr(0,2) == browser_language) {
          // Capitalize so that it will match the name in the select.
          browser_language_name = key.charAt(0)+key.substring(1).toLowerCase();
          break;
        }
      }
      $(translate_selectors).each(function (i) {
        var element = $(this);
        button = $("<input type='submit' value='" + Drupal.t('Translate To') + "' />")
        .click(function(){
          translate_element(element, $(this).parent().children('select').val());
        });
        $.translate.ui('select', 'option')
        .change(function(){ //when selecting another language
          translate_element(element, $(this).val());
        })
        .val(browser_language_name)
        .prependTo($(this))
        .parent()
        .prepend(button);
        
      });
    });
  }
}

function translate_element(element, target_language) {
  element.translate(target_language, { 
    //TODO: Make user-configurable exclusions
    not: '.option, #demo, #source, pre, .jq-translate-ui', //exclude these elements
    fromOriginal:true //always translate from orig (even after the page has been translated)
  });
  element.children('div.gBranding').remove();
  $.translate.getBranding().appendTo(element).prepend(Drupal.t("Translation") + " ");
}