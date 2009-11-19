//$Id: jquery.translatablecomments.js,v 1.1 2008/04/15 21:23:05 davetrainer Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function(){
    var max_translate = 500; // Biggest chunk Google will accept

    var link = $("<a />")
    .attr("href", "#")
    .css({ border:"1px solid #555", padding:"1px 3px", margin:"1px", font:"#dff", float:"right", fontSize:".8em", textDecoration:"none" });
    var wrapper = $("<span></span>").css({ float:"right" });
    var languages = [ "fr", "de", "es", "en", "pt"];
    // This probably works poorly after the first language, because it depends
    // on the detection of the language. So the original should be stored and
    // reused, or something...
    $(".comment .content, .user-profile .about-me, .node .content").each(function (i) {  
      // , .view-user-referrals-by-referrer .views-field-body, .view-user-referrals-by-referee .views-field-body
      var w = $(wrapper).clone().insertBefore(this);

      jQuery.each(languages, function(x){
        w.append(link.clone().html(languages[x]).click(function () {
          var text = $(this).parent().next();
          var translated = "";
          var sourceLanguage = "";
          google.language.detect(text.html().substr(0,500), function(result) {
            if (!result.error && result.language) {
              sourceLanguage = result.language;
              for (i=0; i<text.html().length; i+=max_translate) {
                var totranslate = text.html().substr(i,max_translate);
                google.language.translate(totranslate, sourceLanguage, languages[x],
                    function(result) {
                  if (result.translation) {
                    translated += result.translation;
                  }
                });
              };
            }
            if (translated.length) {
              text.html(translated);
            }
            this.blur();
            return false;
          });
        }));
      });
    });
  });
}
