/**
 * Code taken from user.js
 */
Drupal.behaviors.nodeformsettings = function (context) {
  $('div.nodeformsettings-radios input[type=radio]:not(.nodeformsettingsSettings-processed)', context).addClass('nodeformsettingsSettings-processed').click(function () {
    $('div.nodeformsettings-show-settings', context)[['hide', 'show'][this.value]]();
  });
  
  $('div.nodeformsettings-radios-preview input[type=radio]:not(.nodeformsettingsSettings-processed)', context).addClass('nodeformsettingsSettings-processed').click(function () {
    $('div.nodeformsettings-show-preview', context)[['show', 'hide'][this.value]]();
  });
};