/*
    Warning: This is not elegant AT ALL.  However, it is here due to the painstaking work that eventually
    resulted in a full object with all of the proper translations for "Translate to" and all of the languages
    names for languages.  After this is included on a page, and runs successfully, your local environement should
    have a filled out langsObj that, from the console, you can JSON.stringify() and save the output to
    translatableregions_static.js

    Other warning:  This proved inconsistent at best from a local machine.  rfay and illmasterc skyped together
    and saw this before.  If there is a need to rerun this code, and update the staticLanguageList variable,
    do it from a publicly accessible server.
 */

var langsObj = {};
var langsArray = [];

Drupal.behaviors.retrieveTranslations = function (context) {
    var api_key = Drupal.settings.translatableregions.api_key;
    $.translate.load(api_key, 2);
    var langs = $.translate.GLL;

    for (var i in langs) {
        langsArray[langsArray.length] = langs[i];
        langsObj[langs[i]] = {};
        langsObj[langs[i]].translateTo = "Translate to";
        langsObj[langs[i]].languageTranslations = {};

        for (var i2 in langs) {
            langsObj[langs[i]].languageTranslations[langs[i2]] = i2;
        }
    }

    for (var i3 in langsObj) {
        var languagesObj = langsObj[i3].languageTranslations;
        var languagesArray = [];

        for (var i5 in languagesObj) {
            languagesArray[languagesArray.length] = languagesObj[i5];
        }

        $.translate(languagesArray , {
            from:'en',
            to:i3,
            each: function(i4, translation){
                langsObj[this.to].languageTranslations[langsArray[i4]] = translation;
            }
        });

        $.translate('Translate to' , {
            from:'en',
            to:i3,
            complete: function(translation){
                langsObj[this.to].translateTo = translation;
            }
        });
    }
}