Drupal.behaviors.expandMap = function(context) {
    $("#expand_map").click(function(){
        if($(this).html() == 'Expand Map'){

            $(".region-sidebar-first .section").hide(1000);
            $(".sidebar-first #content").animate({
                'margin-left': '0px',
                'width': '100%'
            }, 1000, function(){
                $("#expand_map").html("Collapse Map");
            });

        }
        else {
            $(".region-sidebar-first .section").show(1500);
            $(".sidebar-first #content").animate({
                'margin-left': '240px',
                'width': '720px'
            }, 1500, function(){
                $("#expand_map").html("Expand Map");
            });
        }
            return false;
    });
}