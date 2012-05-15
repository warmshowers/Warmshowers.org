
Drupal.behaviors.expandMap = function(context) {
    // Grab the original map height upon loading of the page
    var originalMapHeight = $('#wsmap_map').height();
    var originalMapBlockHeight = $('div.block-wsmap').height();

    $("#expand_map").click(function(){
        if($(this).html() == 'Expand Map'){
            $(".region-sidebar-first .section").hide(1000);
            $(".region-highlight").hide(1000);
            $(".sidebar-first #content").animate({
                'margin-left': '0px',
                'width': '100%'
            }, 1000, function(){
                $("#expand_map").html("Collapse Map");
            });
            // If the window can handle, let's expand the height too
            if($(window).height() - 150 > $('#wsmap_map').height()){
                $("#wsmap_map").animate({
                    'height': $(window).height()  - 150 + 'px'
                }, 1000);
                $("div.block-wsmap").animate({
                    'height': $(window).height()  - 130 + 'px'
                }, 1000);


            }
        }
        else {
            $(".region-sidebar-first .section").show(1000);
            $(".region-highlight").show(1000);
            $(".sidebar-first #content").animate({
                'margin-left': '240px',
                'width': '720px'
            }, 1000, function(){
                $("#expand_map").html("Expand Map");
            });

            // If the height was expanded on expansion, let's collapse to original height
            if($('#wsmap_map').height() > originalMapHeight){
                $("#wsmap_map").animate({
                    'height':originalMapHeight + 'px'
                }, 1000);
                $("div.block-wsmap").animate({
                    'height':originalMapBlockHeight + 'px'
                }, 1000);

            }
        }
            return false;
    });
}