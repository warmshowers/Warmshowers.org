/**
 * @file
 * Javascript to support translatable regions on the page.
 *
 * Uses the jquery.translate plugin.
 * Adapted from translatableregions module by Dave Trainer.
 */
(function ($) {

  Drupal.behaviors.translatableregions = {
      attach: transform_page
  };

  function transform_page(context, settings) {
    var link = $("<a />").attr("href", "#").attr("class","translator");
    var wrapper = $("<span></span>").attr("class","translator");
    var translate_selectors = settings.translatableregions.translate_selectors;
    var auto_translate = settings.translatableregions.auto_translate;
    var hide_translate_button = settings.translatableregions.hide_translate_button;
    var always_show_translate_buttons = settings.translatableregions.always_show_translate_buttons;
    var api_key = Drupal.settings.translatableregions.api_key;
    var browser_language = Drupal.settings.translatableregions.browser_language;

    // "Translate to" in various languages, created using translatableregions_retrieve_translate_to_translations.js
    var translateTo = {"en": "Translate to", "af":"Vertaal na","be":"Перавесці на","is":"Þýða til","ga":"Aistrigh go","mk":"Преведете да","ms":"Diterjemahkan kepada","sw":"Kutafsiri kwa","cy":"Cyfieithu i","sq":"Translate në","yi":"איבערזעצן צו","ar":"ترجمة إلى","bg":"Превод","ca":"Traduir al","zh":"转换为","zh-TW":"轉換為","hr":"Prevedi na","cs":"Se promítají do","da":"Oversæt til","nl":"Vertalen naar","et":"Tõlgi","tl":"Isalin sa","fi":"Käännä","fr":"Traduire en","gl":"Traducir a","de":"Übersetzen auf","el":"Μετάφραση σε","iw":"תרגום","hi":"अनुवाद करने के लिए","hu":"Fordítás","id":"Terjemahkan ke","ja":"翻訳する。","lv":"Tulkot","ko":"로 번역","lt":"Versti į","it":"Traduci","mt":"Ittraduċi","no":"Oversett til","fa":"ترجمه به ...","ro":"Traduceţi în","pl":"Przekłada się","ru":"Перевести на","sk":"Sa premietajú do","sr":"Преведи на","sl":"Prevedi k","sv":"Översätta till","th":"แปลเป็​​น","es":"Traducir al","tr":"Çevir","uk":"Перекласти на","vi":"Dịch"};

    $.translate.load(api_key, 2);

    $.translate(function() {  // After translation engine is ready.

      // If auto_translate, we'll just translate the block no matter what.
      if (auto_translate) {
        $(translate_selectors).each(function (i) {
          var element = $(this);
          translate_element(element, browser_language);
        });
      }
      // Otherwise, put a select at the top of the block offering translation.
      else if (always_show_translate_buttons || !hide_translate_button) {
        $(translate_selectors).each(function (i) {
          var element = $(this);
          var button = $("<input type='submit' />")
            .val(translateTo[browser_language])
            .click(function(){
              translate_element(element, $(this).parent().children('select').val());
            });

          // Maintainer provided wonderful help on this at
          // http://code.google.com/p/jquery-translate/issues/detail?id=30
          $.translate.ui({
            tags: ["select", "option"],
            filter: $.translate.isTranslatable,
            label: $.translate.toNativeLanguage, //default
            includeUnknown: false
          })
            .val(browser_language)
            .change(function(){ //when selecting another language
              translate_element(element, $(this).val());
            })
            .val(browser_language)
            .prependTo($(this))
            .parent()
            .prepend(button);

        });
      }
    });
  }



  /**
   * Translate an element into a target language.
   * @param element
   *   The element to be translated.
   * @param target_language
   *   The language code to translate to. Must match one of Google's array.
   */
  function translate_element(element, target_language) {
    element.translate(target_language, {
      //TODO: Make user-configurable exclusions
      not: '.option, #demo, #source, pre, .jq-translate-ui', //exclude these elements
      fromOriginal:true //always translate from orig (even after the page has been translated)
    });
  }


})(jQuery);
