//$Id: translatableregions.js,v 1.3 2010/02/24 00:19:05 rfay Exp $
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
  var translate_selectors = Drupal.settings.translatableregions.translate_selectors;
  var auto_translate = Drupal.settings.translatableregions.auto_translate;  
  var hide_translate_button = Drupal.settings.translatableregions.hide_translate_button;
  var always_show_translate_buttons = Drupal.settings.translatableregions.always_show_translate_buttons;
  
  // If auto_translate, we'll just translate the block no matter what.
  if (auto_translate) {
    $.translate(function(){ //when the Google Language API is loaded
      var browser_language = get_browser_language();
      $(translate_selectors).each(function (i) {
        var element = $(this);
        translate_element(element, browser_language);
      });
    });

  } 
  // Otherwise, put a select at the top of the block offering translation.
  else if (always_show_translate_buttons || !hide_translate_button) {
    $.translate(function(){ //when the Google Language API is loaded
      var browser_language = get_browser_language();
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
        .val(browser_language)
        .prependTo($(this))
        .parent()
        .prepend(button);
        
      });
    });
  }
}

/**
 * Translate an element into a target language.
 * @param element
 * @param target_language
 *   The language code to translate to. Must match one of Google's array.
 */
function translate_element(element, target_language) {
  element.translate(target_language, { 
    //TODO: Make user-configurable exclusions
    not: '.option, #demo, #source, pre, .jq-translate-ui', //exclude these elements
    fromOriginal:true //always translate from orig (even after the page has been translated)
  });
  element.children('div.gBranding').remove();
  $.translate.getBranding().appendTo(element).prepend(Drupal.t("Automatic translation") + " ");
}

/**
 * Determine the browser language and then match it to one of Google's languages.
 */
function get_browser_language() {
  var browser_language;
  
  // First, find the language the browser reports to us. Probably
  // something like en-US.
  if (navigator.userLanguage) // Explorer
    browser_language = navigator.userLanguage;
  else if (navigator.language) {
    browser_language = navigator.language;
  } 
  else {
    browser_language = "en";
  }

  // Now get Google's list of languages and see if we can match it up.
  var languages = $.translate.getLanguages(true);
  if (!(browser_language in languages)) {
    // Try it without the country code
    if (browser_language.substr(0,2) in languages) {
      browser_language = browser_language.substr(0,2);
    } 
    // Otherwise we have to look through the whole list looking for a match
    // on the language portion!
    else {
      browser_language = browser_language.substr(0,2);
      for (var langcode in languages) {
        if (languages[langcode].substr(0,2) == browser_language) {
          browser_language = languages[langcode];
          break;
        }
      } 
    }
  }
  return browser_language;
}