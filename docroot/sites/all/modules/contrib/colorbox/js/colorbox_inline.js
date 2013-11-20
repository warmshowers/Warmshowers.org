(function ($) {

Drupal.behaviors.initColorboxInline = function (context) {
  if (!$.isFunction($.colorbox)) {
    return;
  }
  var settings = Drupal.settings.colorbox;
  $.urlParam = function(name, url){
    if (name == 'fragment') {
      var results = new RegExp('(#[^&#]*)').exec(url);
    }
    else {
      var results = new RegExp('[\\?&]' + name + '=([^&#]*)').exec(url);
    }
    if (!results) { return ''; }
    return results[1] || '';
  };
  $('a, area, input', context).filter('.colorbox-inline:not(.initColorboxInline-processed)').addClass('initColorboxInline-processed').colorbox({
    transition:settings.transition,
    speed:settings.speed,
    opacity:settings.opacity,
    slideshow:settings.slideshow,
    slideshowAuto:settings.slideshowAuto,
    slideshowSpeed:settings.slideshowSpeed,
    slideshowStart:settings.slideshowStart,
    slideshowStop:settings.slideshowStop,
    current:settings.current,
    previous:settings.previous,
    next:settings.next,
    close:settings.close,
    overlayClose:settings.overlayClose,
    maxWidth:settings.maxWidth,
    maxHeight:settings.maxHeight,
    innerWidth:function(){
      return $.urlParam('width', $(this).attr('href'));
    },
    innerHeight:function(){
      return $.urlParam('height', $(this).attr('href'));
    },
    title:function(){
      return $.urlParam('title', $(this).attr('href'));
    },
    iframe:function(){
      return $.urlParam('iframe', $(this).attr('href'));
    },
    inline:function(){
      return $.urlParam('inline', $(this).attr('href'));
    },
    href:function(){
      return $.urlParam('fragment', $(this).attr('href'));
    }
  });
};

})(jQuery);
