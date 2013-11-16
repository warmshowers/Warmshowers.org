/**
 * This simple file was temporarily loaded by the module and used to create
 * an array of translations of "Translate to", keyed by language.
 * Then translateToTranslations was dumped from the JS console with
 * JSON.stringify(translateToTranslations);
 */

var translateToTranslations = {};

Drupal.behaviors.retrieveTranslateToTranslations = function (context) {
  var api_key = Drupal.settings.translatableregions.api_key;
  $.translate.load(api_key, 2);
  var langs = $.translate.nativeLanguages;
  var sourceText = 'Translate to';

  for (var targetLanguage in langs) {
    $.translate(sourceText, 'en', targetLanguage, {
      complete: function(translation) {
        translateToTranslations[this.to] = translation;
      }
    });
  }
}
