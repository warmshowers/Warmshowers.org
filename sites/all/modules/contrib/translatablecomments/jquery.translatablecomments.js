// $Id: jquery.translatablecomments.js,v 1.1.4.2 2009/11/20 23:38:06 davetrainer Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function(){
    
    var link = $("<a />").attr("href", "#").attr("class","translator");
    var wrapper = $("<span></span>").attr("class","translator");
    var languages = ["it", "ja", "nl", "pt", "fr", "de", "es", "en"];
  
    $(".comment .content").each(function (i) {    
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
    });
  });
}
