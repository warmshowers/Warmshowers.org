
Drupal.behaviors.expandMap = function(context) {
    // Grab the original map height upon loading of the page
    var originalMapHeight = $('#wsmap_map').height();
    var originalMapBlockHeight = $('div.block-wsmap').height();

    $("#expand_map").click(function(){
        $(".region-sidebar-first .section").hide(1000);
        $(".region-highlight").hide(1000);
        $("body.with-highlight #navigation .section").hide(1000);
        $(".sidebar-first #content").animate({
            marginLeft: "0px",
            width: "100%"
        }, 1000, function(){
            $("#collapse_map").show();
            $("body.with-highlight #navigation .section").css("top","-69px");
            $("body.with-highlight #navigation .section").show("fast");
        });
        // If the window can handle, let's expand the height too
        if($(window).height() - 150 > $('#wsmap_map').height()){
            $("#wsmap_map").animate({
                'height': $(window).height()  - 150 + 'px'
            }, 1000);

            $("#content .block-wsmap").animate({
                'height': $(window).height()  - 130 + 'px'
            }, 1000);

        }

        return false;
    });

    $("#collapse_map").click(function(){
        $("body.with-highlight #navigation .section").hide(1000);
        $(".region-sidebar-first .section").show(1000);
        $(".region-highlight").show(1000);
        $(".sidebar-first #content").animate({
            marginLeft: '240px',
            width: '720px'
        }, 1000, function(){
            $("#expand_map").show();
            $("#collapse_map").hide();
            $("body.with-highlight #navigation .section").css("top","-260px");
            $("body.with-highlight #navigation .section").show("fast");
        });

        // If the height was expanded on expansion, let's collapse to original height
        if($('#wsmap_map').height() > originalMapHeight){
            $("#wsmap_map").animate({
                'height':originalMapHeight + 'px'
            }, 1000);
            $("#content .block-wsmap").animate({
                'height':originalMapBlockHeight + 'px'
            }, 1000);

        }

            return false;
    });
}