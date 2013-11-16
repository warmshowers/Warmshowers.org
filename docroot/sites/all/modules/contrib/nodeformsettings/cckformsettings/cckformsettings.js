// $Id:$

/**
 * Code taken from user.js
 */
Drupal.behaviors.cckformsettings = function (context) {  
  $('div.cckformsettings-radios input[type=radio]:not(.cckformsettingsSettings-processed)', context).addClass('cckformsettingsSettings-processed').click(function () {
    $('div.cckformsettings-show', context)[['hide', 'show'][this.value]]();
  });
};